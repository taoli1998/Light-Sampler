#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <unistd.h> 

#include "joystick.h"
#include "function.h"

#define Joystick_X "/sys/bus/iio/devices/iio:device0/in_voltage2_raw"
#define Joystick_Y "/sys/bus/iio/devices/iio:device0/in_voltage3_raw"
#define A2D_MAX_READING 4095
#define A2D_VOLTAGE_REF_V 1.8

void Joystick_start(void){
    double x;
    double y;
    joystick_readXY(&x,&y);
}

static double Convert_vol(int read){
    double voltage = ((double)read/ A2D_MAX_READING);
    return (voltage - 0.5)*2;
}

void joystick_readXY(double *x, double *y){
    int xread = getVoltage0Reading(Joystick_X);
    int yread = getVoltage0Reading(Joystick_Y);
    *x = Convert_vol(xread)*(-1) ;
    *y = Convert_vol(yread)*(-1) ;
}