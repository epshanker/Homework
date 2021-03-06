/* 
 * File:   newmain.c
 * Author: epshanker
 *
 * Created on March 31, 2015, 10:57 PM
 */

#include<xc.h> // processor SFR definitions
#include<sys/attribs.h> // __ISR macro
#include"i2c_display.h"
#include"i2c_master_int.h"
#include"accel.h"

// DEVCFG0
    #pragma config DEBUG = OFF // no debugging
    #pragma config JTAGEN = OFF // no jtag
    #pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
    #pragma config PWP = OFF // no write protect
    #pragma config BWP = OFF // not boot write protect
    #pragma config CP = OFF // no code protect

// DEVCFG1
    #pragma config FNOSC = PRIPLL // use primary oscillator with pll
    #pragma config FSOSCEN = OFF // turn off secondary oscillator
    #pragma config IESO = OFF // no switching clocks
    #pragma config POSCMOD = HS // high speed crystal mode
    #pragma config OSCIOFNC = OFF // free up secondary osc pins
    #pragma config FPBDIV = DIV_1 // divide CPU freq by 1 for bus clock
    #pragma config FCKSM = CSDCMD // do not enable clock switch
    #pragma config WDTPS = PS1048576 // slowest wdt
    #pragma config WINDIS = OFF // no wdt window
    #pragma config FWDTEN = OFF // wdt off by default
    #pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the CPU clock to 40MHz
    #pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
    #pragma config FPLLMUL = MUL_20 // multiply clock after FPLLIDIV
    #pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 40MHz
    #pragma config UPLLIDIV = DIV_2 // divide 8MHz input clock
    #pragma config UPLLEN = ON // USB clock on

// DEVCFG3
    #pragma config USERID = 0 // some 16bit userid
    #pragma config PMDL1WAY = ON // not multiple reconfiguration, check this
    #pragma config IOL1WAY = ON // not multimple reconfiguration, check this
    #pragma config FUSBIDIO = OFF // USB pins controlled by USB module
    #pragma config FVBUSONIO = OFF // controlled by USB module

int readADC(void);
int pixel_display(char message,int counter);
int display_acc_x(int pixelNums[2]);
int display_acc_y(int pixelNums[2]);

int main() {
    // startup
    __builtin_disable_interrupts();

    // set the CP0 CONFIG register to indicate that
    // kseg0 is cacheable (0x3) or uncacheable (0x2)
    // see Chapter 2 "CPU for Devices with M4K Core"
    // of the PIC32 reference manual
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);
    
    // no cache on this chip!

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to be able to use TDI, TDO, TCK, TMS as digital
    DDPCONbits.JTAGEN = 0;

    acc_setup();
    __builtin_enable_interrupts();
    
    
    // OLED Display
    //display_init();
    //display_clear();
    //char message[100];
    //sprintf(message,"It works!!");
    //int counter=0;
    //while(message[counter]) {
    //    pixel_display(message[counter],counter);
    //    counter++;
    //}
    //display_draw();
    
    
    // OUTLINE FOR ACCELEROMETER BARS ON OLED (HW5)
    // initialize accelerometer, variables
/*    short accels[3], mags[3], temp;
    float gs[3];     // converted acceleration data to g's
    int pixelNums[2]; // number of pixels in OLED (only x and y will be displayed)
    int ii=0;
 * /
    
//    // while loop so that OLED is constantly being read/updated
//    while(1) {
//        // read the acc register for acceleration data
//            // 6 bytes of data (2 bites for each direction of acceleration)
//        acc_read_register(OUT_X_L_A,(unsigned char *) accels,6);
//        display_init();
//        display_clear();
//        for (ii=0;ii<3;ii++) {
//            gs[ii]=(float)accels[ii]/(float) 16384; 
//        }
//        pixelNums[0] = (float) accels[0]/(float)512;
//        pixelNums[1] = (float) accels[1]/(float)1024;
//        display_acc_x(pixelNums);
//        display_acc_y(pixelNums);
//        display_draw();
//    }
    
    
/*    // set up USER pin as input
    ANSELBbits.ANSB13 = 0; // sets pin B13 to digital input

    // set up LED1 as digital output
    TRISBbits.TRISB7 = 0;
    
    // Set LED2 as OC1 using Timer2 at 1kHz
    RPB15Rbits.RPB15R = 0b0101; // sets pin B15 to output compare
    T2CONbits.TCKPS = 0;		// sets pre-scaler to 1
	PR2 = 39999;				// sets maximum counter value
	TMR2 = 0;					// initializes timer 2
	OC1CONbits.OCM = 0b110;		// use without fail safe
	OC1RS = 2000;				// sets duty cycle to 50%
	OC1R = 2000;				// sets initial duty cycle to 50%
	T2CONbits.ON = 1;			// turns timer on
	OC1CONbits.ON = 1;			// turns output control on

    // Read the analog input
    ANSELAbits.ANSA0 = 1; // sets pin A0 to analog input
    AD1CON3bits.ADCS = 3;
    AD1CHSbits.CH0SA = 0;
    AD1CON1bits.ADON = 1;
    
    while (1) {
        _CP0_SET_COUNT(0); // set core timer to 0
        LATBINV = 0b10000000; // invert pin B7 for LED1

        // wait for half a second, setting LED brightness to pot angle
        while (_CP0_GET_COUNT() < 10000000) {
            int val;
            val = readADC();
            OC1RS = val * 39; // value/1023 * 400000 simplifies to about 39

            if (PORTBbits.RB13 == 1) {
                // nothing
            } else {
                LATBINV = 0b10000000; // invert LED1 as fast as possible
            }
        }
    }
*/    

    // Set pin B3 to OC1 @ 1kHz
    RPB3Rbits.RPB3R = 0b0101; // sets pin B15 to output compare
    T2CONbits.TCKPS = 0;		// sets pre-scaler to 1
	PR2 = 39999;				// sets maximum counter value
	TMR2 = 0;					// initializes timer 2
	OC1CONbits.OCM = 0b110;		// use without fail safe
	T2CONbits.ON = 1;			// turns timer on
	OC1CONbits.ON = 1;			// turns output control on
    
    // Set pin A1 to OC2 @ 1kHz
    RPA1Rbits.RPA1R = 0b0101;   // sets pin B7 to output compare
    T3CONbits.TCKPS = 0;		// sets pre-scaler to 1
	PR3 = 39999;				// sets maximum counter value
	TMR3 = 0;					// initializes timer 2
	OC2CONbits.OCM = 0b110;		// use without fail safe
	T3CONbits.ON = 1;			// turns timer on
	OC2CONbits.ON = 1;			// turns output control on
   
    // while loop for hard-coded PWM for motors
    // use LEDs for pwm test
    while(1) {
        if (_CP0_GET_COUNT()<80000000) {       // first 3 seconds
            // code for 25% pwm both wheels
            OC1RS = 10000;
            OC1R = 10000;
            OC2RS = 10000;
            OC2R = 10000;
        }
        else if (_CP0_GET_COUNT()<160000000) {
            // code for 50% pwm both wheels
            OC1RS = 20000;
            OC1R = 20000;
            OC2RS = 20000;
            OC2R = 20000;
        }
        else if (_CP0_GET_COUNT()<240000000) {
            // code for 100% pwm both wheels
            OC1RS = 39999;
            OC1R = 39999;
            OC2RS = 39999;
            OC2R = 39999;
        }
        else if (_CP0_GET_COUNT()<320000000) {
            // code for turn right (right pwm < left pwm)
            OC1RS = 39999;
            OC1R = 39999;
            OC2RS = 20000;
            OC2R = 20000;
        }
        else if (_CP0_GET_COUNT()<400000000) {
            // code for turn left (left pwm < right pwm)
            OC1RS = 20000;
            OC1R = 20000;
            OC2RS = 39999;
            OC2R = 39999;
        }
        else if (_CP0_GET_COUNT()>=400000000) {
            _CP0_SET_COUNT(0);
        }
    }
    
    
}

