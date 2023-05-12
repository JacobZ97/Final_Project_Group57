#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include "loghistory.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow),
    power(false), sensorAttached(false), mainMenu(true), settings(false), logHistorySelected(false), coherence(false),
    redLight(false), blueLight(false), greenLight(false), sensorTracker(false), highScoreCheck(true),
    mediumScoreCheck(false), lowScoreCheck(false), timeHighSec(0), timeMediumSec(0), logDetailsSelected(false), logSelected(0), timeLowSec(0),
    mainMenuItems(QStringList({"New Session", "Settings", "Log/History"})),
    settingItems(QStringList({"Challenge Level", "Breath Pacer", "Device Reset"})),
    setChallenges(QStringList({"Beginner", "Normal", "Adpet" ,"Advanced"})),
    baseItem(HeartWaveControl()),
    lowScore(QList<float>({0.1,0.2,0.3,0.1,0.1,0.2,0.2,0.2,0.3,0.4})),
    mediumScore(QList<float>({1.0,1.5,1.8,2.4,3.1,1.9,1.5,2.4,3.1,2.9})),
    highScore(QList<float>({7.0,16,15,9.2,14.1,12.7,12.0,9.3,10.9,11.1})) {


    ui->setupUi(this);

    ui->sensorConnect->setVisible(false);

    connectButtons();
    powerOnOff();

    // iterators
    lIt = lowScore.begin();
    mIt = mediumScore.begin();
    hIt = highScore.begin();

    ui->highBox->toggle();

    ui->customPlot->setVisible(false);


}

MainWindow::~MainWindow()
{
    delete ui;


}

// Connecters
void MainWindow::connectButtons() {

    // Connecters
    connect(ui->SELECT_button, SIGNAL(released()), this, SLOT(selectButton()));
    connect(ui->UP_button, SIGNAL(released()), this, SLOT(upButton()));
    connect(ui->DOWN_button, SIGNAL(released()), this, SLOT(downButton()));
    connect(ui->BackButton, SIGNAL(released()), this, SLOT(backButton()));
    connect(ui->homeButton, SIGNAL(released()), this, SLOT(homeButton()));
    connect(ui->POWER_button, SIGNAL(released()), this, SLOT(powerButton()));
    connect(ui->SensorVar, SIGNAL(clicked()), this, SLOT(sensor()));
    connect(ui->highBox, SIGNAL(clicked()), this, SLOT(scoreHigh()));
    connect(ui->mediumBox, SIGNAL(clicked()), this, SLOT(scoreMedium()));
    connect(ui->lowBox, SIGNAL(clicked()), this, SLOT(scoreLow()));




}

// Start
void MainWindow::powerOnOff() {

    if (ui->BatteryVar->value() > 5) {

        ui->PowerBar->setValue(ui->BatteryVar->value());
        this->powerLevel = ui->BatteryVar->value();

    }

    if (this->power){

        ui->Main_Screen->setEnabled(true);
        makeMenu();

        ui->SELECT_button->setEnabled(true);
        ui->UP_button->setEnabled(true);
        ui->DOWN_button->setEnabled(true);
        ui->RIGHT_button->setEnabled(true);
        ui->LEFT_button->setEnabled(true);
        ui->BackButton->setEnabled(true);
        ui->homeButton->setEnabled(true);
        ui->POWER_button->setEnabled(true);
        ui->PowerBar->setEnabled(true);
        ui->redLight->setEnabled(true);
        ui->blueLight->setEnabled(true);
        ui->greenLight->setEnabled(true);
        ui->coherenceScore->setEnabled(true);
        ui->timeCounter->setEnabled(true);
        ui->achScore->setEnabled(true);

        // Read the JSON file and load the logs into logHistory
        QFile jsonFile("/home/student/Desktop/Team-57-Project-main/HeartWave/log_data.json"); // location of the json file
        if (jsonFile.open(QIODevice::ReadOnly)) {
            QByteArray jsonData = jsonFile.readAll();
            jsonFile.close();

            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
            if (jsonDoc.isObject()) {
                QJsonObject jsonObj = jsonDoc.object();
                if (jsonObj.contains("logs") && jsonObj["logs"].isArray()) {
                    logHistory.loadLogs(jsonObj["logs"].toArray());
                }
            }
        }


    } else {

        ui->Main_Screen->setEnabled(false);
        ui->Main_Screen->clear();
        ui->UP_button->setEnabled(false);
        ui->DOWN_button->setEnabled(false);
        ui->RIGHT_button->setEnabled(false);
        ui->LEFT_button->setEnabled(false);
        ui->BackButton->setEnabled(false);
        ui->homeButton->setEnabled(false);
        ui->SELECT_button->setEnabled(false);
        ui->PowerBar->setEnabled(false);
        ui->redLight->setEnabled(false);
        ui->blueLight->setEnabled(false);
        ui->greenLight->setEnabled(false);
        ui->coherenceScore->setEnabled(false);
        ui->timeCounter->setEnabled(false);
        ui->achScore->setEnabled(false);

        ui->customPlot->setVisible(false);


    }

}

