
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

  transmission(0x01, 0x04, 0x00, 0x1E, 0x00, 0x02);

  delay(5000);
}



void transmission(byte slave_add, byte func_code, byte add_hi, byte add_lo, byte point_hi, byte point_lo) {

  byte Modbus_req1[] = {slave_add, func_code, add_hi, add_lo, point_hi, point_lo};
  unsigned short value = crc.Modbus(Modbus_req1, 0, 6);
  byte input_c1 = value & 0xFF ;
  byte input_c2 = value >> 8;
  //Serial.println(c2,HEX);
  byte Modbus_req[] = {slave_add, func_code, add_hi, add_lo, point_hi, point_lo, input_c1, input_c2};
//  for (int i = 0; i < 8; i++) {
//    Serial.print(Modbus_req[i], HEX);
//    Serial.print(" ");
//  }
  Serial.println("");
  RS485.write(Modbus_req, sizeof(Modbus_req));
  RS485.flush();
  RS485.readBytes(Modbus_buf, sizeof(Modbus_buf));
  int sizee = Modbus_buf[2] + 5;
  byte output_c1 = Modbus_buf[sizee - 1];
  byte output_c2 = Modbus_buf[sizee - 2];
//  Serial.println(x1,HEX);
//  Serial.println(x2,HEX);
  byte neww[20];
  for (int i = 0; i < sizee-2; i++) {
    neww[i] = Modbus_buf[i];
  }
  unsigned short value1 = crc.Modbus(neww, 0, sizee-2);
  byte generated_c1 = value1 & 0xFF;
  byte generated_c2 = value1 >> 8;
//  Serial.println(y1,HEX);
//  Serial.println(y2,HEX);
  
  if (output_c1 == generated_c2 && output_c2 == generated_c1){
    
    Serial.println("Correct");
    for (int i = 0; i < sizee; i++){
      Serial.print(Modbus_buf[i], HEX);
      Serial.print(" ");
    }
    Serial.print("");
  }
  else{
  
  Serial.print("Incorrect");
}
}
