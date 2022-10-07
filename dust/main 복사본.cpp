#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

String url = "https://apis.data.go.kr/B552584/ArpltnInforInqireSvc/getMsrstnAcctoRltmMesureDnsty?serviceKey=0rN5chS2jIxok5bIDCgxDlwxR5MBa%2F37sN11L89HA2HepPFAWmK9r74i7t6zY6FHZhny2VoR23JE8Qa4uMLLpQ%3D%3D&returnType=xml&numOfRows=1&pageNo=1&stationName=%EC%A2%85%EB%A1%9C%EA%B5%AC&dataTerm=DAILY&ver=1.0";

const char* ssid = "";
const char* password = "";

void wifi(){
  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  

}


void setup() {
  Serial.begin(9600);
  Serial.println();
  wifi();
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(14,OUTPUT);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) 
  {
    WiFiClient client; 
    HTTPClient http; 

    if (http.begin(client, url)) {  
      int httpCode = http.GET();

      if (httpCode > 0) { 
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString(); 
          
          int start_point = payload.indexOf("<pm10Value>");
          int end_point = payload.indexOf("</pm10Value>");
          String pm10=payload.substring(start_point+11,end_point); 
          Serial.println("PM 10 : " + pm10);
          
          int pm_10 = pm10.toInt();  
          
          //LED
          if (pm_10>0 & pm_10<=30){
            digitalWrite(12, HIGH); //blue = 0 ~ 30
            }
          else if (pm_10>30 & pm_10<=80){
            digitalWrite(14, HIGH); //green 30 ~ 80
            }
          else if (pm_10 > 80){
            digitalWrite(13, HIGH); //red higher 80
            } 
           
         
        }
      } else {
        Serial.printf("[HTTP] GET... 실패, 에러코드: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
    } else {
      Serial.printf("[HTTP] 접속 불가\n");
    }
    ESP.deepSleep(3.6e9);
  }
}