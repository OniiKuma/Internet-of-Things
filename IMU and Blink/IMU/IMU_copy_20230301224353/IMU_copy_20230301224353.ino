/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC-Plus sample source code
*                          配套  M5StickC-Plus 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/m5stickc_plus
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/m5stickc_plus
*
* Describe: IMU.
* Date: 2021/9/14
*
* modified by Andrew Dames Jr., 2023/2/27
*anddames@uat.edu
*
* goal: to make the M5StickC Plus display max accel value in each direction
*
*******************************************************************************
*/
#include <M5StickCPlus.h>
#define LED_BUILTIN 10

float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;

float gyroX = 0.0F;
float gyroY = 0.0F;
float gyroZ = 0.0F;

float maxX = 0.0F;
float maxY = 0.0F;
float maxZ = 0.0F;

float xSq = 0.0F;
float ySq = 0.0F;
float zSq = 0.0F;

float vectMag = 0.0F;
float maxVectMag = 0.0F;
/* After M5StickC Plus is started or reset
  the program in the setUp () function will be run, and this part will only be
  run once. 在 M5StickC Plus
  启动或者复位后，即会开始执行setup()函数中的程序，该部分只会执行一次。 */
void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  M5.begin();     // Init M5StickC Plus.  初始化 M5StickC Plus
  M5.Imu.Init();  // Init IMU.  初始化IMU
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED off by making the voltage LOW
  M5.Lcd.setRotation(3);            // Rotate the screen. 将屏幕旋转
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(80, 15);  // set the cursor location.  设置光标位置
  M5.Lcd.println("IMU TEST");
  M5.Lcd.setCursor(30, 30);
  M5.Lcd.println("  X       Y       Z");
  M5.Lcd.setCursor(30, 70);
  M5.Lcd.println("  maxX   maxY    maxZ");
  M5.Lcd.setCursor(80, 90);
  M5.Lcd.println("Max Vector");
}

/* After the program in setup() runs, it runs the program in loop()
The loop() function is an infinite loop in which the program runs repeatedly
在setup()函数中的程序执行完后，会接着执行loop()函数中的程序
loop()函数是一个死循环，其中的程序会不断的重复运行 */
void loop() {
  M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);
  M5.IMU.getAccelData(&accX, &accY, &accZ);
  //find maxes
  if (maxX < accX) {
    maxX = accX;
  }
  if (maxY < accY) {
    maxY = accY;
  }
  if (maxZ < accZ) {
    maxZ = accZ;
  }
  //make xSq, ySq, zSq
  xSq = sq(accX);
  ySq = sq(accY);
  zSq = sq(accZ);

  //DETERMINE vectMag
  vectMag = sqrt(xSq + ySq + zSq);

  //determine maxVectMag
  if (vectMag > maxVectMag) {
    maxVectMag = vectMag;
    if (maxVectMag > 13) {
      digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
      delay(1000);
      digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
      delay(1000);                      // wait for a second
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
      delay(1000);                      // wait for a second
      digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
      delay(1000);                      // wait for a second
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
      delay(1000);                      // wait for a second
      digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
      delay(1000);                      // wait for a second
    }
  }



  Serial.println("Vector");
  Serial.println(vectMag);

  M5.Lcd.setCursor(30, 40);
  M5.Lcd.printf("%6.2f  %6.2f  %6.2f      ", gyroX, gyroY, gyroZ);
  M5.Lcd.setCursor(170, 40);
  M5.Lcd.print("o/s");
  M5.Lcd.setCursor(30, 50);
  M5.Lcd.printf(" %5.2f   %5.2f   %5.2f   ", accX, accY, accZ);
  M5.Lcd.setCursor(170, 50);
  M5.Lcd.print("G");
  M5.Lcd.setCursor(30, 80);
  M5.Lcd.printf(" %5.2f   %5.2f   %5.2f   ", maxX, maxY, maxZ);
  M5.Lcd.setCursor(90, 105);
  M5.Lcd.printf("  %5.2f   ", maxVectMag);

  //serial printing
  Serial.printf(" %5.2f   %5.2f   %5.2f   ", maxX, maxY, maxZ);
  Serial.println("");
  Serial.println("Squares");
  M5.Lcd.printf(" %5.2f   %5.2f   %5.2f   ", xSq, ySq, zSq);
  Serial.println("");

  //button stuff
  M5.update();
  if (M5.BtnA
        .wasReleased()) {  // If the button A is pressed.  如果按键 A 被按下
    maxX = 0.0F;
    maxY = 0.0F;
    maxZ = 0.0F;
    maxVectMag = 0.0f;
  }
}
