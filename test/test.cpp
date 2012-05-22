
#include "test.h"

#include "readitlaterprovider.h"

#include <QCoreApplication>
#include <QDebug>
#include <QVariantMap>

#include <iostream>

#include <SignOn/Identity>
#include <SignOn/AuthService>

Test::Test() : service(new SignOn::AuthService())
{
    std::cerr << "starting" << std::endl;
    /*
    ReadItLaterPlugin *ril = new ReadItLaterPlugin();
    connect(ril, SIGNAL(result(const SignOn::SessionData &)), SLOT(result(const SignOn::SessionData &)));
    connect(ril, SIGNAL(error(const SignOn::Error &)), SLOT(error(const SignOn::Error &)));
    QVariantMap map;
    map.insert("username", QVariant(""));
    map.insert("password", QVariant(""));
    SignOn::SessionData data(map);
    ril->process(data, "ReadItLater");
    */

    connect(service, SIGNAL(error(const SignOn::Error &)), SLOT(error(const SignOn::Error &)));
    connect(service, SIGNAL(methodsAvailable(const QStringList &)), SLOT(methodsAvailable(const QStringList &)));
    connect(service, SIGNAL(mechanismsAvailable(const QString &, const QStringList &)), SLOT(mechanismsAvailable(const QString &, const QStringList &)));
    connect(service, SIGNAL(identities(const QList<SignOn::IdentityInfo> &)), SLOT(identities(const QList< SignOn::IdentityInfo> &)));

//    service->queryMethods();
    service->queryIdentities();
}

void Test::methodsAvailable(const QStringList &methods)
{
    qDebug() << "Methods: " << methods;
    for (int i = 0; i < methods.size(); i++) {
        service->queryMechanisms(methods.at(i));
    }
}

void Test::mechanismsAvailable(const QString &method, const QStringList &mechanisms)
{
    qDebug() << "Method: " << method << " Mechanisms: " << mechanisms;
}

void Test::identities(const QList< SignOn::IdentityInfo> &ids)
{
    for (int i = 0; i < ids.size(); i++) {
        qDebug() << "Identity: username: " << ids.at(i).userName();
    }
}


void Test::result(const SignOn::SessionData &data)
{
    std::cerr << "Result" << std::endl;
}

void Test::error(const SignOn::Error &error)
{
    std::cerr << error.type() << " " << error.message().toAscii().constData() << std::endl;
}


int main(int argc, char *argv[])
{
    std::cerr << "main" << std::endl;
    QCoreApplication app(argc, argv);
    Test m;


    /*
    Accounts::Manager manager("sharing");
    Accounts::ServiceList services = manager.serviceList();
    qDebug() << "AccountList: " << manager.accountList();
    qDebug() << "AccountListEnabled: " << manager.accountListEnabled();
    qDebug() << "AccountList: " << manager.accountList("readitlater");
    qDebug() << "AccountListEnabled: " << manager.accountListEnabled("readitlater");

    for (int i = 0; i < services.count(); i++) {
        qDebug() << "Service: " << services.at(i)->name();
    }
    Accounts::ProviderList providers = manager.providerList();
    for (int i = 0; i < providers.count(); i++) {
        qDebug() << "Providers: " << providers.at(i)->name();
    }

    qDebug() << "Done";
    */

    return app.exec();
}
