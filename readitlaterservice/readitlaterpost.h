#ifndef READITLATERPOST_H
#define READITLATERPOST_H

#include <WebUpload/PostBase>

namespace SignOn {
class AuthSession;
class SessionData;
}

class QNetworkReply;
class ReadItLaterAuth;

class ReadItLaterPost : public WebUpload::PostBase
{
    Q_OBJECT
public:
    ReadItLaterPost(QObject *parent = 0);
    virtual WebUpload::AuthBase *getAuthPtr();

public Q_SLOTS:
    virtual void stopMediaUpload();
protected Q_SLOTS:
    virtual void uploadMedia (WebUpload::Media * media);
    void uploadFinished(QNetworkReply *reply);
private:
    ReadItLaterAuth *m_authBase;
    QString m_username, m_password;
};

#endif // READITLATERPOST_H
