#include <Servo.h> 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); 
#define BTN_PLAY A0 

SoftwareSerial serialdobluetooth(4, 5);
int valordobluetooth = 0;
int valorBTempo = 0;
bool escolhaIntensidade = false; 
bool escolhaTempo = false; 
bool blueconect = false;
bool iselect = false;

int nea= 0;

int play_tempo; 
bool contagem_ativa = false; 
unsigned long tempo_inicial; 
unsigned long tempo_atual; 
int tempo_selecionado = 60; 
int tempo_restante; 
bool play = false; 

Servo motor; 
Servo motor2; 
int b1 = 5; 
int b2 = 6; 
const int pinoVE = 11; 
const int pinoAZ = 10; 
const int pinoVD = 9; 
int contador = 0; 
int estadoBotao1 = 0; 
int estadoBotao2 = 0; 
int tempo = 15; 
bool Bt = false;
int intensidade = 1; 

unsigned long tempoUltimoMovimento = 0; 
unsigned long tempoUltimaAtualizacao = 0; 
int intervaloMovimento = 5000;  // Intervalo padrão de 5 segundos 
int intervaloAtualizacao = 1000; // Atualização do LCD a cada 1 segundo 

void setup() { 
  serialdobluetooth.begin(9600);

  pinMode(BTN_PLAY, INPUT_PULLUP); 
  pinMode(pinoVE, OUTPUT);
  pinMode(pinoAZ, OUTPUT); 
  pinMode(pinoVD, OUTPUT); 

  lcd.init(); 
  lcd.backlight(); 
  motor.attach(8); 
  motor2.attach(7); 

  pinMode(b1, INPUT_PULLUP); 
  pinMode(b2, INPUT_PULLUP); 

  lcd.begin(16, 2); 
  Serial.begin(9600); 
} 

