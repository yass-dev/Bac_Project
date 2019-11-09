#include "mainwindow.h"

MainWindow::MainWindow()
    : canAdvance(false), canRecule(false), canTurnRight(false), canTurnLeft(false),
      doNothingPedals(false), doNothingWheel(false)
{
    setupUi(this);

    port = new QSerialPort(this);

    initPort();

    qDebug() << "Port opened : " << port->isOpen();

    timer = new QTimer();
    timer->start(900);
    advanceTimer = new QTimer();
    advanceTimer->start(10);

    QObject::connect(port, &QSerialPort::errorOccurred, this, &MainWindow::printSerialError);
    connect(timer, &QTimer::timeout, this, &MainWindow::sendData);
    connect(advanceTimer, &QTimer::timeout, this, &MainWindow::advance);
    connect(port, &QSerialPort::readyRead, this, &MainWindow::readData);

    startButton->setEnabled(true);
}

void MainWindow::sendData()
{
    QFile fichier("C:/Users/Yassou/Documents/Unreal Projects/MyProject/test.txt");

    if(fichier.open(QIODevice::ReadOnly))
    {
        data = fichier.readAll();
        data.replace("\r\n", "");
        if(data != exData)
        {
            port->write(data.toStdString().c_str());
            sentValueLabel->setText(data.toLatin1());
            exData = data;
        }
    }
    else
    {
        QMessageBox::warning(this, "Error", "Ouverture du fichier impossible");
    }
}

void MainWindow::initPort()
{
    port->setPortName("COM6");
    port->setBaudRate(QSerialPort::Baud115200);
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setFlowControl(QSerialPort::NoFlowControl);
    port->open(QIODevice::ReadWrite);
    if(!port->isOpen())
    {
        QMessageBox::warning(this, "port error", "Impossible to open the port!");
    }
}

void MainWindow::readData()
{
    QByteArray data = port->readAll();
    charBuffer.clear();
    charBuffer.append(data);
    QString str(data);
    receiveValueLabel->setText(charBuffer);
    qDebug() << str;

    if(str == "2")
    {
        canAdvance = true;
        doNothingPedals = false;
    }
    else if(str == "1")
    {
        canRecule = true;
        doNothingPedals = false;
    }
    else if(str == "3")
    {
        canTurnRight = true;
        doNothingWheel = false;
    }
    else if(str == "4")
    {
        canTurnLeft = true;
        doNothingWheel = false;
    }
    else if(str == "0" || str.contains("0"))
    {
        doNothingPedals = true;
        canAdvance = false;
        canRecule = false;
    }
    else if(str == "5" || str.contains("0"))
    {
        doNothingWheel = true;
        canTurnRight = false;
        canTurnLeft = false;
    }

    charBuffer = "";
    data = nullptr;
}

void MainWindow::advance()
{
    //qDebug() << canAdvance << " " << canRecule << " " << canTurnRight << " " << canTurnLeft;
    if(canAdvance)
    {
        keybd_event(0x28, 0, KEYEVENTF_KEYUP, 0);
        keybd_event(0x5A, 0, 0, 0);
    }
    else if(canRecule)
    {
        keybd_event(0x5A, 0, KEYEVENTF_KEYUP, 0);
        keybd_event(0x28, 0, 0, 0);
    }
    if(canTurnRight)
    {
        keybd_event(0x25, 0, KEYEVENTF_KEYUP, 0);
        keybd_event(0x27, 0, 0, 0);
    }
    else if(canTurnLeft)
    {
        keybd_event(0x27, 0, KEYEVENTF_KEYUP, 0);
        keybd_event(0x25, 0, 0, 0);
    }
    if(doNothingPedals)
    {
        keybd_event(0x28, 0, KEYEVENTF_KEYUP, 0);
        keybd_event(0x5A, 0, KEYEVENTF_KEYUP, 0);
    }
    if(doNothingWheel)
    {
        keybd_event(0x25, 0, KEYEVENTF_KEYUP, 0);
        keybd_event(0x27, 0, KEYEVENTF_KEYUP, 0);
    }
}

void MainWindow::on_startButton_clicked()
{
    qDebug() << "Clicked !";
    port->write("20;20");
}

void MainWindow::printSerialError()
{
    qDebug() << "Error on Serial port " << port->portName() << " : " << port->errorString();
}