// Slots
void MainWindow::selectButton() {

    std::cout << ui->Main_Screen->currentRow() << std::endl;
    if (this->mainMenu == true) {

        if (this->mainMenuItems[ui->Main_Screen->currentRow()] == QString("New Session") && this->sensorAttached == true) {

            qInfo("Nothing for now");
            this->coherence = true;
            this->mainMenu = false;

            //For Testing comment out later
            //ui->Main_Screen->clear();
            //ui->Main_Screen->addItems({"TEST","Coherence"});
            //ui->Main_Screen->setCurrentRow(0);

            // Function for start the coherence test
            // coherenceTest();
            ui->customPlot->setVisible(true);
            setupPlot(ui->customPlot);


        } else if (this->mainMenuItems[ui->Main_Screen->currentRow()] == QString("Settings")) {

            this->settings = true;
            this->mainMenu = false;
            setMenu();

        } else if (this->mainMenuItems[ui->Main_Screen->currentRow()] == QString("Log/History")){

            this->logHistorySelected = true;
            this->mainMenu = false;

            // Clear the Main_Screen widget and fill it with log entry timestamps
            ui->Main_Screen->clear();
            QVector<QDateTime> timestamps = logHistory.getTimestamps();

            for (const QDateTime &timestamp : timestamps) {
                QString logEntry = timestamp.toString("yyyy-MM-dd hh:mm:ss");
                ui->Main_Screen->addItem(logEntry);
            }
            ui->Main_Screen->setCurrentRow(0);


        } else if (this->logHistorySelected == true) {

            int selectedLogIndex = ui->Main_Screen->currentRow();
            LogEntry selectedLog = logHistory.getLogs()[selectedLogIndex];

            ui->Main_Screen->clear();
            ui->Main_Screen->addItem(QString("Length: %1").arg(QString::number(selectedLog.length)));
            ui->Main_Screen->addItem(QString("Average Coherence: %1").arg(QString::number(selectedLog.average_coherence)));
            ui->Main_Screen->addItem(QString("Achievement Score: %1").arg(QString::number(selectedLog.achievement_score)));

            this->logDetailsSelected = true;
            this->logHistorySelected = false;

        } else {

            qInfo("Nothing on main menu");

        }

    } else if (this->settings == true) {

        if (this->settingItems[ui->Main_Screen->currentRow()] == QString("Challenge Level")) {

            this->setChallenge = true;
            this->settings = false;
            ui->Main_Screen->clear();
            ui->Main_Screen->addItems(this->setChallenges);
            ui->Main_Screen->setCurrentRow(0);

        } else if (this->settingItems[ui->Main_Screen->currentRow()] == QString("Breath Pacer")) {

            this->setBreath = true;
            this->settings = false;
            ui->Main_Screen->clear();

            for (int i = 0; i < 30; i++ ){

                int j = i + 1;

                ui->Main_Screen->addItems({QString("%1").arg(j)});

            }

            ui->Main_Screen->setCurrentRow(0);

        } else if (this->settingItems[ui->Main_Screen->currentRow()] == QString("Device Reset")) {

            this->resetDevice = true;
            this->settings = false;

            this->baseItem.setBreath(10);
            this->baseItem.setChallenge(1);

            ui->Main_Screen->clear();
            ui->Main_Screen->addItems({"Deivce reset", "Press Select to continue"});
            ui->Main_Screen->setCurrentRow(0);

        } else {

            qInfo("Nothing on settings");

        }



    } else if (this->logHistorySelected == true) {

        int selectedLogIndex = ui->Main_Screen->currentRow();
        LogEntry selectedLog = logHistory.getLogs()[selectedLogIndex];

        this->logSelected = selectedLogIndex;

        displayLogDetails(selectedLog);

    } else if (this->coherence == true) {

        qInfo("End Session");

        this->coherence = false;

        // For Testing
        homeButton();

        // Collect data and show summary page
        logSession();
        showSessionSummary();

        // Reset Page
        ui->customPlot->setVisible(false);
        ui->redLight->setText(" ");
        ui->blueLight->setText(" ");
        ui->greenLight->setText(" ");
        ui->coherenceScore->setValue(0);
        ui->timeCounter->setValue(0);
        ui->achScore->setValue(0);
        ui->breathPacer->setValue(0);

        this->timeHighSec = 0;
        this->timeMediumSec = 0;
        this->timeLowSec = 0;


    } else if (this->setBreath == true) {

        // function to set breath setting
        this->baseItem.setBreath(ui->Main_Screen->currentRow() + 1);

        // Show
        //qInfo(QString(this->baseItem.getSetBreath()));

        // back to settings
        backButton();

    } else if (this->setChallenge == true) {

        // function to set challenge setting
        this->baseItem.setChallenge(ui->Main_Screen->currentRow() + 1);

        // Show
        //qInfo(this->baseItem.getSetChallengeLvl());

        // back to settings
        backButton();

    } else if (this->resetDevice == true) {

        // functions to reset device
        // ui->Main_Screen->currentRow()
        this->settings = true;
        this->resetDevice = false;

        // Reset the log history
        logHistory.resetLog();

        homeButton();

    } else if (this->logDetailsSelected == true && ui->Main_Screen->currentRow() == 4) {

        logHistory.deleteLog(this->logSelected);

        backButton();

    }

}

