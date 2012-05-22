#include "controller.h"

#include <QDebug>
#include <QFile>
#include <QSettings>
#include <QDeclarativeView>
#include <QDeclarativeItem>
#include <QDeclarativeProperty>
#include <QDeclarativeContext>
#include <QMessageBox>
#include <QProgressDialog>
#include <QDesktopServices>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkConfigurationManager>
#include <QSignalMapper>
#include <QTimer>
#include <QUrl>
#include <QtWebKit/QWebSettings>
#include <qplatformdefs.h>

#include "globals.h"
#include "downloadthread.h"

#ifdef MEEGO_EDITION_HARMATTAN
#include "identitymanager.h"
#endif

Controller::Controller(QDeclarativeView *view, QSettings *config, QObject *parent) :
    QObject(parent), m_view(view), m_config(config), m_networkManager(new QNetworkAccessManager()),
    m_networkConfig(new QNetworkConfigurationManager()), m_downloadThread(new DownloadThread())
{
    m_networkManager->setConfiguration(m_networkConfig->defaultConfiguration());
    m_sendNetworkWentOnline = m_networkManager->networkAccessible() != QNetworkAccessManager::Accessible;

    connect(m_networkManager, SIGNAL(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)),
            SLOT(onNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)));

    QFile urlCacheFile(Global::UrlCache());
    if (urlCacheFile.exists() && urlCacheFile.open(QIODevice::ReadOnly)) {
        QDataStream stream(&urlCacheFile);
        stream >> m_urlCache;
        urlCacheFile.close();
    }

    connect(m_downloadThread, SIGNAL(itemDownloaded(QObject *, const QString &)),
            SLOT(itemDownloaded(QObject *, const QString &)));
#ifdef MEEGO_EDITION_HARMATTAN
    m_identityManager = new IdentityManager(this);
    connect(m_identityManager, SIGNAL(noAccount()), SIGNAL(quit()));
    connect(m_identityManager, SIGNAL(credentials(QString, QString)), SLOT(credentials(QString, QString)));
    connect(m_identityManager, SIGNAL(credentialsError(QString)), SLOT(error(QString)));
    m_identityManager->getCredentials();
#endif
}

void Controller::onNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility na)
{
    emit networkAccessibleChanged();
    if (m_sendNetworkWentOnline && na == QNetworkAccessManager::Accessible) {
        m_sendNetworkWentOnline = false;
        networkWentOnline();
    }
}

void Controller::itemDownloaded(QObject *obj, const QString &newUrl)
{
    emit itemDownloadFinished(obj, newUrl);
    m_urlCache[obj->property("id").toInt()] = newUrl;
}

void Controller::save(const QString &dump)
{
    qDebug() << "Controller.save()";
    if (!Global::MyDir().exists()) {
        Global::MyDir().mkpath(".");
    }

    QFile f(cacheFile());
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        // TODO: Error handling
        return;
    }
    f.write(dump.toUtf8());
    f.close();
}

QString Controller::load()
{
    QFile f(cacheFile());
    if (!f.open(QIODevice::ReadWrite)) {
        // TODO: Error handling
        return QString();
    }
    QString dump = QString::fromUtf8(f.readAll());
    f.close();
    return dump;
}

void Controller::downloadItem(QObject *item)
{
    m_downloadThread->queueItem(item);
}

void Controller::openExternal(const QUrl &url)
{
    QDesktopServices::openUrl(url);
}

bool Controller::isNetworkAccessible()
{
    return m_networkManager->networkAccessible() != 0;
}

QVariant Controller::configValue(const QString &name, const QVariant &default_)
{
#ifdef MEEGO_EDITION_HARMATTAN
    // We have credentials from Account Manager on Harmattan
    if (name == "username") {
        return m_username;
    }
    if (name == "password") {
        return m_password;
    }
#endif
    // Set output type according to default value
    if (default_.type() == QVariant::Bool)
        return m_config->value(name, default_).toBool();
    else if (default_.type() == QVariant::Int)
        return m_config->value(name, default_).toInt();
    else
        return m_config->value(name, default_);
}

void Controller::setConfigValue(const QString &name, const QVariant &value)
{
    m_config->setValue(name, value);
}

QString Controller::cacheFile()
{
    return Global::MyDir().absoluteFilePath("cache.json");
}

void Controller::credentials(QString username, QString password)
{
    qDebug() << "Controller::credentials";
    m_username = username;
    m_password = password;
    emit showGUI();
}

bool Controller::isDownloaded(int id)
{
    return Global::CacheDir().exists(QString("%1").arg(id));
}

void Controller::error(const QString &message)
{
    emit showError(message);
}
