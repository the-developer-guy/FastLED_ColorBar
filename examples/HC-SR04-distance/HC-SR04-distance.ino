/*
 * The Developer Guy
 * Example video with a 16 LED ring:
 * https://www.youtube.com/watch?v=3qkVAp3Qvqo
 * 
 * Other videos, tutorials:
 * https://www.youtube.com/thedeveloperguy
 * Also, the latest version of this on GitHub:
 * https://github.com/the-developer-guy/
 * If you like my work, please consider supporting:
 * (not just me, independent content creators in general)
 * https://www.patreon.com/thedeveloperguy
 * https://www.paypal.me/thedeveloperguy
 * 
 * 
 * A basic demo with a HC-SR04 ultrasonic sensor and Neopixels.
 * For different programmable LEDs, 
 * consult the FastLED library and change in my code.
*/
#include "tdg-ColorBar.h"

// fixed-point math 
// Speed of sound: 0,0343 cm / s
// 0,0343 0,0686  0,1372  0,2744  0,5488  1,0976  2,1952  4,3904  8,7808  17,5616 35,1232 70,2464 140,4928  280,9856  561,9712  1123,9424
#define SPEED_OF_SOUND_MULTIPLIER (1124)
#define SPEED_OF_SOUND_SHIFT (16)
#define DISTANCE_MEASUTEMENT_SCHEDULE_MS  (100)
 
void triggerDistance(void);
void isr_DistanceMeter(void);

const int trigPin = 7;
const int echoPin = 2;
const int ledCount = 16;
const int ledPin = 3;
uint16_t minDistance = 10;
uint16_t maxDistance = 300;
bool measurementFinished = false;

unsigned long startTimestamp;
unsigned long endTimestamp;
unsigned long distance;
unsigned long triggerSchedule = 0;

ColorBar colorBar(ledCount, ledPin, minDistance, maxDistance);
 
void setup() {
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  pinMode(2, INPUT);
  Serial.begin(115200);
  delay(1000);
  attachInterrupt(digitalPinToInterrupt(2), isr_DistanceMeter, CHANGE);
  triggerDistance();
}
 
void loop() {
  if(triggerSchedule <= millis())
  {
    triggerDistance();
    triggerSchedule = millis() + DISTANCE_MEASUTEMENT_SCHEDULE_MS;    
  }
 
  if(measurementFinished == true)
  {
    measurementFinished = false;
    distance = calculateDistanceCm(endTimestamp - startTimestamp);
    colorBar.setDistance(distance);
    colorBar.display();
    Serial.print("Distance: ");
    Serial.println(distance);
  }
}
 
void triggerDistance(void)
{
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
}
 
unsigned long calculateDistanceCm(unsigned long duration)
{
  return (duration * SPEED_OF_SOUND_MULTIPLIER) >> SPEED_OF_SOUND_SHIFT;
}
 
void isr_DistanceMeter(void)
{
  if(digitalRead(2) == HIGH)
  {
    measurementFinished = false;
    startTimestamp = micros();
  }
  else
  {
    endTimestamp = micros();
    measurementFinished = true;
  }  
}
