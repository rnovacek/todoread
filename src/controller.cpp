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
#include <QUrl>

#include <SignOn/AuthService>

#include "globals.h"
#include "downloadthread.h"

Controller::Controller(QDeclarativeView *view, QSettings *config, QObject *parent) :
    QObject(parent), m_view(view), m_config(config), m_downloadThread(new DownloadThread()),
    m_authService(new SignOn::AuthService(this))
{   
    if (!isConfigured()) {
        // TODO: fix it
        //m_view->rootObject()->showLogin();
    }

    connect(m_authService, SIGNAL(error(const SignOn::Error &)), SLOT(signOnError(const SignOn::Error &)));
    connect(m_authService, SIGNAL(identities(const QList<SignOn::IdentityInfo> &)), SLOT(signOnIdentities(const QList<SignOn::IdentityInfo> &)));
    m_authService->queryIdentities();

    // TODO: reading file
    /*
    m_markedFile = QFile(Global::MyDir().absoluteFilePath("marked.json"));
    m_markedFile.open(QIODevice::ReadOnly);

    m_markedAsRead = loadFrom(m_markedFile);
    m_markedFile.close();
    */

    connect(m_downloadThread, SIGNAL(itemDownloaded(QObject *, const QString &)),
            SIGNAL(itemDownloadFinished(QObject *, const QString &)));
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


void Controller::getCredentials(QString &username, QString &password) const
{
}

bool Controller::isConfigured()
{
    return m_config->contains("username");
}

QVariant Controller::configValue(const QString &name, const QVariant &default_)
{
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

void Controller::signOnError(const SignOn::Error &error)
{
    qDebug() << "Listing identites failed with error: " << error.message() << "(#" << error.type() << ")";
}

void Controller::signOnIdentities(const QList<SignOn::IdentityInfo> &identities)
{

    for (int i = 0; i < identities.size(); i++) {
        qDebug() << "Identity: username: " << identities.at(i).userName() <<
                    " realms: " << identities.at(i).realms() <<
                    " owner: " << identities.at(i).owner() <<
                    " acl: " << identities.at(i).accessControlList();
    }
}

