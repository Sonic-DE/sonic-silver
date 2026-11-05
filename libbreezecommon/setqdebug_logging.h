#pragma once

#include "QDebug"
#include "iostream"
#include <QDateTime>
#include <QDir>
#include <QTextStream>
#include <cstdio>

namespace Breeze
{

using namespace std;
void setDebugOutput(const QString &rawTargetFilePath_)
{
    static QString rawTargetFilePath;
    rawTargetFilePath = rawTargetFilePath_;
    class HelperClass
    {
    public:
        static void messageHandler(QtMsgType type, const QMessageLogContext &, const QString &message_)
        {
            QString message;

            switch (type) {
            case QtDebugMsg: {
                message = message_;
                break;
            }
            case QtWarningMsg: {
                message.append("Warning: ");
                message.append(message_);
                break;
            }
            case QtCriticalMsg: {
                message.append("Critical: ");
                message.append(message_);
                break;
            }
            case QtFatalMsg: {
                message.append("Fatal: ");
                message.append(message_);
                break;
            }
            default: {
                break;
            }
            }

            QString currentTargetFilePath;
            currentTargetFilePath = QDir::homePath() + rawTargetFilePath;
            if (!QFileInfo::exists(currentTargetFilePath)) {
                QDir().mkpath(QFileInfo(currentTargetFilePath).path());
            }

            // Same limit and name as Sonic Logging's rotation.
            constexpr qint64 rotationSizeBytes = 2097152;
            if (QFileInfo(currentTargetFilePath).size() > rotationSizeBytes) {
                std::rename(QFile::encodeName(currentTargetFilePath).constData(), QFile::encodeName(currentTargetFilePath + QStringLiteral(".last")).constData());
            }

            QFile file(currentTargetFilePath);
            file.open(QIODevice::WriteOnly | QIODevice::Append);

            QTextStream textStream(&file);
            textStream << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz") << ": " << message << Qt::endl;
        }
    };
    qInstallMessageHandler(HelperClass::messageHandler);
}
}
