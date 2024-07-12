#include "Wire.h"
#include <MPU6050_light.h>
#include <Servo.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#define MPU6050_REG_GYRO_XOUT_H 0x43
volatile int obstacle_detected = 0;
static volatile int pulse = 0;
static volatile int i = 0;
int counter, pre_count,turn_count,pre_turn_count;
int dis_right, dis_left,turning,state,state_turn, pre_state_turn;
int distance;
Servo myServo;

MPU6050 mpu(Wire);
unsigned long timer = 0;
void ultrasonic_trigger() {


    //message for debugging purposes
    //uart_tx_str("Triggering HC-SR04...\n");


    //set TRIG_PIN (PB3) to high
    PORTB |= (1 << PB3);


    // Wait for a short duration (10 microseconds minimum)
    //the datasheet confirms 10us
    _delay_us(12);


    // Set TRIG_PIN (PB3) low
    PORTB &= ~(1 << PB3);


   
}
void servo_rotate(int angle) {
    // Calculate OCR1A value for given angle (0 to 180 degrees)
   
    OCR0A = angle;
}
void setup() {
    Serial.begin(9600);
    Wire.begin();
    byte status = mpu.begin();
    while (status != 0) {}  // stop everything if could not connect to MPU6050
    Serial.println(F("Calculating offsets, do not move MPU6050"));
    delay(1000);
    mpu.calcOffsets();  // gyro and accelero

    DDRD |= (1 << PD5); // Set PD5 as output for PWM
DDRB |= (1 << PB3);
    // Configure Timer0 for PWM generation
    TCCR0A |= (1 << COM0B1) | (1 << WGM00) | (1 << WGM01); // Fast PWM mode, clear OC0B on compare match
    TCCR0B |= (1 << CS01); // Set prescaler to 8

DDRD |= (1 << PD4); // Set PD4 as output for fan

    // Configure Timer0 for PWM generation
    PORTD |= (1 << PD4);
    myServo.attach(6); // Attach servo to pin 6
OCR0B = 255;
}

