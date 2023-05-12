#ifndef HEARTWAVECONTROL_H
#define HEARTWAVECONTROL_H

#include "settings.h"
#include "loghistory.h"
#include "coherence.h"
#include <QStringList>
#include <QString>
#include <QList>

class HeartWaveControl {

    public:

        HeartWaveControl();

        // Gets
        QString getSetChallengeLvl();
        int getChallNum();
        int getSetBreath();
        void setBreath(int);
        void setChallenge(int);
        QStringList getLogHistory();
        int achScore(int,int);
        QString challengeLvl(int,float);

        // LogHistory
        void deleteLog();
        void addLog();

        // Coherence
        int scores();
        int getHeartRate();

    protected:


    private:

        Coherence calculator;
        Settings settor;
        //LogHistory logger;

        // Data
        QList<float> heartRate;
        QList<int> cohereneceScore;
        int timeInLow;
        int timeInMedium;
        int timeInHigh;


};

#endif // HEARTWAVECONTROL_H
