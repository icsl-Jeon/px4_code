cmake_minimum_required(VERSION 2.8.3)
project(px4_code)

add_compile_options(-std=c++14)
set(CMAKE_BUILD_TYPE Release)
find_package(catkin REQUIRED COMPONENTS 
message_generation
nav_msgs
mavros_msgs
roscpp
geometry_msgs
tf
tf_conversions
#qt_build
rosgraph_msgs
)

find_package(Eigen3 REQUIRED NO_MODULE)
message("${qt_gui_cpp_USE_QT_MAJOR_VERSION}")


find_package(Qt5Widgets REQUIRED)
 include_directories(${Qt5Widgets_INCLUDES})
 add_definitions(${Qt5Widgets_DEFINITIONS})
  
set(CMAKE_AUTORCC true) # to include qt resource collection
set(CMAKE_INCLUDE_CURRENT_DIR ON) # This finds ui_*.h



##############################################################################
# Qt Environment
##############################################################################



set(QT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/qt_src/px4_gcs)
set(QT_FORMS ${QT_DIR}/mainwindow.ui) # by adding this, the catkin build make ui_mainWindow.h
file(GLOB QT_MOC   ${QT_DIR}/*.h)

file(GLOB QT_SOURCES ${QT_DIR}/*.cpp)


qt5_wrap_cpp(QT_MOC_HPP ${QT_MOC})
qt5_wrap_ui(QT_FORMS_HPP ${QT_FORMS})



add_service_files(
  FILES
  InitHome.srv
  KeyboardInput.srv
  SwitchMode.srv
  )


## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs  # Or other packages containing msgs
  mavros_msgs
  nav_msgs
)


###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if your package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
 INCLUDE_DIRS include
  LIBRARIES px4_code
#  CATKIN_DEPENDS other_catkin_pkg
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
include
${catkin_INCLUDE_DIRS}
)


## Declare a C++ library
add_library(${PROJECT_NAME}
  src/MavWrapper.cpp  
 )


add_executable(mav_wrapper_node src/mav_wrapper_main.cpp src/MavWrapper.cpp)
target_link_libraries(mav_wrapper_node
  ${catkin_LIBRARIES} 
)

add_executable(mav_gcs_node ${QT_DIR}/main.cpp  ${QT_FORMS_HPP} ${QT_SOURCES} ${QT_MOC_HPP})
target_link_libraries(mav_gcs_node ${catkin_LIBRARIES}  Qt5::Widgets)


add_executable(tf2poseStamped_node src/tf_to_posestamped_node.cpp )
target_link_libraries(tf2poseStamped_node ${catkin_LIBRARIES} )


add_executable(poseStamped2tf_node src/posestamped_to_tf_node.cpp )
target_link_libraries(poseStamped2tf_node ${catkin_LIBRARIES} )


