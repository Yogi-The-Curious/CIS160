/*
  Le Tea Maker 3000

I present to you my masterpiece of the week! 
A servo with a rudimentary winch system that dips your teabag a predetermined amount of time 
I wanted to feel a little more sophisticated so I added a dht 11 sensor and a while loop 
so the tea maker will not run unless there is hot water in your cup,
I added a time condition into the while loop to cease the humidity checks after roughly 5 minutes.
eventually this will turn the whole arduino off until the next tea time. 

ideas for further improvement: 
1. It turns out tea bag strings are made with quite a varying length, and some with no string at all
and so I think adding a fixed chain with an alligator clip at the end will solve the issue
and add some consistency to the process

2. Adding a button will be necessary for practical use. 

3. perhaps connecting it to my pi, making classes for different teas (different steep times) 
and creating a program that will list options for teas I want to brew and then begin the process upon my input. 

4. if all the previous meet with success and time permits I will package it up, it seems I have almost infinite brews 
with my 6000mAh battery pack. I believe I saw a board for rather cheap that I could soder my equipment to, keeping the cost 
fairly low and making it reasonable to keep around. (dismantling my projects always is a little sad). 



  
I pulled from some of the basic arduino examples for 
stepper motor
dht sensor
blink without delay

many thanks to the endless forum posts and brilliant community surrounding arduino.

Ben Ruland

*/
// Include the Arduino Stepper.h library and dht.h library
#include <Stepper.h>
#include <dht.h>
#include <time.h>
//calling dht, dht is pin 3
dht DHT;

// DHT wired to pin 3
#define DHT11_PIN 3

  
// Define number of steps per rotation:
const int stepsPerRevolution = 2048;

// Wiring:
// Pin 8 to IN1 on the ULN2003 driver
// Pin 9 to IN2 on the ULN2003 driver
// Pin 10 to IN3 on the ULN2003 driver
// Pin 11 to IN4 on the ULN2003 driver

// Create stepper object called 'myStepper', note the pin order:
Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
  // Set the speed to  rpm:
  myStepper.setSpeed(15);
  
  // Begin Serial communication at a baud rate of 9600:
  Serial.begin(9600);
  //bring the tea bag up
  Serial.println("full up");
  myStepper.step(4000);
}

void loop() {
  //Here We wait untill the DHT11 tells us there is water in the cup before lowering the bag. 
  //We check every 7 seconds. And for current testing purposes our serial will read results

  while ( (DHT.humidity < 50 and DHT.temperature < 95)== true) {
    
    unsigned long currentMillis = millis();  // here we create a timer to end the function should we forget to make our tea.
    Serial.print("timer is at");
    Serial.print((300000 - currentMillis)/1000);
    Serial.println(" Seconds left");
    if (currentMillis > 300000) {
      Serial.print("function timed out, try again");
      //eventually add some code here to turn the arduino off, a push button is in order i think. 
      while(1);
    }  
    int chk = DHT.read11(DHT11_PIN);
    Serial.print("Temperature = ");
    Serial.println(DHT.temperature);
    Serial.print("Humidity = ");
    Serial.println(DHT.humidity);
    delay(7000);   
  }
  
  // Here we commence lowering the tea bag then steeping it. 
  // If I ever come back to this project I would simply create 
  // a function to do this.
  
  Serial.println("Full Down");
  myStepper.setSpeed(5);
  myStepper.step(-4000);
  delay(30000);

  Serial.println("Third up");
  myStepper.setSpeed(10);
  myStepper.step(2000);
  delay(2000);

  Serial.println("Third Down");
  myStepper.step(-2000); 
  delay(15000);

  Serial.println("Third up");
  myStepper.step(2000);
  delay(5000);

  Serial.println("Third Down");
  myStepper.step(-2000); 
  delay(15000);

  Serial.println("Third up");
  myStepper.step(2000);
  delay(5000);

  Serial.println("Third Down");
  myStepper.step(-2000);
  delay(120000);  

  //taking it out of the cup

  Serial.println("Full up");
  myStepper.step(4000);
  Serial.println("End");

  // forcing the pins low on the motor, as its detent torque is more then enough to hold a couple wet tea bags. 
  digitalWrite(8,LOW);
  digitalWrite(9,LOW);
  digitalWrite(10,LOW);
  digitalWrite(11,LOW);
  
  while(1); //it would be nice to find a way to simply sleep the arduino untill another event triggers it (say push of a button)
  // Im suspecting the key might be in the cave pearl logger code.  
}
