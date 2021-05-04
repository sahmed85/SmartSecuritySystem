/*
Copyright (c) 2011 Anthony Buckton (abuckton [at] blackink [dot} net {dot} au)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <mbed.h>
#include <string>
#include <list>

#include <mpr121.h>

#include "rgb.h"

RGBLed RGBLed(p23, p22, p21);
DigitalOut lock(p24);

// Create the interrupt receiver object on pin 26
InterruptIn interrupt(p26);

RawSerial  pi(USBTX, USBRX);

// Setup the i2c bus on pins 28 and 27
I2C i2c(p9, p10);

// Setup the Mpr121:
// constructor(i2c object, i2c address of the mpr121)
Mpr121 mpr121(&i2c, Mpr121::ADD_VSS);

static int password[4] = {0,1,2,3};
int buffer[4] = {-1,-1,-1,-1};
volatile int idx = 0;
int blinkLED = 0;
int unlock = 0;
Timer keyPadTimer;

void fallInterrupt()
{
    int key_code=0;
    int i=0;
    int value=mpr121.read(0x00);
    value +=mpr121.read(0x01)<<8;

    i=0;
    for (i=0; i<11; i++) {

        if (((value>>i)&0x01)==1) key_code=i+1;
    }
    if(key_code != 0) {
        buffer[idx] = key_code - 1;
        if(idx < 3)
            idx++;
        else if(buffer[0] == password[0] && buffer[1] == password[1]
                && buffer[2] == password[2] && buffer[3] == password[3]) {

            idx = 0;
            blinkLED = 0;
            unlock = 1;
        } else {
            idx = 0;
            blinkLED = 1;
        }

        keyPadTimer.reset();
        keyPadTimer.start();
    }

}

int main()
{
    myRGBled.write(1.0,0.0,0.0);
    lock = 0;
    interrupt.fall(&fallInterrupt);
    interrupt.mode(PullUp);

    while (1) {
        if ((keyPadTimer.read() >= 3 || blinkLED) && !unlock) {
            keyPadTimer.stop();
            for(int i=0; i < 3; i++) {
                myRGBled.write(0.0,0.0,0.0);
                wait(0.2);
                myRGBled.write(1.0,0.0,0.0);
                wait(0.2);
            }
            blinkLED = 0;
            idx = 0;
            keyPadTimer.reset();
        } else if (unlock) {
            keyPadTimer.stop();
            keyPadTimer.reset();
            lock = 1;
            pi.putc('1');
            myRGBled.write(0.0,1.0,0.0);
            wait(2);
            myRGBled.write(1.0,0.0,0.0);
            lock = 0;
            pi.putc('0');
            unlock = 0;
            blinkLED = 0;
        }
    }
}



