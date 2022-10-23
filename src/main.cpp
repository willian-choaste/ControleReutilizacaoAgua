/*-------- Bibliotecas ----------- */
#include <WiFi.h>
#include <WiFiClient.h>

/* -------- Declaração Portas Digitais ----------- */
#define nivelMaxCax 36
#define nivelMedCax 39
#define nivelMinCax 34
#define nivelMaxCis 27
#define nivelMedCis 14
#define nivelMinCis 12
#define nivelLadCis 13
#define valvulaHorta 32
#define valvulaSanitario 33
#define valvulaLimpeza 25
#define bombaCaixa 26
#define bombaLadrao 5
#define ledHorta 4
#define ledSanitario 2
#define ledLimpeza 15
#define botaoManualHorta 23
#define botaoManualSanitario 22
#define botaoManualLimpeza 1

/* -------- Declaração Variavéis Globais ----------- */
int nivelAguaCaixa = 0;
int nivelAguaCisterna = 0;
int leituraPh = 0;
bool flagFaltaAguaCaixa = false;
bool flagNivelAguaCisterna = false;
bool flagExcedeNivelAguaCisterna = false;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  pinMode(nivelMaxCax, INPUT);
  pinMode(nivelMinCax, INPUT);
  pinMode(nivelMedCax, INPUT);
  pinMode(nivelMaxCis, INPUT);
  pinMode(nivelMedCis, INPUT);
  pinMode(nivelMinCis, INPUT);
  pinMode(nivelLadCis, INPUT);
  pinMode(botaoManualHorta, INPUT);
  pinMode(botaoManualSanitario, INPUT);
  pinMode(botaoManualLimpeza, INPUT);
  pinMode(valvulaHorta, OUTPUT);
  pinMode(valvulaSanitario, OUTPUT);
  pinMode(valvulaLimpeza, OUTPUT);
  pinMode(bombaCaixa, OUTPUT);
  pinMode(bombaLadrao, OUTPUT);
  pinMode(ledHorta, OUTPUT);
  pinMode(ledSanitario, OUTPUT);
  pinMode(ledLimpeza, OUTPUT);  
}

void nivelCisterna(){

  bool statusNivelMinCisterna = digitalRead(nivelMinCis);
  bool statusNivelMedCisterna = digitalRead(nivelMedCis);
  bool statusNivelMaxCisterna = digitalRead(nivelMaxCis);
  bool statusNivelLadCisterna = digitalRead(nivelLadCis);

  if(statusNivelLadCisterna == HIGH && statusNivelMaxCisterna == HIGH){
    Serial.print("Excede Nivel Maximo \n");
    flagExcedeNivelAguaCisterna = true;      // Defini o valor como verdadeiro para a variavel flagExcedeNivelAguaCisterna
    nivelAguaCisterna = 4;
    delay(1000);
  } 
  else{
    flagExcedeNivelAguaCisterna = false; // Defini o valor como false para a variavel flagExcedeNivelAguaCisterna
    
    if(statusNivelMaxCisterna == HIGH && statusNivelMedCisterna == HIGH){ // Verifica se sensor nivel maximo e sensor nivel medio estão atuados
      Serial.print("Nivel Maximo Cisterna \n"); // Exibe mensangem no monitor serial
  //   Blynk.virtualWrite(V4, 100);   // Escreve o valor 100 no pino virtual V4
      flagNivelAguaCisterna = false;      // Defini o valor como falso para a variavel flagNivelAguaCisterna      
      nivelAguaCisterna = 3;
      delay(1000);
    }
    else if(statusNivelMaxCisterna == LOW && statusNivelMedCisterna == HIGH){ // Verifica se sensor nivel maximo esta desatuado e sensor nivel medio atuado
      Serial.print("Nivel Medio Cisterna \n"); // Exibe mensangem no monitor serial
  //    Blynk.virtualWrite(V4, 65);   // Escreve o valor 65 no pino virtual V4
      flagNivelAguaCisterna = false;       // Defini o valor como falso para a variavel flagNivelAguaCisterna
      nivelAguaCisterna = 2;
      delay(1000);
    } 
    else if(statusNivelMedCisterna == LOW && statusNivelMinCisterna == HIGH){ // Verifica se sensor nivel meido esta desatuado e sensor nivel minimo atuado
      Serial.print("Nivel Minimo Cisterna \n"); // Exibe mensangem no monitor serial
  //    Blynk.virtualWrite(V4,30);         // Escreve o valor 30 no pino virtual V4
      flagNivelAguaCisterna = false;       // Defini o valor como falso para a variavel flagNivelAguaCisterna
      nivelAguaCisterna = 1;
      delay(100);
    }   
    else{
      Serial.print("Falta água na cisterna \n"); // Exibe mensangem no monitor serial
  //    Blynk.virtualWrite(V4, 0);               // Escreve o valor 0 no pino virtual V4
      flagNivelAguaCisterna = true;              // Defini o valor como verdadeiro para a variavel flagNivelAguaCisterna
      delay(1000);    
    }
  }
}

