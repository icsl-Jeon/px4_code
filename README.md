# px4_code 

___



<img src="https://github.com/icsl-Jeon/px4_code/blob/master/img/manual.gif">

Visit here for instruction: 

https://www.youtube.com/watch?v=9QvqmMlA_oY

## Introduction 

<img src="https://github.com/icsl-Jeon/px4_code/blob/master/img/mav_wrapper_diagram.png">

 This package is a module for px4 position control from user input or trajectory planner. Currently, SITL (refer mavros and px4 for details) was validated with gazebo.  The package implemented two nodes: 1) gcs with gui(mav_gcs_node) and 2) wrapping module(mav_wrapping_node).  To run those simulator, you have to properly configure your QGroundControl parameter so that px4 can listen external pose topic(refer). 



<img src="https://github.com/icsl-Jeon/px4_code/blob/master/img/main_ui.png">

## Installation 

### 0. Dependencies

#### (1) px4 firmware 

This package is not a ROS package! In our case, this package is required for SITL(simulation) gazebo. This package will help us spawn drone on gazebo and mimic the behavior of actual drone. 

```
git clone https://github.com/PX4/Firmware
```

*No build is mandatory at present step. Follow [make instruction](##mavros)*

#### (2) mavros

This package is for mavlink-ROS communication with px4 on SITL. As a ROS user, we can access px4 via this ROS node.  Install it from source.  

#### (3) gazebo2rviz

In this package, we will extract tf information coming from gazebo. In order for it,  the following package will publish tf information of all objects in gazebo simulator. Basically, this is fake gps. 

<http://wiki.ros.org/gazebo2rviz>

# 

### 1. Run the simulation environment (SITL) 

#### (1) git clone 

```
git clone https://github.com/PX4/Firmware.git
```

#### (2) building for SITL (simulation in the loop) and run gazebo simulator 

```
cd /path/to/Firmware
make  px4_sitl_default gazebo
or 
HEADLESS=1 make  px4_sitl_default gazebo
```

This process is basically the same procedure to prepare an actual drone.

Try it : pxh>commander takeoff  in the terminal 

The simulation calls the launch file: Firmware/Tools/sitl_gazebo/worlds/*.world

**For the gazebo_ros wrapper (recommended), run ```source /home/jbs/lib/Firmware/Tools/setup_gazebo.bash /home/jbs/lib/Firmware /home/jbs/lib/Firmware/build/px4_sitl_default```.  Then  add the following lines in bashrc **

```
export ROS_PACKAGE_PATH=$ROS_PACKAGE_PATH:/home/jbs/lib/Firmware
export ROS_PACKAGE_PATH=$ROS_PACKAGE_PATH:/home/jbs/lib/Firmware/Tools/sitl_gazebo
```

Run the following command to start up the px4 + gazebo + ros(gazebo server)

```
roslaunch px4 posix_sitl.launch
```



#### (3) run px4.luanch 

```
roslaunch mavros px4.launch fcu_url:="udp://:14540@127.0.0.1:14557"
```

The udp 14540 is link to the SITL in the gazebo.  The 127.0.0.1:14557 is the local host 

#### (4) (Optional) QGroundControl

### 1.1 ros wrapping for gazebo

```
cd /path/to/Firware

no_sim=1 make px4_sitl_default gazebo
# This following should be executed after a make    
source Tools/setup_gazebo.bash $(pwd) $(pwd)/build/px4_sitl_default

# opt1
roslaunch gazebo_ros empty_world.launch world_name:=$(pwd)/Tools/sitl_gazebo/worlds/iris.world gui:=false

# opt2
roslaunch px4_code run_px4_for_sitl.launch
```



### 2. Custom code 

```
roslaunch px4_code run_px4_for_sitl.launch
```





