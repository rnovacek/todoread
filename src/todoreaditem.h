#ifndef TODOREADITEM_H
#define TODOREADITEM_H

#include <QtCore/QObject>
#include "globals.h"

class TodoReadItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(Status status READ status WRITE setStatus NOTIFY statusChanged)
    Q_ENUMS(Status);
public:
    enum Status { NEW, READ, DELETED };
    static Status StatusFromInt(int);
    static int StatusToInt(Status);

    TodoReadItem(QString id, QString title=QString(), QString url=QString(), Status status=NEW, qint64 updated=Global::Now(), QObject *parent=NULL);
//     TodoReadItem(const TodoReadItem &item, QObject *parent=NULL);
    //TodoReadItem &operator=(const TodoReadItem &);

    QString id() const { return m_id; }
    QString title() const { return m_title; }
    void setTitle(QString title) { m_title = title; emit titleChanged(m_title); }
    QString url() const { return m_url; }
    void setUrl(QString url) { m_url = url; emit urlChanged(m_url); }
    Status status() const { return m_status; }
    void setStatus(Status status);
    qint64 updated() const { return m_updated; }
    QVariant toVariant() const;
    static TodoReadItem *fromVariant(QVariant v, QObject *parent=NULL);
public slots:
    void markAsRead();
    void unmarkAsRead();
    void markAsDeleted();
    void unmarkAsDeleted();
signals:
    void titleChanged(QString);
    void urlChanged(QString);
    void statusChanged(Status);
private:
    QString m_id, m_title, m_url;
    Status m_status;
    qint64 m_updated;
};

#endif // TODOREADITEM_H
