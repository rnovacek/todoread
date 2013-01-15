/*
 *
 */

#include "readitlater.h"
#include "json.h"
#include "todoreadmodel.h"
#include "todoreaditem.h"

#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtGui/QDesktopServices>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

const QString Pocket(QString::fromLatin1("https://getpocket.com"));
const QString PocketRequestUrl(Pocket + QString::fromLatin1("/v3/oauth/request"));
const QString PocketAuthorizeUrl(Pocket + QString::fromLatin1("/v3/oauth/authorize"));
const QString PocketGetUrl(Pocket + QString::fromLatin1("/v3/get"));
const QString PocketModifyUrl(Pocket + QString::fromLatin1("/v3/send"));

const QString PocketUserAuthUrl(Pocket + QString::fromLatin1("/auth/authorize"));
const QString PocketConsumerKey(QString::fromLatin1("11362-06330fc63d534f11c7492ae8"));

const QString PocketStatusNew(QString::fromLatin1("0"));
const QString PocketStatusArchived(QString::fromLatin1("1"));
const QString PocketStatusDeleted(QString::fromLatin1("2"));

ReadItLater::ReadItLater(TodoReadModel *model, QSettings *settings, QObject *parent) :
    ReadDataSource(model, settings, parent), m_networkAccessManager(new QNetworkAccessManager(this))
{
    connect(m_networkAccessManager, SIGNAL(finished(QNetworkReply*)), SLOT(networkReply(QNetworkReply*)));
}

ReadItLater::~ReadItLater()
{

}

bool ReadItLater::needsAuth()
{
    return !m_settings->value("access_token").isValid();
}

void ReadItLater::auth()
{
    m_server = new QTcpServer(this);
    connect(m_server, SIGNAL(newConnection()), SLOT(serverConnection()));
    m_server->listen();
    QString redirectUri = QString("http://localhost:%1").arg(m_server->serverPort());

    QUrl postData;
    postData.addQueryItem("consumer_key", PocketConsumerKey);
    postData.addQueryItem("redirect_uri", redirectUri);

    QNetworkRequest request(PocketRequestUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded; charset=UTF8");
    request.setRawHeader("X-Accept", "application/x-www-form-urlencoded; charset=UTF8");
    m_networkAccessManager->post(request, postData.encodedQuery());
}

void ReadItLater::networkReply(QNetworkReply *reply)
{
    qDebug() << "ReadItLater::networkReply" << reply->url();
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Pocket server returned error: " << reply->errorString();
        if (reply->request().url() == PocketRequestUrl || reply->request().url() == PocketAuthorizeUrl) {
            emit authDone(false, reply->errorString());
        } else {
            emit syncDone(false, reply->errorString());
        }
        reply->deleteLater();
        return;
    }
    if (reply->request().url() == PocketRequestUrl) {
        // TODO: handle error codes
        QUrl url;
        url.setEncodedQuery(reply->readAll());
        m_requestToken = url.queryItemValue("code");

        QUrl authUrl(PocketUserAuthUrl);
        authUrl.addQueryItem("request_token", m_requestToken);
        QString redirectUri = QString("http://localhost:%1").arg(m_server->serverPort());
        authUrl.addQueryItem("redirect_uri", redirectUri);
        QDesktopServices::openUrl(authUrl);
    } else if (reply->request().url() == PocketAuthorizeUrl) {
        QUrl url;
        url.setEncodedQuery(reply->readAll());
        m_settings->setValue("access_token", url.queryItemValue("access_token"));
        m_settings->setValue("username", url.queryItemValue("username"));
        emit authDone(true, QString());
    } else if (reply->request().url() == PocketGetUrl) {
        parseDownloaded(QString::fromUtf8(reply->readAll()));
    } else if (reply->request().url() == PocketModifyUrl) {
        QString data(QString::fromUtf8(reply->readAll()));
        bool success = false;
        QVariant result = QtJson::parse(data, success);
        if (!success) {
            qWarning() << "Unable to parse server reply:" << data;
            emit syncDone(false, "Unable to parse server reply");
        } else {
            QString status = result.toMap().value("status", "-1").toString();
            if (status.toInt() != 1) {
                qWarning() << "Updating data on server failed:" << data;
                emit syncDone(false, "Updating data on server failed");
            }
            m_settings->setValue("lastSync", Global::Now());
        }
    }
    reply->deleteLater();
}