void nivelCaixa(){
  
  bool statusNivelMinCaixa = digitalRead(nivelMinCax);
  bool statusNivelMedCaixa = digitalRead(nivelMedCax);
  bool statusNivelMaxCaixa = digitalRead(nivelMaxCax);
  
  if(statusNivelMaxCaixa == HIGH && statusNivelMedCaixa == HIGH){ // Verifica se sensor nivel maximo e sensor nivel medio estão atuados
    Serial.print("Nivel Maximo Caixa \n"); // Exibe mensangem no monitor serial
 //   Blynk.virtualWrite(V4, 100);   // Escreve o valor 100 no pino virtual V4
    flagFaltaAguaCaixa = false;      // Defini o valor como falso para a variavel flagFaltaRacao
    nivelAguaCaixa = 3;
    delay(1000);
  }
  else if(statusNivelMaxCaixa == LOW && statusNivelMedCaixa == HIGH){ // Verifica se sensor nivel maximo esta desatuado e sensor nivel medio atuado
   	Serial.print("Nivel Medio Caixa \n"); // Exibe mensangem no monitor serial
//    Blynk.virtualWrite(V4, 65);   // Escreve o valor 65 no pino virtual V4
    flagFaltaAguaCaixa = false;     // Defini o valor como falso para a variavel flagFaltaRacao
    nivelAguaCaixa = 2;
    delay(1000);
  } 
  else if(statusNivelMedCaixa == LOW && statusNivelMinCaixa == HIGH){ // Verifica se sensor nivel meido esta desatuado e sensor nivel minimo atuado
   	Serial.print("Nivel Minimo Caixa\n"); // Exibe mensangem no monitor serial
//    Blynk.virtualWrite(V4,30);     // Escreve o valor 30 no pino virtual V4
    flagFaltaAguaCaixa = false;      // Defini o valor como falso para a variavel flagFaltaRacao
    nivelAguaCaixa = 1;
    delay(100);
  }   
  else{
    Serial.print("Falta água na caixa \n"); // Exibe mensangem no monitor serial
//    Blynk.virtualWrite(V4, 0);            // Escreve o valor 0 no pino virtual V4
    flagFaltaAguaCaixa = true;              // Defini o valor como verdadeiro para a variavel flagFaltaRacao
    delay(1000);    
  }
}

void funcionamentoBombaCaixa(){

  nivelCaixa();
  nivelCisterna();

  if(flagFaltaAguaCaixa == true){
    while(nivelAguaCaixa != 3 && flagNivelAguaCisterna == false){
      nivelCaixa();
      nivelCisterna();
      digitalWrite(bombaCaixa, HIGH);
    }    
  } else{
    digitalWrite(bombaCaixa, LOW);
  }
}

void funcionamentoBombaLadrao(){

  if(flagExcedeNivelAguaCisterna == true){
    digitalWrite(bombaLadrao, HIGH);
  }else{
    digitalWrite(bombaLadrao, LOW);
  }
}

void comandoManual(){
}

void comandoApp(){
}

void controlePh(){ // Verifica os niveis de PH na água

  if(leituraPh == 3){
    digitalWrite(valvulaHorta, HIGH);
    digitalWrite(valvulaSanitario, HIGH);
    digitalWrite(valvulaLimpeza, HIGH);
  }else if(leituraPh == 2){
    digitalWrite(valvulaHorta, LOW);
    digitalWrite(valvulaSanitario, HIGH);
    digitalWrite(valvulaLimpeza, HIGH);
  }else if(leituraPh == 1){
    digitalWrite(valvulaHorta, LOW);
    digitalWrite(valvulaSanitario, LOW);
    digitalWrite(valvulaLimpeza, HIGH);
  }else{
    digitalWrite(valvulaHorta, LOW);
    digitalWrite(valvulaSanitario, LOW);
    digitalWrite(valvulaLimpeza, LOW);
  }
}

void controleTds() { // Verifica os niveis de TDS na água
}

void funcionamentoSistema(){
}

void loop(){
  leituraPh = 3;
  delay(10);
  controlePh();
  funcionamentoBombaCaixa();
  funcionamentoBombaLadrao();
}
