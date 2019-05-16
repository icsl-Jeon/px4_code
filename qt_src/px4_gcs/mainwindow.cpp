#include "mainwindow.h"
#include <QPixmap>
#include <QSettings>
#include <QKeyEvent>

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
    
    // editing text board is not permitted 
    ui->textEdit->setReadOnly(true);

    ui->pushButton_keyboard->setChecked(true);
    ui->pushButton_planner->setChecked(false);

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

void MainWindow::keyPressEvent(QKeyEvent* event){
    std::string input_string = event->text().toStdString();
    textEdit_write((input_string + " is pressed.").c_str());
    px4_code::KeyboardInput keyboard_srv;
    keyboard_srv.request.key = input_string;
    if (not qnode->keyboard_client.call(keyboard_srv))
        textEdit_write("[GCS] key input not received.");                
}





void MainWindow::on_pushButton_keyboard_clicked(bool checked)
{
    if(checked){
        px4_code::InitHome srv_home;        
        px4_code::SwitchMode srv_mode;
        srv_mode.request.mode = 0;

        if (qnode->init_home_client.call(srv_home) and qnode->is_connected){
            textEdit_write("Init current des pose from key input");        
            px4_code::SwitchMode srv;
            srv.request.mode = 0;
            qnode->switch_mode_client.call(srv);            
        }           
        else
        {
            textEdit_write("Init current des pose or mode change failed. Is mav_wrapper running?");         
            ui->pushButton_keyboard->setChecked(false); // release
        }                                        
    }    
    // already checked, but again checked
    else{
        textEdit_write("Aleady checked");           
        ui->pushButton_keyboard->setChecked(true);
    }    
}

void MainWindow::on_pushButton_planner_clicked(bool checked)
{

            // first, see if ros connected 
    bool is_connected = qnode->is_connected;
    if(checked){


        // If then,                
        if (is_connected){                    
            ros::V_string node_name;
            // is this node running? If then, we request. 
            node_name.push_back(qnode->planner_node_name());   
            bool is_planner_exist = ros::master::getNodes(node_name);       
            // second, see whether planner node is running      
            if (is_planner_exist){
                px4_code::SwitchMode srv;
                srv.request.mode = 1;
                if(qnode->switch_mode_client.call(srv)){
                    ui->pushButton_planner->setChecked(true);
                    ui->pushButton_keyboard->setChecked(false);
                }
                else{
                    textEdit_write("Mode change denied. Is planning pose being published?");
                    ui->pushButton_planner->setChecked(false);
                }
            }        
            else{
                // request failed 
                ui->pushButton_planner->setChecked(false);
                textEdit_write("Planner node not found");           
            }
        }
        else{
            // request failed
            ui->pushButton_planner->setChecked(false);
            textEdit_write("Connect to ros first");
        }


    }else{
        // release the button 
        px4_code::SwitchMode srv;        
        srv.request.mode = 0;
        if (qnode->switch_mode_client.call(srv)) 
            ui->pushButton_keyboard->setChecked(true);
        else{
            textEdit_write("Keyboard mode denied.");
            ui->pushButton_planner->setChecked(true);
        }


    }    

}
