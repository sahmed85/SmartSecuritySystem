#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <stdlib.h>
#include <cstring>
using namespace std::chrono;
using namespace std;

uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

 
int main(int argc, char ** argv) {
  int fd;
  char buf[256];
  int n;
  // Open the Port. We want read/write, no "controlling tty" status, 
  // and open it no matter what state DCD is in
  fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);  
  //open mbed's USB virtual com port
  if (fd == -1) {  
    perror("open_port: Unable to open /dev/ttyACM0 - ");
    return(-1);
  }
  // Turn off blocking for reads, use (fd, F_SETFL, FNDELAY) if you want that
  fcntl(fd, F_SETFL, 0);
  //Linux Raw serial setup options
  struct termios options; 
  tcgetattr(fd,&options);   //Get current serial settings in structure
  cfsetspeed(&options, B9600);   //Change a only few
  options.c_cflag &= ~CSTOPB;
  options.c_cflag |= CLOCAL;
  options.c_cflag |= CREAD;
  cfmakeraw(&options);
  tcsetattr(fd,TCSANOW,&options);    //Set serial to new settings
  sleep(1);
  // Write to the port
  n = write(fd,"Hello mbed\r",11);
  if (n < 0) {
    perror("Write failed - ");
    return -1;
  }
  // Read the characters from the port if they are there
  sleep(2);
  n = read(fd, buf, 11);
  if (n < 0) {
    perror("Read failed - ");
    return -1;
  } else if (n == 0) printf("No data on port\n");
  else {
    buf[n] = 0;
    printf("%i bytes read back: %s\n\r", n, buf);
  }
  sleep(1);
  //Send command to blink mbed led 10 times at one second rate
  //mbed code turns on led2 with a '1' and off with a '0'
  //mbed echoes back each character
  buf[1] = '\r'; // end of line
  buf[2] = 0; // end of string
  uint64_t time = 0;
  string sys_call = "";
  char* char_sys_call;
  while(true) {
      read(fd,buf,1); //read echo character
      printf("%s\n\r",buf);   //print in terminal window
      if(buf[0] == '1'){
		time = timeSinceEpochMillisec();
		sys_call = "/home/pi/dynamoDBPut/build/app SmartSecurityUnlockEvents unlock timestamp=" + to_string(time) + " &";
		char_sys_call = &sys_call[0];
		system(char_sys_call);
		std::cout<<"Unlock Event at "<< time << std::endl;
		
	  }
	  else if(buf[0] == '0'){
		std::cout<<"Locked"<<std::endl;
	  }
      sleep(2.5);
  }
  // Don't forget to clean up and close the port
  tcdrain(fd);
  close(fd);
  return 0;
}