void loop() { 
  if (serialdobluetooth.available()) {
    valordobluetooth = serialdobluetooth.read();
    blueconect = true;
    Serial.println("valor enviado:");
    Serial.println(valordobluetooth);
    
    if (blueconect == true && iselect == false && valordobluetooth== 48) {
      lcd.clear(); 
      lcd.setCursor(0, 0); 
      lcd.print("Bluetooth");
      lcd.setCursor(0, 1); 
      lcd.print("conectado");
      delay(2500);
      lcd.clear(); 
      lcd.setCursor(0, 0); 
      lcd.print("Selecione a");
      lcd.setCursor(0, 1); 
      lcd.print("intensidade");
    }

    if (valordobluetooth == '1') {
      escolhaIntensidade = true;
      iselect = true;
      lcd.clear(); 
      lcd.setCursor(0, 0); 
      lcd.print("Intensidade 1");
      lcd.setCursor(0, 1); 
      lcd.print("selecionada");
      Serial.println("Intensidade 1 recebida via Bluetooth");
      intensidade = 1;
    } else if (valordobluetooth == '2') {
      intensidade = 2;
      escolhaIntensidade = true;
      iselect = true;
      lcd.clear(); 
      lcd.setCursor(0, 0); 
      lcd.print("Intensidade 2");
      lcd.setCursor(0, 1); 
      lcd.print("selecionada");
      Serial.println("Intensidade 2 recebida via Bluetooth");
    } else if (valordobluetooth == '3') {
      intensidade = 3;
      escolhaIntensidade = true;
      iselect = true;
      lcd.clear(); 
      lcd.setCursor(0, 0); 
      lcd.print("Intensidade 3");
      lcd.setCursor(0, 1); 
      lcd.print("selecionada");
      Serial.println("Intensidade 3 recebida via Bluetooth");
    } else if (valordobluetooth == '4') {
      intensidade = 4;
      escolhaIntensidade = true;
      iselect = true;
      lcd.clear(); 
      lcd.setCursor(0, 0); 
      lcd.print("Intensidade 4");
      lcd.setCursor(0, 1); 
      lcd.print("selecionada");
      Serial.println("Intensidade 4 recebida via Bluetooth");
    } else if (valordobluetooth == '5') {
      intensidade = 5;
      escolhaIntensidade = true;
      lcd.clear(); 
      lcd.setCursor(0, 0); 
      lcd.print("Intensidade 5");
      lcd.setCursor(0, 1); 
      lcd.print("selecionada");
      Serial.println("Intensidade 5 recebida via Bluetooth");
    }
  }

  if (serialdobluetooth.available() && escolhaIntensidade && !escolhaTempo) {
    valorBTempo = serialdobluetooth.read();
    if (valorBTempo == '6') {
      tempo = 15;
      escolhaTempo = true;
      Serial.println("Tempo 15 segundos recebido via Bluetooth");
    } else if (valorBTempo == '7') {
      tempo = 30;
      escolhaTempo = true;
      Serial.println("Tempo 30 segundos recebido via Bluetooth");
    } else if (valorBTempo == '8') {
      tempo = 45;
      escolhaTempo = true;
      Serial.println("Tempo 45 segundos recebido via Bluetooth");
    } else if (valorBTempo == '9') {
      tempo = 60;
      escolhaTempo = true;
      Serial.println("Tempo 60 segundos recebido via Bluetooth");
    } else if (valorBTempo == '0') {
      tempo = 75;
      escolhaTempo = true;
      Serial.println("Tempo 90 segundos recebido via Bluetooth");
    }
  }

  if (escolhaIntensidade && escolhaTempo) {
    Serial.println("Tempo e intensidade escolhidos");
    contagem_ativa = true;
    tempo_inicial = millis();
    tempo_restante = tempo;

    if (contagem_ativa) { 
      tempo_atual = millis(); 

     if (tempo_atual - tempoUltimaAtualizacao >= intervaloAtualizacao) { 
      tempoUltimaAtualizacao = tempo_atual; 
      int tempo_decorrido = (tempo_atual - tempo_inicial) / 1000; 
      tempo_restante = tempo - tempo_decorrido; 

      if (tempo_restante >= 0) { 
        lcd.clear();
        lcd.setCursor(0, 0); 
        lcd.print("Tempo restante:"); 
        lcd.setCursor(0, 1);  
        lcd.print(tempo_restante); 
        lcd.print(" seg "); 
      } else { 
        contagem_ativa = false; 
        escolhaIntensidade = false; 
        escolhaTempo = false;
        play == false;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Tempo esgotado!"); 
        Serial.println("contagem ativa?:");
        Serial.println(contagem_ativa);
       } 
      }
    }

    if (tempo_atual - tempoUltimoMovimento >= intervaloMovimento) { 
      tempoUltimoMovimento = tempo_atual; 
      if (intensidade == 1) { 
        Serial.println("servos giram");
        motor.write(180); 
        motor2.write(180); 
        delay(3000); 

        motor.write(90); 
        motor2.write(90); 
        delay(10000); 

        motor.write(0); 
        motor2.write(0); 
        delay(3000); 

        motor.write(90); 
        motor2.write(90); 
        delay(10000); 

      } else if (intensidade == 2) { 
        motor.write(180); 
        motor2.write(180); 
        delay(3000); 

        motor.write(90); 
        motor2.write(90); 
        delay(8000); 

        motor.write(0); 
        motor2.write(0); 
        delay(3000);

        motor.write(90); 
        motor2.write(90); 
        delay(8000); 

      } else if (intensidade == 3) { 
        motor.write(180); 
        motor2.write(180); 
        delay(3000); 

        motor.write(90); 
        motor2.write(90); 
        delay(6000); 

        motor.write(0); 
        motor2.write(0); 
        delay(3000); 

        motor.write(90); 
        motor2.write(90); 
        delay(6000); 

      } else if (intensidade == 4) { 
        motor.write(180); 
        motor2.write(180); 
        delay(3000); 

        motor.write(90); 
        motor2.write(90); 
        delay(4000); 

        motor.write(0); 
        motor2.write(0); 
        delay(3000); 

        motor.write(90); 
        motor2.write(90); 
        delay(4000); 

      } else if (intensidade == 5) { 
        motor.write(180); 
        motor2.write(180); 
        delay(3000); 

        motor.write(90); 
        motor2.write(90); 
        delay(3000); 

        motor.write(0); 
        motor2.write(0); 
        delay(3000); 

        motor.write(90); 
        motor2.write(90); 
        delay(3000); 
      }
    }
  }

  estadoBotao1 = !digitalRead(b1); 
  estadoBotao2 = !digitalRead(b2); 
   

  if (estadoBotao1 == 0 && contador == 0 && blueconect == false) { 
    lcd.setCursor(0, 0); 
    lcd.print("Clique no botao"); 
    lcd.setCursor(0, 1); 
    lcd.print("para iniciar"); 
    contador++; 
    Serial.print(1); 
  } 


  if (estadoBotao1 == 1 && nea==0 && contador == 1 && play == false) { 
    lcd.clear(); 
    lcd.setCursor(0, 0); 
    lcd.print("Selecione a"); 
    lcd.setCursor(0, 1);
    lcd.print("velocidade:"); 
    lcd.print(intensidade); 
    Serial.print(2);
    nea=1;
  } 

  if (estadoBotao2 == 1 && Bt == false && nea == 1) { 
    delay(300); 
    intensidade = intensidade + 1; 

    if (intensidade > 5) { 
      intensidade = 1; 
    } 

    lcd.setCursor(11, 1); 
    lcd.print(intensidade); 
    contador = 2; 
    Serial.print(3); 
  } 

  if (estadoBotao1 == 1 && !contagem_ativa && contador == 2 && play == false) { 
    lcd.clear(); 
    lcd.setCursor(0, 0); 
    lcd.print("Selecione o"); 
    lcd.setCursor(0, 1); 
    lcd.print("tempo:"); 
    lcd.print(tempo); 
    Serial.print(4); 
    Bt = true; 
  } 

  if (estadoBotao2 == 1 && Bt == true) { 
    delay(300); 
    tempo = tempo + 15; 

    if (tempo > 75) { 
      tempo = 15; 
    } 

    lcd.setCursor(6, 1); 
    lcd.print(tempo); 
    contador = 4;
    play = true; 
    Serial.print(5); 
    contador = 3; 
  } 

  if (estadoBotao1 == 1 && !contagem_ativa && contador == 3 && play == true) { 
    contagem_ativa = true; 
    tempo_inicial = millis(); 
    tempo_restante = tempo; 
    lcd.clear(); 
    contador = 4; 
    Serial.print(6); 
  } 

  if (contagem_ativa) { 
    tempo_atual = millis(); 

    if (tempo_atual - tempoUltimaAtualizacao >= intervaloAtualizacao) { 
      tempoUltimaAtualizacao = tempo_atual; 
      int tempo_decorrido = (tempo_atual - tempo_inicial) / 1000; 
      tempo_restante = tempo - tempo_decorrido; 

      if (tempo_restante >= 0) { 
        lcd.setCursor(0, 0); 
        lcd.print("Tempo restante:"); 
        lcd.setCursor(0, 1); 
        lcd.print(tempo_restante); 
        lcd.print(" seg "); 
      } else { 
        estadoBotao1 = 0; 
        contador = 0; 
        play = false; 
        Bt = false; 
        contagem_ativa = false; 
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Tempo esgotado!"); 
      } 
    } 

    if (tempo_atual - tempoUltimoMovimento >= intervaloMovimento) { 
      tempoUltimoMovimento = tempo_atual; 

      if (intensidade == 1) { 
        motor.write(180); 
        motor2.write(180); 
        delay(3000); 

        motor.write(90); 
        motor2.write(90); 
        delay(10000); 

        motor.write(0); 
        motor2.write(0); 
        delay(3000); 

        motor.write(90); 
        motor2.write(90); 
        delay(10000); 

      } else if (intensidade == 2) { 
        motor.write(180); 
        motor2.write(180); 
        delay(3000); 

        motor.write(90); 
        motor2.write(90); 
        delay(8000); 

        motor.write(0); 
        motor2.write(0); 
        delay(3000);

        motor.write(90); 
        motor2.write(90); 
        delay(8000); 

      } else if (intensidade == 3) { 
        motor.write(180); 
        motor2.write(180); 
        delay(3000); 

        motor.write(90); 
        motor2.write(90); 
        delay(6000); 

        motor.write(0); 
        motor2.write(0); 
        delay(3000); 

        motor.write(90); 
        motor2.write(90); 
        delay(6000); 

      } else if (intensidade == 4) { 
        motor.write(180); 
        motor2.write(180); 
        delay(3000); 

        motor.write(90); 
        motor2.write(90); 
        delay(4000); 

        motor.write(0); 
        motor2.write(0); 
        delay(3000); 

        motor.write(90); 
        motor2.write(90); 
        delay(4000); 

      } else if (intensidade == 5) { 
        motor.write(180); 
        motor2.write(180); 
        delay(3000); 

        motor.write(90); 
        motor2.write(90); 
        delay(3000); 

        motor.write(0); 
        motor2.write(0); 
        delay(3000); 

        motor.write(90); 
        motor2.write(90); 
        delay(3000); 
      } 
    } 
  } 
}
