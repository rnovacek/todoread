#ifndef TODOREAD_H
#define TODOREAD_H

#include <QtGui/QApplication>

#include "controller.h"

class QmlApplicationViewer;
class QSettings;
class QGLWidget;

class TodoRead : public QApplication
{
    Q_OBJECT
public:
    explicit TodoRead(int &argc, char **argv);
    virtual ~TodoRead();
    
signals:
    
public slots:
    void aboutToQuitHandler();
    
private:
    QmlApplicationViewer *m_view;
    QSettings *m_config;
    Controller *m_controller;
    QGLWidget *m_glw;
};

#endif // TODOREAD_H
