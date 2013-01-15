/*
 *
 */

#ifndef TODOREADMODEL_H
#define TODOREADMODEL_H

#include <QtCore/QAbstractTableModel>

class TodoReadItem;

class TodoReadModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    TodoReadModel(QObject *parent = 0);
    ~TodoReadModel();
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual TodoReadItem *objectAt(int index) const;
    virtual TodoReadItem *get(const QString &id) const;
    virtual QList<TodoReadItem *> all() const;
    virtual QList<TodoReadItem *> updated(int since) const;
    virtual void addItem(TodoReadItem *item);
    virtual void deleteItem(const QString &id);
    bool save(QString fileName);
    bool load(QString fileName);
public slots:
    void clear();
protected:
    QHash<QString, TodoReadItem *> m_data;
    QList<TodoReadItem *> m_indexes;
};

#endif // TODOREADMODEL_H
