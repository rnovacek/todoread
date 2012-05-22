#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QUrl>

#include <QVariant>

class QSettings;
class QDeclarativeView;
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
    explicit Controller(QDeclarativeView *view, QSettings *config, QObject *parent = 0);
signals:
    void showReadChanged();
    void itemDownloadFinished(QObject *item, const QString &newUrl);
    void networkAccessibleChanged();
    void networkWentOnline();
    void showGUI();
    void showError(const QString &message);
    void quit();
public slots:
    void save(const QString &dump);
    QString load();
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
    QDeclarativeView *m_view;
    QSettings *m_config;
    QNetworkAccessManager *m_networkManager;
    QNetworkConfigurationManager *m_networkConfig;
    DownloadThread *m_downloadThread;
    IdentityManager *m_identityManager;
    QString m_username, m_password;
    QHash<int, QString> m_urlCache;
};

#endif // CONTROLLER_H
