/*
  Software serial multple serial test

  Receives from the hardware serial, sends to software serial.
  Receives from software serial, sends to hardware serial.

  The circuit:
   RX is digital pin 2 (connect to TX of other device)
   TX is digital pin 3 (connect to RX of other device)

  Note:
  Not all pins on the Mega and Mega 2560 support change interrupts,
  so only the following can be used for RX:
  10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

  Not all pins on the Leonardo support change interrupts,
  so only the following can be used for RX:
  8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

  created back in the mists of time
  modified 25 May 2012
  by Tom Igoe
  based on Mikal Hart's example

  This example code is in the public domain.

*/
#include <SoftwareSerial.h>
#include <Crc16.h>
//Crc 16 library (XModem)
Crc16 crc;

byte Modbus_buf[50];
SoftwareSerial RS485(5, 6); // RX, TX

void setup() {
  Serial.begin(9600);
  RS485.begin(9600);
}

void loop() // run over and over
{
  //  byte Modbus_req1[] = {0x01, 0x04, 0x00, 0x1E, 0x00, 0x02};
  //  unsigned short value = crc.Modbus(Modbus_req1,0,6);
  //  byte c1 = value & 0xFF ;
  //  byte c2 = value >> 8;
  //  byte Modbus_req[] = {0x01, 0x04, 0x00, 0x1E, 0x00, 0x02,c1,c2};
  //  for (int i = 0; i < 8; i++) {
  //    Serial.print(Modbus_req[i], HEX);
  //    Serial.print(" ");
  //  }
  //  Serial.println("");
  //
  //  RS485.write(Modbus_req, sizeof(Modbus_req));
  //  RS485.flush();
  //  // if (RS485.available())
  //  RS485.readBytes(Modbus_buf, sizeof(Modbus_buf));
  byte result[10];
  result[10] = transmission(0x01, 0x04, 0x00, 0x1E, 0x00, 0x02);
  for (int i = 0; i < 15; i++)
    Serial.print(result[i], HEX);
  delay(5000);
}



byte transmission(byte slave_add, byte func_code, byte add_hi, byte add_lo, byte point_hi, byte point_lo) {

  byte Modbus_req1[] = {slave_add, func_code, add_hi, add_lo, point_hi, point_lo};
  unsigned short value = crc.Modbus(Modbus_req1, 0, 6);
  byte c1 = value & 0xFF ;
  byte c2 = value >> 8;
  //Serial.println(c2,HEX);
  byte Modbus_req[] = {slave_add, func_code, add_hi, add_lo, point_hi, point_lo, c1, c2};
  for (int i = 0; i < 8; i++) {
    Serial.print(Modbus_req[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");
  RS485.write(Modbus_req, sizeof(Modbus_req));
  RS485.flush();
  RS485.readBytes(Modbus_buf, sizeof(Modbus_buf));
  int sizee = Modbus_buf[2] + 5;
  for (int i = 0; i < sizee; i++) {
    Serial.print(Modbus_buf[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");
  return Modbus_buf;

}
