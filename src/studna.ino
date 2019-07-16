#include "U8glib.h"
 
#define ECHOPIN 2
#define TRIGPIN 3
#define DELAY 100

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);

int arrayPos = 0;
int values[128];

void setup() {
  Serial.begin(9600);
  pinMode(ECHOPIN, INPUT);
  pinMode(TRIGPIN, OUTPUT);
}

void loop() 
{
  float distance = measureDistance();
  Serial.print(distance);
  Serial.print("cm\n");

  values[arrayPos] = (int)distance;

  u8g.firstPage();  
  do 
  {
    u8g.setFont(u8g_font_7x14);
    u8g.setPrintPos(0, 14); 
    u8g.print(String(distance) + "cm");

    u8g.setColorIndex(0);
    u8g.drawBox(0,15,128,50);

    u8g.setColorIndex(1);

    int min = 9999;
    int max = 0;

    for (int i=0;i<128;i++)
    {
      if (values[i] > max)
        max = values[i];
      if (values[i] < min)
        min = values[i];
    }

    max += 20;
    min -= 20;
    for (int i=0;i<128;i++)
    {
      int x = 128 - i;
      int y = (int)(64 - ((float)(values[(arrayPos+i)%128] - min) / (float)(max - min))*50);
      u8g.drawPixel(x,y);
    }    
  } 
  while( u8g.nextPage() );

  arrayPos = (arrayPos+1)%128;

  delay(DELAY);
}

float measureDistance()
{
  float distance = 0;
  int tries = 100;
  do 
  {
    digitalWrite(TRIGPIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGPIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGPIN, LOW);
  
    distance = pulseIn(ECHOPIN, HIGH);
    distance= distance*0.017315f;
    tries--;
  } while (distance > 500 && tries > 0);
  return distance > 500 ? 500 : distance;
}

