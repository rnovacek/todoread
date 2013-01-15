#ifndef TESTMODEL_H
#define TESTMODEL_H

#include <QtTest/QtTest>

class TestModel : public QObject
{
    Q_OBJECT
private slots:
    void test();
};

#endif // TESTMODEL_H