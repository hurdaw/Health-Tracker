#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
#include "heartRate.h"
#include "MAX30105.h"           // Thư viện MAX30102 từ SparkFun
#include <Adafruit_MLX90614.h>  // Thư viện cho cảm biến MLX90614
#include <Firebase_ESP_Client.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

// Khai báo Firebase
#define FIREBASE_HOST "https://tkt1-15e2-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "aNjO01miLRKcv88d1bheYWArXunrqo1b1LVmRiqo"
// Thông tin Firebase
#define API_KEY "AIzaSyCo8OPFkNwVBfNjs6uCNac5GqOeuG83_I0"
#define DATABASE_URL "https://tkt1-15e42-default-rtdb.firebaseio.com/"
// Đối tượng Firebase
FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;
const long interval = 500;
unsigned long firebaseSendMillis = 0;
const long firebaseInterval = 1000;
unsigned long previousMillis = 0;

bool signupOK = false;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 25200);  // UTC+7 time offset

String weekDays[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
String months[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };


const int activateHour = 2;
const int activateMinute = 9;
const int deactivateHour = 2;
const int deactivateMinute = 10;

char intro[] = "PTIT | By TKT";
int x, minX;
bool deviceActive = false;

MAX30105 particleSensor;  // MAX30105 sensor class
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// Heart rate calculation variables
const byte RATE_SIZE = 4;
byte rates[RATE_SIZE];
byte rateSpot = 0;
long lastBeat = 0;
float beatsPerMinute;
int beatAvg;

float bodyTemp = 0;

void wifiConnect() {
  WiFiManager wifiManager;
  wifiManager.setDebugOutput(false);
  wifiManager.autoConnect("AutoConnectAP");

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println("WiFi");
  display.setCursor(0, 30);
  display.println("Connected");
  display.display();
  delay(2000);
}

void clockDisplay() {
  timeClient.update();
  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();
  String am_pm = (currentHour < 12) ? "AM" : "PM";
  if (currentHour == 0) currentHour = 12;
  else if (currentHour > 12) currentHour -= 12;

  String timeStr = String(currentHour) + ":" + (currentMinute < 10 ? "0" : "") + String(currentMinute) + " " + am_pm;
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(25, 29);
  display.print(timeStr);

  time_t epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime(&epochTime);
  String dateStr = String(weekDays[timeClient.getDay()]) + ", " + String(ptm->tm_mday) + "-" + months[ptm->tm_mon];

  display.setTextSize(1);
  display.setCursor(28, 50);
  display.println(dateStr);
  // display.display();
}

void displaySensorData() {
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("HR: ");
  display.print(beatAvg);
  display.setCursor(0, 10);
  display.print("T: ");
  display.print(bodyTemp, 1);
  display.print(" C");
  display.display();


  // Output data to Serial Monitor
  Serial.print("Heart Rate: ");
  Serial.print(beatAvg);
  Serial.print(" BPM, ");
  Serial.print("Body Temp: ");
  Serial.print(bodyTemp, 1);
  Serial.println(" C");
}

void readSensors() {
  // Heart rate sensing using MAX30102
  long irValue = particleSensor.getIR();
  if (checkForBeat(irValue)) {
    long delta = millis() - lastBeat;
    lastBeat = millis();
    beatsPerMinute = 60 / (delta / 1000.0);
    if (beatsPerMinute < 255 && beatsPerMinute > 20) {
      rates[rateSpot++] = (byte)beatsPerMinute;
      rateSpot %= RATE_SIZE;
      beatAvg = 0;
      for (byte x = 0; x < RATE_SIZE; x++) beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }
  // Body temperature sensing using MLX90614
  bodyTemp = mlx.readObjectTempC();
}
boolean checkForBeat(long irValue) {
  static long lastIrValue = 0;
  static bool beatDetected = false;
  if (irValue > 50000 && irValue > lastIrValue) {
    if (!beatDetected) {
      beatDetected = true;
      return true;
    }
  } else if (irValue < lastIrValue) beatDetected = false;
  lastIrValue = irValue;
  return false;
}

void setup() {
  Serial.begin(115200);  // Initialize Serial for terminal output
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  // display.display();
  wifiConnect();
  timeClient.begin();
  // Cấu hình Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  // Bắt đầu Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  // Đăng ký
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Signup successful");
    signupOK = true;
  } else {
    Serial.printf("Signup failed: %s\n", config.signer.signupError.message.c_str());
  }

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30105 was not found.");
    while (1)
      ;
  }

  byte ledBrightness = 0x1F;
  byte sampleAverage = 8;
  byte ledMode = 3;
  int sampleRate = 100;
  int pulseWidth = 411;
  int adcRange = 4096;
  // particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);
  particleSensor.setup(0x1F, 8, 3, 100, 411, 4096);  //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A);         //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0);          //Turn off Green LED

  mlx.begin();
  for (int x = 0; x < RATE_SIZE; x++) rates[x] = 0;
  x = display.width();
  minX = -6 * strlen(intro);
}

void loop() {
  clockDisplay();
  readSensors();
  displaySensorData();
  // sent data to firebase
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= firebaseInterval) {
    // Gửi dữ liệu lên Firebase sau mỗi khoảng thời gian định trước
    previousMillis = currentMillis;
    Firebase.RTDB.setFloat(&firebaseData, "sensor/objectTemp", bodyTemp);
    Firebase.RTDB.setFloat(&firebaseData, "sensor/heartRate", beatAvg);
  }
}