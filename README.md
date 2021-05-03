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
It also assumes that the Python Package Installer (pip) is installed. It also assumes that you have a AWS account setup and ready to create cloud resources on. 
### Mbed
### Raspberry Pi
To run our project, OpenCV and AWS SDK must be installed on the Rpi. The steps below show the methods used to install and build these libraries:
#### OpenCV:
The version of OpenCV installed on the Rpi for this project is 4.2.0. To install OpenCV follow this guide by [Q-engineering](https://github.com/sahmed85/SmartSecuritySystem/blob/master/ECE%204180%20Final%20Project%20Architecture.png?raw=true) [1].
Once completing the guide, make sure to set the swap size back to its orginal value. The guide also includes methods to check if the installaton was correct using python. Once completed, we can use the Makefile inside the /MotionDetection directory to build the executable (details in the Code section).
### AWS Resources Setup:
Three AWS resources is needed: 1 S3 Bucket, 2 DynamoDB tables and 1 Identity and Access Management(IAM) User.   
To setup the S3 bucket navigate to the S3 console in aws.amazon.com. Create a new S3 bucket for hosting the static website and enable static website hosting for the bucket. Uncheck Block all public access under Bucket setting for Block Public Access section. The bucket name must be unique. Once created, go to the settings of the bucket and add a bucket policy shown below enable public access to the videos uploaded:. 
```
{
"Version": "2012-10-17",
"Statement": [
{
"Sid": "PublicReadGetObject",
"Effect": "Allow",
"Principal": "*",
"Action": "s3:GetObject",
"Resource": "arn:aws:s3:::mybucketname/*"
}
]
}
```
**Note that in a real application a bucket would not configured for Public Access.**

To setup the DynamoDB tables navigate to the DynamoDB console in aws.amazon.com. Create a table named *SmartSecurityUnlockEvents* using the setup wizard. Use all default settings in the setup. Repeat this step to create another table called *SmartSecurityVideoEvents*.   
To setup the IAM user, navigate to the IAM console in aws.amazon.com. Select the *User* from the dropdown on the left then click Add User. Following the setup wizard, name the user and make the Access Type *Programmatic Access*. Next under permission, click attach existing policies directly and search for *AmazonDynamoDBFullAccess* and *AmazonS3FullAccess* and click their respective check boxes. This will give our user full access to S3 resources and DynamoDB tables. In a real applicaton, IAM roles would use custom policies for a specific resources instead allowing full access. Next in the setup wizard, skip the tags section and create the user. The wizard will then show the Access Key ID and Secret Access Key that we will need to configure our AWS CLI and Flask Application. Store this in a safe place as the Secret Key will only be accessible here.
### AWS Command Line Interface (CLI) Setup:
The AWS CLI must be setup to configure our AWS acceess key, secret access key and region. This is an important part as it will allow us to access the AWS API in our C++ code to upload object to S3 and put items in DynamoDB. The AWS SDKs and CLIs use *provider chains* to look for AWS credentials in several different places, including system/user environment variables and local AWS configuration files. For details, see [Credentials Providers](https://github.com/aws/aws-sdk-cpp/blob/master/Docs/Credentials_Providers.md) in the aws-sdk-cpp repository in GitHub.
To install the AWS CLI, use pip install command in your Rpi terminal:
```
$ sudo pip install awscli
```
Verify that is installed and working:
```
$ aws --version
```
Next use the AWS configure command in the terminal to insert the Access Key, Secret Access Key from the IAM Role that we created earlier [2]:
```
$ aws configure
AWS Access Key ID [None]: AKIAIOSFODNN7EXAMPLE
AWS Secret Access Key [None]: wJalrXUtnFEMI/K7MDENG/bPxRfiCYEXAMPLEKEY
Default region name [None]: us-east-1
Default output format [None]: json
```
#### AWS C++ SDK Setup:
The lastest version of the AWS C++ SDK needs to be installed and built to be used in our project. To install and build the AWS SDK follow this guide provided by [AWS](https://docs.aws.amazon.com/es_es/sdk-for-cpp/v1/developer-guide/setup-linux.html) [3]. **Note** that we do not need to build all parts of the SDK, instead we only need S3 and DynamoDB. **Tweak the cmake command in the AWS guide to**:
```
$ cmake -DCMAKE_BUILD_TYPE=Debug -D BUILD_ONLY="s3;dynamodb"
```
Follow the rest of the guide to successfully install and build the SDK.
## Code
### Mbed 
### Rpi Serial Communication with Mbed

### Rpi Motion Detection
### Rpi AWS S3 Put Object
### Rpi AWS DynamoDB Put Item
### Flask Web App
## Demo
[![IMAGE ALT TEXT](http://img.youtube.com/vi/1eew0ciX4P0/0.jpg)](http://www.youtube.com/watch?v=1eew0ciX4P0 "Project Demo")
## References
[1] OpenCV 4.2.0 Installation Guide: https://qengineering.eu/install-opencv-4.1-on-raspberry-pi-4.html  
[2] AWS CLI Configureation Basics: https://docs.aws.amazon.com/cli/latest/userguide/cli-configure-quickstart.html   
[3] AWS C++ SDK Setup for Linux: https://docs.aws.amazon.com/es_es/sdk-for-cpp/v1/developer-guide/setup-linux.html
