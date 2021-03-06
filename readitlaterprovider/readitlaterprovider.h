#ifndef READITLATERPROVIDER_H
#define READITLATERPROVIDER_H

#include <SignOn/AuthPluginInterface>

class QNetworkAccessManager;

class ReadItLaterSessionData : public SignOn::SessionData
{
public:
    SIGNON_SESSION_DECLARE_PROPERTY(QString, SecretPassword);
};


class ReadItLaterPlugin : public AuthPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(AuthPluginInterface)

public:
    ReadItLaterPlugin(QObject *parent = 0);
    virtual ~ReadItLaterPlugin();

public Q_SLOTS:
    QString type() const;
    QStringList mechanisms() const;

    void cancel();
    void abort();

    void process(const SignOn::SessionData &inData, const QString &mechanism = QString());
    void authenticationFinished();

private:
    QNetworkAccessManager *m_networkManager;
    ReadItLaterSessionData m_data;
};

#endif // READITLATERPROVIDER_H
