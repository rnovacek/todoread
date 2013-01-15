/*
 *
 */

#ifndef READDATASOURCE_H
#define READDATASOURCE_H

#include <QtCore/QObject>

class TodoReadModel;
class QSettings;

class ReadDataSource : public QObject
{
    Q_OBJECT
public:
    ReadDataSource(TodoReadModel *model, QSettings *settings, QObject *parent = NULL);
    virtual ~ReadDataSource();
    TodoReadModel *model() { return m_model; }
public slots:
    virtual void sync() = 0;
    virtual bool needsAuth() = 0;
    virtual void auth() = 0;
signals:
    void authDone(bool, QString);
    void syncDone(bool, QString);
protected:
    TodoReadModel *m_model;
    QSettings *m_settings;
};

#endif // READDATASOURCE_H
