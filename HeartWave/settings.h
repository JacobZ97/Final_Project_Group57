#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QStringList>
#include <iostream>

class Settings {

    public:

        Settings();

        // Gets and Sets
        QString getChallengeLvl(); // matchs int with a Qstring
        int getChallNum();
        int getBreathPacer();
        void setChallengeLvl(int);
        void setBreathPacer(int);

    protected:

    private:

        int breathPacer;
        int challengeLvl;
        QStringList levels;



};

#endif // SETTINGS_H
