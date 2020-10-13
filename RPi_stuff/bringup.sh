#!/bin/bash

export ROS_IP=$(hostname -I | grep -o '192.168.[0-9].[0-9]*')
bash -c "source /home/ubuntu/catkin_ws/devel/setup.bash && roslaunch warshbot_driver minimal.launch"
