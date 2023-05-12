#ifndef LOGHISTORY_H
#define LOGHISTORY_H

#include <QDateTime>
#include <QVector>
#include <QJsonArray>

struct CoherenceData {
    QString level;
    int time;
};

struct LogEntry {
    QDateTime time;
    int length;
    QVector<CoherenceData> coherence;
    double average_coherence;
    int achievement_score;
    QVector<double> plotXData;
    QVector<double> plotYData;
};

class LogHistory
{
public:
    QVector<QDateTime> getTimestamps() const;
    LogHistory();
    void loadLogs(const QJsonArray &jsonArray);
    QVector<LogEntry> getLogs() const;
    void addLogEntry(const LogEntry &entry);
    void resetLog();
    void deleteLog(int);

private:
    QVector<LogEntry> logs;
};

#endif // LOGHISTORY_H
