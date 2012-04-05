#include "todoread.h"

#include <QDeclarativeView>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QSettings>
#include <QDir>
#include <QDebug>
#include <QtOpenGL/QGLWidget>

#include "qmlapplicationviewer.h"
#include "globals.h"

TodoRead::TodoRead(int &argc, char **argv):
    QApplication(argc, argv)
{
    // TODO: what does it do?
    //setProperty("NoMStyle", true);

    // Declarative view
    m_view = new QmlApplicationViewer();

    connect(m_view->engine(), SIGNAL(quit()), SLOT(quit()));

    // Settings
    m_config = new QSettings(Global::ConfigFile());
    m_view->rootContext()->setContextProperty("config", m_config);

    // Controller
    m_controller = new Controller(m_view, m_config);
    m_view->rootContext()->setContextProperty("controller", m_controller);

    m_view->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    // Set qml source
    QDir dir;
    dir.cdUp();
    dir.cd("todoread");
    dir.cd("qml");
    if (dir.exists("main.qml")) {
        m_view->setMainQmlFile(QString::fromLatin1("qml/main.qml"));
    } else {
        m_view->setMainQmlFile(QString::fromLatin1("/opt/todoread/qml/main.qml"));
    }
    qDebug() << "TodoRead errors:" << m_view->errors();

    // Main window
    m_view->showExpanded();

    connect(this, SIGNAL(aboutToQuit()), SLOT(aboutToQuitHandler()));
}

TodoRead::~TodoRead()
{
    delete m_view;
    delete m_config;
    delete m_controller;
}

void TodoRead::aboutToQuitHandler()
{
    m_config->sync();
}
