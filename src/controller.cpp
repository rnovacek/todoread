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
    QObject(parent), m_view(view), m_config(config), m_downloadThread(new DownloadThread())
{
    connect(m_downloadThread, SIGNAL(itemDownloaded(QObject *, const QString &)),
            SIGNAL(itemDownloadFinished(QObject *, const QString &)));
#ifdef MEEGO_EDITION_HARMATTAN
    m_identityManager = new IdentityManager(this);
    connect(m_identityManager, SIGNAL(noAccount()), SIGNAL(quit()));
    connect(m_identityManager, SIGNAL(credentials(QString, QString)), SLOT(credentials(QString, QString)));
    m_identityManager->getCredentials();
#endif
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

bool Controller::isConfigured()
{
    return m_config->contains("username");
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

QUrl Controller::getIcon(QString urlString)
{
    QUrl url(urlString);
    QString iconPath = QString("%1.png").arg(url.host());
    QString absPath = Global::CacheDir().absoluteFilePath(iconPath);
    if (!Global::CacheDir().exists(iconPath)) {
        QIcon icon = QWebSettings::iconForUrl(url);
        QPixmap pixmap = icon.pixmap(32, 32);
        pixmap.save(absPath);
    }
    qDebug() << "getIcon: " << absPath;
    return QUrl::fromLocalFile(absPath);
}
