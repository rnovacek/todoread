#include "identitymanager.h"

#include <QDebug>

#include <SignOn/AuthService>
#include <SignOn/AuthSession>
#include <SignOn/Error>
#include <SignOn/Identity>

#include <AccountSetup/ProviderPluginProxy>
#include <Accounts/Manager>
#include <Accounts/Account>

IdentityManager::IdentityManager(QObject *parent) :
    QObject(parent), state(STARTED), firstError(false), m_manager(new Accounts::Manager(this))
{
    m_proxy = new AccountSetup::ProviderPluginProxy(this);
    connect(m_proxy, SIGNAL(finished()), SLOT(accountCreated()));
}

void IdentityManager::getCredentials()
{
    state = GET_ACCOUNT;
    Accounts::AccountIdList list = m_manager->accountListEnabled("sharing");

    // Try to find existing account
    int j;
    for (int i = 0; i < list.size(); ++i) {
        m_account = m_manager->account(list[i]);
        qDebug() << "Account: " << m_account->allKeys();

        Accounts::ServiceList serviceList = m_account->enabledServices();
        for (j = 0; j < serviceList.size(); ++j) {
            qDebug() << "\tService: " << serviceList[j]->name();
            if (serviceList[j]->name() == "readitlater") {
                accountFound();
                return;
            }
        }
    }
    // Not found, creating new one
    qDebug() << "Account was not found, create a new one";
    m_proxy->createAccount(m_manager->provider("readitlater"), "sharing");
}

void IdentityManager::accountCreated()
{
    if (!m_proxy->accountCreated()) {
        qDebug() << "creating account cancelled";
        emit noAccount();
    } else {
        qDebug() << "new account created";
        m_account = m_manager->account(m_proxy->createdAccountId());
        accountFound();
    }
}

void IdentityManager::accountFound()
{
    state = GET_PASSWORD;
    quint32 id = m_account->valueAsInt("CredentialsId", 0);
    qDebug() << "Process account #" << id;
    if (id == 0) {
        id = m_account->credentialsId();
    }
    SignOn::Identity *identity = SignOn::Identity::existingIdentity(id, this);
    connect(identity, SIGNAL(error(SignOn::Error)), SLOT(error(SignOn::Error)));

    if (identity) {
        qDebug() << "Identity was obtained: " << identity;
        m_session = identity->createSession(QLatin1String("readitlater"));
        connect(m_session, SIGNAL(response(SignOn::SessionData)), SLOT(identityResponse(SignOn::SessionData)));
        connect(m_session, SIGNAL(error(SignOn::Error)), SLOT(error(SignOn::Error)));
        m_session->process(SignOn::SessionData(), QLatin1String("GetPassword"));
    } else {
        qDebug() << "No SSO identity for the account " << m_account->credentialsId();
        m_proxy->editAccount(m_account, "sharing");
    }
}

void IdentityManager::identityResponse(const SignOn::SessionData &data)
{
    state = DONE;
    if (!data.propertyNames().contains("SecretPassword")) {
        qDebug() << "Identity is missing secret";
        m_proxy->editAccount(m_account, "sharing");
    } else {
        qDebug() << "Got secret credentials";
        emit credentials(data.getProperty("UserName").toString(), data.getProperty("SecretPassword").toString());
    }
}

void IdentityManager::error(const SignOn::Error &error)
{
    qDebug() << "IdentityManager::error: " << error.message() << "(#" << error.type() << ")";
    if (!firstError) {
        // We haven't get any error yet, retry
        firstError = true;
        if (m_account)
            m_proxy->editAccount(m_account, "sharing");
        return;
    }
    emit credentialsError(error.message());
}
