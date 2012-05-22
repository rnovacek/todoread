#ifndef TEST_H
#define TEST_H

#include <QObject>
#include <SignOn/SessionData>
#include <SignOn/Identity>
#include <SignOn/AuthService>
#include <SignOn/Error>

class Test : public QObject
{
    Q_OBJECT
public:
    Test();
public slots:
    void result(const SignOn::SessionData &data);
    void error(const SignOn::Error &error);
    void methodsAvailable(const QStringList &);
    void mechanismsAvailable(const QString &, const QStringList &);
    void identities(const QList<SignOn::IdentityInfo> &);
private:
    SignOn::AuthService *service;
};

#endif // TEST_H