void MainWindow::upButton() {

    ui->Main_Screen->setCurrentRow(ui->Main_Screen->currentRow() - 1);

}

void MainWindow::downButton() {

    ui->Main_Screen->setCurrentRow(ui->Main_Screen->currentRow() + 1);

}

void MainWindow::rightButton() {

    qInfo("right");

}

void MainWindow::leftButton() {

    qInfo("left");

}

void MainWindow::backButton() {

    if (this->mainMenu == true){

        qInfo("Now where to go back to");
        makeMenu();

    } else if (this->settings == true) {

        this->mainMenu = true;
        this->settings = false;
        makeMenu();

    } else if (this->logHistorySelected == true) {

        this->mainMenu = true;
        this->logHistorySelected = false;
        makeMenu();


    } else if (this->setBreath == true) {

        this->settings = true;
        this->setBreath = false;
        setMenu();


    } else if (this->setChallenge == true) {

        this->settings = true;
        this->setChallenge = false;
        setMenu();

    } else if (this->resetDevice == true) {

        this->settings = true;
        this->setChallenge = false;
        setMenu();

    }

    else if (this->logDetailsSelected == true) {

        this->logHistorySelected = true;
        this->logDetailsSelected = false;

        // Hide the graph
        ui->customPlot->setVisible(false);

        // Clear the Main_Screen widget and fill it with log entry timestamps
        ui->Main_Screen->clear();
        QVector<QDateTime> timestamps = logHistory.getTimestamps();

        for (const QDateTime &timestamp : timestamps) {
            QString logEntry = timestamp.toString("yyyy-MM-dd hh:mm:ss");
            ui->Main_Screen->addItem(logEntry);
        }
        ui->Main_Screen->setCurrentRow(0);

    } else {

        qInfo("Can't back out of Coherence Session!!!. Press Select to stop the session");
        this->mainMenu = true;
        makeMenu();

    }

}