int readADC(void) {
    int elapsed = 0;
    int finishtime = 0;
    int sampletime = 20;
    int a = 0;

    AD1CON1bits.SAMP = 1;
    elapsed = _CP0_GET_COUNT();
    finishtime = elapsed + sampletime;
    while (_CP0_GET_COUNT() < finishtime) {
    }
    AD1CON1bits.SAMP = 0;
    while (!AD1CON1bits.DONE) {
    }
    a = ADC1BUF0;
    return a;
}

static const char ASCII[96][5] = {
 {0x00, 0x00, 0x00, 0x00, 0x00} // 20  (space)
,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
,{0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
,{0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
,{0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
,{0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
,{0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
,{0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
,{0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
,{0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
,{0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
,{0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
,{0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
,{0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
,{0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
,{0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
,{0x02, 0x04, 0x08, 0x10, 0x20} // 5c �
,{0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
,{0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
,{0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
,{0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
,{0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
,{0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
,{0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
,{0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
,{0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j
,{0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
,{0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
,{0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
,{0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
,{0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
,{0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
,{0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
,{0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
,{0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
,{0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
,{0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
,{0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
,{0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
,{0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
,{0x10, 0x08, 0x08, 0x10, 0x08} // 7e ?
,{0x00, 0x06, 0x09, 0x09, 0x06} // 7f ?
}; // end char ASCII[96][5]

int display_acc_x(int pixelNums[2]) {
    int startx = 64;
    int starty = 32;
    int mm,nn;
    if (pixelNums[0] >= 0) {
        for (mm=0;mm<pixelNums[0];mm++) {
            if ((startx+mm)<128) {
                display_pixel_set(starty,(startx-mm),1);
            }
        }
    }
    if (pixelNums[0] < 0) {
        for (nn=0;nn>pixelNums[0];nn--) {
            if (startx-nn>0) {
                display_pixel_set(starty,(startx-nn),1);
            }
        }
    }
}

int display_acc_y(int pixelNums[2]) {
    int startx = 64;
    int starty = 32;
    int oo,pp;
    if (pixelNums[1] >= 0) {
        for (oo=0;oo<pixelNums[1];oo++) {
            if ((starty+oo)<64) {
                display_pixel_set((starty-oo),startx,1);
            }
        }
    }
    if (pixelNums[1] < 0) {
        for (pp=0;pp>pixelNums[1];pp--) {
            if (starty-pp>0) {
                display_pixel_set((starty-pp),startx,1);
            }
        }
    }
}


int pixel_display(char message,int counter) {
        int ASCII_var, ASCII_array[5];
        int ii, jj, dummy[8][1] = {1, 1, 1, 1, 1, 1, 1, 1}, line, symbol[8], pixels[8][5];

        for (ii=0; ii<5; ii++) {
            ASCII_var = message - 0x20; // gets row number in ASCII
            ASCII_array[ii] = ASCII[ASCII_var][ii]; // gives one array of 5 hex numbers from ASCII

            line = ASCII_array[ii];
            for (jj=0; jj<8; jj++) { // for loop creates an array out of the hex value in ASCII_array
                pixels[jj][ii] = line & 1;
                line>>=1;
            }
        }

        int row, col;
        for (col=0; col<5; col++) {
            for (row=0; row<8; row++) {
                display_pixel_set(row,col+(counter*5),pixels[row][col]);
            }
        }
}