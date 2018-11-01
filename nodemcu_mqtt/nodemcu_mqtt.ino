#include <DHT.h>
#include <DHT_U.h>

// Get ESP8266 going with Arduino IDE
// - https://github.com/esp8266/Arduino#installing-with-boards-manager
// Required libraries (sketch -> include library -> manage libraries)
// - PubSubClient by Nick ‘O Leary
// - DHT sensor library by Adafruit

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define wifi_ssid "nettverksnavn" // NETTVERKSNAVN
#define wifi_password "passord" // NETTVERKSPASSORD

#define mqtt_server "ip-adresse" // ip-adresse til mqtt-server
#define mqtt_user "mqtt-bruker" // brukernavn til mqtt-server
#define mqtt_password "mqtt-passord" // passord til bruker på mqtt-server
#define clientId "ESP8266ClientGuestroom" // Unik id per klient. Må endres så den ikke krasjer i mosquitto-serveren mot andre klienter.
#define SENSOR_TOPIC "sensor/guestroom/tempandhumidity"

#define DHTTYPE DHT11
// #define DHTTYPE DHT22 // Bruk denne for DHT22-sensor
#define DHTPIN 2

#define SECONDS 10000

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE); // 11 works fine for ESP8266

void setup()
{
  Serial.begin(115200);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect(clientId, mqtt_user, mqtt_password))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

long lastMsg = 0;
float temp = 0.0;
float hum = 0.0;
float diff = 1.0;

void loop()
{

  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  delay(2000); // Kan fjernes dersom DHT-type ikke er 11
  
  long now = millis();
  if (now - lastMsg > SECONDS)
  {

    StaticJsonBuffer<300> JSONbuffer;
    JsonObject &JSONencoder = JSONbuffer.createObject();

    lastMsg = now;

    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    float hic = dht.computeHeatIndex(temp, hum, false);
 
    JSONencoder["temp"] = temp;
    JSONencoder["humidity"] = hum;
    JSONencoder["heatIndex"] = hic;

    char JSONmessageBuffer[100];
    JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));

    if (client.publish(SENSOR_TOPIC, JSONmessageBuffer) == true)
    {
      Serial.println("Success sending message");
    }
    else
    {
      Serial.println("Error sending message");
    }
  }
}
