
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "student";
const char* password = "12345678";

//Your Domain name with URL path or IP address with path
//String serverName;// = "http://192.168.1.106:1880/update-sensor";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

int motor1 = D0;
int motor2 = D1;
int motor3 = D2;
int motor4 = D3;
 const int trigPin = D5;   
 const int echoPin = D6;   
 long duration;  
 int distance;

void setup() {
  Serial.begin(115200); 
   pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output  
 pinMode(echoPin, INPUT); // Sets the echoPin as an Input  
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(motor3, OUTPUT);
  pinMode(motor4, OUTPUT);

   digitalWrite(motor1, LOW);
      digitalWrite(motor2, LOW);
      digitalWrite(motor3, LOW);
     digitalWrite(motor4, LOW);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {

     digitalWrite(trigPin, LOW);  
 delayMicroseconds(2);  
 digitalWrite(trigPin, HIGH);  
 delayMicroseconds(10);  
 digitalWrite(trigPin, LOW);  
  duration = pulseIn(echoPin, HIGH);  
 distance= duration*0.034/2;  
  
 Serial.print("Distance: ");  
 Serial.println(distance);  
  // Send an HTTP POST request depending on timerDelay
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      // "Enter the HTTP link "	 =50";
      String serverPath = "  "Enter the HTTP link "   /getChairById.php";
      
      // Your Domain name with URL path or IP address with path
      http.begin(client,serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      Serial.println(httpResponseCode);
      
  if (httpResponseCode>0)
  {
     Serial.print("HTTP Response code: ");
     Serial.println(httpResponseCode);
     String payload = http.getString();
     Serial.println((payload[9]));

     
    if(payload[9] == '1')
    {
      if(distance >  15)
      {
        
      digitalWrite(motor1, HIGH);
      digitalWrite(motor2, LOW);
      digitalWrite(motor3, HIGH);
     digitalWrite(motor4, LOW);
      }
      else
      {
         digitalWrite(motor1, LOW);
      digitalWrite(motor2, LOW);
      digitalWrite(motor3, LOW);
     digitalWrite(motor4, LOW);
      }
    }
    else if(payload[9] == '2')
    {
      digitalWrite(motor1, LOW);
      digitalWrite(motor2, HIGH);
      digitalWrite(motor3, LOW);
      digitalWrite(motor4, HIGH);
      
    }
    else if(payload[9] == '3')
    {
      digitalWrite(motor1, LOW);
      digitalWrite(motor2, LOW);
      digitalWrite(motor3, LOW);
     digitalWrite(motor4, LOW);
      
    }
    else if(payload[9] == '4')
    {
      digitalWrite(motor1, HIGH);
      digitalWrite(motor2, LOW);
      digitalWrite(motor3, LOW);
     digitalWrite(motor4, HIGH);
      delay(3000);
    }
    else if(payload[9] == '5')
    {
      digitalWrite(motor1, LOW);
      digitalWrite(motor2, HIGH);
      digitalWrite(motor3, HIGH);
     digitalWrite(motor4, LOW);
     
    }
      }
      
else 
{
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
 }
      // Free resources
      http.end();
   
    
 }
else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
  
}