void MainWindow::homeButton() {

    if (this->coherence == true) {

        qInfo("Must end session by pressing select");

    } else {

        this->mainMenu = true;
        this->settings = false;
        this->logHistorySelected = false;
        this->logDetailsSelected = false;
        this->coherence  = false;

        ui->customPlot->setVisible(false);

        makeMenu();

    }

}

void MainWindow::powerButton() {

    if (this->power) {

        this->power = false;
        powerOnOff();

    } else {

        if (ui->BatteryVar->value() > 5) {

            this->power = true;
            powerOnOff();

        } else {

            qInfo("Need more power to turn on");

        }

    }

}

void MainWindow::sensor() {

    if (this->sensorAttached) {

        this->sensorAttached = false;
        ui->sensorConnect->setVisible(false);

    } else {

        this->sensorAttached = true;
        ui->sensorConnect->setVisible(true);

    }

    std::cout << this->sensorAttached << std::endl;

}

void MainWindow::scoreHigh() {

    if (this->highScoreCheck == true) {

        this->highScoreCheck = false;

    } else {

        this->highScoreCheck = true;

    }

}

void MainWindow::scoreMedium() {

    if (this->mediumScoreCheck == true) {

        this->mediumScoreCheck = false;

    } else {

        this->mediumScoreCheck = true;

    }

}

void MainWindow::scoreLow() {

    if (this->lowScoreCheck == true) {

        this->lowScoreCheck = false;

    } else {

        this->lowScoreCheck = true;

    }

}

// Other Functions
void MainWindow::makeMenu() {

    ui->Main_Screen->clear();
    ui->Main_Screen->addItems(this->mainMenuItems);
    ui->Main_Screen->setCurrentRow(0);

}

void MainWindow::setMenu() {

    ui->Main_Screen->clear();
    ui->Main_Screen->addItems(this->settingItems);
    ui->Main_Screen->setCurrentRow(0);

}

void MainWindow::losePower(int lose) {

    this->powerLevel = this->powerLevel - lose;
    ui->PowerBar->setValue(this->powerLevel);

    if (this->powerLevel < 5) {

        selectButton();

    }

}

void MainWindow::breathPace(int sec) {




}

void MainWindow::lights() {

    // Reset all lights to the off state
    ui->redLight->setStyleSheet("QLabel { background-color: white; border: 1px solid black; }");
    ui->blueLight->setStyleSheet("QLabel { background-color: white; border: 1px solid black; }");
    ui->greenLight->setStyleSheet("QLabel { background-color: white; border: 1px solid black; }");

    // Turn on the appropriate light
    if (this->redLight == true){
        ui->redLight->setStyleSheet("QLabel { background-color: red; border: 1px solid black; }");
    } else if (this->blueLight == true) {
        ui->blueLight->setStyleSheet("QLabel { background-color: blue; border: 1px solid black; }");
    } else if (this->greenLight == true) {
        ui->greenLight->setStyleSheet("QLabel { background-color: green; border: 1px solid black; }");
    }
}



