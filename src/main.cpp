
#ifdef TODOREAD_CASCADES
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
using namespace bb::cascades;
#else
#include <QtGui/QApplication>
#endif

#include <QtCore/QSettings>
#include <QtCore/QDebug>

#include "todoread.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
#ifdef TODOREAD_CASCADES
    Application app(argc, argv);
    app.setAutoExit(false);
#else
    QApplication app(argc, argv);
#endif
    TodoRead todoRead;
#ifdef TODOREAD_CASCADES
    QObject::connect(&app, SIGNAL(manualExit()), &todoRead, SLOT(aboutToQuitHandler()));
#else
    QObject::connect(&app, SIGNAL(lastWindowClosed()), &todoRead, SLOT(aboutToQuitHandler()));
#endif
    return app.exec();
}
