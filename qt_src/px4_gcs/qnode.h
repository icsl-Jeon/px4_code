#ifndef QNODE_H
#define QNODE_H

#ifndef Q_MOC_RUN
#include <ros/ros.h>
#endif

// mavros 
#include "mavros_msgs/CommandBool.h"
#include "mavros_msgs/SetMode.h"


// px4_code 
#include "px4_code/InitHome.h"

#include <QThread>
#include <QStringListModel>

class QNode : public QThread {
Q_OBJECT

public:
    // constructor 
    QNode(int argc, char** argv, const std::string &name );
    ~QNode();
    // void run(); // the entire while loop 
    QStringListModel* loggingModel() { return &logging; }
    const std::string& nodeName() { return node_name; }
    void ros_comms_init();

    // clients 
    ros::ServiceClient arming_client;              
    ros::ServiceClient set_mode_client;
    ros::ServiceClient init_home_client;

    // flags 
    bool is_connected = false;



Q_SIGNALS:
   void loggingUpdated();
   void rosShutdown();
   void writeOnBoard(QString);

private Q_SLOTS:    
    void on_init(bool* result);
    void shutdown();
protected:
   int init_argc;
   char** init_argv;
   QStringListModel logging;
   const std::string node_name;


private:


};


#endif // QNODE_H
