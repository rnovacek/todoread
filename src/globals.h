#ifndef GLOBALS_H
#define GLOBALS_H

#include <QtCore/QDir>
#include <QtCore/QDateTime>

class Global
{
public:
    static const QDir MyDir()
    {
        QDir dir = QDir::home();
        if (!dir.cd(".todoread")) {
            dir.mkdir(".todoread");
            dir.cd(".todoread");
        }
        return dir;
    }

    static const QDir CacheDir()
    {
        QDir dir = Global::MyDir();
        dir.cd("cache");
        return dir;
    }

    static const QString ConfigFile()
    {
        return MyDir().absoluteFilePath("config.ini");
    }

    static const QString UrlCache()
    {
        return MyDir().absoluteFilePath("urlcache.dat");
    }

    static qint64 Now()
    {
        return QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000;
    }
};

#endif // GLOBALS_H
