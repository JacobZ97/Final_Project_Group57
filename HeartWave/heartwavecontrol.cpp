#include "heartwavecontrol.h"

HeartWaveControl::HeartWaveControl() : settor(Settings()), calculator(Coherence()) {

}

QString HeartWaveControl::getSetChallengeLvl() {

    return this->settor.getChallengeLvl();

}

int HeartWaveControl::getChallNum() {

    return this->settor.getChallNum();

}

int HeartWaveControl::getSetBreath() {

    return this->settor.getBreathPacer();

}

void HeartWaveControl::setBreath(int newPacer) {

    this->settor.setBreathPacer(newPacer);

}

void HeartWaveControl::setChallenge(int newLvl) {

    this->settor.setChallengeLvl(newLvl);

}

int HeartWaveControl::achScore(int one, int two) {

    return this->calculator.achScore(one,two);

}

QString HeartWaveControl::challengeLvl(int chLvl, float cohScore) {

    return this->calculator.challengeLvl(chLvl, cohScore);

}
