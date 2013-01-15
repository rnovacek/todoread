/*
 *
 */

#ifndef BBREADMODEL_H
#define BBREADMODEL_H

#include "todoreadmodel.h"
#include <bb/cascades/GroupDataModel>
using namespace bb::cascades;

class BBReadModel : public GroupDataModel
{
    Q_OBJECT
public:
    BBReadModel(TodoReadModel *model);
private:
    TodoReadModel *m_model;
private slots:
    void onLayoutChanged();
    void onRowsInserted(QModelIndex,int,int);
    void onRowsRemoved(QModelIndex,int,int);
};

#endif // BBREADMODEL_H
