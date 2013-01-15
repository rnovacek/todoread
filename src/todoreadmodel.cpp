/*
 *
 */

#include "todoreadmodel.h"

#include "todoreaditem.h"
#include "json.h"

#include <QtCore/QDebug>
#include <QFile>

TodoReadModel::TodoReadModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

TodoReadModel::~TodoReadModel()
{
    qDebug() << "TodoReadModel::~TodoReadModel";
}

QVariant TodoReadModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    TodoReadItem *item = m_indexes.at(index.row());
    switch (index.column()) {
        case 0:
            return item->id();
        case 1:
            return item->title();
        case 2:
            return item->url();
        case 3:
            return item->status();
        default:
            qWarning() << "No such index: " << index.column();
            return QVariant();
    }
}

int TodoReadModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

int TodoReadModel::rowCount(const QModelIndex &parent) const
{
    return m_indexes.size();
}

TodoReadItem *TodoReadModel::objectAt(int index) const
{
    if (index >= m_indexes.size()) {
        return NULL;
    }
    return m_indexes.at(index);
}

TodoReadItem *TodoReadModel::get(const QString &id) const
{
    m_data.value(id, NULL);
}

QList<TodoReadItem *> TodoReadModel::all() const
{
    return m_data.values();
}

QList<TodoReadItem *> TodoReadModel::updated(int since) const
{
    QList< TodoReadItem *> list;
    foreach (TodoReadItem *item, m_data) {
        if (item->updated() > since) {
            list << item;
        }
    }
    return list;
}

void TodoReadModel::addItem(TodoReadItem* item)
{
    beginInsertRows(QModelIndex(), m_indexes.size(), m_indexes.size());
    item->setParent(this);
    m_indexes << item;
    m_data.insert(item->id(), item);
    endInsertRows();
}

void TodoReadModel::deleteItem(const QString& id)
{
    QList<TodoReadItem *>::iterator it;
    int i = 0;
    for (it = m_indexes.begin(); it < m_indexes.end(); ++it) {
        if ((*it)->id() == id) {
            beginRemoveRows(QModelIndex(), i, i);
            m_indexes.erase(it);
            endRemoveRows();
        }
        i++;
    }
}

void TodoReadModel::clear()
{
    m_indexes.clear();
    m_data.clear();
    emit layoutChanged();
}

bool TodoReadModel::load(QString fileName)
{
    QFile f(fileName);
    if (!f.open(QIODevice::ReadOnly)) {
        qDebug() << "Unable to open cache file" << f.errorString();
        return false;
    }
    QByteArray data(f.readAll());
    f.close();
    bool success = false;
    QVariant v = QtJson::parse(QString::fromUtf8(data), success);
    if (!success) {
        qDebug() << "Unable to parse cache file";
        return false;
    }
    QList<TodoReadItem> items;
    foreach (QVariant item, v.toMap()) {
        TodoReadItem *it = TodoReadItem::fromVariant(item.toMap(), this);
        m_data.insert(it->id(), it);
        m_indexes.append(it);
    }
    emit layoutChanged();
    return true;
}

bool TodoReadModel::save(QString fileName)
{
    QVariantMap map;
    foreach (TodoReadItem *item, m_data) {
        map.insert(item->id(), item->toVariant());
    }
    bool status = false;
    QByteArray data(QtJson::serialize(map, status));
    if (!status) {
        qDebug() << "Unable to serialize model";
        return false;
    }
    QFile f(fileName);
    if (!f.open(QIODevice::WriteOnly)) {
        qDebug() << "Unable to open cache file";
        return false;
    }
    if (f.write(data) != data.size()) {
        qDebug() << "Unable to write cache";
        return false;
    }
    f.close();
    return true;
}
