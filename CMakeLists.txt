cmake_minimum_required(VERSION 2.8.3)
project(px4_code)

add_compile_options(-std=c++11)
set(CMAKE_BUILD_TYPE Release)
find_package(catkin REQUIRED COMPONENTS 
message_generation
nav_msgs
mavros_msgs
roscpp
geometry_msgs
tf
qt_build
)

##############################################################################
# Qt Environment
##############################################################################
set(CMAKE_AUTOUIC ON) 
# this comes from qt_build's qt-ros.cmake which is automatically 
# included via the dependency call in manifest.xml
rosbuild_prepare_qt4(QtCore QtGui) # Add the appropriate components to the component list here
set(QT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/qt_src/px4_gcs)
set(QT_FORMS ${QT_DIR}/mainwindow.ui) # by adding this, the catkin build make ui_mainWindow.h
file(GLOB QT_MOC   ${QT_DIR}/*.h)
file(GLOB QT_SOURCES ${QT_DIR}/*.cpp)
QT4_WRAP_UI(QT_FORMS_HPP ${QT_FORMS})
message("qt form: ${QT_DIR}")
QT4_WRAP_CPP(QT_MOC_HPP ${QT_MOC})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
message("qt form path: ${CMAKE_CURRENT_BINARY_DIR}")

add_service_files(
  FILES
  InitHome.srv)


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
#  LIBRARIES px4_code
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

add_executable(mav_wrapper_node src/mav_wrapper_main.cpp src/MavWrapper.cpp)
target_link_libraries(mav_wrapper_node
  ${catkin_LIBRARIES} 
)

add_executable(mav_gcs_node ${QT_DIR}/main.cpp  ${QT_FORMS_CPP} ${QT_SOURCES} ${QT_MOC_HPP})
target_link_libraries(mav_gcs_node ${catkin_LIBRARIES} ${QT_LIBRARIES})

