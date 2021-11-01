//===============================================
// I2C Write command
//===============================================
void receiveEvent(void)
{
  command = 0;
  command = TinyWireS.read();

  switch (command)
  {
    case RESET_WINDCOUNT:
      windCount = 0;
      break;

    case RESET_RAINCOUNT:
      rainCount = 0;
      break;

    case RESET_MINWINDPERIOD:
      break;

    case LED_TOGGLE:
      LEDState = !LEDState;
      break;
  }
}

//===============================================
// I2C Read command
//===============================================
void requestEvent(void)
{
  switch (command)
  {
    case RD_WINDCOUNT:
      windCount++;
      TinyWireS.write((windCount >> 8) & 0xFF);
      TinyWireS.write(windCount & 0xFF);
      break;

    case RD_RAINCOUNT:
      rainCount++;
      TinyWireS.write((rainCount >> 8) & 0xFF);
      TinyWireS.write(rainCount & 0xFF);
      break;

    default:
      blinkLED(5);
  }
}
