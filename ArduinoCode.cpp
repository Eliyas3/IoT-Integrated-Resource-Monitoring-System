#include <WiFi.h>
#include <FirebaseESP32.h>
#include <DHT.h>

#define WIFI_SSID "your_SSID"
#define WIFI_PASSWORD "your_PASSWORD"

#define FIREBASE_HOST "your-project-id.firebaseio.com"
#define FIREBASE_AUTH "your-database-secret-key"

FirebaseData firebaseData;

#define DHTPIN 4 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define ACS712_PIN 34  

void setup() {
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("\nConnected to WiFi!");

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);

    dht.begin();
}

void loop() {

    float temperature = dht.readTemperature();

    int rawValue = analogRead(ACS712_PIN);
    float voltage = rawValue * (5.0 / 4095.0);
    float current = (voltage - 2.5) / 0.185; 

    if (Firebase.setFloat(firebaseData, "/resource_monitor/temperature", temperature)) {
        Serial.println("Temperature updated!");
    } else {
        Serial.println(firebaseData.errorReason());
    }

    if (Firebase.setFloat(firebaseData, "/resource_monitor/current", current)) {
        Serial.println("Current updated!");
    } else {
        Serial.println(firebaseData.errorReason());
    }

    delay(5000); 
}
