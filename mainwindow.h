#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "command_list.h"

#include <QMainWindow>
#include <QUdpSocket>
#include <QKeyEvent>
#include <QTimer>

#define QUADR_PORT 3232
#define QUADR_IP "192.168.31.255"

#define SYNC_TIMER_INTERVAL 100
#define KEYBOARD_TIMER_INTERVAL 10

#define THROTTLE_STEP 0.1
#define PITCH_STEP 0.1
#define ROLL_STEP 0.1
#define YAW_STEP 0.1

#define KEY_QUADR_UP Qt::Key_R
#define KEY_QUADR_DOWN Qt::Key_F
#define KEY_QUADR_FORWARD Qt::Key_W
#define KEY_QUADR_BACKWARD Qt::Key_S
#define KEY_QUADR_LEFT Qt::Key_A
#define KEY_QUADR_RIGHT Qt::Key_D
#define KEY_QUADR_CLOCKWISE Qt::Key_E
#define KEY_QUADR_COUNTERCLOCKWISE Qt::Key_Q

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QUdpSocket socket;
    QTimer syncTimer;
    QTimer keyboardTimer;

    set_throttle currentThrottle;
    set_throttle sentThrottle;
    set_orientation currentOrientation;
    set_orientation sentOrientation;

    const int port = QUADR_PORT;

    bool send(char *data);
    bool sendThrottle(float value);
    bool sendOrientation(float pitch, float roll, float yaw);

public slots:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void dataReceived();
    void syncQuadrData();
    void increaseThrottle();
    void decreaseThrottle();
    void increasePitch();
    void decreasePitch();
    void increaseRoll();
    void decreaseRoll();
    void increaseYaw();
    void decreaseYaw();
};

#endif // MAINWINDOW_H
