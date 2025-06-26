#include "loghandler.h"
#include "qdir.h"
#include <QFile>
#include <QDateTime>
#include <QStandardPaths>
#include <QTextStream>
#include <QMutex>

void logHandler(QtMsgType type,
                const QMessageLogContext &ctx,
                const QString &msg)
{
    static QMutex mtx;
    QMutexLocker lock(&mtx);

    /* --- ленивое открытие файла один раз ---------------------- */
    static QFile logFile;            // не копируем, просто храним
    if (!logFile.isOpen()) {
        QString dir = QStandardPaths::writableLocation(
            QStandardPaths::AppDataLocation);
        QDir().mkpath(dir);
        logFile.setFileName(dir + "/vpso.log");
        logFile.open(QIODevice::Append | QIODevice::Text);
    }

    QTextStream ts(&logFile);
    ts << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");

    switch (type) {
    case QtDebugMsg:    ts << "DBG "; break;
    case QtInfoMsg:     ts << "INF "; break;
    case QtWarningMsg:  ts << "WRN "; break;
    case QtCriticalMsg: ts << "CRT "; break;
    case QtFatalMsg:    ts << "FTL "; break;
    }
    ts << msg;
#ifdef QT_DEBUG
    ts << " (" << ctx.file << ':' << ctx.line << ')';
#endif
    ts << '\n';
    ts.flush();

    if (type == QtFatalMsg)
        abort();                 // crash как обычно
}
