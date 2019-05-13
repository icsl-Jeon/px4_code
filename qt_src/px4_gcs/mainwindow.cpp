#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QSettings>

#include "qnode.h"


MainWindow::MainWindow(QNode* qnode,QWidget *parent) :
    QMainWindow(parent),qnode(qnode),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(qnode, SIGNAL(rosShutdown()), this, SLOT(close()));
    QObject::connect(qnode,SIGNAL(writeOnBoard(QString)),this,SLOT(textEdit_write(QString)));
    QObject::connect(this,SIGNAL(ros_comm_start(bool*)),qnode,SLOT(on_init(bool *)));
    QObject::connect(this,SIGNAL(ros_comm_end()),qnode,SLOT(shutdown()));

    // color 
    ui->pushButton_ros->setStyleSheet("QPushButton:checked{background-color: rgba(200, 20, 80,20);\
                                          }");
    // color 
    ui->pushButton_arm->setStyleSheet("QPushButton:checked{background-color: rgba(200, 20, 80,20);\
                                          }");    

}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_ros_clicked(bool checked){
    if (checked) { // turn on
        bool is_connected; 
        Q_EMIT ros_comm_start(&is_connected);
        if (is_connected){
            ui->textEdit->append("connection estabilished");    
            qnode->is_connected = true; 
        }else{
            ui->textEdit->append("connection failed");
            ui->pushButton_ros->setChecked(false);            
        }    
    } // turn off
    else{
        qnode->is_connected = false; 
        Q_EMIT ros_comm_end();
        ui->textEdit->append("gcs terminated.");    
    }
}

void MainWindow::on_pushButton_init_clicked(){
    // service call for initialize the px4's homing position 
    px4_code::InitHome init_home;
    
    bool is_success = qnode->init_home_client.call(init_home);

    if (is_success)
        textEdit_write("homing position initialized.");
    else
        textEdit_write("homing position initialization requested but failed.");
     
}

void MainWindow::on_pushButton_arm_clicked(bool checked){
    
    // arming request
    if(checked){  

        mavros_msgs::CommandBool arm_cmd;
        arm_cmd.request.value = true;
        bool is_success = qnode->arming_client.call(arm_cmd) and arm_cmd.response.success;
        if (is_success){
            ui->pushButton_arm->setText("Armed");            
        }
        else{
            ui->pushButton_arm->setChecked(false);            
            textEdit_write("Arming requested but service call failed");
            
        }
        
    }
    // disarming request
    else{ 
        mavros_msgs::CommandBool arm_cmd;
        arm_cmd.request.value = false;
        bool is_success = qnode->arming_client.call(arm_cmd) and arm_cmd.response.success;
        if (is_success){
            ui->pushButton_arm->setText("Disarmed");            
        }
        else{
            ui->pushButton_arm->setChecked(true);            
            textEdit_write("Disarming requested but service call failed");
        }

    }
}

void MainWindow::on_pushButton_offboard_clicked(){

    if (qnode->is_connected){    
        mavros_msgs::SetMode offb_set_mode;
        offb_set_mode.request.custom_mode = "OFFBOARD";    
        bool is_success = qnode->set_mode_client.call(offb_set_mode) and offb_set_mode.response.mode_sent; 
           if (is_success)
                textEdit_write("offboard mode sent.");
           else
                textEdit_write("requested offboard but failed.");                                          
    }else{
        textEdit_write("please invoke setmode after 1)turning mav_wrapper 2) ros connect");  
        ui->pushButton_arm->setChecked(false);      
    }
}


void MainWindow::textEdit_write(QString text){
    ui->textEdit->append(text);
}
