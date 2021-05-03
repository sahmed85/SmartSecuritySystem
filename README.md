# Smart Security System

## Team Members:
  * Shadman Ahmed - Computer Engineering - Georgia Tech
  * Momen Yacoub - Computer Engineering -  Georgia Tech

## Project Description and Architecture:
[INSERT PROJECT DESCRIPTION]  
The overall project architecture can be seen here:
![ECE 4180 Final Project Architecture](https://github.com/sahmed85/SmartSecuritySystem/blob/master/ECE%204180%20Final%20Project%20Architecture.png?raw=true)

## Parts List

## Schematic and Connections

## Setup
This setup assumes that the mbed is on the latest firmware. It also assumes that Raspian OS is installed on your Rpi with the latest package updates using:
```
$ sudo apt-get update
$ sudo apt-get upgrade 
```
It also assumes that the Python Package Installer (pip) is installed.
### Mbed
### Raspberry Pi
To run our project, OpenCV and AWS SDK must be installed on the Rpi. The steps below show the methods used to install and build these libraries:

#### OpenCV:
The version of OpenCV installed on the Rpi for this project is 4.2.0. To install OpenCV follow this guide by [Q-engineering](https://github.com/sahmed85/SmartSecuritySystem/blob/master/ECE%204180%20Final%20Project%20Architecture.png?raw=true) [1].

Once completing the guide, make sure to set the swap size back to its orginal value. The guide also includes methods to check if the installaton was correct using python. Once completed, we can use the Makefile inside the /MotionDetection directory to build the executable (details in the Code section).

### AWS Command Line Interface (CLI) Setup:
The AWS CLI must be setup to configure our AWS acceess key, secret access key and region. This is an important part as it will allow us to access the AWS API in our C++ code to upload object to S3 and put items in DynamoDB.

To install the AWS CLI, use pip install command in your Rpi terminal:
```
$ sudo pip install awscli
```
Verify that is installed and working:
```
$ aws --version
```
Next use the AWS configure command in the terminal to insert the Access Key, Secret Access Key, region name [2]:
```
$ aws configure
AWS Access Key ID [None]: AKIAIOSFODNN7EXAMPLE
AWS Secret Access Key [None]: wJalrXUtnFEMI/K7MDENG/bPxRfiCYEXAMPLEKEY
Default region name [None]: us-west-2
Default output format [None]: json
```

#### AWS SDK Setup:

### AWS Resources Setup:



## Code
### Mbed 
### Rpi Serial Communication with Mbed
### Rpi Motion Detection
### Rpi AWS S3 Put Object
### Rpi AWS DynamoDB Put Item

## Demo

## Future Work

## References
[1] OpenCV 4.2.0 Installation Guide: https://qengineering.eu/install-opencv-4.1-on-raspberry-pi-4.html  
[2] AWS CLI Configureation Basics: https://docs.aws.amazon.com/cli/latest/userguide/cli-configure-quickstart.html
