#define BLYNK_PRINT Serial
#define DHTPIN 15     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define BUTTON 5
#define PIR 4
#define LED 2

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <Wire.h>
#include <BH1750.h>


BlynkTimer timer;
WidgetLED led1(V2);
WidgetLED led2(V3);
DHT dht(DHTPIN, DHTTYPE);
BH1750 lightMeter;

char auth[] = "TOKEN_DO_BLYNK";
char ssid[] = "NOME_DA_REDE";
char pass[] = "SENHA_DA_REDE";

int interruptor_blynk = 0;
int alarme_blynk = 0;
int interruptor_fisico = 0;

void sendSensor()
{
  float temp = dht.readTemperature();
  float umid = dht.readHumidity();
  uint16_t lux = lightMeter.readLightLevel();

  if (isnan(temp) || isnan(umid)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Blynk.virtualWrite(V4, temp);      // Enviar temperatura
  Blynk.virtualWrite(V5, umid);      // Enviar umiddade
  Blynk.virtualWrite(V6, lux);       // Enviar luminosidade
  
  delay(100);
}

BLYNK_WRITE(V0)
{   
  if(param.asInt() == 1)
  {
    interruptor_blynk = !interruptor_blynk;
  }
}

BLYNK_WRITE(V1)
{   
  alarme_blynk = param.asInt();
}

void setup()
{
  Serial.begin(115200);
  dht.begin();
  Wire.begin();
  lightMeter.begin();

  pinMode(LED, OUTPUT);

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();

  int presence = digitalRead(PIR);
  
  if(digitalRead(BUTTON) == 1)
  {
    interruptor_fisico = !interruptor_fisico;
  }
  
  if(interruptor_blynk == 0 && interruptor_fisico == 1)
  {
    digitalWrite(LED, HIGH);
    led1.on();
  }
  else if(interruptor_blynk == 1 && interruptor_fisico == 0)
  {
    digitalWrite(LED, HIGH);
    led1.on();
  }
  else if(interruptor_blynk == 0 && interruptor_fisico == 0)
  {
    digitalWrite(LED, LOW);
    led1.off();
  }
  else if(interruptor_blynk == 1 && interruptor_fisico == 1)
  {
    digitalWrite(LED, LOW);
    led1.off();
  }

  if(alarme_blynk == 1 && presence == 1)
  {
    led2.on();
  }
  else
  {
    led2.off();
  }
  delay(100);
}
