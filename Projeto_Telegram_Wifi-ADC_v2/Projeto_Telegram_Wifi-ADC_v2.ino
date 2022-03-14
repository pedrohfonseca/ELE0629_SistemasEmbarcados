#include <driver/adc.h>
#include <esp_adc_cal.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

char ssid[] = "NETJAT_PAULO HENRIQUE";
char password[] = "994974069";

#define BOTtoken "1644238684:AAHfWsQ0ZSoMuY8K9DiNJKd2-H4ONJKtQp4"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

String id, text, msg;
unsigned long tempo;

esp_adc_cal_characteristics_t adc_cal; // Estrutura de calibração
esp_adc_cal_characteristics_t adc_cal2;

void setup() {
  Serial.begin(115200);
  
  // WIFI
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { // Conecta ao Wi-Fi
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // CALIBRAÇÃO DO ADC
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_0); // Configura o canal
  adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);

  esp_adc_cal_value_t adc_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_0, ADC_WIDTH_BIT_12, 1100, &adc_cal); // Inicializa a calibração
  esp_adc_cal_value_t adc_type2 = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_cal2);
}

void loop() {
  if (millis() - tempo > 2000)  {

    tempo = millis();//Reseta o tempo
 
    int newmsg = bot.getUpdates(bot.last_message_received + 1); // Faz a verificação de novas mensagens
    for(int i = 0; i < newmsg; i++){
      id = bot.messages[i].chat_id; // Quando recebe novo update, armazena o ID de quem enviou
      text = bot.messages[i].text; // Quando recebe novo update, armazena o texto da mensagem
      text.toUpperCase(); // Transforma o texto em maiúsculo 

      if(text.indexOf("TEMPERATURA") > -1){
        msg = String("Temperatura: " + getTemperature() + " °C");
        bot.sendMessage(id, msg, ""); // Envia a mensagem pro ID
      }
      else if(text.indexOf("ILUMINANCIA") > -1){
        msg = String("Iluminância: " + getIlluminance() + " lux");
        bot.sendMessage(id, msg, "");
      }
      else if(text.indexOf("TUDO") > -1){
        msg = String("Temperatura: " + getTemperature() + " °C\nIluminância: " + getIlluminance() + " lux");
        bot.sendMessage(id, msg, "");
      } else {
        bot.sendMessage(id, "Comando inválido", "");
      }
    }
  }
}
String getTemperature(){
  unsigned long int voltage = 0;
  String celsius;
  for(int i = 0; i < 100; i++){
    voltage += adc1_get_raw(ADC1_CHANNEL_7);
    delay(30);
  }
  voltage /= 100;
  voltage = esp_adc_cal_raw_to_voltage(voltage, &adc_cal);
  celsius = String(0.1*voltage);
  return celsius;
}

String getIlluminance(){
  unsigned long int voltage = 0;
  double RLDR = 0;
  String lux;
  for(int i = 0; i < 100; i++){
    voltage += adc1_get_raw(ADC1_CHANNEL_6);
    delay(30);
  }
  voltage /= 100;
  voltage = esp_adc_cal_raw_to_voltage(voltage, &adc_cal2);
  RLDR = (10000.0 * (3300 - voltage))/voltage; 
  lux = String(12518931*pow(RLDR, -1.405));
  return lux;
}
