#include "todoread.h"

#include <QDeclarativeView>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QSettings>
#include <QDir>
#include <QDebug>
#include <QtOpenGL/QGLWidget>
#include <qplatformdefs.h>

#include "qmlapplicationviewer.h"
#include "globals.h"

TodoRead::TodoRead(int &argc, char **argv):
    QApplication(argc, argv)
{
    QGLFormat format = QGLFormat::defaultFormat();
    format.setSampleBuffers(false);
    QGLWidget *glWidget = new QGLWidget(format);
    // Comment the following line if you get display problems
    // (usually when the top-level element is an Item and not a Rectangle)
    glWidget->setAutoFillBackground(false);

    // Declarative view
    m_view = new QmlApplicationViewer();
    m_view->setViewport(glWidget);

    connect(m_view->engine(), SIGNAL(quit()), SLOT(quit()));

    // Settings
    m_config = new QSettings(Global::ConfigFile(), QSettings::IniFormat, this);
    m_view->rootContext()->setContextProperty("config", m_config);

    // Controller
    m_controller = new Controller(m_view, m_config, this);
    connect(m_controller, SIGNAL(quit()), SLOT(quit()));
    connect(m_controller, SIGNAL(showGUI()), SLOT(showGUI()));
    m_view->rootContext()->setContextProperty("controller", m_controller);

    m_view->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);

    // Set qml source    
    QDir dir(__FILE__);
    dir.cdUp();
    dir.cdUp();
    dir.cd("qml");
    qDebug() << dir.absolutePath();
    if (dir.exists("main.qml")) {
        m_view->setMainQmlFile(dir.absoluteFilePath("main.qml"));
    } else {
        m_view->setMainQmlFile(QString::fromLatin1("/opt/todoread/qml/main.qml"));
    }
    qDebug() << "TodoRead errors:" << m_view->errors();

    connect(this, SIGNAL(aboutToQuit()), SLOT(aboutToQuitHandler()));

#ifdef MEEGO_EDITION_HARMATTAN
#warning Meego
#else
#warning Desktop
    // On harmattan platform, GUI will be shown after checking if some account exists
    qDebug() << "Showing GUI imediatelly";
    showGUI();
#endif
}

TodoRead::~TodoRead()
{
    delete m_view;
}

void TodoRead::showGUI()
{
    // Main window
    m_view->showFullScreen();
}

void TodoRead::aboutToQuitHandler()
{
    m_config->sync();
}
