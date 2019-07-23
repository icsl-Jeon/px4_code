# px4_code 

___



<img src="https://github.com/icsl-Jeon/px4_code/blob/master/img/manual.gif">

<img src="https://github.com/icsl-Jeon/px4_code/blob/master/img/HITL_flight.gif">

Visit here for instruction video for SITL: 

https://www.youtube.com/watch?v=9QvqmMlA_oY



## Introduction 

<img src="https://github.com/icsl-Jeon/px4_code/blob/master/img/mav_wrapper_diagram.png">

 This package is a module for px4 position control from user input or trajectory planner. Currently, SITL (refer mavros and px4 for details) with Gazebo and HITL (real hard ware) were validated.  The package implemented two nodes: 1) gcs with gui(mav_gcs_node) and 2) wrapping module(mav_wrapping_node). 

*mav_gcs_node* features various service clients for initialization or re-initalization of setpoint for the case of keyboard control (**initialize homing** button in gui), arming and disarming in offboard control (**Arming button**) and offboard mode change for px4(**Setoffboard and flight**). Also, it alternates setpoint publish mode between keyboard and planner by **keyboard** & **planner** button . 

*mav_wrapper_node* overrides the local position of px4 with external pose (/mavros/vision_pose/pose). The external pose will be fused with IMU of px4 to yield /mavros/local_position/pose which will be used for position control in offboard mode.  Also, it provides clients for the mentioned service in mav_gcs_node in onboard computer side. 

The *px4_code* equipped with *mav_gcs_node* and *mav_wrapper_node* will help you in the following cases: 1) real world MAV position control in either motion capture system or VIO (ZED stereo camera VIO was tested in my hardware environment) 2) gazeob SITL simulation from px4.

​        

### (1) mav_gcs_node

#### gui of mav_gcs_node to be run in ground station(labtop) 

<img src="https://github.com/icsl-Jeon/px4_code/blob/master/img/main_ui.png">

I didn't instate the setpoint publisher in ground station. Instead, I put it in onboard side. Thus, to change the setpoint from keyboard input from gcs, I put multiple service clients in GCS with the above gui. 

### (2) mav_wrapper_node

Node [/mav_wrapper_node]
Publications: 

 * /mavros/setpoint_position/local [geometry_msgs/PoseStamped]
 * /mavros/vision_pose/pose [geometry_msgs/PoseStamped]

Subscriptions: 
 * /mav_wrapper/setpoint_planning [geometry_msgs/PoseStamped]
 * /mavros/state [mavros_msgs/State]
 * /tf [tf2_msgs/TFMessage]
 * /tf_static [unknown type]

Services: 
 * /mav_wrapper/init_home : initialize keyboard setpoint with current /mavros/local_position/pose 

 * /mav_wrapper/keyboard_in : change the setpoint from keyboard mode. 

   w: forward / s: backword / a:move left / d: move right (those who enjoys shooting game, you will..) 

   z: descent / c: ascent / q: spin left / e: spin right   

 * /mav_wrapper/swtich_mode: determine setpoint publishing in either from keyboard or planner which publishes /mav_wrapper/setpoint_planning [geometry_msgs/PoseStamped]. 0: keyboard 1:planner  for request. 

   

## Installation 

___



### 1. Dependencies

#### (1) px4 firmware 

This package is not a ROS package! In our case, this package is required for SITL(simulation) gazebo. This package will help us spawn drone on gazebo and mimic the behavior of actual drone. 

```
git clone https://github.com/PX4/Firmware.git
```

