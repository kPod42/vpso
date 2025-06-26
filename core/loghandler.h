#pragma once
#include <QLoggingCategory>

void logHandler(QtMsgType type,
                const QMessageLogContext &ctx,
                const QString &msg);
