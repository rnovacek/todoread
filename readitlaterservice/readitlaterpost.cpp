#include "readitlaterpost.h"

#include "readitlaterauth.h"

#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <Accounts/Manager>
#include <Accounts/Account>
#include <SignOn/Identity>

#include <WebUpload/Error>
#include <WebUpload/Media>

ReadItLaterPost::ReadItLaterPost(QObject *parent) : WebUpload::PostBase(parent)
{
}

WebUpload::AuthBase *ReadItLaterPost::getAuthPtr()
{
    qDebug() << "ReadItLaterPost::getAuthPtr";
    m_authBase = new ReadItLaterAuth(this);
    return m_authBase;
}

void ReadItLaterPost::stopMediaUpload()
{

}

void ReadItLaterPost::uploadMedia(WebUpload::Media *media)
{
    qDebug() << "ReadItLaterPost::uploadMedia";

    // TODO: is it necessary to parse it?
    QString url;
    foreach (QString part, media->copiedTextData().split(';')) {
        if (part.contains("charset=")) {
            url = part.right(part.size() - part.indexOf(',') - 1);
            qDebug() << "URL: " << url;
            break;
        }
    }

    QByteArray data("{\"0\":{\"title\":\"");
    data.append(QUrl::toPercentEncoding(media->title()));
    data.append("\",\"url\":\"");
    data.append(url.toLatin1());
    data.append("\"}}");

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(uploadFinished(QNetworkReply*)));

    QNetworkRequest request(QUrl("https://readitlaterlist.com/v2/send"));
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    QString apikey("926TnD0Vg5663lb9e7dc565vF9p4cW5a");

    qDebug() << "Username:" << m_authBase->username() << "password" << m_authBase->password();
    QUrl params;
    params.addQueryItem("username", m_authBase->username());
    params.addQueryItem("password", m_authBase->password());
    params.addQueryItem("apikey", apikey);
    params.addEncodedQueryItem("new", data);

    qDebug() << "Using query: " << params.encodedQuery();
    manager->post(request, params.encodedQuery());
}

void ReadItLaterPost::uploadFinished(QNetworkReply *reply)
{
    if (reply->hasRawHeader("X-Error")) {
        qDebug() << "uploadFinished with error: " << reply->rawHeader("X-Error");
    } else {
        qDebug() << "ReadItLaterPost::uploadFinished";
    }
    if (reply->error() == QNetworkReply::NoError) {
        emit done();
    } else {
        qDebug() << "Sending failed with error " << reply->error();
        emit error(WebUpload::Error::noConnection());
    }
}


/*
void ReadItLaterPost::upload(WebUpload::Entry *entry, WebUpload::Error err)
{
    qCritical() << "ReadItLaterPost::upload";
    if (err.code() != WebUpload::Error::CODE_NO_ERROR) {
        emit error(err);
    }
    m_entry = entry;
    Accounts::Manager *manager = new Accounts::Manager(this);
    Accounts::AccountIdList list = manager->accountListEnabled("sharing");
    Accounts::Account *account;

    int j;
    for (int i = 0; i < list.size(); ++i) {
        account = manager->account(list[i]);
        qDebug() << "Account: " << account->allKeys();

        Accounts::ServiceList serviceList = account->enabledServices();
        for (j = 0; j < serviceList.size(); ++j) {
            if (serviceList[j]->name() == "readitlater") {
                quint32 id = account->valueAsInt("CredentialsId", 0);
                if (id == 0) {
                    id = account->credentialsId();
                }

                SignOn::Identity *identity = SignOn::Identity::existingIdentity(id, this);
                connect(identity, SIGNAL(error(SignOn::Error)), SLOT(identityError(SignOn::Error)));

                if (identity) {
                    qDebug() << "Identity was obtained: " << identity;
                    m_session = identity->createSession(QLatin1String("password"));
                    connect(m_session, SIGNAL(response(SignOn::SessionData)), SLOT(identityResponse(SignOn::SessionData)));
                    connect(m_session, SIGNAL(error(SignOn::Error)), SLOT(identityError(SignOn::Error)));
                    m_session->process(SignOn::SessionData(), QLatin1String("password"));
                } else {
                    qDebug() << "No SSO identity for the account " << account->credentialsId();
                    emit error(WebUpload::Error::authorizationFailed());
                }
                return;
            }
        }
    }
}

void ReadItLaterPost::identityResponse(const SignOn::SessionData &data)
{
    if (!data.propertyNames().contains("Secret")) {
        qDebug() << "Identity is missing secret";
        emit error(WebUpload::Error::authorizationFailed());
    } else {
        doUpload(data.UserName(), data.Secret());
    }
}

void ReadItLaterPost::doUpload(const QString &username, const QString &password)
{
    WebUpload::Media *media;
    QStringList urls;
    for (unsigned int i = 0; i < m_entry->mediaCount(); i++) {
        media = m_entry->mediaAt(i);
        urls.push_back(QString("\"%1\":{\"title\":\"%2\",\"url\":\"%3\"}").arg(i).arg(media->title()).arg(media->origURI().toString()));
    }

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            SLOT(uploadFinished(QNetworkReply*)));

    QNetworkRequest request(QUrl("https://readitlaterlist.com/v2/send"));
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    QString apikey("926TnD0Vg5663lb9e7dc565vF9p4cW5a");

    QUrl params;
    params.addQueryItem("username", username);
    params.addQueryItem("password", password);
    params.addQueryItem("apikey", apikey);
    params.addQueryItem("new", QString("{%1}").arg(urls.join(",")));

    manager->post(request, params.encodedQuery());
}

void ReadItLaterPost::stop()
{

}
*/
