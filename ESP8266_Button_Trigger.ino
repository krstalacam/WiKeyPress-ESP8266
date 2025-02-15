#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "TurkTelekom_TP36C2_2.4GHz"; // wifi adınızı girin
const char* password = "VtwcWcqwhd7P"; // Şifrenizi girin

ESP8266WebServer server(80);
const int buttonPin = D1;  // D2 yerine D1 kullanılabilir
// Python sunucusunun portu
const int serverPort = 5000;
String serverIP = "";

void setup() {
  Serial.begin(115200);
  
  // Wi-Fi'ye bağlanma işlemi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi'ye bağlanıldı!");
  Serial.print("ESP8266 IP Adresi: ");
  Serial.println(WiFi.localIP());
  
  // Python sunucusunun IP adresini oluştur (aynı ağdaki bilgisayarın IP'si)
  serverIP = WiFi.localIP().toString();
  serverIP = serverIP.substring(0, serverIP.lastIndexOf('.'))
  + ".102"; // burada 100 yerine serveri çalıştırdığımız cihazın ip adresinizin son rakamlarini girmeniz lazim

  Serial.print("Python Sunucu IP Adresi: ");
  Serial.println(serverIP);

  pinMode(buttonPin, INPUT_PULLUP);
  server.begin();
}

void loop() {
  if (digitalRead(buttonPin) == HIGH) {  // Butona basıldığında
    sendDataToServer();
  }
  server.handleClient();
}

void sendDataToServer() {
  WiFiClient client;
  HTTPClient http;

  String url = "http://" + serverIP + ":" + String(serverPort) + "/receiveData";
  Serial.print("Bağlanılacak URL: ");
  Serial.println(url);

  http.begin(client, url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpResponseCode = http.POST("buttonPressed=true");
  
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("HTTP Yanıt Kodu: " + String(httpResponseCode));
    Serial.println("Sunucu Yanıtı: " + response);
  } else {
    Serial.println("Hata Kodu: " + String(httpResponseCode));
  }
  
  http.end();
}
