#include <WiFi.h>
#include "Arduino.h"
#include "DHTesp.h"
#include "FirebaseESP32.h"
#include <time.h>

const char* ssid = "TRUEBUSINESS";
const char* password = "Geova$2019";
const int pinDHT = 15;
DHTesp dht;

#define FIREBASE_HOST "https://monitor-bebes-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "YFZ31vqNWjMIEvGHxTwg2dLuqQGxDa1cFrtyN2HX"

#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_G3  196
#define NOTE_A3  220
#define NOTE_B3  247

bool buzz = false;
const int sensorSonido = 14;
const int pinBuzzer = 13;
FirebaseData firebaseData;
FirebaseConfig firebaseConfig;

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  // Wi-Fi connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  firebaseConfig.api_key = FIREBASE_AUTH;
  firebaseConfig.database_url = FIREBASE_HOST;

  Firebase.begin(&firebaseConfig, nullptr);
  Firebase.reconnectWiFi(true);

  dht.setup(pinDHT, DHTesp::DHT11);
  pinMode(pinBuzzer, OUTPUT);
  pinMode(sensorSonido, INPUT);
  analogReadResolution(10);

}

void loop(){
  time_t now = time(nullptr);

  // Convertir la época UNIX a una estructura de tiempo
  struct tm timeinfo;
  localtime_r(&now, &timeinfo);

  // Formatear la fecha y hora como una cadena
  char formattedDate[50];
  strftime(formattedDate, sizeof(formattedDate), "%d/%m/%Y %H:%M:%S", &timeinfo);

  // Convertir la cadena a String (si lo necesitas)
  String dateString = String(formattedDate);

  TempAndHumidity data = dht.getTempAndHumidity();
  String info = String(data.temperature,2);
  if (Firebase.setString(firebaseData, "/LECTURAS/temp", info)) {
    Serial.println("Datos enviados a Firebase con éxito."); 
    Firebase.pushString(firebaseData, "/Sensores/Temperatura/lecturas", dateString + " " + info);
  } else {
    Serial.println("Error al enviar a Firebase: " + firebaseData.errorReason()); 
  }
  
  Serial.println(info);
  buzz = Firebase.getBool(firebaseData, "/BUZZER/Valor");
  Serial.println(buzz);
  if (buzz){
    melodia();
    buzz = false;
    Firebase.setBool(firebaseData, "/BUZZER/Valor", false);
  }
  Serial.println(buzz);

  int ruido = analogRead(sensorSonido); // Leer el valor analógico del sensor
  String desc;
  Serial.println(ruido);
  Serial.print(" valor de sonido");
  if (ruido <= 300 ){
    desc = "Silencio";
  } else if(ruido <= 600){
    desc = "Poco Ruido";
  } else if(ruido <= 800){
    desc = "Ruido Moderado";
  } else {
    desc = "Mucho Ruido";
  }
  Serial.println(desc);
  if (Firebase.setString(firebaseData, "/LECTURAS/sonido", desc)){
    Firebase.pushString(firebaseData, "/Sensores/Sonido/lecturas", dateString + " " + desc);
  } else {
    Serial.println("Error al enviar a Firebase: " + firebaseData.errorReason()); 
  }
  Serial.println("datosenviados");

  delay(1000);
}

void melodia (){
  int melodia_3[] = {
    NOTE_C4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_C4, NOTE_G4,
    NOTE_A4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_C4, NOTE_G4, NOTE_A4, NOTE_G4,
    NOTE_F4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_G4, NOTE_A4, NOTE_G4,
    NOTE_F4, NOTE_E4, NOTE_C4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_E4,
    NOTE_C4
  };

  int duraciones_3[] = {
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    2
  };
    
  tocarMelodia(melodia_3, duraciones_3, sizeof(melodia_3) / sizeof(melodia_3[0]));
  delay(2000);
}

void tocarMelodia(int *melodia, int *duraciones, int tamano) {
  for (int nota = 0; nota < tamano; nota++) {
    int duracionNota = 1000 / duraciones[nota];
    tone(pinBuzzer, melodia[nota], duracionNota);
    delay(duracionNota * 1.3);
    noTone(pinBuzzer);
    delay(duracionNota * 0.1); // Pausa entre notas
  }
}