void MainWindow::setupPlot(QCustomPlot *customPlot) {

    setWindowTitle("Heart Wave");
    statusBar()->clearMessage();
    customPlot->clearGraphs();

    customPlot->replot();

    customPlot->setNotAntialiasedElements(QCP::aeAll);
    //set font for x, y axis and legend
    QFont font;
    font.setStyleStrategy(QFont::NoAntialias);
    customPlot->xAxis->setTickLabelFont(font);
    customPlot->yAxis->setTickLabelFont(font);
    customPlot->legend->setFont(font);

    // add new gragh blue line
    customPlot->addGraph();
    customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));

    // add new gragh red line
    customPlot->addGraph();
    customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    if (this->highScoreCheck == true || this->mediumScoreCheck == true){

        customPlot->graph(1)->setVisible(false);

    } else if (this->lowScoreCheck == true) {

        customPlot->graph(0)->setVisible(false);

    }


    QSharedPointer<QCPAxisTickerFixed> timeTicker(new QCPAxisTickerFixed);
    timeTicker->setTickStep(5);
    customPlot->xAxis->setTicker(timeTicker);
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->yAxis->setRange(50, 100);


    // make left and bottom axes transfer their ranges to right and top axes:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(runHeartRythm()));
    dataTimer.start(100); // Interval 0 means to refresh as fast as possible  // Affects FPS

}

void MainWindow::runHeartRythm() {

    if (this->coherence == true) {

        static QTime timeStart = QTime::currentTime();
        static QTime newFive = QTime::currentTime();
        static QTime breath = QTime::currentTime();

        // calculate two new data points:
        double key = timeStart.msecsTo(QTime::currentTime())/1000.0; // time elapsed since start in seconds
        static double lastPointKey = 0;

        // update coherence and other things
        double five = newFive.msecsTo(QTime::currentTime())/1000.0;
        static double sec = 0;

        // breath Pacer;
        double newPace = breath.msecsTo(QTime::currentTime())/1000.0;
        static double pacer = 0;

        //


        if (key - lastPointKey > 0.002) //add point every 2 ms (0.002)
        {
            // add data to lines:
            ui->customPlot->graph(0)->addData(key, (72.5 + 17.5 * qSin(key)));
            ui->customPlot->graph(1)->addData(key, 72.5 + 16*qCos(key)+std::rand()/(double)RAND_MAX*7*qSin(key/0.001));
            // rescale value (vertical) axis to fit the current data:
            ui->customPlot->graph(0)->rescaleValueAxis(true);
            ui->customPlot->graph(1)->rescaleValueAxis(true);
            lastPointKey = key;

            // Other Stuff
            //std::cout << (fi) << std::endl;

            if (newPace - pacer > (((baseItem.getSetBreath()/2)/5))) {

                sensorFunction();
                pacer = newPace;

            }

            if ((five - sec) > 5 ) {

                sec = five;

                if (this->sensorAttached == true){

                    losePower(1);


                    // test

                    //qInfo("hello");

                    if (this->highScoreCheck == true) {

                        if (hIt != highScore.end()) {

                            coherenceTest(*hIt);

                            hIt++;

                        } else {

                            hIt = highScore.begin();

                            coherenceTest(*hIt);

                            hIt++;

                        }

                    } else if (this->mediumScoreCheck == true) {

                        if (mIt != mediumScore.end()) {

                            coherenceTest(*mIt);

                            mIt++;

                        } else {

                            mIt = mediumScore.begin();

                            coherenceTest(*mIt);

                            mIt++;

                        }

                    } else if (this->lowScoreCheck == true) {

                        if (lIt != lowScore.end()) {

                            coherenceTest(*lIt);

                            lIt++;

                        } else {

                            lIt = lowScore.begin();

                            coherenceTest(*lIt);

                            lIt++;

                        }

                    }


                } else {

                    // end session
                    selectButton();

                }



            }


        }
        // make key axis range scroll with the data (at a constant range size of 8)
        ui->customPlot->xAxis->setRange(key, 8, Qt::AlignRight);
        ui->customPlot->replot();

        // calculate frames per second:
        /*
        static double lastFpsKey;
        static int frameCount;
        ++frameCount;
        if (key-lastFpsKey > 2) // average fps over 2 seconds
        {
          statusBar()->showMessage(
                QString("%1 FPS, Total Data points: %2")
                .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
                .arg(ui->customPlot->graph(0)->data()->size()+ui->customPlot->graph(1)->data()->size())
                , 0);
          lastFpsKey = key;
          frameCount = 0;
        }
        */

    }

}

