#include <Adafruit_Fingerprint.h>

const int ledGreenPin = 7;
const int ledRedPin = 8;

SoftwareSerial mySerial(2, 3); // RX, TX
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
  pinMode(ledGreenPin, OUTPUT);
  pinMode(ledRedPin, OUTPUT);

  Serial.begin(9600);
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Leitor biométrico conectado.");
  } else {
    Serial.println("Falha na conexão com o leitor biométrico.");
    while (1);
  }
}

void loop() {
  if (Serial.available()) {
    char command = Serial.read();
    if (command == '1') {
      digitalWrite(ledGreenPin, HIGH);
      digitalWrite(ledRedPin, LOW);
      Serial.println("Pronto para ler a digital.");
      
      // Inicia a leitura da digital
      int id = getFingerprintID();
      if (id != -1) {
        // Envia o ID da digital reconhecida para o sistema
        Serial.print("ID: ");
        Serial.println(id);
      } else {
        // Caso a leitura falhe, acende o LED vermelho
        digitalWrite(ledGreenPin, LOW);
        digitalWrite(ledRedPin, HIGH);
        Serial.println("Falha na leitura da digital.");
      }
    } else if (command == '0') {
      digitalWrite(ledGreenPin, LOW);
      digitalWrite(ledRedPin, HIGH);
      Serial.println("Leitura finalizada.");
    }
  }
}

int getFingerprintID() {
  finger.getImage();
  int p = finger.image2Tz();
  if (p != FINGERPRINT_OK) {
    return -1;
  }

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) {
    return -1;
  }
  
  return finger.fingerID; // Retorna o ID da digital
}
