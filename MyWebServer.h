// MyWebServer.h
#ifndef MY_WEB_SERVER_H
#define MY_WEB_SERVER_H

#include <WebServer.h>
#include "index_html.h"
#include "style_css.h"
#include "script_js.h"

WebServer server(80);

// Forward declaration fungsi dari .ino
void resetAlarmTabung();
void mulaiIsiAir();

// Variabel eksternal dari file .ino
extern bool relayHeaterState;
extern bool relayMotorState;
extern bool relayValveState;
extern bool relayAirState;
extern bool relayEm4State;
extern bool relayMolaseState;
extern bool buzzerState;
extern float suhu;
extern float phValue;
extern float tekanan;
extern float levelAir;
extern bool isHeaterManual;
extern bool isValveManual;
extern bool isMotorManual;
extern bool alarmTabungPenuh;
extern bool alarmKritis;
extern bool alarmOverheat;
extern bool isMotorRunning;
extern bool isIsiAirRunning;
extern bool isDosingRunning;
extern unsigned long waktuTabungDiganti;
extern int counterIsiAir;

// Fungsi kontrol dari .ino
extern void resetAlarmTabung();
extern void mulaiIsiAir();

void handleData() {
  String json = "{";
  json += "\"suhu\":" + String(suhu) + ",";
  json += "\"ph\":" + String(phValue) + ",";
  json += "\"tekanan\":" + String(tekanan) + ",";
  json += "\"level\":" + String(levelAir) + ",";
  json += "\"relay_heater\":" + String(relayHeaterState ? "true" : "false") + ",";
  json += "\"relay_motor\":" + String(relayMotorState ? "true" : "false") + ",";
  json += "\"relay_valve\":" + String(relayValveState ? "true" : "false") + ",";
  json += "\"relay_air\":" + String(relayAirState ? "true" : "false") + ",";
  json += "\"relay_em4\":" + String(relayEm4State ? "true" : "false") + ",";
  json += "\"relay_molase\":" + String(relayMolaseState ? "true" : "false") + ",";
  json += "\"is_heater_manual\":" + String(isHeaterManual ? "true" : "false") + ",";
  json += "\"is_valve_manual\":" + String(isValveManual ? "true" : "false") + ",";
  json += "\"is_motor_manual\":" + String(isMotorManual ? "true" : "false") + ",";
  json += "\"alarm_tabung\":" + String(alarmTabungPenuh ? "true" : "false") + ",";
  json += "\"alarm_kritis\":" + String(alarmKritis ? "true" : "false") + ",";
  json += "\"alarm_overheat\":" + String(alarmOverheat ? "true" : "false") + ",";
  json += "\"motor_running\":" + String(isMotorRunning ? "true" : "false") + ",";
  json += "\"isi_air_running\":" + String(isIsiAirRunning ? "true" : "false") + ",";
  json += "\"dosing_running\":" + String(isDosingRunning ? "true" : "false") + ",";
  json += "\"counter_isi_air\":" + String(counterIsiAir);
  json += "}";
  server.send(200, "application/json", json);
}

void handleControl() {
  String device = server.arg("device");
  String state = server.arg("state");
  bool val = (state == "1");

  if (device == "valve") {
    isValveManual = true;
    relayValveState = val;
    digitalWrite(17, val ? HIGH : LOW);
  } else if (device == "motor") {
    isMotorManual = true;
    relayMotorState = val;
    digitalWrite(8, val ? HIGH : LOW);
  } else if (device == "heater") {
    isHeaterManual = true;
    relayHeaterState = val;
    digitalWrite(18, val ? HIGH : LOW);
  } else if (device == "auto_all") {
    isHeaterManual = false;
    isValveManual = false;
    isMotorManual = false;
  } else if (device == "auto_valve") {
    isValveManual = false;
  } else if (device == "auto_heater") {
    isHeaterManual = false;
  } else if (device == "auto_motor") {
    isMotorManual = false;
  } else if (device == "reset_tabung") {
    resetAlarmTabung();
  } else if (device == "isi_air") {
    mulaiIsiAir();
  }

  server.send(200, "text/plain", "OK");
}

void handleRoot() { server.send_P(200, "text/html", index_html); }
void handleCSS() { server.send_P(200, "text/css", style_css); }
void handleJS() { server.send_P(200, "application/javascript", script_js); }

void setupWebServer() {
  server.on("/", handleRoot);
  server.on("/style.css", handleCSS);
  server.on("/script.js", handleJS);
  server.on("/data", handleData);
  server.on("/control", handleControl);
  server.begin();
  Serial.println("Web Server started!");
}

void handleWebClient() {
  server.handleClient();
}

#endif