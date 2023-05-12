#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QVector>
#include "heartwavecontrol.h"
#include "qcustomplot.h"
#include "loghistory.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {

    Q_OBJECT

    public:

        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

        void displayLogs();


    private:

        QTimer dataTimer;

        Ui::MainWindow *ui;

        // Main Controller
        HeartWaveControl baseItem;

        // Connecters function
        void connectButtons();
        void powerOnOff();

        // Sensors
        int powerLevel;
        bool power;
        bool sensorAttached;

        //grapgh
        void setSelected();
        void setupPlot(QCustomPlot *customPlot);
        void setupStaticLogPlot(QCustomPlot *customPlot, const QVector<double> &xData, const QVector<double> &yData, int achievement_score);
        void getPlotData(QCustomPlot *customPlot, QVector<double> &xData, QVector<double> &yData);
        // Variables
        // Menus
        bool mainMenu;
        QStringList mainMenuItems;

        bool settings;
        QStringList settingItems;

        bool logHistorySelected;
        int logSelected;
        //void getLogHistory();

        LogHistory logHistory;
        bool logDetailsSelected;
        void displayLogDetails(const LogEntry &log);

        bool coherence;
        void coherenceTest(float);
        void sessionEnd();
        //bool session;
        void showSessionSummary();
        void logSession();


        // Setting Sub Menus
        bool setBreath;
        bool setChallenge;
        QStringList setChallenges;
        bool resetDevice;

        void makeMenu();
        void setMenu();
        void losePower(int);
        void breathPace(int);
        void lights();
        //void scoreCoherence();
        void sensorFunction();
        bool sensorTracker;

        bool redLight;
        bool blueLight;
        bool greenLight;

        bool highScoreCheck;
        bool mediumScoreCheck;
        bool lowScoreCheck;

        int timeHighSec;
        int timeMediumSec;
        int timeLowSec;

        QList<float> lowScore;
        QList<float> mediumScore;
        QList<float> highScore;
        QList<float>::iterator lIt;
        QList<float>::iterator mIt;
        QList<float>::iterator hIt;

    private slots:

        void selectButton();
        void upButton();
        void downButton();
        void rightButton();
        void leftButton();
        void backButton();
        void homeButton();
        void powerButton();
        void sensor();
        void runHeartRythm();
        void scoreHigh();
        void scoreMedium();
        void scoreLow();
        void on_logEntrySelected(QListWidgetItem *item);



};
#endif // MAINWINDOW_H