*No build is mandatory at present step in STIL. Follow [make instruction](##mavros)*

#### (2) mavros

This package is for mavlink-ROS communication with px4 on SITL. As a ROS user, we can access px4 via this ROS node.  Install it from source.  

#### (3) gazebo2rviz

In this package, we will extract tf information coming from gazebo in SITL. In order for it,  the following package will publish tf information of all objects in gazebo simulator. Basically, this is fake gps. 

<http://wiki.ros.org/gazebo2rviz>

### (4) traj_gen

You might want to try this package for path generation where you can directly infuse control pose. 

Clone here :  <https://github.com/icsl-Jeon/traj_gen>

## Getting started

____



### 0. COMMON CONFIG 

You have to configure px4 parameters in QGroundControl so that the pixhawk can utilize the external position estimate from vicon or VIO module (you can even fuse them with GPS in HTIL). In order for that, 

* Change EKF2 estimator group rather than LPE.

* EKF2_AID_MASK = Vision 

* EKF2_HGT_MODE = Vision 

  

### 1. SITL

#### (1) git clone 

```
git clone https://github.com/PX4/Firmware.git
```

Add the following in your bash (to enable roscd px4)

```
export ROS_PACKAGE_PATH=$ROS_PACKAGE_PATH:/path/to/Firmware
export ROS_PACKAGE_PATH=$ROS_PACKAGE_PATH:/path/to/Firmware/Tools/sitl_gazebo
```

#### (2) build for SITL (simulation in the loop) and run gazebo simulator 

#### a. Build px4 firmware. Recommended every time you try to launch a SITL  <a name = "a" ></a> 

```
cd path/to/px4_firmware  
no_sim=1 make px4_sitl_default gazebo
```

#### b. sourcing should be executed every after a make in the terminal where you will launch the following (NOTE!!!) 

In another terminal (could be any terminal except the above), let us launch SITL with gazebo. Should ```source``` ( first line) per every launch(second line)  

```
source path/to/px4_firmware/Tools/setup_gazebo.bash $(pwd) $(pwd)/build/px4_sitl_default
roslaunch px4_code run_px4_for_sitl.launch 
```

### 3) launch px4_code pacakges

```
roslaunch px4_code run_mav_gcs_wrapper_sitl.launch
```

This will run the two nodes mentioned in introduction. You might have to modify ```external_pose_topic```  in the parameter setting in the launch file. 

If things was configured whell, your px4 shell [px4_shell](#a) will say after the px4_code launch  

```
INFO  [ecl/EKF] EKF commencing external vision position fusion
INFO  [ecl/EKF] EKF commencing external vision yaw fusion
```

- check if /mavros/local_position/pose and /mavros/vision_pose/pose similar(they should be very close.). If not, check whether the two has same time stamp! 

- If mav_wrapper_node received external pose topic and ```mavros/local_position/pose``` , the following will appear from px4_code terminal 

  ```
  [ INFO] [1563892246.976191576, 241.848000000]: received external pose.
  [ INFO] [1563892247.004177214, 241.876000000]: [Mav Wrapper] Initializing the homing point with the current pose
  [ INFO] [1563892247.028217821, 241.900000000]: Start publishing setpoint.
  [ INFO] [1563892299.438848425, 294.224000000]: [Mav Wrapper] Initializing the homing point with the current pose
  ```

  Also, you could see the topics and tf in as the following rviz.

<img src="https://github.com/icsl-Jeon/px4_code/blob/master/img/SITL_rviz.png">

As can be seen, the /mavros/local_position/pose should be almost same with the tf coming from gazebo.  

The red arrow denotes the setpoint which is currently initialized with the current pose. 

### (4) So. let's try px4_code!  

```
rosrun px4_code mav_gcs_node 
```

* Step 1 : press ros connect in gui! Once done, mav_gcs_node will appear in rosnode list
* Step 2: press initialize button. This will initialize the current setpoint from keyboard. 
* Step 3: press disarmed button.  From this, you wll see rotor0 operating 
* Step 4. press set offboard button. The flight mode will change from manual into offboard from which mavros will listen setpoint as control object. Move mav with keyboard! 
* Step 5. If you have node which publishes /mav_wrapper/setpoint_planning [geometry_msgs/PoseStamped], try pressing planner button. This will listen the setpoint from planner.  

### 3. HITL 

 From this, you will have to run mav_gcs_node and mav_wrapper separately. We should run mav_wrapper in onboard computer where ```mavros``` will be launched 

#### Onboard side

#### In terminal A

````
roslaunch mavros px4.launch
````

#### In terminal B  

```
roslaunch px4_code run_mav_wrapper.launch
```

#### On the side of labtop or other computer which provides external position estimate 

make sure ```<param name="external_pose_topic" value="$(some topic)"/>```

```
roalaunch vicon_bridge vicon.launch (in my setting, it provides pose topic )
rosrun px4_code mav_gcs_node
#TODO roslaunch px4_code run_mav_gcs_node_onboard.launch (including rviz)
```



###  

