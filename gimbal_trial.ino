// reference https://howtomechatronics.com/tutorials/arduino/arduino-and-mpu6050-accelerometer-and-gyroscope-tutorial/

#include <Wire.h>
#include <Servo.h>
#include <math.h>

const int MPU = 0x68; // MPU6050 I2C address
Servo SG90;
int ServoPin = 9;

// accelerometer variables
float AccX, AccY, AccZ; 
float AccAngleX, AccAngleY;
float offset = 0;

void setup() {
  SG90.attach(ServoPin); // attach servo to pin number
  
  Wire.begin(); 
  Wire.beginTransmission(MPU); // transmit to MPU
  Wire.write(0x6B); // talk to register (0x6B)
  Wire.write(0x00);
  Wire.endTransmission(true);

  Serial.begin(9600); // communicate with serial monitor at baudrate9600 (bits per second)
}

void loop() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // talk to register (ACCEL_XOUT 0x3B)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);

  AccX = Wire.read() << 8 | Wire.read();
  AccY = Wire.read() << 8 | Wire.read();
  AccZ = Wire.read() << 8 | Wire.read();

  AccAngleX = atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * (180 / M_PI);
  AccAngleY = atan(-AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * (180 / M_PI);

  Serial.print("AccX = "); 
  Serial.print(AccX);
  Serial.print(" | AccY = ");
  Serial.print(AccY);
  Serial.print(" | AccZ = ");
  Serial.println(AccZ);

  Serial.print("AccAngleX = ");
  Serial.print(AccAngleX);
  Serial.print(" degrees | AccAngleY = ");
  Serial.print(AccAngleY);
  Serial.println(" degrees");

  float angle = 90 + AccAngleX + (-offset);
  SG90.write(angle); // servo continously at angle

  if (Serial.available() > 0) { // if data is waiting to be read
    offset = Serial.parseFloat(); // retrieve offset
    Serial.read(); // -1 if no data is read, deal with "extra" bytes after parseFloat
  }
  Serial.print("Offset = ");
  Serial.println(offset);
  delay(50);
}