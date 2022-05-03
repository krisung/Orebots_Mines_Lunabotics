# Lunabotics RMC - Mines Orebots
This repository contains all of the code used in the development and in the 
complete project for the Mines NASA Lunabotics RMC team.

This repository is managed and monitored by Kristen Ung.
kristen.ung23@gmail.com

The finalized programs for the project can be found in the finalPrograms
folder. The esp32Controller.ino program should be uploaded to the ESP32
controller via the Arduino IDE, while the GroundStation.py should be
run on the ground station computer. In order for the ground station to 
properly connect, the ESP32 needs to boot up first to create the port
for wireless connection.

The pinouts for the ESP32 can be found in the header of the esp32Controller
script, while the keyboard commands can be found in the header for the 
GroundStation script.

The files named "video reciever.py" and "Video Transmitter.py" are the python 
scripts to stream video from the cameras on the robot to the ground station. The
cameras on the robot use the file "Video Transmitter.py" and run on a Raspberry Pi 
and use a USB webcam. The Ground Station uses the file "video reciever.py". Both the scripts were written in Python 3.9 and in order to work should be run in the order of "video reciever.py" from the Ground Station followed by "Video Transmitter.py" being run on the robot.
