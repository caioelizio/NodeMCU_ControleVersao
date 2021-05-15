
// BIBLIOTECAS
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include <FS.h>

#include <TimeLib.h>
#include <ArduinoJson.h>

// DEFINIÇÕES DE CONSTANTES
// Wi-Fi
const char* WIFI_SSID     = "adriene.vivo";
const char* WIFI_PASSWORD = "developer";


// Intervalo NTP (60s * 60m * 6 = 6h)
const int NTP_INT         = 60 * 60 * 6;

// Porta Servidor Web
const byte WEBSERVER_PORT = 80;
// Fuso Horário
const int8_t TIMEZONE     = -3;

// Tamanho do Objeto JSON
const   size_t JSON_SIZE  = JSON_OBJECT_SIZE(8) + 390;
//erro reniciando const   size_t JSON_SIZE  = JSON_OBJECT_SIZE(8) + 1000;

// ENTIDADES
  // Classe WebServer para ESP8266
  ESP8266WebServer  server(WEBSERVER_PORT);
