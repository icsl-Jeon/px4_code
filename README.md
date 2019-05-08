# px4_code for my project

## Installation 

### 1. Firmware Settings

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





