#ifndef TODOREAD_H
#define TODOREAD_H

#include <QtCore/QObject>
#include <QtDeclarative/QDeclarativeContext>

class QmlApplicationViewer;
class QSettings;
class QGLWidget;
class Controller;

class TodoRead : public QObject
{
    Q_OBJECT
public:
    explicit TodoRead(QObject *parent=0);
public slots:
    void showGUI();
private slots:
    void aboutToQuitHandler();
private:
    QDeclarativeContext *rootContext;
    QSettings *m_config;
    Controller *m_controller;
    QGLWidget *m_glw;
};

#endif // TODOREAD_H
