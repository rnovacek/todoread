#ifndef READITLATERAUTH_H
#define READITLATERAUTH_H

#include <QString>
#include <WebUpload/AuthBase>

class ReadItLaterAuth : public WebUpload::AuthBase
{
public:
    ReadItLaterAuth(QObject *parent = 0);
    virtual void handleResponse (const SignOn::SessionData &sessionData);
    QString username() { return m_username; }
    QString password() { return m_password; }
protected:
    virtual bool isAuthRequired();
    virtual WebUpload::AuthData getAuthData();
private:
    QString m_username, m_password;
};

#endif // READITLATERAUTH_H
