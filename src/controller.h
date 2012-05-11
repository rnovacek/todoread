#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QUrl>

#include <QVariant>

class QSettings;
class QDeclarativeView;

class DownloadThread;
class IdentityManager;

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QDeclarativeView *view, QSettings *config, QObject *parent = 0);
    bool isConfigured();
signals:
    void showReadChanged();
    void itemDownloadFinished(QObject *item, const QString &newUrl);
    void showGUI();
    void showError(const QString &message);
    void quit();
public slots:
    void save(const QString &dump);
    QString load();
    void credentials(QString, QString);
    void downloadItem(QObject *item);

    void openExternal(const QUrl &url);

    QVariant configValue(const QString &name, const QVariant &_default);
    void setConfigValue(const QString &name, const QVariant &value);

    QString cacheFile();

    bool isDownloaded(int id);
    void error(const QString &message);
private:
    QDeclarativeView *m_view;
    QSettings *m_config;
    DownloadThread *m_downloadThread;
    IdentityManager *m_identityManager;
    QString m_username, m_password;
};

#endif // CONTROLLER_H
