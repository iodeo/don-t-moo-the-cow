#include <LowPower.h>
#include <PCM.h>

#include "meuh_1.h"
#include "meuh_2.h"

void setup() {

  /******************************************/
  /*** Power reduction ***/
  
  ADCSRA = ADCSRA & B01111111; // Disable ADC
  ACSR = B10000000; // Disable analog comparator

  /******************************************/
  /*** IO definitions ***/
  
  pinMode(2, INPUT); // touch detection (PIN 2 = ISR 0)
  pinMode(6, OUTPUT); // vibration motor
  digitalWrite(6, LOW);
  // NB: Speaker is connected to PIN 11 (see PCM.c)

}

void wakeUp() {
  // just a handler for pin interrupt
}

void vibrate(byte times, int time_high, int time_low) {
  for (byte i = 0; i < times; i++) {
    digitalWrite(6, HIGH); delay(time_high);
    digitalWrite(6, LOW); delay(time_low);
  }
}

byte count = 0;

void loop() {

  // Define wake up interrupt from pin 2 on low trigger
  attachInterrupt(0, wakeUp, LOW); // 0 is for pin 2 on Atmega328P

  // Enter sleep mode
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);

  // On wake up, first detach interrupt
  detachInterrupt(0);

  // Play moo sound and vibrate
  if (count++ < 3) {
    //sequence #1
    startPlayback(meuh_1, sizeof(meuh_1));
    vibrate(4, 150, 150);
  }
  else {
    //sequence #2 (1 time over 3)
    count = 0;
    startPlayback(meuh_2, sizeof(meuh_2));
    vibrate(3, 400, 200);
  }  

  // Wait a bit before loop to sleep
  delay(500);
  
}

    
