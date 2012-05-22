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
    delete m_config;
    delete m_controller;
}

void TodoRead::showGUI()
{
    // Main window
    m_view->showExpanded();
}

void TodoRead::aboutToQuitHandler()
{
    m_config->sync();
}
