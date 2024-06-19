#include <WiFi.h>
#include <DHT.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define DHTPIN 5    // Pin tempat sensor DHT11 dihubungkan
#define DHTTYPE DHT11 // Jenis sensor DHT (DHT11 atau DHT22, bisa disesuaikan)

const char* ssid = "YgpunyaWIBU";     // SSID WiFi
const char* password = "Voldigoad93"; // Password WiFi
const char* serverAddress = "http://192.168.247.207:5000/api/data"; // Alamat server web untuk POST data

DHT dht(DHTPIN, DHTTYPE); // Inisialisasi objek DHT

void setup() {
  Serial.begin(9600);
  Serial.println("Hello, ESP32!");

  dht.begin(); // Mulai sensor DHT

  // Menunggu hingga terhubung ke WiFi
  connectToWiFi();

  // Setup untuk Serial Monitor
  Serial.println("Setup done.");
}

void loop() {
  // Baca nilai kelembapan dan suhu dari sensor DHT
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Memastikan data yang dibaca valid
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000); // Tunggu 2 detik sebelum mengulangi
    return;
  }

  // Membuat DynamicJsonDocument untuk menyimpan data JSON
  DynamicJsonDocument doc(200);
  doc["humidity"] = humidity;
  doc["temperature"] = temperature;
  doc["timestamp"] = millis();

  // Serialize JSON ke dalam String
  String jsonStr;
  serializeJson(doc, jsonStr);

  // Kirim data ke server web melalui HTTP POST
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverAddress);
    http.addHeader("Content-Type", "application/json");

    int httpCode = http.POST(jsonStr);

    if (httpCode > 0) { // Jika respons diterima dari server
      if (httpCode == HTTP_CODE_OK) {
        Serial.println("Data sent successfully");
      } else {
        Serial.println("Unexpected HTTP code: " + String(httpCode));
      }
    } else {
      Serial.println("Error on sending POST: " + http.errorToString(httpCode));
    }

    http.end(); // Selesai dengan objek HTTPClient
  } else {
    Serial.println("WiFi Disconnected. Reconnecting...");
    connectToWiFi(); // Coba untuk menyambungkan kembali ke WiFi
  }

  delay(15000); // Tunggu 15 detik sebelum mengirim data lagi
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}


