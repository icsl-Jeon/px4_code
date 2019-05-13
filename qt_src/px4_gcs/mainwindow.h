#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qnode.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QNode* qnode,QWidget *parent = 0);
    ~MainWindow();

Q_SIGNALS:
    void ros_comm_start(bool* result);  
    void ros_comm_end();
    
private Q_SLOTS:

    void on_pushButton_ros_clicked(bool checked);

    void on_pushButton_init_clicked();

    void on_pushButton_arm_clicked(bool checked);

    void on_pushButton_offboard_clicked();

    void textEdit_write(QString);

private:
    Ui::MainWindow *ui;
    QNode* qnode;
};

#endif // MAINWINDOW_H
