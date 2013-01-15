
#include "test.h"

#include <QtTest/QTest>
#include "../src/readitlater.h"
#include "../src/todoreadmodel.h"

void TestPocket::testAuth()
{
    QTemporaryFile f;
    f.open();
    f.close();
    QSettings settings(f.fileName());
    ReadItLater *readitlater = new ReadItLater(&settings);
    QVERIFY(readitlater->needsAuth() == true);
    QSignalSpy spy(readitlater, SIGNAL(authDone(bool,QString)));
    readitlater->auth();
    while (spy.count() == 0) {
        QTest::qWait(250);
    }
    QVERIFY(spy.at(0).at(0).toBool());

    QSignalSpy spy2(readitlater, SIGNAL(downloadDone(bool, QString)));
    readitlater->download();
    while (spy2.count() == 0) {
        QTest::qWait(250);
    }
    QVERIFY(spy2.at(0).at(0).toBool());

    qDebug() << "Model: " << readitlater->model()->rowCount() << "x" << readitlater->model()->columnCount();
    for (int i = 0; i < readitlater->model()->rowCount(); ++i) {
        for (int j = 0; j < readitlater->model()->columnCount(); ++j) {
            qDebug() << readitlater->model()->index(i, j).data();
        }
    }
}

QTEST_MAIN(TestPocket)
