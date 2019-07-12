#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "Timer.h"
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti wifiMulti;

#define HTTP_QUERY_SIZE 1400

// TODO: Edit wifi connection info
const char* ssid = "UCSD-GUEST";
const char* pw = "";

uint8_t httpQuery[HTTP_QUERY_SIZE];

static WiFiClient client;

char *server = "api.openweathermap.org";
char *apiKey = "5d7171bef3c4d2055fcff9885bb2cf66";
char *cityNames[] = {"San Diego, US"};
char *currentWeather = "GET /data/2.5/weather?";


uint16_t queryLength = 0;
bool isRunning = false;
String httpResponse = "";
int numberOfCity = 1;

void setup() {
  // put your setup code here, to run once:  
  Serial.begin(115200);
  while (!Serial);// wait for serial port to connect.
  Serial.println();

  // Connect to Wifi network
  WiFi.begin(ssid,pw);


  // Print the network that the chip is connected to
  Serial.println(WiFi.localIP());

  numberOfCity = sizeof(cityNames) / sizeof(char*);
  
}

void loop() {  
  // while there's is a request from arduino, fetch
  /*
  while (Serial.read() == 1) {
    apiLoop();
    httpLoop();
  }
  */
  apiLoop();
  httpLoop();
  delay(10000);
}

void apiLoop()
{

  static int index = 0;

  queryLength = getQueryCurrentWeather(httpQuery, cityNames[index++],
      apiKey);
  if(index >= numberOfCity) {
    index = 0;
  }
    
  httpResponse = "";
  
  // Use object of WiFiClient WiFiClient class to create TCP connections
  if(client.connect(server, 80))
  {
    isRunning = true;
    client.print((char *) httpQuery);
  
  }
}

char buf[1024];
uint8_t buf_int = (uint8_t)atoi(buf);

String f = "\r\n\r\n";
void httpLoop()
{
  if(isRunning)  
  {
    // Count the amount of data availble for reading
    uint16_t cnt = client.available();

    if(cnt > 0)
    {
      // If the number of bytes to be read is greater than 128, just
      // ready 128 bytes. Otherwise, fill the bytes.
      //
      uint8_t tryRead = cnt > 128 ? 128 : cnt;
      memset(buf, 0, 1024);

      // Store read bytes in recv
      //uint16_t recv = WiFiClient.read(buf_int, tryRead);
      String recv = client.readStringUntil('\r');
      httpResponse = httpResponse + buf;      

      Serial.println("Received");
      Serial.println(recv);
      //Save index of last occurence of the string f
      int idx = httpResponse.lastIndexOf(f);
      if(idx >= 0)
        httpResponse = httpResponse.substring(idx + 4);
    }
    
    if(!client.connected())
    {
      isRunning = false;
      Serial.println("Client not connected");
      
    }
  }
}

uint16_t writeString(const char* string, uint8_t* buf, uint16_t pos) 
{
    const char* idp = string;
    uint16_t i = 0;
 
    while (*idp) 
    {
        buf[pos++] = *idp++;
        i++;
    }
    
    return pos;
}

uint16_t getQueryCurrentWeather(uint8_t* query, char* cityName, 
    char* apiKey)

{
  uint16_t queryLength = 0;

  queryLength = writeString(currentWeather, query, queryLength);
  queryLength = writeString("q=", query, queryLength);
  queryLength = writeString(cityName, query, queryLength);
  queryLength = writeString("&units=metric&appid=", httpQuery, queryLength);
  queryLength = writeString(apiKey, query, queryLength);  

  queryLength = writeString(
      " HTTP/1.1\r\nHost: api.openweathermap.org\r\n", query, queryLength);
  queryLength = writeString(
      "Connection: Closed\r\n\r\n", query, queryLength);
  
  return queryLength;
}

