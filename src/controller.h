#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QUrl>

#include <QVariant>

class ReadDataSource;
class TodoReadModel;
class QSettings;
class QDeclarativeContext;
class QNetworkConfigurationManager;

#include <QtNetwork/QNetworkAccessManager>

class DownloadThread;
class IdentityManager;

class Controller : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool networkAccessible READ isNetworkAccessible NOTIFY networkAccessibleChanged)
    // Only changed when network goes from offline to online state
    Q_PROPERTY(bool networkOnline READ isNetworkAccessible NOTIFY networkWentOnline)
public:
    explicit Controller(QDeclarativeContext *context, QSettings *config, QObject *parent = 0);
    virtual ~Controller();
    TodoReadModel *model() { return m_model; }
signals:
    void showReadChanged();
    void itemDownloadFinished(QObject *item, const QString &newUrl);
    void networkAccessibleChanged();
    void networkWentOnline();
    void showGUI();
    void showError(const QString &message);
    void quit();
public slots:
    void reloadModel();
    void save();
    void load();
    void credentials(QString, QString);
    void downloadItem(QObject *item);
    void itemDownloaded(QObject *, const QString &);

    void openExternal(const QUrl &url);

    QVariant configValue(const QString &name, const QVariant &_default);
    void setConfigValue(const QString &name, const QVariant &value);

    QString cacheFile();

    bool isNetworkAccessible();
    void onNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility na);
    QString getDownloadedUrl(int id);

    void error(const QString &message);
private:
    bool m_sendNetworkWentOnline;
    QDeclarativeContext *m_context;
    QSettings *m_config;
    QNetworkAccessManager *m_networkManager;
    QNetworkConfigurationManager *m_networkConfig;
    DownloadThread *m_downloadThread;
    IdentityManager *m_identityManager;
    QString m_username, m_password;
    QHash<int, QString> m_urlCache;
    TodoReadModel *m_model;
    ReadDataSource *m_dataSource;
};

#endif // CONTROLLER_H
