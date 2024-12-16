#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

#define echoPin 18  // pin sensor
#define trigPin 5   // pin sensor

const char* ssid = "INDRA_4G";                           // SSID WIFI
const char* password = "alhamdulillah1234";              // Password WIFI
const char* api = "http://192.168.1.3:3000/api/sensor";  // API

long duration = 0;             // Variabel durasi sensor
double distance = 0;           // Variabel jarak sensor dalam meter
unsigned long lastmillis = 0;  // Variabel menyimpan waktu proses pengiriman data

void setup() {
  Serial.begin(115200);
  WiFi.disconnect();
  delay(1000);
  WiFi.begin(ssid, password);
  Serial.println("Konek Ke Wifi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  pinMode(echoPin, INPUT);   // Setup PIN INPUT
  pinMode(trigPin, OUTPUT);  // Setup PIN OUTPUT
}

void loop() {
  ReadSensor();                        // Memanggil fungsi ReadSensor di looping
  if (millis() - lastmillis > 30000) { // Data dikirim tiap 30 detik
    SendData();
    lastmillis = millis();
  }
  delay(100);
}

// Fungsi Mengirim Data ke API
void SendData(void) {
  HTTPClient http;
  http.begin(api);
  http.addHeader("Content-Type", "application/json");

  // Konversi data menjadi string JSON
  String jsonData;
  if (distance < 1.0) { // Jika jarak kurang dari 1 meter
    jsonData = String("{\"sensor\":") + String(distance, 2) + String("}"); // Kirim 2 desimal
  } else {
    jsonData = String("{\"sensor\":") + String(distance, 1) + String("}"); // Kirim 1 desimal
  }

  int httpResponseCode = http.POST(jsonData); // Kirim data
  http.end();

  Serial.print("Data Sent: ");
  Serial.println(jsonData); // Cetak data yang dikirim untuk debugging
}

// Fungsi Membaca Sensor
void ReadSensor(void) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);

  // Menghitung jarak dalam meter
  distance = (duration * 0.0344 / 2) / 100.0;

  Serial.print("Distance: ");
  Serial.print(distance, 2); // Cetak jarak dalam meter dengan 2 desimal
  Serial.println(" m");
}


// #include <WiFi.h>
// #include <WiFiClientSecure.h>
// #include <HTTPClient.h>

// #define echoPin 18  //pin sensor
// #define trigPin 5   //pin sensor

// const char* ssid = "INDRA_4G";                           // SSID WIFI
// const char* password = "alhamdulillah1234";              //Password WIFI
// const char* api = "http://192.168.1.3:3000/api/sensor";  // API

// long duration = 0;             // Variabel Durasi Sensor
// int distance = 0;              // Variabel Jarak Sensor
// unsigned long lastmillis = 0;  // Variabel menyimpan Waktu Proses Pengiriman data

// void setup() {
//   Serial.begin(115200);
//   WiFi.disconnect();
//   delay(1000);
//   WiFi.begin(ssid, password);
//   Serial.println("Konek Ke Wifi");
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//     //tft.print(".");
//   }
//   pinMode(echoPin, INPUT);   // Setup PIN INPUT
//   pinMode(trigPin, OUTPUT);  //Setup PIN OUTPUT
// }

// void loop() {
//   ReadSensor();                         // Memanggil Fungsi ReadSensor di Looping
//   if (millis() - lastmillis > 60000) {  // Data di Kirim tiap 1 menit/ 60 Detik
//     SendData();
//     lastmillis = millis();
//   }
//   delay(100);
// }
// //Fungsi Mengirim Data ke API
// void SendData(void) {
//   HTTPClient http;
//   http.begin(api);
//   http.addHeader("Content-Type", "application/json");
//   String jsonData = String("{\"sensor\":") + distance + String("}");
//   int httpResponseCode = http.POST(jsonData);
//   http.end();
// }

// // Fungsi Membaca Sensor
// void ReadSensor(void) {
//   digitalWrite(trigPin, LOW);
//   delayMicroseconds(2);
//   digitalWrite(trigPin, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(trigPin, LOW);
//   duration = pulseIn(echoPin, HIGH);
//   distance = duration * 0.0344 / 2;
//   Serial.print("Distance: ");
//   Serial.print(distance);
//   Serial.println(" cm");
// }