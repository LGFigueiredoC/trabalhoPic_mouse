/*
    MPU6050 Triple Axis Gyroscope & Accelerometer. Simple Accelerometer Example.
    Read more: http://www.jarzebski.pl/arduino/czujniki-i-sensory/3-osiowy-zyroskop-i-akcelerometr-mpu6050.html
    GIT: https://github.com/jarzebski/Arduino-MPU6050
    Web: http://www.jarzebski.pl
    (c) 2014 by Korneliusz Jarzebski
*/	

#include <Wire.h>
#include <MPU6050.h>
#include <Mouse.h>

#define BUTTON_LEFT 6
#define BUTTON_MIDDLE 7
#define BUTTON_RIGHT 8

#define BOUNCE_TIME 50

MPU6050 mpu;

int dirX, dirY;
int previousMillis = 0;
int buttonState;
int mouseState = LOW;
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0; 

void setup() 
{
  Serial.begin(115200);

  Serial.println("Initialize MPU6050");

  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }

  pinMode(BUTTON_LEFT, INPUT);
  pinMode(BUTTON_MIDDLE, INPUT);
  pinMode(BUTTON_RIGHT, INPUT);
  

  // If you want, you can set accelerometer offsets
  // mpu.setAccelOffsetX();
  // mpu.setAccelOffsetY();
  // mpu.setAccelOffsetZ();
  
  checkSettings();

  Mouse.begin();
}

void checkSettings()
{
  Serial.println();
  
  Serial.print(" * Sleep Mode:            ");
  Serial.println(mpu.getSleepEnabled() ? "Enabled" : "Disabled");
  
  Serial.print(" * Clock Source:          ");
  switch(mpu.getClockSource())
  {
    case MPU6050_CLOCK_KEEP_RESET:     Serial.println("Stops the clock and keeps the timing generator in reset"); break;
    case MPU6050_CLOCK_EXTERNAL_19MHZ: Serial.println("PLL with external 19.2MHz reference"); break;
    case MPU6050_CLOCK_EXTERNAL_32KHZ: Serial.println("PLL with external 32.768kHz reference"); break;
    case MPU6050_CLOCK_PLL_ZGYRO:      Serial.println("PLL with Z axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_YGYRO:      Serial.println("PLL with Y axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_XGYRO:      Serial.println("PLL with X axis gyroscope reference"); break;
    case MPU6050_CLOCK_INTERNAL_8MHZ:  Serial.println("Internal 8MHz oscillator"); break;
  }
  
  Serial.print(" * Accelerometer:         ");
  switch(mpu.getRange())
  {
    case MPU6050_RANGE_16G:            Serial.println("+/- 16 g"); break;
    case MPU6050_RANGE_8G:             Serial.println("+/- 8 g"); break;
    case MPU6050_RANGE_4G:             Serial.println("+/- 4 g"); break;
    case MPU6050_RANGE_2G:             Serial.println("+/- 2 g"); break;
  }  

  Serial.print(" * Accelerometer offsets: ");
  Serial.print(mpu.getAccelOffsetX());
  Serial.print(" / ");
  Serial.print(mpu.getAccelOffsetY());
  Serial.print(" / ");
  Serial.println(mpu.getAccelOffsetZ());
  
  Serial.println();
}

int retornaVelocidade (float acc) {
    if (acc < 0) acc *= -1;
    if (acc < 5) return 0.5;
    if (acc < 10) return 1;
    else return 1.5;
}

int verificaDirecao (float acc) {
  if (acc < 0) return -1;
  else return 1;
}

void loop() {

  Vector normAccel = mpu.readNormalizeAccel();


  // Serial.print(" Xnorm = ");
  // Serial.print(normAccel.XAxis);
  // Serial.print(" Ynorm = ");
  // Serial.print(normAccel.YAxis);

  int readingButtonLeft = digitalRead(BUTTON_LEFT); 
  int readingButtonRight = digitalRead(BUTTON_RIGHT); 
  int readingButtonMiddle = digitalRead(BUTTON_MIDDLE); 

  Serial.println(readingButtonLeft);
  if (readingButtonLeft == HIGH) {
    Mouse.click(MOUSE_LEFT);
    dirX = verificaDirecao(normAccel.XAxis), dirY = verificaDirecao(normAccel.YAxis);
    Mouse.move(dirX*retornaVelocidade(normAccel.XAxis), -(dirY*retornaVelocidade(normAccel.YAxis)), 0);
  }

  if (readingButtonRight == HIGH) {
    Mouse.click(MOUSE_RIGHT);
    dirX = verificaDirecao(normAccel.XAxis), dirY = verificaDirecao(normAccel.YAxis);
    Mouse.move(dirX*retornaVelocidade(normAccel.XAxis), -(dirY*retornaVelocidade(normAccel.YAxis)), 0);
  }

  if (readingButtonMiddle == HIGH) {
    Mouse.click(MOUSE_MIDDLE);
    dirX = verificaDirecao(normAccel.XAxis), dirY = verificaDirecao(normAccel.YAxis);
    Mouse.move(dirX*retornaVelocidade(normAccel.XAxis), -(dirY*retornaVelocidade(normAccel.YAxis)), 0);
  }


  lastButtonState = buttonState;

  dirX = verificaDirecao(normAccel.XAxis), dirY = verificaDirecao(normAccel.YAxis);
  Mouse.move(dirX*retornaVelocidade(normAccel.XAxis), -(dirY*retornaVelocidade(normAccel.YAxis)), 0);
  
}
