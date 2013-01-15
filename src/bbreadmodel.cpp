/*
 *
 */

#include "bbreadmodel.h"
#include "todoreaditem.h"

BBReadModel::BBReadModel(TodoReadModel* model) :
    GroupDataModel(), m_model(model)
{
    qDebug() << "BBReadModel::BBReadModel" << model;
    setSortingKeys(QStringList() << "id");
    setGrouping(ItemGrouping::None);
    connect(m_model, SIGNAL(layoutChanged()), SLOT(onLayoutChanged()));
    connect(m_model, SIGNAL(rowsInserted(QModelIndex,int,int)), SLOT(onRowsInserted(QModelIndex,int,int)));
    connect(m_model, SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)), SLOT(onRowsRemoved(QModelIndex,int,int)));
    onLayoutChanged();
}

void BBReadModel::onLayoutChanged()
{
    qDebug() << "BBReadModel::onLayoutChanged" << m_model->rowCount();
    clear();
    TodoReadItem *item;
    for (int i = 0; i < m_model->rowCount(); ++i) {
        item = m_model->objectAt(i);
        if (item == NULL) {
            qWarning() << "NULL item in model: " << i;
            continue;
        }
        insert(item);
    }
}

void BBReadModel::onRowsInserted(QModelIndex parent, int start, int end)
{
    TodoReadItem *item;
    for (int i = start; i <= end; ++i) {
        item = m_model->objectAt(i);
        if (item == NULL) {
            continue;
        }
        insert(item);
    }
}

void BBReadModel::onRowsRemoved(QModelIndex parent, int start, int end)
{
    TodoReadItem *item;
    for (int i = start; i <= end; ++i) {
        item = m_model->objectAt(i);
        if (item != NULL) {
            remove(item);
        }
    }
}

