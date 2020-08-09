/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  

  This example shows how to send values to the Blynk App,
  when there is a widget, attached to the Virtual Pin and it
  is set to some frequency

  Project setup in the app:
    Value Display widget attached to V5. Set any reading
    frequency (i.e. 1 second)
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT SwSerial


#include <SoftwareSerial.h>
SoftwareSerial SwSerial(10, 11); // RX, TX

#include <BlynkSimpleStream.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "68ioEx8wOc5gbCQe7UTSOVkv_yobDdie";

// Use Virtual pin 5 for uptime display
#define PIN_UPTIME V5
#define PIN_SECTIME V6
#define PIN_TEMP V1
#define PIN_HUM V2

// Attach widget terminal to V3
WidgetTerminal terminal(V3);

// Double map function
double dmap(double x, double in_min, double in_max, double out_min, double out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double decRound(double x, int dec){
  double toRound = x * dec * 10;
//  terminal.clear();
  terminal.println(x);
  terminal.println(toRound);
  terminal.println(round(toRound));
  terminal.println(round(toRound) / (dec * 10.0));
  terminal.println();
  return round(toRound) / (dec * 10.0);
}

String secToHms(long sec){
  int t = sec;
  
  int s = t%60;
  t = (t-s)/60;
  int m = t%60;
  t = (t-m)/60;
  int h = t;
  return (String)h + ":" + (String)m + ":" + (String)s;
}

// This function tells Arduino what to do if there is a Widget
// which is requesting data for Virtual Pin (5)
BLYNK_READ(PIN_UPTIME)
{
  // This command writes Arduino's uptime in seconds to Virtual Pin (5)
  Blynk.virtualWrite(PIN_UPTIME, secToHms(millis() / 1000));
  Blynk.virtualWrite(PIN_SECTIME, millis() / 1000);
}

BLYNK_READ(PIN_TEMP)
{
  // This command writes the temperature in °C to Virtual Pin (1)
  uint16_t val;
  double dat;
  val = analogRead(A0);//Connect LM35 on Analog 0
  dat = (double) val * (5/10.24);
  Blynk.virtualWrite(PIN_TEMP, decRound(dat, 1));
}

BLYNK_READ(PIN_HUM)
{
  // This command writes humidity in % to Virtual Pin (2)
  const double waterValue = 250;
  const double airValue = 537;
  uint16_t val;
  double dat;
  val=analogRead(A1);//Connect LM35 on Analog 0
  dat = dmap(val, airValue, waterValue, 0, 100);
  Blynk.virtualWrite(PIN_HUM, decRound(dat, 1));
}

void setup()
{
  // Debug console
  SwSerial.begin(9600);

  // Blynk will work through Serial
  // Do not read or write this serial manually in your sketch
  Serial.begin(9600);
  Blynk.begin(Serial, auth);
}

void loop()
{
  Blynk.run();
}
