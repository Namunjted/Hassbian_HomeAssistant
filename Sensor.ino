#include <ShiftLCD.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#define DHTTYPE DHT11   // DHT 11
// Cập nhật thông tin
// Thông tin về wifi
#define ssid "lophocvui3.com"
#define password "88889999"
// Thông tin về MQTT Broker
#define mqtt_server "192.168.1.243" // Thay bằng địa chỉ IP 
#define mqtt_topic_pub "test"   //Giữ nguyên nếu bạn tạo topic tên là demo
#define mqtt_topic_sub "test"
#define mqtt_user "HA"    //Giữ nguyên nếu bạn tạo user là lophocvui và pass là 123456
#define mqtt_pwd "123456"
 
const uint16_t mqtt_port = 1808; //Port của MQTT
 
 
WiFiClient espClient;
PubSubClient client(espClient);
 
const int DHTPin = D1;       //Đọc dữ liệu từ DHT11 ở chân D4 trên mạch esp8266
 
long lastMsg = 0;
char msg[50];
int value = 0;
// Timers auxiliar variables
long now = millis();
long lastMeasure = 0;
 
// Thiet Lap cam bien DHT.
DHT dht(DHTPin, DHTTYPE);
ShiftLCD lcd(13, 12, 14);
void setup() {
  Serial.begin(115200);
  setup_wifi();
  pinMode(D4,INPUT);
  pinMode(D2,INPUT);
  client.setServer(mqtt_server, mqtt_port); 
  dht.begin();
 
}
// Hàm kết nối wifi
void setup_wifi() {
  delay(10);
  Serial.println();
  lcd.begin(16, 2);
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
}
// Hàm reconnect thực hiện kết nối lại khi mất kết nối với MQTT Broker
void reconnect() {
  // Chờ tới khi kết nối
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Thực hiện kết nối với mqtt user và pass
    if (client.connect("ESP8266Client",mqtt_user, mqtt_pwd)) {
      Serial.println("connected");
      // Khi kết nối sẽ publish thông báo
      client.publish(mqtt_topic_pub, "ESP_reconnected");
      // ... và nhận lại thông tin này
      client.subscribe(mqtt_topic_sub);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Đợi 5s
      delay(5000);
    }
  }
}
void PIR()
{
  if(digitalRead(D4))
  {
    client.publish("sensor/pir", "Motioned");
    Serial.print("\nMotioned");
  }
  else
  { 
    client.publish("sensor/pir", "No");
    Serial.print("No");
  }
}
void LDR()
{
  if(digitalRead(D2))
  {
    client.publish("sensor/ldr", "Light");
    Serial.print("\tLight");
  }
  else
  { 
    client.publish("sensor/ldr", "Dark");
    Serial.print("\tDark");
  }
}
void loop() {
 
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("ESP8266Client");

  now = millis();
  // Publishes new temperature and humidity every 30 seconds
  if (now - lastMeasure > 1000) {
    lastMeasure = now;
    PIR();
    LDR();
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
      Serial.println("\nFailed to read from DHT sensor!");
      return;
    }
    static char temperatureTemp[7];
    dtostrf(t, 6, 2, temperatureTemp);
    
    static char humidityTemp[7];
    dtostrf(h, 6, 2, humidityTemp);

    lcd.setCursor(0, 0);
    lcd.printf("Temp:%s*C",temperatureTemp);
    lcd.setCursor(0, 1);
    lcd.printf("Hum:%s",humidityTemp);
    lcd.print("%");

    client.publish("sensor/temperature", temperatureTemp);
    client.publish("sensor/humidity", humidityTemp);
    
    Serial.print("\nHumidity: ");
    Serial.print(h);
    Serial.print(" %\t Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print("\n ");
  }
}
