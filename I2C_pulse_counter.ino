// James Hughes -2021
//I2C pulse counter for wind cup/anemometer
//Code not tested yet
//v 0.1


#include <TinyWireS.h>
#include <avr/io.h>
#include <avr/interrupt.h>

//6 pins available, I use 5
//PB2 is SCL
//PB0 is SDA

#define WIND_PIN PB5
#define RAIN_PIN PB3
#define LED_PIN PB4

#define I2C_SLAVE_ADDRESS 0x30

int i = 0;

//===============================================
// setup
//===============================================
void setup()
{
  TinyWireS.begin(I2C_SLAVE_ADDRESS); // join i2c network
  TinyWireS.onReceive(receiveEvent);
  TinyWireS.onRequest(requestEvent);

  //Interrupt pin for counter setup
  cli();
  PCMSK |= (1 << PCINT4) | (1 << PCINT5);   // Enable interrupt handler (ISR) for counter 0 and counter 1
  GIMSK |= (1 << PCIE);                     // Enable PCINT interrupt in the general interrupt mask
  pinMode(WIND_PIN, INPUT_PULLUP);
  pinMode(RAIN_PIN, INPUT_PULLUP);
  sei();

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
}

//===============================================
// I2C Write command
//===============================================
void receiveEvent(void)
{

}

//===============================================
// I2C Read command
//===============================================
void requestEvent(void)
{

}

//===============================================
// I2C Read command
//===============================================
void blinkLED(int count)
{
  int loop;
  for (loop = 0; loop < count; loop++)
  {
    digitalWrite(LED_PIN, LOW);
    delay(100);
    digitalWrite(LED_PIN, HIGH);
    delay(500);
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
  i++;
}
