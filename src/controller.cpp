#include "controller.h"

#include "globals.h"

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QSettings>
#include <QtCore/QSignalMapper>
#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeItem>
#include <QtDeclarative/QDeclarativeProperty>
#include <QtDeclarative/QDeclarativeContext>
#include <QtGui/QMessageBox>
#include <QtGui/QProgressDialog>
#include <QtGui/QDesktopServices>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkConfigurationManager>

#include "downloadthread.h"
#include "todoreadmodel.h"
#include "readdatasource.h"
#include "readitlater.h"

#ifdef TODOREAD_HARMATTAN
#include "identitymanager.h"
#endif

Controller::Controller(QDeclarativeContext *context, QSettings *config, QObject *parent) :
    QObject(parent), m_context(context), m_config(config), m_networkManager(new QNetworkAccessManager()),
    m_networkConfig(new QNetworkConfigurationManager()), m_downloadThread(new DownloadThread()),
    m_model(new TodoReadModel(this)),
    m_dataSource(new ReadItLater(m_model, config, this)) // TODO: let user select data source
{;
    m_networkManager->setConfiguration(m_networkConfig->defaultConfiguration());
    m_sendNetworkWentOnline = m_networkManager->networkAccessible() != QNetworkAccessManager::Accessible;

    connect(m_networkManager, SIGNAL(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)),
            SLOT(onNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)));

    load();

    connect(m_downloadThread, SIGNAL(itemDownloaded(QObject *, const QString &)),
            SLOT(itemDownloaded(QObject *, const QString &)));
#ifdef TODOREAD_HARMATTAN
    m_identityManager = new IdentityManager(this);
    connect(m_identityManager, SIGNAL(noAccount()), SIGNAL(quit()));
    connect(m_identityManager, SIGNAL(credentials(QString, QString)), SLOT(credentials(QString, QString)));
    connect(m_identityManager, SIGNAL(credentialsError(QString)), SLOT(error(QString)));
    m_identityManager->getCredentials();
#endif
}

Controller::~Controller()
{
    qDebug() << "Controller::~Controller";
}


void Controller::reloadModel()
{
    m_dataSource->sync();
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

void Controller::save()
{
    qDebug() << "Controller.save()" << m_model->rowCount();
    m_model->save(cacheFile());
}

void Controller::load()
{
    qDebug() << "Controller.load()";
    m_model->load(cacheFile());
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
#ifdef TODOREAD_HARMATTAN
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
    if (!Global::MyDir().exists()) {
        Global::MyDir().mkpath(".");
    }

    return Global::MyDir().absoluteFilePath("cache.json");
}

QString Controller::getDownloadedUrl(int id)
{
    QHash<int, QString>::const_iterator it = m_urlCache.find(id);
    if (it != m_urlCache.end()) {
        return it.value();
    } else {
        return QString();
    }
}

void Controller::credentials(QString username, QString password)
{
    qDebug() << "Controller::credentials";
    m_username = username;
    m_password = password;
    emit showGUI();
}


void Controller::error(const QString &message)
{
    emit showError(message);
}