void MainWindow::coherenceTest(float cohScore) {

    // Coherence score calculations
    //std::cout << baseItem.getChallNum() << std::endl;
    //std::cout << (cohScore) << std::endl;

    QString newLvl = baseItem.challengeLvl(baseItem.getChallNum(), cohScore);

    if (newLvl == "Low") {

        this->redLight = true;
        this->blueLight = false;
        this->greenLight = false;

        this->timeLowSec += 5;

    } else if (newLvl == "Medium") {

        this->redLight = false;
        this->blueLight = true;
        this->greenLight = false;

        this->timeMediumSec += 5;

    } else if (newLvl == "High") {

        this->redLight = false;
        this->blueLight = false;
        this->greenLight = true;

        this->timeHighSec += 5;

    }

    //lights
    lights();

    // Change Screen Coherence Score
    ui->coherenceScore->setValue(cohScore);

    // Change time on screen
    ui->timeCounter->setValue(ui->timeCounter->value() + 5);

    // Change achScore on Screen
    ui->achScore->setValue(this->baseItem.achScore(ui->achScore->value(), cohScore));

    // test
    std::cout << "Time High: " << this->timeHighSec << std::endl;
    std::cout << "Time Medium: " << this->timeMediumSec << std::endl;
    std::cout << "Time Low: " << this->timeLowSec << std::endl;

}

void MainWindow::sensorFunction() {

    if (ui->breathPacer->value() >= 100) {

        this->sensorTracker = true;

    } else if (ui->breathPacer->value() <= 0) {

        this->sensorTracker = false;

    }

    if (this->sensorTracker == true) {

        //down
        ui->breathPacer->setValue(ui->breathPacer->value() - (100/5));

    } else {

        //up
        ui->breathPacer->setValue(ui->breathPacer->value() + (100/5));


    }

}

void MainWindow::displayLogDetails(const LogEntry &selectedLog)
{
    ui->Main_Screen->clear();
    ui->Main_Screen->addItem(QString("Length: %1").arg(QString::number(selectedLog.length)));
    ui->Main_Screen->addItem(QString("Average Coherence: %1").arg(QString::number(selectedLog.average_coherence)));
    ui->Main_Screen->addItem(QString("Achievement Score: %1").arg(QString::number(selectedLog.achievement_score)));
    ui->Main_Screen->addItem(QString(" "));
    ui->Main_Screen->addItem(QString("Delete"));

    ui->Main_Screen->setCurrentRow(4);

    this->logDetailsSelected = true;
    this->logHistorySelected = false;

    // Display the graph
    ui->customPlot->setVisible(true);
    setupStaticLogPlot(ui->customPlot, selectedLog.plotXData, selectedLog.plotYData, selectedLog.achievement_score);

}


void MainWindow::showSessionSummary() {
    ui->Main_Screen->clear();
    double totalTime = timeHighSec + timeMediumSec + timeLowSec;

    double percentLow = (timeLowSec / totalTime) * 100;
    double percentMedium = (timeMediumSec / totalTime) * 100;
    double percentHigh = (timeHighSec / totalTime) * 100;

    // Calculate the total number of coherence scores
    int totalScores = lowScore.size() + mediumScore.size() + highScore.size();

    // Calculate the sum of all coherence scores
    double scoreSum = 0;
    for (float score : lowScore) scoreSum += score;
    for (float score : mediumScore) scoreSum += score;
    for (float score : highScore) scoreSum += score;

    // Calculate the average coherence value
    double averageCoherenceValue = scoreSum / totalScores;

    // Calculate the achievement score value
    double achievementScoreValue = lowScore.size() * 1 + mediumScore.size() * 2 + highScore.size() * 3;

    QString summary = QString("Summary of the session:\n\n"
                              "Percentage of time in low coherence: %1%\n"
                              "Percentage of time in medium coherence: %2%\n"
                              "Percentage of time in high coherence: %3%\n"
                              "Average coherence: %4\n"
                              "Length of session: %5 seconds\n"
                              "Achievement score: %6")
            .arg(percentLow)
            .arg(percentMedium)
            .arg(percentHigh)
            .arg(averageCoherenceValue)
            .arg(totalTime)
            .arg(achievementScoreValue);

    ui->Main_Screen->addItem(summary);
}

