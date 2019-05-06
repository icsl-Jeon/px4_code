# px4_code for my project

## Installation 

### 1. Firmware installation for px4 

#### (1) git clone 

```
git clone https://github.com/PX4/Firmware.git
```

#### (2) building for SITL (simulation in the loop) and run gazebo simulator 

```
cd /path/to/Firmware
make  px4_sitl_default gazebo
```

This process is basically the same procedure to prepare an actual drone.

Try it : pxh>commander takeoff  in the terminal 

The simulation calls the launch file: Firmware/Tools/sitl_gazebo/worlds/*.world



#### (3) run px4.luanch 

```
roslaunch mavros px4.launch fcu_url:="udp://:14540@127.0.0.1:14557"
```

The udp 14540 is link to the SITL in the gazebo.  The 127.0.0.1:14557 is the local host 

#### (4) (Optional) QGroundControl





