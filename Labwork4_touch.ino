//
//    FILE: HTU21_getHumidityCompensated.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo
//     URL: https://github.com/RobTillaart/SHT2x

#include "Wire.h"
#include "SHT2x.h"
#include "BluetoothSerial.h"

HTU21 sht;

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

const int touchPin = 2;
const int ledPin = 16;
const int threshold = 300; // Need to be tuned
int touchValue;
int led_logic = 0;

void setup()
{
  Serial.begin(115200);
  SerialBT.begin("C2R2_TOUCH_TEMP"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  Serial.println();
  Serial.println(__FILE__);
  Serial.print("SHT2x_LIB_VERSION: \t");
  Serial.println(SHT2x_LIB_VERSION);
  Serial.println();
  Wire.begin();
  sht.begin();
  uint8_t stat = sht.getStatus();
  Serial.print(stat, HEX);
  Serial.println();
  pinMode (ledPin, OUTPUT);
}

void loop()
{
  touchValue = touchRead(touchPin);
  if (touchValue <= threshold)
  {
    led_logic =! led_logic;
    digitalWrite(ledPin, HIGH);
    sht.read();
    Serial.print(touchValue);
    Serial.print(",");
    Serial.print(sht.getTemperature(), 1);
    Serial.print(",");
    Serial.print(sht.getHumidity(), 1);
    Serial.print("\n");
    if (SerialBT.connected()) SerialBT.printf("TOUCH detected!!\n");
  }
  else
  {
    digitalWrite(ledPin, LOW);
    Serial.print("No TOUCH detected\n");
  }
  delay(500);
}