#include "settings.h"

Settings::Settings(): challengeLvl(1), breathPacer(10),
    levels(QStringList({"Beginner", "Normal", "Adpet" , "Advanced"})){

}

QString Settings::getChallengeLvl() {

    return this->levels[challengeLvl - 1];

}

int Settings::getChallNum() {

    return this->challengeLvl;

}

int Settings::getBreathPacer() {

    return this->breathPacer;

}

void Settings::setChallengeLvl(int newLvl) {

    this->challengeLvl = newLvl;

    std::cout << "New Challenge Lvl: " << this->challengeLvl << std::endl;

}

void Settings::setBreathPacer(int newPacer) {

    this->breathPacer = newPacer;

    std::cout << "New Breath Pacer: " << this->breathPacer << std::endl;

}
