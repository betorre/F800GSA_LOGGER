#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
//---WIFI
const char* ssid     = "CASTILLO";
const char* password = "3123126060";
//---TIME
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -18000;//3600 * 5, but as it is -5 GMT, need to add "-" Sign at beggining
const int   daylightOffset_sec = 0; //No Daylight savings for Colombia
//----->CUSTOM FUNCTIONS
//1. Print Local time on serial console, for debugging.
void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %d %b %Y %H:%M:%S");
}
//2. Update Time from internet, assumes Internet connection is OK
void UpdateTime()
{
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Time and Date have been updated correctly!");
}
//3. Connect to Internet, either thru Cellphone AP or thru GSM
void Connect()
{
  //Disconnect, any current connection, and give 200ms as window for ESP32 to get ready for new connection
  Serial.println("Disconnecting From any previous connection");
  WiFi.mode(WIFI_MODE_STA);//Set mode as WIFI Station or client
  WiFi.disconnect();
  delay(200);
  //SETUP ESP32 as Station, meaning it can connect thru internet
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void setup()
{
    Serial.begin(115200);
    
    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_MODE_STA);//Set mode as WIFI Station or client
    Serial.println("Disconnecting Current AP...");
    WiFi.disconnect();
        delay(200);
    Serial.println("Disconnected OK");
    /*
    //START WIFI Connection to pre-defined AP
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    */
  UpdateTime();
  Connect();
}

void loop()
{
  delay(1000);
  printLocalTime();
}