#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "command_list.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    bool bindResult = socket.bind(QHostAddress::Any, port);
    if (bindResult)
        exit(EXIT_FAILURE);

    connect(&socket, SIGNAL(QUdpSocket::readyRead()), this, SLOT(dataReceived()));

    keyboardTimer.setInterval(KEYBOARD_TIMER_INTERVAL);
    syncTimer.start(SYNC_TIMER_INTERVAL);
    connect(&syncTimer, SIGNAL(QTimer::timeout()), this, SLOT(syncQuadrData()));

    ui->setupUi(this);
    grabKeyboard();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::send(char *data)
{
    return socket.writeDatagram(
        data,
        sizeof (data),
        QHostAddress(QUADR_IP),
        port
    ) == sizeof (data);
}

bool MainWindow::sendThrottle(float value)
{
    uint8_t data[2 + sizeof(struct command)];
    struct command* data_ptr = reinterpret_cast<struct command*> (data + 2);
    data_ptr->command_id = set_throttle_comm;
    struct set_throttle* throttleData = new set_throttle;
    throttleData->value = value;
    data_ptr->data = throttleData;
    return send(reinterpret_cast<char *>(data));
}

bool MainWindow::sendOrientation(float pitch, float roll, float yaw)
{
    uint8_t data[2 + sizeof(struct command)];
    struct command* data_ptr = reinterpret_cast<struct command*> (data + 2);
    data_ptr->command_id = set_orientation_comm;
    struct set_orientation* orientationData = new set_orientation;
    orientationData->pitch = pitch;
    orientationData->roll = roll;
    orientationData->yaw = yaw;
    data_ptr->data = orientationData;
    return send(reinterpret_cast<char *>(data));
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    switch (event->key()) {
        case KEY_QUADR_UP:
            connect(&keyboardTimer, SIGNAL(QTimer::timeout()), this, SLOT(MainWindow::increaseThrottle()));
            break;
        case KEY_QUADR_DOWN:
            connect(&keyboardTimer, SIGNAL(QTimer::timeout()), this, SLOT(MainWindow::decreaseThrottle()));
            break;
        case KEY_QUADR_FORWARD:
            connect(&keyboardTimer, SIGNAL(QTimer::timeout()), this, SLOT(MainWindow::increasePitch()));
            break;
        case KEY_QUADR_BACKWARD:
            connect(&keyboardTimer, SIGNAL(QTimer::timeout()), this, SLOT(MainWindow::decreasePitch()));
            break;
        case KEY_QUADR_RIGHT:
            connect(&keyboardTimer, SIGNAL(QTimer::timeout()), this, SLOT(MainWindow::increaseRoll()));
            break;
        case KEY_QUADR_LEFT:
            connect(&keyboardTimer, SIGNAL(QTimer::timeout()), this, SLOT(MainWindow::decreaseRoll()));
            break;
        case KEY_QUADR_CLOCKWISE:
            connect(&keyboardTimer, SIGNAL(QTimer::timeout()), this, SLOT(MainWindow::increaseYaw()));
            break;
        case KEY_QUADR_COUNTERCLOCKWISE:
            connect(&keyboardTimer, SIGNAL(QTimer::timeout()), this, SLOT(MainWindow::decreaseYaw()));
            break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    switch (event->key()) {
        case KEY_QUADR_UP:
            disconnect(&keyboardTimer, SIGNAL(QTimer::timeout()), this, SLOT(MainWindow::increaseThrottle()));
            break;
        case KEY_QUADR_DOWN:
            disconnect(&keyboardTimer, SIGNAL(QTimer::timeout()), this, SLOT(MainWindow::decreaseThrottle()));
            break;
        case KEY_QUADR_FORWARD:
            disconnect(&keyboardTimer, SIGNAL(QTimer::timeout()), this, SLOT(MainWindow::increasePitch()));
            break;
        case KEY_QUADR_BACKWARD:
            disconnect(&keyboardTimer, SIGNAL(QTimer::timeout()), this, SLOT(MainWindow::decreasePitch()));
            break;
        case KEY_QUADR_RIGHT:
            disconnect(&keyboardTimer, SIGNAL(QTimer::timeout()), this, SLOT(MainWindow::increaseRoll()));
            break;
        case KEY_QUADR_LEFT:
            disconnect(&keyboardTimer, SIGNAL(QTimer::timeout()), this, SLOT(MainWindow::decreaseRoll()));
            break;
        case KEY_QUADR_CLOCKWISE:
            disconnect(&keyboardTimer, SIGNAL(QTimer::timeout()), this, SLOT(MainWindow::increaseYaw()));
            break;
        case KEY_QUADR_COUNTERCLOCKWISE:
            disconnect(&keyboardTimer, SIGNAL(QTimer::timeout()), this, SLOT(MainWindow::decreaseYaw()));
            break;
    }
}

void MainWindow::syncQuadrData()
{
    if (currentThrottle.value != sentThrottle.value)
    {
        if (sendThrottle(currentThrottle.value))
        {
            sentThrottle.value = currentThrottle.value;
        }
    }

    if (
        currentOrientation.pitch != sentOrientation.pitch
        || currentOrientation.roll != sentOrientation.roll
        || currentOrientation.yaw != sentOrientation.yaw
    )
    {
        if (sendOrientation(currentOrientation.pitch, currentOrientation.roll, currentOrientation.yaw))
        {
            sentOrientation.pitch = currentOrientation.pitch;
            sentOrientation.roll = currentOrientation.roll;
            sentOrientation.yaw = currentOrientation.yaw;
        }
    }
}

void MainWindow::increaseThrottle()
{
    currentThrottle.value += THROTTLE_STEP;
}

void MainWindow::decreaseThrottle()
{
    currentThrottle.value -= THROTTLE_STEP;
}

void MainWindow::increasePitch()
{
    currentOrientation.pitch += PITCH_STEP;
}

void MainWindow::decreasePitch()
{
    currentOrientation.pitch -= PITCH_STEP;
}

void MainWindow::increaseRoll()
{
    currentOrientation.roll += ROLL_STEP;
}

void MainWindow::decreaseRoll()
{
    currentOrientation.roll -= ROLL_STEP;
}

void MainWindow::increaseYaw()
{
    currentOrientation.yaw += YAW_STEP;
}

void MainWindow::decreaseYaw()
{
    currentOrientation.yaw -= YAW_STEP;
}

void MainWindow::dataReceived()
{
    QHostAddress sender;
    u_int16_t port;
    while (socket.hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(socket.pendingDatagramSize());
        socket.readDatagram(datagram.data(), datagram.size(), &sender, &port);
        std::cerr << "Message :: " << datagram.toStdString() << std::endl;
    }
}

