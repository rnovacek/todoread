/*
 *
 */

#include "readdatasource.h"
#include "todoreadmodel.h"

ReadDataSource::ReadDataSource(TodoReadModel* model, QSettings* settings, QObject* parent) :
    QObject(parent), m_model(model), m_settings(settings)
{

}

ReadDataSource::~ReadDataSource()
{

}
