// James Hughes -2021
//I2C pulse counter for wind cup/anemometer
//Code not tested yet
//v 0.1


#include <TinyWireS.h>
#include <avr/io.h>
#include <avr/interrupt.h>

//6 pins available, I use 4
//PB2 is SCL
//PB0 is SDA
#define INTERRUPT_PIN PCINT1          // PB1
#define INT_PIN PB1                   // PB1
#define LED_PIN PB4                   // PB4

#define I2C_SLAVE_ADDRESS 0x30

int i = 0;

void setup()
{
  TinyWireS.begin(I2C_SLAVE_ADDRESS); // join i2c network
  //TinyWireS.onReceive(receiveEvent); // not using this
  TinyWireS.onRequest(requestEvent);

  //Interrupt pin for counter setup
  cli();
  PCMSK |= (1 << INTERRUPT_PIN);    // Enable interrupt handler (ISR) for our chosen interrupt pin (PCINT1/PB1/pin 6)
  GIMSK |= (1 << PCIE);             // Enable PCINT interrupt in the general interrupt mask
  pinMode(INT_PIN, INPUT_PULLUP);
  sei();

  // Turn on LED when program starts
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
}

void loop()
{
  // This needs to be here
  TinyWireS_stop_check();
}

// Gets called when the ATtiny receives an i2c request
void requestEvent()
{
  TinyWireS.write(i);
  //Reset accumulated counter after read
  i = 0;
}

//===================================
//ISR
//===================================
ISR(PCINT_VECTOR)
{
  //software debounce TBD
  int current = millis();
  //or possible simple delay to see if we can keep busy
  digitalWrite(LED_PIN, LOW);
  delay(30);
  digitalWrite(LED_PIN, HIGH);
  i++;
}
