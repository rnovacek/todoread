#include "todoread.h"

#include "globals.h"
#include "controller.h"

#ifdef TODOREAD_CASCADES
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/NavigationPane>
using namespace bb::cascades;
#include "bbreadmodel.h"
#elif TODOREAD_QTQUICK
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeContext>
#include "qmlapplicationviewer.h"
#else
#include <QtCore/QCoreApplication>
#include <QtGui/QTableView>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#endif
#include <QtCore/QSettings>
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/QGLFormat>
#include <QtGui/QMessageBox>

#include "todoreadmodel.h"

TodoRead::TodoRead(QObject *parent):
    QObject(parent), rootContext(NULL)
{
#ifdef TODOREAD_CASCADES
    QmlDocument *qml = QmlDocument::create("asset:///main.qml");
    if (qml->hasErrors()) {
        QString err;
        foreach (const QDeclarativeError e, qml->errors()) {
            err += e.toString();
        }
        QMessageBox::critical(NULL, "error", err);
        return;
    }
    rootContext = qml->documentContext();
#elif TODOREAD_QTQUICK
    QGLFormat format = QGLFormat::defaultFormat();
    format.setSampleBuffers(false);
    QGLWidget *glWidget = new QGLWidget(format);
    // Comment the following line if you get display problems
    // (usually when the top-level element is an Item and not a Rectangle)
    glWidget->setAutoFillBackground(false);

    // Declarative view
    QmlApplicationViewer *m_view = new QmlApplicationViewer();
    m_view->setViewport(glWidget);
    m_view->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    rootContext = m_view->rootContext();

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

    // On harmattan platform, GUI will be shown after checking if some account exists

    connect(QCoreApplication::instance(), SIGNAL(aboutToQuit()), SLOT(aboutToQuitHandler()));
#else
    connect(QCoreApplication::instance(), SIGNAL(aboutToQuit()), SLOT(aboutToQuitHandler()));
#endif
    // Settings
    m_config = new QSettings(Global::ConfigFile(), QSettings::IniFormat, this);

    // Controller
    m_controller = new Controller(rootContext, m_config, this);
    connect(m_controller, SIGNAL(showGUI()), SLOT(showGUI()));

#ifdef TODOREAD_CASCADES
    rootContext->setContextProperty("bbReadModel", new BBReadModel(m_controller->model()));
    rootContext->setContextProperty("config", m_config);
    rootContext->setContextProperty("controller", m_controller);
    NavigationPane *navPane = qml->createRootObject<NavigationPane>();
    if (navPane) {
        Application::instance()->setScene(navPane);
    }
    showGUI();
#elif TODOREAD_QTQUICK
    rootContext->setContextProperty("config", m_config);
    rootContext->setContextProperty("controller", m_controller);
#else
    QWidget *w = new QWidget();
    QToolBar *bar = new QToolBar(w);
    bar->addAction("Reload", m_controller, SLOT(reloadModel()));
    QTableView *table = new QTableView(w);
    table->setModel(m_controller->model());
    QVBoxLayout *l = new QVBoxLayout();
    l->addWidget(bar);
    l->addWidget(table);
    w->setLayout(l);
    w->show();
#endif
}

void TodoRead::showGUI()
{
    // Main window
#ifdef TODOREAD_QTQUICK
    m_view->showFullScreen();
#else
    //m_view->showNormal();
#endif
}

void TodoRead::aboutToQuitHandler()
{
    m_controller->save();
#ifdef TODOREAD_CASCADES
    Application::instance()->quit();
#endif
}
