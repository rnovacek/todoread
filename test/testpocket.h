#ifndef TESTPOCKET_H
#define TESTPOCKET_H

#include <QtTest/QtTest>

class TestPocket : public QObject
{
    Q_OBJECT
private slots:
    void testAuth();
};

#endif // TESTPOCKET_H