void MainWindow::logSession() {
    // Create a new LogEntry object
    LogEntry newLog;

    // Populate the LogEntry object with session data
    newLog.time = QDateTime::currentDateTime(); // Use the current date and time
    newLog.length = timeHighSec + timeMediumSec + timeLowSec; // Calculate the session length

    // Populate the coherence data
    CoherenceData lowData = {"low", timeLowSec};
    CoherenceData mediumData = {"medium", timeMediumSec};
    CoherenceData highData = {"high", timeHighSec};
    newLog.coherence.append(lowData);
    newLog.coherence.append(mediumData);
    newLog.coherence.append(highData);

    // Calculate the average coherence and achievement score using the method you previously implemented
    int totalScores = lowScore.size() + mediumScore.size() + highScore.size();
    double scoreSum = 0;
    for (float score : lowScore) scoreSum += score;
    for (float score : mediumScore) scoreSum += score;
    for (float score : highScore) scoreSum += score;
    newLog.average_coherence = scoreSum / totalScores;

    QVector<double> xData, yData;


    if (this->highScoreCheck == true || this->mediumScoreCheck == true) {

            QCPDataContainer<QCPGraphData> *graphData = ui->customPlot->graph(0)->data().data();

            for (int i = 0;  i < graphData->size(); i++) {

                xData.append(graphData->at(i)->key);
                yData.append(graphData->at(i)->value);

            }

            //delete graphData;

    } else {

            QCPDataContainer<QCPGraphData> *graphData = ui->customPlot->graph(1)->data().data();

            for (int i = 0;  i < graphData->size(); i++) {

                    xData.append(graphData->at(i)->key);
                    yData.append(graphData->at(i)->value);

            }

            //delete graphData;

    }


    newLog.plotXData = xData;
    newLog.plotYData = yData;

    // Calculate the achievement score value based on your desired formula
    newLog.achievement_score = lowScore.size() * 1 + mediumScore.size() * 2 + highScore.size() * 3;

    // Add the new LogEntry object to the logHistory
    logHistory.addLogEntry(newLog);
}

void MainWindow::setupStaticLogPlot(QCustomPlot *customPlot, const QVector<double> &xData, const QVector<double> &yData, int achievement_score) {
    customPlot->clearPlottables();

    QCPGraph *graph = customPlot->addGraph();
    graph->setData(xData, yData);
    graph->setPen(QPen(Qt::blue));

    customPlot->xAxis->setLabel("Time");
    customPlot->yAxis->setLabel("Coherence");

    customPlot->xAxis->setRange(xData.at(0), xData.at(0) + 50);
    customPlot->yAxis->setRange(50, 100);

    // rescale value (vertical) axis to fit the current data:
    customPlot->graph(0)->rescaleValueAxis(true);

    customPlot->replot();
}

void MainWindow::on_logEntrySelected(QListWidgetItem *item) {

    int selectedIndex = ui->Main_Screen->row(item);
    LogEntry selectedLog = logHistory.getLogs().at(selectedIndex);

    // Use the plotXData and plotYData from the log entry
    QVector<double> xData = selectedLog.plotXData;
    QVector<double> yData = selectedLog.plotYData;

    // Get the achievement score from the log entry
    int achievement_score = selectedLog.achievement_score;

    // Call the setupStaticLogPlot function to set up the plot
    setupStaticLogPlot(ui->customPlot, xData, yData, achievement_score);


    // Show the customPlot widget
    ui->customPlot->setVisible(true);
}





