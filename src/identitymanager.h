#ifndef IDENTITYMANAGER_H
#define IDENTITYMANAGER_H

#include <QObject>

namespace SignOn {
class AuthService;
class AuthSession;
class Error;
class Identity;
class IdentityInfo;
class SessionData;
}

namespace Accounts {
class Manager;
class Account;
}

namespace AccountSetup {
class ProviderPluginProxy;
}

class IdentityManager : public QObject
{
    Q_OBJECT
public:
    explicit IdentityManager(QObject *parent = 0);
    
signals:
    /** Valid credentials found
     * \param username Username for readitlater service
     * \param password Password for given username
     */
    void credentials(const QString &username, const QString &password);

    /** No account was found and creating new was cancelled
     */
    void noAccount();

    /** Checking the credentials failed
     * \param message Error message
     */
    void credentialsError(const QString &message);
public slots:
    void getCredentials();
private slots:
    void error(const SignOn::Error &);
    void identityResponse(const SignOn::SessionData &data);
    void accountCreated();
    void accountFound();
private:
    enum State { STARTED, GET_ACCOUNT, GET_PASSWORD, DONE };
    State state;
    bool firstError;
    Accounts::Manager *m_manager;
    AccountSetup::ProviderPluginProxy *m_proxy;
    SignOn::AuthSession *m_session;
    Accounts::Account *m_account;
};

#endif // IDENTITYMANAGER_H
