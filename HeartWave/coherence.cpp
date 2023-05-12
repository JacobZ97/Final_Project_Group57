#include "coherence.h"

Coherence::Coherence() {

}

int Coherence::coherenceScore(float hz, int someInt){


}

int Coherence::achScore(int one, int two) {

    return one + two;

}

QString Coherence::challengeLvl(int chLvl, float cohScore) {

    std::cout << chLvl << " " << cohScore << std::endl;

    if (chLvl == 1) {

        if (0 < cohScore & cohScore <= 0.5) {

            return QString("Low");

        } else if (0.5 < cohScore & cohScore<= 0.9) {

            return QString("Medium");

        } else if (0.9 < cohScore) {

            return QString("High");

        }

    } else if (chLvl == 2) {

        if (0 < cohScore & cohScore <= 0.6) {

            return QString("Low");

        } else if (0.6 < cohScore & cohScore <= 2.1) {

            return QString("Medium");

        } else if (2.1 < cohScore) {

            return QString("High");

        }

    } else if (chLvl == 3) {

        if (0 < cohScore & cohScore <= 1.8) {

            return QString("Low");

        } else if (1.8 < cohScore & cohScore <= 4.0) {

            return QString("Medium");

        } else if (4.0 < cohScore) {

            return QString("High");

        }

    } else if (chLvl == 4) {

        if (0.0 < cohScore & cohScore <= 4.0) {

            return QString("Low");

        } else if (4.0 < cohScore & cohScore <= 6.0) {

            return QString("Medium");

        } else if (6.0 < cohScore ) {

            return QString("High");

        }


    }


}

