#include "downloadthread.h"

#include <QDebug>
#include <QMutex>
#include <QProcess>
#include <QUrl>
#include <QVariant>

#include "globals.h"

DownloadThread::DownloadThread(QObject *parent) :
    QThread(parent), m_mutex(new QMutex()), m_process(NULL)
{
    m_downloadProgram = "wget";
    m_downloadArgs << "-nv" << "--page-requisites" << "--html-extension" << "--convert-links"
                   << "--restrict-file-names=unix" << "--no-parent";

}

void DownloadThread::queueItem(QObject *item)
{
    m_mutex->lock();
    m_queue.append(item);
    m_mutex->unlock();

    if (!isRunning()) {
        start();
    }
}

void DownloadThread::run()
{
    if (m_process == NULL) {
        m_process = new QProcess();
    }
    QObject *item;

    while (true) {
        if (m_queue.size() == 0) {
            return;
        }
        m_mutex->lock();
        item = m_queue.takeFirst();
        m_mutex->unlock();

        QUrl url(item->property("origUrl").toString());
        QString id(item->property("id").toString());

        QDir dir(Global::CacheDir());
        dir.mkdir(id);
        dir.cd(id);

        QStringList args = m_downloadArgs;
        args << QString("--domains=%1").arg(url.host())
             << QString("--directory-prefix=%1").arg(dir.absolutePath())
             << item->property("origUrl").toString();

        m_process->start(m_downloadProgram, args);


        QString newUrl = QString("file://%1/%2/%3/%4").arg(Global::CacheDir().absolutePath()).arg(id).arg(url.host()).arg(url.path());
        if (url.path().endsWith('/')) {
            newUrl.append("index.html");
        } else if (!url.path().endsWith(".html")) {
            newUrl.append(".html");
        }
        qDebug() << "new url: " << newUrl << " from: " << url;

        if (m_process->waitForFinished(30000)) {
            emit itemDownloaded(item, newUrl);
        } else {
            emit itemDownloadFailed(item);
        }
    }
}

int DownloadThread::remaining()
{
    return m_queue.size();
}

void DownloadThread::clearQueue()
{
    m_mutex->lock();
    m_queue.clear();
    m_mutex->unlock();
}
