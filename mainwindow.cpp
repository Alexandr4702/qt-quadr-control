#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

#include "command_list.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    int result = socket.bind(QHostAddress::Any, port);
    cerr << result << "  result bind" << endl;
    connect(&socket, SIGNAL(readyRead()), this, SLOT(udp_data_recived()));

    calc_task.setInterval(10);
//    connect(&calc_task, &QTimer :: timeout, [](){cerr << "hello" << endl;});
    calc_task.start();

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    switch (event->key()) {
    case Qt::Key_W:
    {
        uint8_t data[2 + sizeof(struct set_throttle)];
        struct set_throttle* data_ptr = reinterpret_cast<struct set_throttle*> (data + 2);
        data_ptr->value = 0.5;
        std::cerr << socket.writeDatagram(
                         reinterpret_cast<char *>(data),
                         sizeof (data),
                         QHostAddress("192.168.31.255"), port)<< std ::endl;
    }
        break;
    case Qt::Key_S:
    {
        uint8_t data[2 + sizeof(struct set_throttle)];
        struct set_throttle* data_ptr = reinterpret_cast<struct set_throttle*> (data + 2);
        data_ptr->value = 0.0;
        std::cerr << socket.writeDatagram(
                         reinterpret_cast<char *>(data),
                         sizeof (data),
                         QHostAddress("192.168.31.255"), port)<< std ::endl;
    }
        break;
    case Qt::Key_A:
        std::cerr << "button presed " << 'a' << std :: endl;
        break;
    case Qt::Key_D:
        std::cerr << "button presed " << 'd' << std :: endl;
        break;
    case Qt::Key_E:
        std::cerr << "button presed " << 'e' << std :: endl;
        break;
    case Qt::Key_Q:
        std::cerr << "button presed " << 'q' << std :: endl;
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    switch (event->key()) {
    case Qt::Key_W:
        std::cerr << "button released " << 'w' << std :: endl;
        break;
    case Qt::Key_S:
        std::cerr << "button released " << 's' << std :: endl;
        break;
    case Qt::Key_A:
        std::cerr << "button released " << 'a' << std :: endl;
        break;
    case Qt::Key_D:
        std::cerr << "button released " << 'd' << std :: endl;
        break;
    case Qt::Key_E:
        std::cerr << "button released " << 'e' << std :: endl;
        break;
    case Qt::Key_Q:
        std::cerr << "button released " << 'q' << std :: endl;
        break;
    }
}

void MainWindow::udp_data_recived()
{
//    std::cerr << "in !" << std::endl;
    QHostAddress sender;
    u_int16_t port;
    while (socket.hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(socket.pendingDatagramSize());
        socket.readDatagram(datagram.data(),datagram.size(),&sender,&port);
//        cerr <<"Message From :: " << sender.toString().toStdString() << "  " << endl;
//        cerr <<"Port From :: "<< port << endl;
        cerr <<"Message :: " << datagram.toStdString();// << endl;
    }
}

