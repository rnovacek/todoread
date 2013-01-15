/*
 *
 */

#ifndef READITLATER_H
#define READITLATER_H

#include <QtCore/QObject>
#include <QtCore/QAbstractItemModel>
#include "readdatasource.h"

class TodoReadModel;
class QSettings;
class QNetworkAccessManager;
class QNetworkReply;
class QTcpServer;

class ReadItLater : public ReadDataSource
{
    Q_OBJECT
public:
    ReadItLater(TodoReadModel *model, QSettings *settings, QObject *parent = NULL);
    virtual ~ReadItLater();
public slots:
    virtual void sync();
    virtual bool needsAuth();
    virtual void auth();
private slots:
    void authorize();
    void serverConnection();
    void networkReply(QNetworkReply*);
    void parseDownloaded(const QString &data);
    void get();
private:
    QNetworkAccessManager *m_networkAccessManager;
    QTcpServer *m_server;
    QString m_requestToken;
};

#endif // READITLATER_H
