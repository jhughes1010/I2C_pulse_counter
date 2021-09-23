// James Hughes -2021
//Thomas Krebs

//I2C pulse counter for wind cup/anemometer
//Code not tested yet
//v 0.1


#include <TinyWireS.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "defines.h"

//6 pins available, I use 5
//PB2 is SCL
//PB0 is SDA

#define WIND_PIN PB5
#define RAIN_PIN PB3
#define LED_PIN PB4

#define I2C_SLAVE_ADDRESS 0x30

volatile uint8_t command = 0;
volatile uint16_t windCount = 0x3c74;
volatile uint16_t rainCount = 0xaa55;

//===============================================
// setup
//===============================================
void setup()
{
  TinyWireS.begin(I2C_SLAVE_ADDRESS); // join i2c network
  TinyWireS.onReceive(receiveEvent);
  TinyWireS.onRequest(requestEvent);

  /*
    //Interrupt pin for counter setup
    cli();
    PCMSK |= (1 << PCINT4) | (1 << PCINT5);   // Enable interrupt handler (ISR) for counter 0 and counter 1
    GIMSK |= (1 << PCIE);                     // Enable PCINT interrupt in the general interrupt mask
    pinMode(WIND_PIN, INPUT_PULLUP);
    pinMode(RAIN_PIN, INPUT_PULLUP);
    sei();
  */

  // Turn on LED when program starts
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
}

//===============================================
// loop
//===============================================
void loop()
{
  // This needs to be here
  TinyWireS_stop_check();
  blinkLED(2);
  delay(1000);
}

//===============================================
// I2C Write command
//===============================================
void receiveEvent(void)
{
  command = 0;
  command = TinyWireS.read();
}

//===============================================
// I2C Read command
//===============================================
void requestEvent(void)
{
  switch (command)
  {
    case RD_WINDCOUNT:
      TinyWireS.write(windCount >> 8 & 0xFF);
      TinyWireS.write(windCount & 0xFF);
      break;

    case RD_RAINCOUNT:
      TinyWireS.write(rainCount >> 8 & 0xFF);
      TinyWireS.write(rainCount & 0xFF);
      break;

    case RESET_WINDCOUNT:
      windCount = 0;
      break;

    case RESET_RAINCOUNT:
      rainCount = 0;
      break;

    case RESET_MINWINDPERIOD:
      break;

    default:
      blinkLED(5);
  }
}

//===============================================
// Blink LED
//===============================================
void blinkLED(int count)
{
  int loop;
  for (loop = 0; loop < count; loop++)
  {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(400);
  }
}

//===============================================
//ISR
//===============================================
ISR(PCINT_VECTOR)
{
  //determine interrupt pin mask that fired ISR

  //software debounce TBD
  int current = millis();
  //or possible simple delay to see if we can keep busy
  delay(10);
}