void ReadItLater::parseDownloaded(const QString& data)
{
    bool success = false;
    QVariantMap parsed = QtJson::parse(data, success).toMap();
    if (!success) {
        emit syncDone(false, "Unable to parse data");
        return;
    }
    QStringList updatedIds;
    foreach (TodoReadItem *item, model()->all()) {
        if (item->updated() > m_settings->value("lastSync", 0).toInt()) {
            updatedIds << item->id();
        }
    }
    TodoReadItem *item;
    QVariantList list;
    foreach (QVariant listitem, parsed["list"].toMap()) {
        QVariantMap it = listitem.toMap();
        QString id = it["item_id"].toString();
        item = m_model->get(id);
        if (item == NULL) {
            QString pocketStatus = it["status"].toString();
            TodoReadItem::Status status = TodoReadItem::NEW;
            if (pocketStatus == PocketStatusNew) {
                status = TodoReadItem::NEW;
            } else if (pocketStatus == PocketStatusArchived) {
                status = TodoReadItem::READ;
            } else if (pocketStatus == PocketStatusDeleted) {
                // Item should be deleted but it's not even here => nothing to do
                continue;
            } else {
                qWarning() << "Unknown status code: " << pocketStatus;
            }
            item = new TodoReadItem(id,
                it["resolved_title"].toString(),
                it["resolved_url"].toString(),
                status, Global::Now(), model());
            model()->addItem(item);
        } else {
            if (it.contains("resolved_url")) {
                item->setUrl(it.value("resolved_url").toString());
            }
            if (it.contains("resolved_title")) {
                item->setTitle(it.value("resolved_title").toString());
            }

            if (updatedIds.contains(id)) {
                // This item has been changed since last change => will override status
                continue;
            } else {
                if (it.contains("status")) {
                    // Was not updated => read the status
                    QString pocketStatus = it["status"].toString();
                    if (pocketStatus == PocketStatusNew) {
                        item->setStatus(TodoReadItem::NEW);
                    } else if (pocketStatus == PocketStatusArchived) {
                        item->setStatus(TodoReadItem::READ);
                    } else if (pocketStatus == PocketStatusDeleted) {
                        // Delete item
                        model()->deleteItem(id);
                    } else {
                        qWarning() << "Unknown status code: " << pocketStatus;
                    }
                }
            }
        }
    }
    QVariantList actions;
    foreach (QString id, updatedIds) {
        item = model()->get(id);
        QVariantMap map;
        map.insert("item_id", item->id());
        map.insert("time", QString("%1").arg(item->updated()));
        switch (item->status()) {
            case TodoReadItem::NEW:
                map.insert("action", "readd");
                break;
            case TodoReadItem::READ:
                map.insert("action", "archive");
                break;
            case TodoReadItem::DELETED:
                map.insert("action", "delete");
                break;
        }
        actions.append(map);
    }

    // TODO: send actions
    if (actions.size() == 0) {
        qDebug() << "Nothing to send";
        emit syncDone(true, QString());
        return;
    }
    success = false;
    QByteArray actionsArray = QtJson::serialize(actions, success);
    if (!success) {
        emit syncDone(false, "Unable to format data");
        return;
    }
    qDebug() << "Modify" << actionsArray;
    QUrl postData;
    postData.addQueryItem("actions", actionsArray);
    postData.addQueryItem("consumer_key", PocketConsumerKey);
    postData.addQueryItem("access_token", m_settings->value("access_token").toString());
    QNetworkRequest request(PocketModifyUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded; charset=UTF8");
    m_networkAccessManager->post(request, postData.encodedQuery());
}

void ReadItLater::serverConnection()
{
    QTcpSocket *sock;
    while ((sock = m_server->nextPendingConnection()) != NULL) {
        // TODO: check if correct connection
        QByteArray ba;
        ba = "HTTP/1.0 200 Ok\r\n"
             "Content-Type: text/html; charset=\"utf-8\"\r\n"
             "\r\n"
             "<h1>You can close this window.</h1>\n";
        sock->write(ba);
        authorize();
    }
}

void ReadItLater::authorize()
{
    QUrl postData;
    postData.addQueryItem("consumer_key", PocketConsumerKey);
    postData.addQueryItem("code", m_requestToken);

    QNetworkRequest request(PocketAuthorizeUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded; charset=UTF8");
    request.setRawHeader("X-Accept", "application/x-www-form-urlencoded; charset=UTF8");
    m_networkAccessManager->post(request, postData.encodedQuery());
}

void ReadItLater::sync()
{
    qDebug() << "ReadItLater::sync";
    if (needsAuth()) {
        connect(this, SIGNAL(authDone(bool, QString)), SLOT(get(bool, QString)));
        auth();
    } else {
        get();
    }
}

void ReadItLater::get()
{
    QUrl postData;
    postData.addQueryItem("consumer_key", PocketConsumerKey);
    postData.addQueryItem("access_token", m_settings->value("access_token").toString());
    // TODO: support other types too
    postData.addQueryItem("contentType", "article");
    postData.addQueryItem("state", "all");
    postData.addQueryItem("detailType", "complete");
    if (m_settings->contains("lastSync")) {
        postData.addQueryItem("since", QString("%1").arg(m_settings->value("lastSync").toInt()));
    }

    QNetworkRequest request(PocketGetUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded; charset=UTF8");
    m_networkAccessManager->post(request, postData.encodedQuery());
}
