#include "mbed.h"
#include <cstdio>
#include "functions.h"
#include "TextLCD_I2C.h"



// variabelen en timer voor ultrasoonsensor
int afstand;
long duration;
Timer timer_ultrasoon;

// pins voor ultrasoonsensor
DigitalOut trigger(D3); // moet pwm poort zijn
DigitalIn echo(D4); 

int ultrasoon(){
     timer_ultrasoon.reset();  //berekening voor ultrasoonsensor
        duration = 0;

        trigger = 0;
        wait_us(2);
        trigger = 1;
    
        wait_us(10);
        trigger = 0;
    
        while(!echo);
        timer_ultrasoon.start();
    
        while(echo);
        timer_ultrasoon.stop();
    
        duration = timer_ultrasoon.read_us();
        afstand = (duration * 0.0340)/2;

        return afstand;
        }

        AnalogIn accu(A0);


float accupercentage() {
   accu.set_reference_voltage(3.3);
    while(true){
        float voltage = accu.read_voltage()*3.20481928;
        if(voltage<=7.980&&voltage>=7.500){
           return 95;
        }
        if(voltage<7.500&&voltage>=7.320){
            return 85;
        }
        if(voltage<7.320&&voltage>=7.308){
            return 75;
        }
        if(voltage<7.308&&voltage>=7.278){
            return 65;
        }
        if(voltage<7.268&&voltage>=7.248){
            return 55;
        }
        if(voltage<7.248&&voltage>=7.218){
            return 45;
        }
        if(voltage<7.218&&voltage>=6.714){
            return 35;
        }
        if(voltage<6.714){
            return 30;
        }
    }
}