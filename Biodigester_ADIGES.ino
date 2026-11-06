#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>
#include "MyWebServer.h"

// ================= KONFIGURASI WIFI =================
const char* ssid = "hakim";
const char* password = "hakimhakim";

// ================= KONFIGURASI PIN =================  
#define PIN_DS18B20 4
#define PIN_PH 5
#define PIN_PRESSURE 6
#define PIN_BUZZER 7
#define PIN_RELAY_VALVE 17
#define PIN_RELAY_HEATER 18
#define PIN_RELAY_MOTOR 8
#define PIN_I2C_SDA 10
#define PIN_I2C_SCL 11
#define PIN_RELAY_EM4 13
#define PIN_RELAY_MOLASE  14
#define PIN_RELAY_AIR 1
#define PIN_TOMBOL_MOTOR 2
#define PIN_TRIG 15
#define PIN_ECHO 16

// ================= KONFIGURASI FITUR =================
#define USE_ULTRASONIC false  // Set true jika sensor sudah terpasang
const int LEVEL_AIR_DEFAULT = 75;

// ================= KONSTANTA SISTEM =================
const unsigned long LCD_INTERVAL = 2000;
const unsigned long DURASI_ISI_AIR = 10000;      // 10 detik
const unsigned long DOSING_EM4 = 5000;           // 5 detik
const unsigned long DOSING_MOLASE = 3000;        // 3 detik
const unsigned long DURASI_MOTOR_MANUAL = 60000; // 1 menit
const unsigned long DURASI_MOTOR_AUTO = 60000;   // 1 menit
const unsigned long INTERVAL_MOTOR_AUTO = 3600000;// 1 jam
const unsigned long AUTO_RETURN_MOTOR = 7200000; // 2 jam
const unsigned long MAX_ISI_AIR_PER_JAM = 3;
const unsigned long WINDOW_ISI_AIR = 3600000;    // 1 jam

// Setpoint
const float SUHU_MIN = 30.0;
const float SUHU_MAX = 34.0;
const float SUHU_OVERHEAT = 45.0;
const float SUHU_ISI_AIR = 40.0;
const float TEKANAN_MIN_BUKA = 0.15;
const float TEKANAN_MIN_TUTUP = 0.12;
const float TEKANAN_MAX_TABUNG = 1.2;
const float TEKANAN_KRITIS = 1.5;
const float LEVEL_MAKSIMAL = 90.0;
const float DELTA_LEVEL_PENGURASAN = 20.0;

// ================= KONFIGURASI SENSOR =================
OneWire oneWire(PIN_DS18B20);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27, 16, 2);
NewPing sonar(PIN_TRIG, PIN_ECHO, 200);

// ================= VARIABEL GLOBAL =================
float suhu = 0.0;
float phValue = 0.0;
float tekanan = 0.0;
float levelAir = LEVEL_AIR_DEFAULT;
float levelAirSebelumnya = LEVEL_AIR_DEFAULT;

bool relayHeaterState = false;
bool relayMotorState = false;
bool relayValveState = false;
bool relayAirState = false;
bool relayEm4State = false;
bool relayMolaseState = false;
bool buzzerState = false;

bool isHeaterManual = false;
bool isValveManual = false;
bool isMotorManual = false;

bool alarmTabungPenuh = false;
bool alarmKritis = false;
bool alarmOverheat = false;

unsigned long lastLCDUpdate = 0;
unsigned long waktuMotorMulai = 0;
unsigned long waktuMotorAutoTerakhir = 0;
unsigned long waktuManualTerakhir = 0;
unsigned long waktuIsiAirMulai = 0;
unsigned long waktuDosingMulai = 0;
unsigned long waktuPengurasanTerakhir = 0;
unsigned long waktuTabungDiganti = 0;
unsigned long waktuAlarmMulai = 0;

int counterIsiAir = 0;
unsigned long waktuResetCounterIsiAir = 0;

bool isMotorRunning = false;
bool isIsiAirRunning = false;
bool isDosingRunning = false;
bool isPengurasan = false;

