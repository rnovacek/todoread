#include <QtGui/QApplication>
#include "qmlapplicationviewer/qmlapplicationviewer.h"

#include "todoread.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    TodoRead app(argc, argv);
    // TODO: fullscreen on mobile
    //app.view.showFullScreen()
    return app.exec();
}
