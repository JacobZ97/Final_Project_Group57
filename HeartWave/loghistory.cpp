#include "loghistory.h"
#include <QJsonObject>
#include <QDebug>

LogHistory::LogHistory()
{
}

void LogHistory::loadLogs(const QJsonArray &jsonArray)
{
    logs.clear();

    for (const QJsonValue &value : jsonArray) {
        if (value.isObject()) {
            QJsonObject logObject = value.toObject();
            if (logObject.contains("time") && logObject["time"].isString()
                && logObject.contains("length") && logObject["length"].isDouble()
                && logObject.contains("coherence") && logObject["coherence"].isArray()
                && logObject.contains("average_coherence") && logObject["average_coherence"].isDouble()
                && logObject.contains("achievement_score") && logObject["achievement_score"].isDouble()) {

                LogEntry log;
                log.time = QDateTime::fromString(logObject["time"].toString(), Qt::ISODate);
                log.length = logObject["length"].toInt();
                log.average_coherence = logObject["average_coherence"].toDouble();
                log.achievement_score = logObject["achievement_score"].toInt();

                QJsonArray coherenceArray = logObject["coherence"].toArray();
                for (const QJsonValue &coherenceValue : coherenceArray) {
                    if (coherenceValue.isObject()) {
                        QJsonObject coherenceObject = coherenceValue.toObject();
                        if (coherenceObject.contains("level") && coherenceObject["level"].isString()
                            && coherenceObject.contains("time") && coherenceObject["time"].isDouble()) {

                            CoherenceData coherenceData;
                            coherenceData.level = coherenceObject["level"].toString();
                            coherenceData.time = coherenceObject["time"].toInt();
                            log.coherence.append(coherenceData);
                        }
                    }
                }

                if (logObject.contains("plotXData") && logObject["plotXData"].isArray()
                    && logObject.contains("plotYData") && logObject["plotYData"].isArray()) {

                    QJsonArray plotXDataArray = logObject["plotXData"].toArray();
                    for (const QJsonValue &xDataValue : plotXDataArray) {
                        if (xDataValue.isDouble()) {
                            log.plotXData.append(xDataValue.toDouble());
                        }
                    }

                    QJsonArray plotYDataArray = logObject["plotYData"].toArray();
                    for (const QJsonValue &yDataValue : plotYDataArray) {
                        if (yDataValue.isDouble()) {
                            log.plotYData.append(yDataValue.toDouble());
                        }
                    }
                }


                logs.append(log);

                // Output the log to the console
                qDebug() << "Time:" << log.time.toString("yyyy-MM-dd hh:mm:ss")
                         << "Length:" << log.length
                         << "Average Coherence:" << log.average_coherence
                         << "Achievement Score:" << log.achievement_score;
            }
        }
    }
}

QVector<LogEntry> LogHistory::getLogs() const
{
    return logs;
}

QVector<QDateTime> LogHistory::getTimestamps() const {
    QVector<QDateTime> timestamps;
    for (const LogEntry &log : logs) {
        timestamps.append(log.time);
    }
    return timestamps;
}

void LogHistory::addLogEntry(const LogEntry &entry) {

    logs.append(entry);
}

void LogHistory::resetLog(){

    logs.clear();

}

void LogHistory::deleteLog(int log) {

    logs.removeAt(log);

}