// ================= FUNGSI BACA SENSOR =================
void readSensors() {
  sensors.requestTemperatures();
  suhu = sensors.getTempCByIndex(0);
  if (suhu == DEVICE_DISCONNECTED_C) suhu = 0.0;

  int phRaw = analogRead(PIN_PH);
  phValue = (phRaw / 4095.0) * 14.0;

  int presRaw = analogRead(PIN_PRESSURE);
  float voltage = (presRaw / 4095.0) * 3.3;
  tekanan = (voltage - 0.33) * (1.2 / (2.97 - 0.33));
  if (tekanan < 0) tekanan = 0;

  #if USE_ULTRASONIC
    int distance = sonar.ping_cm();
    if (distance == 0) distance = 250;
    levelAirSebelumnya = levelAir;
    levelAir = ((250.0 - distance) / 250.0) * 100.0;
    if (levelAir < 0) levelAir = 0;
    if (levelAir > 100) levelAir = 100;

    // Deteksi pengurasan (level turun drastis)
    if ((levelAirSebelumnya - levelAir) > DELTA_LEVEL_PENGURASAN) {
      isPengurasan = true;
      waktuPengurasanTerakhir = millis();
      Serial.println("PENGURASAN TERDETEKSI! Katup gas ditutup sementara.");
    }

    // Reset status pengurasan setelah 30 detik stabil
    if (isPengurasan && (millis() - waktuPengurasanTerakhir > 30000)) {
      isPengurasan = false;
      Serial.println("Pengurasan selesai. Katup gas kembali normal.");
    }
  #else
    levelAir = LEVEL_AIR_DEFAULT;
  #endif
}

// ================= FUNGSI BUZZER =================
void buzzerBeep(int jumlahBeep, int durasiOn, int durasiOff) {
  static unsigned long lastBeep = 0;
  static int beepCount = 0;
  static bool isOn = false;

  if (jumlahBeep == -1) {
    // Mode alarm terus menerus
    if (millis() - lastBeep > (isOn ? durasiOn : durasiOff)) {
      isOn = !isOn;
      digitalWrite(PIN_BUZZER, isOn ? HIGH : LOW);
      buzzerState = isOn;
      lastBeep = millis();
    }
  } else {
    // Mode beep terbatas
    if (beepCount < jumlahBeep) {
      if (millis() - lastBeep > (isOn ? durasiOn : durasiOff)) {
        isOn = !isOn;
        digitalWrite(PIN_BUZZER, isOn ? HIGH : LOW);
        buzzerState = isOn;
        lastBeep = millis();
        if (!isOn) beepCount++;
      }
    } else {
      digitalWrite(PIN_BUZZER, LOW);
      buzzerState = false;
      beepCount = 0;
    }
  }
}

