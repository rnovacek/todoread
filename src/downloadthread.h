#ifndef DOWNLOADTHREAD_H
#define DOWNLOADTHREAD_H

#include <QThread>
#include <QStringList>

class QMutex;
class QProcess;

class DownloadThread : public QThread
{
    Q_OBJECT
public:
    explicit DownloadThread(QObject *parent = 0);
    int remaining();
signals:
    void itemDownloaded(QObject *, const QString &);
    void itemDownloadFailed(QObject *);
public slots:
    virtual void run();
    void queueItem(QObject *item);
    void clearQueue();
private:
    QMutex *m_mutex;
    QList<QObject *> m_queue;
    QProcess *m_process;
    QString m_downloadProgram;
    QStringList m_downloadArgs;
};

#endif // DOWNLOADTHREAD_H
