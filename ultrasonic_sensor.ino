#include <HTTPClient.h>
#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>

const int trigPin = 12;
const int echoPin = 13;
const int servoPin = 5;

LiquidCrystal_I2C lcd(0x27, 16, 2);

Servo myServo;

String URL = "http://........../store_data/store_data.php";

const char* ssid = "..........";
const char* password = "..........";

int distance = 0;
int bottle_count = 0;

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myServo.attach(servoPin);
  myServo.write(0);
  lcd.init(); 
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Bottles: 0");
  connectWiFi(); 
}

void loop() {
  if(WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  Load_data()

  String postData = "Distance = " + String(distance) + "&Bottle count = " + String(bottle_count);

  HTTPClient http;
  http.begin(URL);

  int httpCode = http.POST(postData);
  String payload = http.getString();
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  Serial.print("URL : "); Serial.println(URL); 
  Serial.print("Data: "); Serial.println(postData);
  Serial.print("httpCode: "); Serial.println(httpCode);
  Serial.print("payload : "); Serial.println(payload);
  Serial.println("--------------------------------------------------");
  delay(5000);
}

void connectWiFi() {
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    
  Serial.print("connected to : "); Serial.println(ssid);
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
}

void Load_data() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);

  int distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance < 20) {
    bottle_count++; 
    myServo.write(90);
    delay(1000); 
    myServo.write(0); 
    delay(1000); 

    lcd.setCursor(0, 0);
    lcd.print("Bottles: ");
    lcd.print(bottle_count);
    lcd.print("    "); 

    delay(2000);
  }
  
  if (isnan(distance) || isnan(bottle_count)) {
    Serial.printl("Fail to read value from sensor!!!")
    distance = 0;
    bottle_count = 0;
  }

  delay(500);
}