// ================= FUNGSI KONTROL RELAY =================
void controlRelays() {
  // ============ HEATING PAD ============
  if (!isHeaterManual) {
    if (suhu < SUHU_MIN) {
      digitalWrite(PIN_RELAY_HEATER, HIGH);
      relayHeaterState = true;
    } else if (suhu > SUHU_MAX) {
      digitalWrite(PIN_RELAY_HEATER, LOW);
      relayHeaterState = false;
    }
  } else {
    digitalWrite(PIN_RELAY_HEATER, relayHeaterState ? HIGH : LOW);
  }

  // Safety Overheat
  if (suhu > SUHU_OVERHEAT) {
    digitalWrite(PIN_RELAY_HEATER, LOW);
    relayHeaterState = false;
    isHeaterManual = false;
    alarmOverheat = true;
  }
  if (suhu < (SUHU_OVERHEAT - 5)) alarmOverheat = false;

  // ============ SELENOID VALVE (Normally Open Logic) ============
  if (!isValveManual) {
    // Alarm Tabung Penuh
    if (tekanan >= TEKANAN_MAX_TABUNG) {
      alarmTabungPenuh = true;
      digitalWrite(PIN_RELAY_VALVE, LOW); // Tutup katup
      relayValveState = false;
    }
    // Alarm Kritis
    else if (tekanan >= TEKANAN_KRITIS) {
      alarmKritis = true;
      digitalWrite(PIN_RELAY_VALVE, LOW);
      relayValveState = false;
    }
    // Pengurasan
    else if (isPengurasan) {
      digitalWrite(PIN_RELAY_VALVE, LOW);
      relayValveState = false;
    }
    // Logika Normal (dengan histeresis)
    else if (!alarmTabungPenuh) {
      if (tekanan >= TEKANAN_MIN_BUKA) {
        digitalWrite(PIN_RELAY_VALVE, HIGH);
        relayValveState = true;
      } else if (tekanan < TEKANAN_MIN_TUTUP) {
        digitalWrite(PIN_RELAY_VALVE, LOW);
        relayValveState = false;
      }
    }
  } else {
    digitalWrite(PIN_RELAY_VALVE, relayValveState ? HIGH : LOW);
  }

  // ============ MOTOR PENGADUK ============
  // Cek tombol fisik
  if (digitalRead(PIN_TOMBOL_MOTOR) == HIGH) {
    delay(50); // Debounce
    if (digitalRead(PIN_TOMBOL_MOTOR) == HIGH) {
      if (!isMotorRunning) {
        isMotorRunning = true;
        waktuMotorMulai = millis();
        waktuManualTerakhir = millis();
        isMotorManual = true;
        Serial.println("Motor triggered via tombol fisik!");
      }
    }
  }

  // Logika Motor
  if (isMotorRunning) {
    digitalWrite(PIN_RELAY_MOTOR, HIGH);
    relayMotorState = true;
    if (millis() - waktuMotorMulai > DURASI_MOTOR_MANUAL) {
      isMotorRunning = false;
      digitalWrite(PIN_RELAY_MOTOR, LOW);
      relayMotorState = false;
      Serial.println("Motor selesai.");
    }
  } else if (!isMotorManual) {
    // Mode Otomatis: nyala 1 menit setiap 1 jam
    if (millis() - waktuMotorAutoTerakhir > INTERVAL_MOTOR_AUTO) {
      waktuMotorAutoTerakhir = millis();
      waktuMotorMulai = millis();
      isMotorRunning = true;
      digitalWrite(PIN_RELAY_MOTOR, HIGH);
      relayMotorState = true;
      Serial.println("Motor auto-start.");
    }
  }

  // Auto-return ke otomatis setelah 2 jam tanpa interaksi manual
  if (isMotorManual && (millis() - waktuManualTerakhir > AUTO_RETURN_MOTOR)) {
    isMotorManual = false;
    Serial.println("Auto-return motor ke mode otomatis.");
  }

  // ============ POMPA AIR (Isi Air) ============
  if (isIsiAirRunning) {
    if (millis() - waktuIsiAirMulai > DURASI_ISI_AIR) {
      isIsiAirRunning = false;
      digitalWrite(PIN_RELAY_AIR, LOW);
      relayAirState = false;
      Serial.println("Isi air selesai.");
    }
  }

  // Reset counter isi air setiap 1 jam
  if (millis() - waktuResetCounterIsiAir > WINDOW_ISI_AIR) {
    counterIsiAir = 0;
    waktuResetCounterIsiAir = millis();
  }

  // ============ EM4 & MOLASE (Logika AND) ============
  bool slurryMasuk = (levelAir > LEVEL_AIR_DEFAULT + 5);
  bool airMasuk = relayAirState;
  bool motorNyala = relayMotorState;

  if (slurryMasuk && airMasuk && motorNyala) {
    if (!isDosingRunning) {
      isDosingRunning = true;
      waktuDosingMulai = millis();
      digitalWrite(PIN_RELAY_EM4, HIGH);
      digitalWrite(PIN_RELAY_MOLASE, HIGH);
      relayEm4State = true;
      relayMolaseState = true;
      Serial.println("Dosing EM4 + Molase aktif!");
    }
  }

  if (isDosingRunning) {
    if (millis() - waktuDosingMulai > max(DOSING_EM4, DOSING_MOLASE)) {
      isDosingRunning = false;
      digitalWrite(PIN_RELAY_EM4, LOW);
      digitalWrite(PIN_RELAY_MOLASE, LOW);
      relayEm4State = false;
      relayMolaseState = false;
      Serial.println("Dosing selesai.");
    }
  }
}

