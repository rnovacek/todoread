
#include "todoreaditem.h"
#include "globals.h"
#include <QtCore/QVariant>

TodoReadItem::TodoReadItem(QString id, QString title, QString url, Status status, qint64 updated, QObject *parent) :
    QObject(parent), m_id(id), m_title(title), m_url(url), m_status(status), m_updated(updated)
{
}

/*
TodoReadItem::TodoReadItem(const TodoReadItem& item, QObject *parent) :
    QObject(parent), m_id(item.id()), m_title(item.title()), m_url(item.url()),
    m_status(item.status())
{

}
*/

TodoReadItem::Status TodoReadItem::StatusFromInt(int i)
{
    return (Status) i;
}

int TodoReadItem::StatusToInt(TodoReadItem::Status status)
{
    return (int) status;
}

TodoReadItem *TodoReadItem::fromVariant(QVariant v, QObject *parent)
{
    QVariantMap map = v.toMap();
    Status status = StatusFromInt(map.value("status").toInt());
    TodoReadItem *item = new TodoReadItem(
        map.value("id").toString(),
        map.value("title").toString(),
        map.value("url").toString(),
        status, map.value("updated").toInt(), parent);
    item->m_updated = map.value("updated", QVariant::fromValue<int>(0)).toInt();
    return item;
}

QVariant TodoReadItem::toVariant() const
{
    QVariantMap map;
    map.insert("id", m_id);
    map.insert("title", m_title);
    map.insert("url", m_url);
    map.insert("status", StatusToInt(m_status));
    map.insert("updated", m_updated);
    return map;
}

void TodoReadItem::setStatus(TodoReadItem::Status status)
{
    m_status = status;
    emit statusChanged(m_status);
}

void TodoReadItem::markAsRead()
{
    m_updated = Global::Now();
    setStatus(READ);
}

void TodoReadItem::unmarkAsRead()
{
    m_updated = Global::Now();
    setStatus(NEW);
}

void TodoReadItem::markAsDeleted()
{
    m_updated = Global::Now();
    setStatus(DELETED);
}

void TodoReadItem::unmarkAsDeleted()
{
    m_updated = Global::Now();
    setStatus(NEW);
}
