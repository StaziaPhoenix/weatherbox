#include <ArduinoJson.h>
#include "Controller.cpp"
#include "Lights.cpp"

unsigned long timeoutOver;
unsigned long lastFetch;
unsigned long lastActuation;
unsigned long fetchTime_ms = 900000; //15 min = 900,000 mSec
unsigned long actuationTime_ms = 15000; //15 sec = 15,000 mSec

int lastWeatherCode;

//Set Up Actuation
Controller controller;
Lights lights;

//Set Up ESP 
char espBuf[1024];  //buffer for new Rx from ESP chip
DynamicJsonBuffer jsonBuffer;

void setup() 
{ 
  lastWeatherCode = 800;  //clear weather
  
  //Init Serial Port for communication to ESP chip
  Serial.begin(115200);
  
  //Turn off all actuators
  controller.allOff();
  lights.off();

  if(sendESPCmd(1))
  {
    //Parse JSON & Set Weather
    if(parseESPJson() != 1)
    {  
      //Error
    }
  }

  //**Now we can actually begin normal operation**

  //Start System Time
  lastFetch = millis();
  lastActuation = millis();
}

void loop() 
{
  //check if time to actuate current JSON
  if(millis() - lastActuation >= actuationTime_ms)
  {
    setWeather(String(lastWeatherCode));
    lastActuation = millis();
  }
  //check if time to fetch new JSON
  if(millis() - lastFetch >= fetchTime_ms)
  {
    if(sendESPCmd(1))
    {
      //Parse JSON & Set Weather
      if(parseESPJson() != 1)
      {  
        //Error
      }
    }
    
    lastFetch = millis();
  }
}

int sendESPCmd(int _cmd)
{
  int count = 0;
  
  //Reset ESP Rx buffer
  memset(espBuf, 0, 1024);

  //cmd 1 = fetch JSON
  Serial.write(_cmd);  //Send ESP chip a command to req from OpenWeatherAPI

  delay(100); //Wait for ESP to process?

  if(Serial.available())
  {
    do
    {
      while(Serial.available())
      {  
        espBuf[count] = Serial.read();
        timeoutOver = millis()+10; //allow 10mSec before timeout/EOT
        count++;
      }
    }
    while(millis()<timeoutOver);
    espBuf[count] = '\0'; //Signifies end
  }
  else
  {
    return 0;
  }

  return 1;
}

int parseESPJson()
{
  JsonObject& root = jsonBuffer.parseObject(espBuf);

  if(!root.success())
  {
    return 0; //error
  }

  if(root["cod"] == 200)
  {
    //For OpenWeatherAPI, the "cod" internal parameter should be 200 
    JsonObject& weather = root["weather"][0];
    String weatherId = weather["id"]; 

    if(setWeather(weatherId))
    {
      //success
      return 1;
    }
    else
    {
      return -2;
    }
  }
  else
  {
    return -1;
  }
}


int setWeather(String _wthrID)
{
  int weather = atoi(&_wthrID[0]);
  lastWeatherCode = weather;
  
  if(weather >= 200 && weather < 300)
  {
    //thunderstorm
    storm(10);
  }
  else if(weather >= 300 && weather < 400)
  {
    //drizzle (lumped in with "light rain")
    rain();
  }
  else if(weather >= 500 && weather < 600)
  {
    //rain
    rain();
  }
  else if(weather >= 700 && weather < 800)
  {
    //atmospheric conditions
    if(weather == 701 || weather == 721)
    {
      lowFog();
    }
    else if(weather == 741 || weather == 781)
    {
      highFog();
    }
    
  }
  else if(weather == 800)
  {
      //clear
      sunrise();
  }
  else
  {
    //unknown weather ID -- error
    return 0;
  }
  return 1; //success
}









/* SUNRISE */
void sunrise() {
  highFog();
  delay(5000);
  controller.allOff();
  delay(100);
  lights.sunrise();
}

/* STORM */
void storm(int seconds) {
  lights.set(0,0,30);
  rain();

  lowFog();
  delay(2000);
  
  for (int i = 0; i < seconds-1; i++) {

    if (i % 2 == 1) {
      lowFog();
    } else {
      highFog();
    }
    lights.flash(255,255,255);
    delay(900);
  }

}

/* RAIN */
void rain() {
  controller.pumpOn();
}

void rain(int redV, int greenV, int blueV) {
  controller.pumpOn();
  lights.on(redV, greenV, blueV);
}


/* LOW FOG */
void lowFog() {
  controller.misterOn();

  controller.fanOn();
  delay(200);
  controller.fanOff();
  
}

void lowFog(int redV, int greenV, int blueV) {
  controller.misterOn();
  lights.on(redV, greenV, blueV);
}


/* HIGH FOG */
void highFog() {
  controller.misterOn();
  controller.fanOn();
}

void highFog(int redV, int greenV, int blueV) {
  controller.misterOn();
  controller.fanOn();
  lights.on(redV, greenV, blueV);
}

