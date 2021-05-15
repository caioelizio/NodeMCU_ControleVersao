
#include "config.h"
#include "ControleVersaoLib.h"
#include "websever.h"


void initSerial(){
  Serial.begin(115200);
  delay(500);
  Serial.println(F("\nIniciando Serial..."));
  Serial.println(F("\nDefinindo pinos como INPUT ou OUTPUT..."));
  Serial.println(F("<---------Definindo pinos como ON ou OFF---------->"));
  Serial.print  (F("pinLedonboard ")); Serial.println(F("  OFF"));
  Serial.println(F("<------------------------------------------------->"));
}
void f_Serial_Sistema(){
  Serial.println();
  Serial.println(F("<----------------Informações Sistema-------------->"));
  Serial.print(F("Software:  "));  Serial.println(VCS_SW_VERSION);
  Serial.print(F("Interface: "));  Serial.println(fsCurrentVersion());
  Serial.println(F("<------------------------------------------------->"));
}


void setup() {
  initSerial();
  log("\nIniciando...");

  // SPIFFS
  if (!SPIFFS.begin()) {
    log(F("SPIFFS ERRO"));
    while (true);
  }

  // Conecta WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  log("Conectando WiFi " + String(WIFI_SSID));
  byte b = 0;
  while(WiFi.status() != WL_CONNECTED) {
    b++;
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  log("WiFi conectado (" + String(WiFi.RSSI()) + ") IP " + ipStr(WiFi.localIP()));

  // Define NTP
  setSyncProvider(timeNTP);
  setSyncInterval(NTP_INT);
  log(timeStatus() == timeSet ? F("Data/hora atualizada") : F("Falha atualizando Data/hora"));

  // WebServer
  server.on(F("/vcs") , handleVCS);
  server.onNotFound(handleHome);
  server.begin();

  // Pronto
  log(F("Pronto"));
}
int delayTime = 10000;
unsigned long lastTime = millis();
void loop() {
  yield(); //WatchDog
    
  // Aguarda o tempo defino em delayTime para executar 
  if ((millis() - lastTime) > delayTime) {
    f_Serial_Sistema();
    lastTime = millis();
  }

  vcsCheck();            // Controle de Versão

  server.handleClient(); // Webserver
}
