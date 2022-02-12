#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "LimitedEditi0n";
const char* password = "nktnktnkt1";
const char* url_get = "https://ecourse.cpe.ku.ac.th/exceed02/api/room/status/all";
const char* url_post = "https://ecourse.cpe.ku.ac.th/exceed02/api/room/post";

char str[50];
int room1;
int room2;
int room3;

const  int _size = 2*JSON_OBJECT_SIZE(10);

StaticJsonDocument<_size> JSONPost;
StaticJsonDocument<_size> JSONGet;

void WiFi_Connect(){
  WiFi.disconnect();
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to the WiFi network");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());
}



void _post(){
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    
    http.begin(url_post);
    http.addHeader("Content-Type", "application/json");

    JSONPost["room1"] = room1;
    JSONPost["room2"] = room2;
    JSONPost["room3"] = room3;

    serializeJson(JSONPost, str);
    Serial.printf("room1: %d room2: %d room3: %d",room1,room2,room3);
    int httpCode = http.POST(str);

    if(httpCode == HTTP_CODE_OK){
      String payload = http.getString();
      Serial.println(httpCode);
      Serial.println("POST result");
      Serial.println(payload);
    } else{
      Serial.println(httpCode);
      Serial.println("ERROR on HTTP Request");
    }
  } else{
    WiFi_Connect();
  }
  delay(100);
}



int sw1 = 22;
int ldr2 = 32;
int ldr3 = 33;
int led1 = 5;
int led2 = 18;
int led3 = 19;



void setup() {
  pinMode(sw1, INPUT_PULLUP);
  pinMode(ldr2, INPUT_PULLUP);
  pinMode(ldr3, INPUT_PULLUP);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  digitalWrite(led1, HIGH);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  Serial.begin(9600);

  xTaskCreatePinnedToCore(Task1,"Task 1",1024*32,NULL,1,NULL,1);
  xTaskCreatePinnedToCore(Task2,"Task 2",1024*32,NULL,1,NULL,1);
  xTaskCreatePinnedToCore(Task3,"Task 3",1024*32,NULL,1,NULL,1);
}


void Task1(void *parameter){
  while(1){
  digitalWrite(led1, digitalRead(sw1));
  if(digitalRead(led1)==0){
    room1 = 0;
  }
  else if (digitalRead(led1)==1){
    room1 = 1;
  }
  delay(10);
  }
}


void Task2(void *parameter){
  while(1){
  if(analogRead(ldr2)>1000){
      digitalWrite(led2,HIGH);
  }
  else{
      digitalWrite(led2,LOW);
  }
  if(digitalRead(led2)==1){
    room2 = 0;
  }
  else if (digitalRead(led2)==0){
    room2 = 1;
  }
  delay(10);
  }
}

void Task3(void *parameter){
  while(1){
  if(analogRead(ldr3)>1000){
      digitalWrite(led3,HIGH);
  }
  else{
      digitalWrite(led3,LOW);
  }
  if(digitalRead(led3)==1){
    room3 = 0;
  }
  else if (digitalRead(led3)==0){
    room3 = 1;
  }
  delay(10);
  }
}

/*void var_room(){
  if(digitalRead(led1)==0){
    room1 = 0;
  }
  else if (digitalRead(led1)==1){
    room1 = 1;
  }
  if(digitalRead(led2)==1){
    room2 = 0;
  }
  else if (digitalRead(led2)==0){
    room2 = 1;
  }
  if(digitalRead(led3)==1){
    room3 = 0;
  }
  else if (digitalRead(led3)==0){
    room3 = 1;
  }
}*/

void loop() {
  _post();
  delay(500);
}
