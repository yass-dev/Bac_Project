#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QtSerialPort>
#include "ui_form.h"

class MainWindow : public QWidget, private Ui::Form
{
    Q_OBJECT

public:
    MainWindow();

    void initPort();

private slots:
    void sendData();
    void readData();
    void advance();
    void printSerialError();

    void on_startButton_clicked();

private:
    QString data;
    QSerialPort *port;
    QString tempData;
    QTimer *timer;
    QTimer *advanceTimer;
    QByteArray charBuffer;

    QString exData;

    bool canAdvance;
    bool canRecule;
    bool canTurnRight;
    bool canTurnLeft;
    bool doNothingPedals;
    bool doNothingWheel;
};

#endif // MAINWINDOW_H