// ================= FUNGSI LCD =================
void updateLCD() {
  if (millis() - lastLCDUpdate < LCD_INTERVAL) return;
  lastLCDUpdate = millis();

  lcd.setCursor(0, 0);
  lcd.print("T:"); lcd.print(suhu, 1); lcd.print("C ");
  lcd.print("pH:"); lcd.print(phValue, 1); lcd.print(" ");

  lcd.setCursor(0, 1);
  lcd.print("P:"); lcd.print(tekanan, 2); lcd.print("M ");
  lcd.print("L:"); lcd.print(levelAir, 0); lcd.print("% ");
}

// ================= FUNGSI KONTROL KHUSUS =================
void resetAlarmTabung() {
  alarmTabungPenuh = false;
  alarmKritis = false;
  waktuTabungDiganti = millis();
  Serial.println("Tabung baru dikonfirmasi. Alarm direset.");
}

void mulaiIsiAir() {
  if (counterIsiAir >= 3) {
    Serial.println("Limit isi air tercapai (max 3x/jam).");
    return;
  }
  if (levelAir >= LEVEL_MAKSIMAL) {
    Serial.println("Level air sudah maksimal. Tidak bisa isi air.");
    return;
  }
  isIsiAirRunning = true;
  waktuIsiAirMulai = millis();
  relayAirState = true;
  digitalWrite(PIN_RELAY_AIR, HIGH);
  counterIsiAir++;
  Serial.println("Isi air dimulai. Counter: " + String(counterIsiAir));
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  // Setup Pin
  pinMode(PIN_RELAY_VALVE, OUTPUT);
  pinMode(PIN_RELAY_HEATER, OUTPUT);
  pinMode(PIN_RELAY_MOTOR, OUTPUT);
  pinMode(PIN_RELAY_EM4, OUTPUT);
  pinMode(PIN_RELAY_MOLASE, OUTPUT);
  pinMode(PIN_RELAY_AIR, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_TOMBOL_MOTOR, INPUT_PULLDOWN);

  digitalWrite(PIN_RELAY_VALVE, LOW);
  digitalWrite(PIN_RELAY_HEATER, LOW);
  digitalWrite(PIN_RELAY_MOTOR, LOW);
  digitalWrite(PIN_RELAY_EM4, LOW);
  digitalWrite(PIN_RELAY_MOLASE, LOW);
  digitalWrite(PIN_RELAY_AIR, LOW);
  digitalWrite(PIN_BUZZER, LOW);

  sensors.begin();

  // Init I2C & LCD
  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("ADIGES Booting...");
  delay(1000);
  lcd.clear();

  // WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  setupWebServer();

  lcd.setCursor(0, 0);
  lcd.print("IP: ");
  lcd.print(WiFi.localIP());
  delay(2000);
  lcd.clear();

  waktuMotorAutoTerakhir = millis();
  waktuResetCounterIsiAir = millis();
}

// ================= LOOP =================
void loop() {
  handleWebClient();
  readSensors();
  controlRelays();

  // Update alarm buzzer
  if (alarmKritis) {
    buzzerBeep(-1, 200, 200); // Alarm kritis: cepat
  } else if (alarmOverheat) {
    buzzerBeep(-1, 300, 300); // Overheat: sedang
  } else if (alarmTabungPenuh) {
    buzzerBeep(-1, 500, 500); // Tabung penuh: lambat
  } else {
    digitalWrite(PIN_BUZZER, LOW);
    buzzerState = false;
  }

  updateLCD();
  delay(100);
}