void loop() {
    mpu.update();
    double gyroZ_deg = mpu.getAngleZ();
    myServo.write(90 + gyroZ_deg);

    // Map gyro angle to PWM duty cycle (adjust as needed)
    int dutyCycle = map(gyroZ_deg, -90, 90, 0, 255);
   // OCR0B = 255; // Set PWM duty cycle
ultrasonic_trigger();
   _delay_ms(20);
delay(100);
ultrasonic_trigger();
    uint16_t pulse_width = pulseIn(2, 1);
     distance = (pulse_width * 0.0343) / 2;
 Serial.println(gyroZ_deg);
//Serial.println("fgthtyhj");

/*Serial.println(yawAngle);
   if (yawAngle > 180) {
            yawAngle -= 360;
        } else if (yawAngle < -180) {
            yawAngle += 360;
        }*/


if(counter <= 5 && distance < 21){ // sensor always displays 0 as a distance value at the begingging


  distance = 100;
}



        // Ensure yaw angle stays within range [-180, 180]
        /*if (yawAngle > 180) {
            yawAngle -= 360;
        } else if (yawAngle < -180) {
            yawAngle += 360;
        }*/
/*
if(  servo_angle < double(90 -69.5) ){
 OCR0A = 16;
_delay_ms(1500);
 OCR0A = 21;

 _delay_ms(500);
}else if( servo_angle > (90 -67)){
 //OCR0A = 26;
//_delay_ms(1000);
 //OCR0A = 21;
 //_delay_ms(500);
}*/
//if(state_turn ==0){


/*if(  servo_angle < (90 -73) ){
 
 // OCR0A = servo_angle -69;
  //v = v+0.01;
OCR0A =  18 ;
_delay_ms(200);
OCR0A =  21 ;
_delay_ms(5);
}else if( servo_angle > (90 - 64)){

 // v = v -0.01;
  OCR0A =  25;
_delay_ms(200);
  OCR0A =  21 ;
_delay_ms(5);
}*/

 //v =21;
 //OCR0A = 21;
//}

//if(state_turn ==0 || pre_state_turn==0){
 // IMU
/*if(servo_angle == 22 || servo_angle == 21){
OCR0A = 21;
_delay_ms(150);
}
else if(servo_angle < 22){
OCR0A = servo_angle/1.1;
_delay_ms(300);
}
else if(servo_angle > 22 && servo_angle < 32){
OCR0A = 1.1*servo_angle ;
_delay_ms(300);
}*/
//}


if ( distance < 36) { // 15CM
    //    obstacle_detected = 1;
    if(distance <36 && pre_count == (counter-1) ){
   
    //break out of 2nd outer if statement6
   
   pre_state_turn =1;
  //  turn_count+=2;
    }//else{
    //  turn_count++;
    //}






   if(state ==0 && turn_count ==0){
    //servo_rotate(0); // pre left
      myServo.write(180);
    // OCR0B = 0;
     //OCR1A = 0;
     //PORTD &= ~(1<<PD4);
    dis_right = distance;
    /*if(dis_right < 3){
ultrasonic_trigger();
delay(50);
dis_right = distance;

    }*/
    delay(2000);
    //OCR0B = 255;
    //_delay_ms(TURN_DELAY+7000);
    pre_count = counter;
    //Serial.println(pre_count);
   
   state_turn=1;
    //state =2;
 Serial.println("left");
    }
}
 
   if(state_turn==1 || pre_state_turn ==1){
 turn_count++;
   }
//ultrasonic_trigger();
//delay(100);
_delay_ms(50);
if(turn_count >=2){
if(pre_state_turn ==1 && distance < 36){
//servo_rotate(37); // pre right
 myServo.write(0);
 dis_left = distance;
pre_count = 0;
/*if(dis_left < 3){
ultrasonic_trigger();
delay(50);
dis_left = distance;

    }*/
     //delay(5);
    state =1;
pre_state_turn = 0;

Serial.println("right");
}
Serial.println("difjriughrgt");


_delay_ms(3500);
counter =0;
OCR0B = 255;
//_delay_ms(500);
//OCR1A = 187;
PORTD |= (1<<PD4);
//delay(1500);
/*if(servo_angle > 18 || servo_angle < 24 ){

//_delay_ms(1750);
}*/



 /* if(gyro_z == -1 ){


    _delay_ms(700);
  }*/
 



distance =100;
servo_rotate(21);
 mpu.update();
    double gyroZ_deg = mpu.getAngleZ();
 myServo.write(90);
delay(1450);
_delay_ms(1450);
OCR0B = 255;


/*for(int i=0;i<9999;i++){


  OCR0B = 187;
_delay_us(10);
if(i > 4000){
OCR1A = 200;
_delay_us(10);
}
}*/


//servo_rotate(21);
state =0;
turn_count =0;
state_turn =0;
}


 
   state =0;


  //}
   //state_turn++;
    //else {
      //  obstacle_detected = 0;
    //}


        //PORTD |= (1<<PD4);
       
       /*
        if (obstacle_detected && counter !=0) {
            // Turn right (90 degrees)
            _delay_ms(TURN_DELAY);
           // OCR0B = 0; // P3
            servo_rotate(0);
           _delay_ms(6000);
           ultrasonic_trigger();
           dis_left = distance;
           Serial.println(dis_left);
           _delay_ms(6000);
ultrasonic_trigger();
            OCR0B = 0; // P3
           dis_left = distance;
            Serial.println(dis_left);
            _delay_ms(TURN_DELAY+2000);


            // Check again for obstacle
           
            OCR0B = 0; // P3
            //_delay_ms(2500);
           
             Serial.println("ifjeufriegurgiu ");
           //OCR0B = 187; // P3
           //dis_right = count_a;
         /*  if ( distance < 15) { // 15CM
        obstacle_detected = 1;
    } else {
        obstacle_detected = 0;
    }
             Serial.println(distance);
            _delay_ms(1000);
            Serial.println(obstacle_detected);
            if ((dis_left < 15) && counter !=0) {
                // Turn left (90 degrees)
                OCR0B = 0; // P3
                servo_rotate(35); //180
               // OCR0B = 255; // P3
                _delay_ms(3000);
             OCR0B = 187; // P3
           _delay_ms(10);
            //dis_left = count_a;
            obstacle_detected = 0;
           
            }else if(dis_left >=15){
              OCR0B = 187; // P3
               _delay_ms(10);
             servo_rotate(10);
              OCR0B = 255; // P3
            }
 //OCR0B = 0; // P3
//_delay_ms(2000);
OCR0B = 255; // P3


           
           _delay_ms(2000);
           // OCR0B = 187; // P3
            // Return to forward position
            servo_rotate(20);
        }*/
if(turning){




}


counter++;








}