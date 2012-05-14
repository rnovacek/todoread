#include <QtGui/QApplication>
#include "qmlapplicationviewer/qmlapplicationviewer.h"

#include "todoread.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    TodoRead app(argc, argv);
    return app.exec();
}
