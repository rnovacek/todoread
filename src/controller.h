#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QUrl>

#include <QVariant>

namespace SignOn {
class AuthService;
class Error;
class IdentityInfo;
}

class QSettings;
class QDeclarativeView;

class DownloadThread;

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QDeclarativeView *view, QSettings *config, QObject *parent = 0);
    bool isConfigured();
signals:
    void showReadChanged();
    void itemDownloadFinished(QObject *item, const QString &newUrl);
public slots:
    void save(const QString &dump);
    QString load();
    void getCredentials(QString &username, QString &password) const;
    void downloadItem(QObject *item);

    void openExternal(const QUrl &url);

    QVariant configValue(const QString &name, const QVariant &_default);
    void setConfigValue(const QString &name, const QVariant &value);

    QString cacheFile();
private slots:
    void signOnError(const SignOn::Error &);
    void signOnIdentities(const QList<SignOn::IdentityInfo> &);
private:
    QDeclarativeView *m_view;
    QSettings *m_config;
    DownloadThread *m_downloadThread;
    SignOn::AuthService *m_authService;
};

#endif // CONTROLLER_H
