#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QKeyEvent>
#include <QTimer>

#include <QDebug>

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
    QTimer calc_task;
    const int port = 3232;
public slots:
    void keyPressEvent( QKeyEvent * event ) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void udp_data_recived();
};
#endif // MAINWINDOW_H
