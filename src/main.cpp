// ==================================================================================================================
// --- Definições de acesso App Blynk ---
#define BLYNK_TEMPLATE_ID "TMPLIFh0Jm-U"
#define BLYNK_DEVICE_NAME "Sistema de Controle e Monitoramento Água da Chuva"
#define BLYNK_AUTH_TOKEN "Token do projeto Blynk"
#define BLYNK_PRINT Serial

char auth[] = BLYNK_AUTH_TOKEN;

// ==================================================================================================================
// --- Bibliotecas ---
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <NTPClient.h> 

// ==================================================================================================================
// --- Configurações rede Wi-fi ---
char ssdi[] = "rede";
char pass[] = "senha";

// ==================================================================================================================
// --- Configurações de relógio on-line ---
WiFiUDP udp;
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000); // Cria um objeto "NTP" com as configurações.utilizada no Brasil
String horaAtual;  

// ==================================================================================================================
// --- Declaração Portas Digitais ---
#define nivelExtCax  4 // Sensor nível execedente caixa d'água
#define nivelMaxCax 36 // Sensor nível máximo caixa d'água
#define nivelMedCax 39 // Sensor nível médio caixa d'água
#define nivelMinCax 34 // Sensor nível mínimo caixa d'água

#define nivelExtCis 27 // Sensor nível execedente caixa cisterna
#define nivelMaxCis 14 // Sensor nível máximo caixa cisterna
#define nivelMedCis 12 // Sensor nível médio caixa cisterna
#define nivelMinCis 13 // Sensor nível mínimo caixa cisterna

#define valvulaHorta       32 // Eletroválvula horta
#define valvulaSanitario   33 // Eletroválvula sanitario
#define valvulaLimpeza     25 // Eletroválvula limpeza
#define valvulaEsgotamento 26 // Eletroválvula esgotamento

#define bombaCaixa  18 // Bomba d'água sentido caixa d'agua
#define bombaLadrao  5 // Bomba d'água sentido esgoto

#define sensorTds   35 // Monitoramento sensor TDS (Total de Sólidos Dissolvidos)

/*
#define ledHorta        3 // Led sinalização eletroválvula horta em funcionamento
#define ledSanitario    2 // Led sinalização eletroválvula sanitario em funcionamento
#define ledLimpeza     15 // Led sinalização eletroválvula limpeza em funcionamento
#define ledEsgotamento 16 // Led sinalização eletroválvula esgotamento em funcionamento

#define seletoraModoManual        23 // Seletora habilita operação em modo manual físico
#define seletoraManualHorta       22 // Seletora habilita funcionamento eltroválvula horta
#define seletoraManualSanitario   21 // Seletora habilita funcionamento eltroválvula sanitário
#define seletoraManualLimpeza     19 // Seletora habilita funcionamento eltroválvula limpeza
#define seletoraManualEsgotamento 17 // Seletora habilita funcionamento eltroválvula esgotamento
*/

// ==================================================================================================================
// --- Variáveis Globais ---
int nivelAguaCaixa    = 0;
int nivelAguaCisterna = 0;
int seletoraManualApp = 0;
int seletoraHortaApp  = 0;
int seletoraSanitarioApp = 0;
int seletoraLimpezaApp   = 0;
int seletoraEsgotamentoCaixaApp = 0;
int ledManualApp     = 0;
int leituraSensorTds = 0;
/*
int ledHortaApp = 0;
int ledSanitarioApp = 0;
int ledLimpezaApp = 0;
int ledEsgotamentoCaixaApp = 0;
*/

double valorTds = 0;
double voltagemTds = 0;

bool flagFaltaAguaCaixa = false;
bool flagNivelAguaCisterna = false;
bool flagExcedeNivelAguaCaixa = false;
bool flagExcedeNivelAguaCisterna = false;
bool habilitaComandoApp = false;

// ==================================================================================================================
// --- Funções para leitura pinos virtuias App Blynk ---

BLYNK_WRITE(V0){ // Atribui o valor de entrada do pino virtual V0 a variável seletoraManualApp
  int pinValue = param.asInt(); 
  seletoraManualApp = pinValue;
}

BLYNK_WRITE(V1){ // Atribui o valor de entrada do pino virtual V1 a variável seletoraHortaApp
  int pinValue = param.asInt(); 
  seletoraHortaApp = pinValue;
}

BLYNK_WRITE(V2){ // Atribui o valor de entrada do pino virtual V2 a variável seletoraSanitarioApp
  int pinValue = param.asInt(); 
  seletoraSanitarioApp = pinValue;
}

BLYNK_WRITE(V3){ // Atribui o valor de entrada do pino virtual V3 a variável seletoraLimpezaApp
  int pinValue = param.asInt(); 
  seletoraLimpezaApp = pinValue;
}

BLYNK_WRITE(V10){ // Atribui o valor de entrada do pino virtual V4 a variável seletoraEsgotamentoCaixaApp
  int pinValue = param.asInt(); 
  seletoraEsgotamentoCaixaApp = pinValue;
}

// ==================================================================================================================
// --- Monitoramento nível de água na cisterna ---
void nivelCisterna(){

  bool statusNivelMinCisterna = digitalRead(nivelMinCis); //Defini a variavel statusNivelMinCisterna como boleano e esta recebe o valor da leitura da entrada nivelMinCis
  bool statusNivelMedCisterna = digitalRead(nivelMedCis); //Defini a variavel statusNivelMedCisterna como boleano e esta recebe o valor da leitura da entrada nivelMedCis
  bool statusNivelMaxCisterna = digitalRead(nivelMaxCis); //Defini a variavel statusNivelMaxCisterna como boleano e esta recebe o valor da leitura da entrada nivelMaxCis
  bool statusnivelExtCisterna = digitalRead(nivelExtCis); //Defini a variavel statusnivelExtCisterna como boleano e esta recebe o valor da leitura da entrada nivelExtCis

 if(statusnivelExtCisterna == HIGH && statusNivelMaxCisterna == HIGH && statusNivelMedCisterna == HIGH && statusNivelMinCisterna == HIGH){ // Verifica se todos os sensores de nivel estão atuados
    Serial.print("Excede Nivel Maximo \n"); // Exibe a mensangem no monitor serial
    flagExcedeNivelAguaCisterna = true;     // Defini o valor como verdadeiro para a variavel flagExcedeNivelAguaCisterna
    nivelAguaCisterna = 4;                  // Defini o valor 4 para a variavel flagNivelAguaCisterna
    delay(1000);
  } 
  else{                                  // Caso o nível de água na cisterna não execeda o máximo
    flagExcedeNivelAguaCisterna = false; // Defini o valor como false para a variavel flagExcedeNivelAguaCisterna
    
    if(statusNivelMaxCisterna == HIGH && statusNivelMedCisterna == HIGH && statusNivelMinCisterna == HIGH){ // Verifica se os sensores de nível máximo, médio e mínimo estão atuados
      Serial.print("Nivel Maximo Cisterna \n"); // Exibe a mensangem no monitor serial
      Blynk.virtualWrite(V8, 100);              // Escreve o valor 100 no pino virtual V8 (visualização do nível de água no aplicativo)
      flagNivelAguaCisterna = false;            // Defini o valor como falso para a variavel flagNivelAguaCisterna      
      nivelAguaCisterna = 3;                    // Defini o valor 3 para a variavel nivelAguaCisterna
      delay(1000);
    }
    else if(statusNivelMaxCisterna == LOW && statusNivelMedCisterna == HIGH && statusNivelMinCisterna == HIGH){ // Verifica se o sensor de nível máximo está desatuado e os sensores de nível médio e mínimo estão atuados
      Serial.print("Nivel Medio Cisterna \n"); // Exibe a mensangem no monitor serial
      Blynk.virtualWrite(V8, 65);              // Escreve o valor 65 no pino virtual V8 (visualização do nível de água no aplicativo)
      flagNivelAguaCisterna = false;           // Defini o valor como falso para a variavel flagNivelAguaCisterna
      nivelAguaCisterna = 2;                   // Defini o valor 2 para a variavel nivelAguaCisterna
      delay(1000);
    } 
    else if(statusNivelMaxCisterna == LOW && statusNivelMedCisterna == LOW && statusNivelMinCisterna == HIGH){ // Verifica se os sensores de nível máximo e médio estão desatuado e o sensor de nível mínimo está atuado
      Serial.print("Nivel Minimo Cisterna \n"); // Exibe mensangem no monitor serial
      //nivel_minimo_caixa_da_agua
      Blynk.virtualWrite(V8,30);                // Escreve o valor 30 no pino virtual V8 (visualização do nível de água no aplicativo)
      flagNivelAguaCisterna = false;            // Defini o valor como falso para a variavel flagNivelAguaCisterna
      nivelAguaCisterna = 1;                    // Defini o valor 1 para a variavel nivelAguaCisterna
      delay(1000);
    }   
    else{                                                              // Caso nenhum sensor estiver atuado, é constatado que não há água na cisterna
      Serial.print("Falta água na cisterna \n");                       // Exibe mensangem no monitor serial
      Blynk.logEvent("falta_agua_cistrena", "Falta água na cisterna"); // Exibe notificação no aplicativo
      Blynk.virtualWrite(V8, 0);                                       // Escreve o valor 0 no pino virtual V8 (visualização do nível de água na cisterna no aplicativo)
      flagNivelAguaCisterna = true;                                    // Defini o valor como verdadeiro para a variavel flagNivelAguaCisterna
      delay(1000);    
    }
  }
}

// ==================================================================================================================
// --- Monitoramento nível de água na caixa d'água ---
void nivelCaixa(){
  
  bool statusNivelMinCaixa = digitalRead(nivelMinCax); // Defini a variavel statusNivelMinCaixa como boleano e esta recebe o valor da leitura da entrada nivelMinCax
  bool statusNivelMedCaixa = digitalRead(nivelMedCax); // Defini a variavel statusNivelMedCaixa como boleano e esta recebe o valor da leitura da entrada nivelMedCax
  bool statusNivelMaxCaixa = digitalRead(nivelMaxCax); // Defini a variavel statusNivelMaxCaixa como boleano e esta recebe o valor da leitura da entrada nivelMaxCax
  bool statusnivelExtCaixa = digitalRead(nivelExtCax); // Defini a variavel statusnivelExtCaixa como boleano e esta recebe o valor da leitura da entrada nivelExtCax 
  
  if(statusnivelExtCaixa == HIGH && statusNivelMaxCaixa == HIGH && statusNivelMedCaixa == HIGH && statusNivelMinCaixa == HIGH){ // Verifica se todos os sensores de nivel estão atuados
    Serial.print("Excede Nivel Maximo na Caixa \n"); // Exibe a mensangem no monitor serial
    flagExcedeNivelAguaCaixa = true;     // Defini o valor como verdadeiro para a variavel flagExcedeNivelAguaCisterna
    nivelAguaCaixa = 4;                  // Defini o valor 4 para a variavel flagNivelAguaCisterna
    digitalWrite(valvulaEsgotamento, HIGH); // Caso o exceda o limite de água na caixa, a valvula de esgotamento é acionada
    delay(1000);
  } 
  else{

    if(statusNivelMaxCaixa == HIGH && statusNivelMedCaixa == HIGH && statusNivelMinCaixa == HIGH){ // Verifica se os sensores de nível máximo, médio e mínimo estão atuados
      Serial.print("Nivel Maximo Caixa \n"); // Exibe a mensangem no monitor serial
      Blynk.virtualWrite(V9, 100);           // Escreve o valor 100 no pino virtual V9 (visualização do nível de água na caixa no aplicativo)
      flagFaltaAguaCaixa = false;            // Defini o valor como falso para a variavel flagFaltaAguaCaixa
      nivelAguaCaixa = 3;                    // Defini o valor 3 para a variavel nivelAguaCaixa
      delay(1000);
    }
    else if(statusNivelMaxCaixa == LOW && statusNivelMedCaixa == HIGH && statusNivelMinCaixa == HIGH){ // Verifica se o sensor de nível máximo está desatuado e os sensores de nível médio e mínimo estão atuados
      Serial.print("Nivel Medio Caixa \n"); // Exibe a mensangem no monitor serial
      Blynk.virtualWrite(V9, 65);           // Escreve o valor 65 no pino virtual V9 (visualização do nível de água na caixa no aplicativo)
      flagFaltaAguaCaixa = false;           // Defini o valor como falso para a variavel flagFaltaAguaCaixa
      nivelAguaCaixa = 2;                   // Defini o valor 2 para a variavel nivelAguaCaixa
      delay(1000);
    } 
    else if(statusNivelMaxCaixa == LOW && statusNivelMedCaixa == LOW && statusNivelMinCaixa == HIGH){ // Verifica se os sensores de nível máximo e médio estão desatuado e o sensor de nível mínimo está atuado
      Serial.print("Nivel Minimo Caixa\n"); // Exibe a mensangem no monitor serial
      Blynk.virtualWrite(V9,30);            // Escreve o valor 30 no pino virtual V9 (visualização do nível de água na caixa no aplicativo)
      flagFaltaAguaCaixa = false;           // Defini o valor como falso para a variavel flagFaltaAguaCaixa
      nivelAguaCaixa = 1;                   // Defini o valor 1 para a variavel nivelAguaCaixa
      delay(100);
    }   
    else{                                                       // Caso nenhum sensor estiver atuado, é constatado que não há água na caixa
      Serial.print("Falta água na caixa \n");                   // Exibe a mensangem no monitor serial
      Blynk.logEvent("falta_agua_caixa", "Falta água na caixa!!"); // Exibe notificação no aplicativo
      Blynk.virtualWrite(V9, 0);                                // Escreve o valor 0 no pino virtual V9 (visualização do nível de água na caixa no aplicativo)
      flagFaltaAguaCaixa = true;                                // Defini o valor como verdadeiro para a variavel flagFaltaAguaCaixa
      delay(1000);    
    }
  } 
}

// ==================================================================================================================
// --- Definição do funcionamento da bomba d'água, água sentido cisterna para caixa ---
void funcionamentoBombaCaixa(){

  if(flagFaltaAguaCaixa == true){     // Verifica se falta água na caixa
    while(nivelAguaCaixa != 3 && flagNivelAguaCisterna == false){ // Este loop aciona a bomba caso o nível da aguá seja diferente de máximo e caso haja água na cisterna
      nivelCaixa();                   // Verfica condições para nível de água na caixa
      nivelCisterna();                // Verfica condições para nível de água na caixa
      digitalWrite(bombaCaixa, HIGH); // Liga a bomba d'água para abastecimento da caixa
    }    
  } else{                             // Caso tenha água na caixa
    digitalWrite(bombaCaixa, LOW);    // Desliga a bomba d'água para abastecimento da caixa
  }
}

// ==================================================================================================================
// --- Definição do funcionamento da bomba d'água, água sentido cisterna para esgoto ---
void funcionamentoBombaLadrao(){

  if(flagExcedeNivelAguaCisterna == true){ // Verifica se o nível de água na cisterna excedeu o máximo
    digitalWrite(bombaLadrao, HIGH);       // Liga a bomba ladrão, que joga água para a rede de esgoto
  }else{                                   // Caso o nível de água na cisterna não execeda o máximo
    digitalWrite(bombaLadrao, LOW);        // Desliga a bomba ladrão, que joga água para a rede de esgoto
  }
}

/*
// ==================================================================================================================
// --- Definição do funcionamento comandos manuais via botoeira física ---
void comandoManual(){

  Blynk.virtualWrite(V12, 1);

  int statusSeletoraHorta = digitalRead(seletoraManualHorta);
  int statusSeletoraSanitario = digitalRead(seletoraManualSanitario);
  int statusSeletoraLimpeza = digitalRead(seletoraManualLimpeza);

  if (statusSeletoraHorta == HIGH){
   // digitalWrite(ledHorta, HIGH);
    digitalWrite(valvulaHorta, HIGH);    
  } else if (statusSeletoraHorta == LOW){
   // digitalWrite(ledHorta, LOW);
    digitalWrite(valvulaHorta, LOW);
  }

  if (statusSeletoraSanitario == HIGH){
    digitalWrite(ledSanitario, HIGH);
    digitalWrite(valvulaSanitario, HIGH);
  } else if (statusSeletoraSanitario == LOW){
    digitalWrite(ledSanitario, LOW);
    digitalWrite(valvulaSanitario, LOW);
  }

  if (statusSeletoraLimpeza == HIGH){
    digitalWrite(ledLimpeza, HIGH);
    digitalWrite(valvulaLimpeza, HIGH);
  } else if (statusSeletoraLimpeza == LOW){
    digitalWrite(ledLimpeza, LOW);
    digitalWrite(valvulaLimpeza, LOW);
  }
}
*/

// ==================================================================================================================
// --- Definição do funcionamento comandos manuais via App ---
void comandoApp(){
  
  Blynk.virtualWrite(V4, 1);                                // Escreve o valor 1 no pino virtual V4 (led visualização no aplicativo do modo manual atuado)
  
  Blynk.virtualWrite(V14, "OFF");                           // Escreve o "OFF" no pino virtual V14 (Visualização no aplicativo do modo automático desabilitado)
  Blynk.virtualWrite(V15, 0);                               // Escreve o valor 0 no pino virtual V15 (led visualização no aplicativo do modo automático desabilitado)

  bool statusHorta       = digitalRead(valvulaHorta);       // Defini a variavel statusHorta como boleano e esta recebe o valor da leitura da entrada valvulaHorta
  bool statusSanitario   = digitalRead(valvulaSanitario);   // Defini a variavel statusSanitario como boleano e esta recebe o valor da leitura da entrada valvulaSanitario
  bool statusLimpeza     = digitalRead(valvulaLimpeza);     // Defini a variavel statusLimpeza como boleano e esta recebe o valor da leitura da entrada valvulaLimpeza
  bool statusEsgotamento = digitalRead(valvulaEsgotamento); // Defini a variavel statusEsgotamento como boleano e esta recebe o valor da leitura da entrada valvulaEsgotamento

  if(flagFaltaAguaCaixa == false){      // Caso tenha água na caixa, permite ao usuário habilitar qualquer válvula
    if(seletoraHortaApp){               // Se a chave seletora Horta estiver atuada no aplicativo
      digitalWrite(valvulaHorta, HIGH); // Defini o valor HIGH para o pino valvulaHorta 
      Blynk.virtualWrite(V5, 1);        // Escreve o valor 1 no pino virtual V5 (led visualização no aplicativo da valvula horta atuada)
    }
    else{                               // Se a chave seletora Horta não estiver atuada no aplicativo
      digitalWrite(valvulaHorta, LOW);  // Defini o valor LOW para o pino valvulaHorta 
      Blynk.virtualWrite(V5, 0);        // Escreve o valor 0 no pino virtual V5 (led visualização no aplicativo da valvula horta desatuada)
    }

    if(seletoraSanitarioApp){               // Se a chave seletora Sanitário estiver atuada no aplicativo
      digitalWrite(valvulaSanitario, HIGH); // Defini o valor HIGH para o pino valvulaSanitario
      Blynk.virtualWrite(V6, 1);            // Escreve o valor 1 no pino virtual V6 (led visualização no aplicativo da valvula sanitario atuada)
    }
    else{                                  // Se a chave seletora Sanitário não estiver atuada no aplicativo
      digitalWrite(valvulaSanitario, LOW); // Defini o valor LOW para o pino valvulaSanitario
      Blynk.virtualWrite(V6, 0);           // Escreve o valor 0 no pino virtual V6 (led visualização no aplicativo da valvula sanitario desatuada)
    }

    if(seletoraLimpezaApp){               // Se a chave seletora Limpeza estiver atuada no aplicativo
      digitalWrite(valvulaLimpeza, HIGH); // Defini o valor HIGH para o pino valvulaLimpeza
      Blynk.virtualWrite(V7, 1);          // Escreve o valor 1 no pino virtual V7 (led visualização no aplicativo da valvula limpeza atuada)
    }
    else{                                // Se a chave seletora Limpeza não estiver atuada no aplicativo
      digitalWrite(valvulaLimpeza, LOW); // Defini o valor LOW para o pino valvulaLimpeza
      Blynk.virtualWrite(V7, 0);         // Escreve o valor 0 no pino virtual V7 (led visualização no aplicativo da valvula limpeza desatuada)
    }

    if(seletoraEsgotamentoCaixaApp){          // Se a chave seletora Esgotamento estiver atuada no aplicativo
      digitalWrite(valvulaEsgotamento, HIGH); // Defini o valor HIGH para o pino valvulaEsgotamento
      Blynk.virtualWrite(V11, 1);             // Escreve o valor 1 no pino virtual V11 (led visualização no aplicativo da valvula esgotamento atuada)
    }
    else{                                    // Se a chave seletora Esgotamento não estiver atuada no aplicativo
      digitalWrite(valvulaEsgotamento, LOW); // Defini o valor LOW para o pino valvulaEsgotamento
      Blynk.virtualWrite(V11, 0);            // Escreve o valor 0 no pino virtual V11 (led visualização no aplicativo da valvula esgotamento desatuada)
    }
  }
  else{                                    // Caso não tenha água na caixa, são desabilitas as válvulas e os leds de visualização. 
    digitalWrite(valvulaHorta, LOW);       // Defini o valor LOW para o pino valvulaHorta
    Blynk.virtualWrite(V5, 0);             // Escreve o valor 0 no pino virtual V5 (led visualização no aplicativo da valvula horta desatuada)

    digitalWrite(valvulaSanitario, LOW);   // Defini o valor LOW para o pino valvulaSanitario
    Blynk.virtualWrite(V6, 0);             // Escreve o valor 0 no pino virtual V6 (led visualização no aplicativo da valvula sanitario desatuada)
    
    digitalWrite(valvulaLimpeza, LOW);     // Defini o valor LOW para o pino valvulaLimpeza
    Blynk.virtualWrite(V7, 0);             // Escreve o valor 0 no pino virtual V7 (led visualização no aplicativo da valvula limpeza desatuada)

    digitalWrite(valvulaEsgotamento, LOW); // Defini o valor LOW para o pino valvulaEsgotamento
    Blynk.virtualWrite(V11, 0);            // Escreve o valor 0 no pino virtual V11 (led visualização no aplicativo da valvula esgotamento desatuada)
  }

  printf("Manual: %d \n", seletoraManualApp);     // Exibe a mensangem no monitor serial o valor da variavél seletoraManualApp
  printf("Horta: %d \n", statusHorta);            // Exibe a mensangem no monitor serial o valor da variavél statusHorta
  printf("Sanitario: %d \n", statusSanitario);    // Exibe a mensangem no monitor serial o valor da variavél statusSanitario
  printf("Limpeza: %d \n", statusLimpeza);        // Exibe a mensangem no monitor serial o valor da variavél statusLimpeza
  printf("Esgotameto: %d \n", statusEsgotamento); // Exibe a mensangem no monitor serial o valor da variavél statusEsgotamento
  delay(1000);
}

// ==================================================================================================================
// --- Definição do funcionamento em modo automático ---
void modoAutomatico(){
  Serial.print("MODO AUTOMÁTICO \n"); // Exibe a mensangem no monitor serial
  Blynk.virtualWrite(V14, "ON");      // Escreve o "ON" no pino virtual V14 (Visualização no aplicativo do modo automático habilitado)
  Blynk.virtualWrite(V15, 1);         // Escreve o valor 1 no pino virtual V15 (led visualização no aplicativo do modo automático habilitado)

  // Blynk.virtualWrite(V12, 0);       // Escreve o valor 0 no pino virtual V12 (led visualização no aplicativo do modo manual físico desatuado)
  Blynk.virtualWrite(V4, 0);          // Escreve o valor 0 no pino virtual V4  (led visualização no aplicativo do modo manual desatuado)

  Blynk.virtualWrite(V1, 0);          // Escreve o valor 0 no pino virtual V1  (desabilita chva seletora horta no aplicativo)
  Blynk.virtualWrite(V2, 0);          // Escreve o valor 0 no pino virtual V2  (desabilita chva seletora sanitario no aplicativo)
  Blynk.virtualWrite(V3, 0);          // Escreve o valor 0 no pino virtual V3  (desabilita chva seletora limpeza no aplicativo)
  Blynk.virtualWrite(V10, 0);         // Escreve o valor 0 no pino virtual V10 (desabilita chva seletora esgotamento no aplicativo)

  if(flagFaltaAguaCaixa == false){          // Verifica se há água na caixa
    if (valorTds <= 100){                   // Verifica se o valor da variavel valorTds está entre 0 e 100, o que significa alta pureza

      digitalWrite(valvulaHorta, HIGH);     // Defini o valor HIGH para o pino valvulaHorta 
      Blynk.virtualWrite(V5, 1);            // Escreve o valor 1 no pino virtual V5 (led visualização no aplicativo da valvula horta atuada) 

      digitalWrite(valvulaSanitario, HIGH); // Defini o valor HIGH para o pino valvulaSanitario
      Blynk.virtualWrite(V6, 1);            // Escreve o valor 1 no pino virtual V6 (led visualização no aplicativo da valvula sanitario atuada)

      digitalWrite(valvulaLimpeza, HIGH);   // Defini o valor HIGH para o pino valvulaLimpeza
      Blynk.virtualWrite(V7, 1);            // Escreve o valor 1 no pino virtual V7 (led visualização no aplicativo da valvula limpeza atuada)

      digitalWrite(valvulaEsgotamento, LOW); // Defini o valor LOW para o pino valvulaEsgotamento
      Blynk.virtualWrite(V11, 0);            // Escreve o valor 1 no pino virtual V11 (led visualização no aplicativo da valvula esgotamento atuada)

    }
    else if (valorTds > 100 && valorTds <= 200){ // Verifica se o valor da variavel valorTds está entre 100 e 200, o que significa água não filtrada  
      
      digitalWrite(valvulaHorta, LOW);        // Defini o valor LOW para o pino valvulaHorta 
      Blynk.virtualWrite(V5, 0);              // Escreve o valor 0 no pino virtual V5 (led visualização no aplicativo da valvula horta desatuada)

      digitalWrite(valvulaSanitario, HIGH);   // Defini o valor HIGH para o pino valvulaSanitario 
      Blynk.virtualWrite(V6, 1);              // Escreve o valor 1 no pino virtual V6 (led visualização no aplicativo da valvula sanitario atuada)

      digitalWrite(valvulaLimpeza, HIGH);    // Defini o valor HIGH para o pino valvulaLimpeza
      Blynk.virtualWrite(V7, 1);             // Escreve o valor 1 no pino virtual V7 (led visualização no aplicativo da valvula limpeza atuada)

      digitalWrite(valvulaEsgotamento, LOW); // Defini o valor LOW para o pino valvulaHorta
      Blynk.virtualWrite(V11, 0);            // Escreve o valor 0 no pino virtual V11 (led visualização no aplicativo da valvula esgotamento desatuada)

    } 
    else if (valorTds > 200 && valorTds <= 300){ // Verifica se o valor da variavel valorTds está entre 200 e 300, o que significa uma margem aceitável
      
      digitalWrite(valvulaHorta, LOW);       // Defini o valor LOW para o pino valvulaHorta
      Blynk.virtualWrite(V5, 0);             // Escreve o valor 0 no pino virtual V5 (led visualização no aplicativo da valvula horta desatuada)

      digitalWrite(valvulaSanitario, HIGH);  // Defini o valor LOW para o pino valvulaSanitario
      Blynk.virtualWrite(V6, 1);             // Escreve o valor 1 no pino virtual V6 (led visualização no aplicativo da valvula sanitario atuada)

      digitalWrite(valvulaLimpeza, LOW);     // Defini o valor LOW para o pino valvulaLimpeza
      Blynk.virtualWrite(V7, 0);             // Escreve o valor 0 no pino virtual V7 (led visualização no aplicativo da valvula limpeza desatuada)

      digitalWrite(valvulaEsgotamento, LOW); // Defini o valor LOW para o pino valvulaHorta
      Blynk.virtualWrite(V11, 0);            // Escreve o valor 0 no pino virtual V11 (led visualização no aplicativo da valvula esgotamento desatuada)

    }
    else {                                   // Caso o valor da variavel valorTds for superior a 300 a água nã é indicada para nenhuma das nossas aplicações
      
      Serial.print("Valor TDS (Total de Sólidos Diluidos), não aceitável\n");                   // Exibe a mensangem no monitor seria        
      Blynk.logEvent("tds_nao_aceitavel", "Valor TDS (Total de Sólidos Diluidos), não aceitável"); // Exibe notificação no aplicativo

      digitalWrite(valvulaHorta, LOW);        // Defini o valor LOW para o pino valvulaHorta
      Blynk.virtualWrite(V5, 0);              // Escreve o valor 0 no pino virtual V5 (led visualização no aplicativo da valvula horta desatuada)
      
      digitalWrite(valvulaSanitario, LOW);    // Defini o valor LOW para o pino valvulaSanitario
      Blynk.virtualWrite(V6, 0);              // Escreve o valor 0 no pino virtual V6 (led visualização no aplicativo da valvula sanitario desatuada)
      
      digitalWrite(valvulaLimpeza, LOW);      // Defini o valor LOW para o pino valvulaLimpeza
      Blynk.virtualWrite(V7, 0);              // Escreve o valor 0 no pino virtual V7 (led visualização no aplicativo da valvula limpeza desatuada)

      digitalWrite(valvulaEsgotamento, HIGH); // Defini o valor HIGH para o pino valvulaEsgotamento
      Blynk.virtualWrite(V11, 1);             // Escreve o valor 1 no pino virtual V11 (led visualização no aplicativo da valvula esgotamento atuada)
      
      Serial.print("Esgotamentamento em andamento...\n");                      // Exibe a mensangem no monitor serial
      Blynk.logEvent("esgotamento_caixa", "Esgotamentamento em andamento..."); // Exibe notificação no aplicativo

    }    
    delay(1000); 
  }
  else{                                    // Caso não tenha água na caixa, são desabilitas as válvulas e os leds de visualização. 
    
    digitalWrite(valvulaHorta, LOW);       // Defini o valor LOW para o pino valvulaHorta
    Blynk.virtualWrite(V5, 0);             // Escreve o valor 0 no pino virtual V5 (led visualização no aplicativo da valvula horta desatuada)

    digitalWrite(valvulaSanitario, LOW);   // Defini o valor LOW para o pino valvulaSanitario
    Blynk.virtualWrite(V6, 0);             // Escreve o valor 0 no pino virtual V6 (led visualização no aplicativo da valvula sanitario desatuada)
    
    digitalWrite(valvulaLimpeza, LOW);     // Defini o valor LOW para o pino valvulaLimpeza
    Blynk.virtualWrite(V7, 0);             // Escreve o valor 0 no pino virtual V7 (led visualização no aplicativo da valvula limpeza desatuada)

    digitalWrite(valvulaEsgotamento, LOW); // Defini o valor LOW para o pino valvulaEsgotamento
    Blynk.virtualWrite(V11, 0);            // Escreve o valor 0 no pino virtual V11 (led visualização no aplicativo da valvula esgotamento desatuada)
  }  
}
// ==================================================================================================================
// --- Monitoramento sensor TDS ---
void controleTds() { // Verifica os niveis de TDS (Total de Sólidos Diluidos) na água
  leituraSensorTds = analogRead(sensorTds);
  voltagemTds = leituraSensorTds*3.3/4095; // Converte a leitura analógica em voltagem
  valorTds    = (133.42*voltagemTds*voltagemTds*voltagemTds - 255.86*voltagemTds*voltagemTds + 857.39*voltagemTds)*0.5; // Essa equação relaciona tensão com ppm
  Serial.print("TDS = ");                  // Exibe a mensangem no monitor serial
  Serial.print(valorTds);                  // Exibe a mensangem no monitor serial o valor da variavél valorTds
  Serial.println(" ppm");                  // Exibe a mensangem no monitor serial
  Blynk.virtualWrite(V13, valorTds);       // Escreve valorTds no pino virtual V13 (visualização do qualidade TDS(Total de Sólidos Diluidos) na água, através do aplicativo) 
  delay(1000);
}

// ==================================================================================================================
// --- Definições do funcionamento do sistema ---
void funcionamentoSistema(){

  horaAtual = ntp.getFormattedTime();
  Serial.printf("Horário: %s\n", horaAtual); //Exibe no monitor serial

  nivelCaixa();               // Executa função - Monitoramento nível de água na caixa d'água
  nivelCisterna();            // Executa função - Monitoramento nível de água na cisterna
  funcionamentoBombaCaixa();  // Executa função - Definição do funcionamento da bomba d'água, água sentido cisterna para caixa
  funcionamentoBombaLadrao(); // Executa função - Definição do funcionamento da bomba d'água, água sentido cisterna para esgoto
  controleTds();              // Executa função - Monitoramento sensor TDS
 
    /*
    int statusSeletoraManual = digitalRead(seletoraModoManual);

    if(statusSeletoraManual == HIGH){ // Verifica se chave para comando manual fisico esta atuada
      Serial.print("MODO MANUAL \n");
      delay(1000);
      comandoManual();

      Blynk.virtualWrite(V0, 0);
      Blynk.virtualWrite(V4, 0);      // Escreve o valor 0 no pino virtual V4

      digitalWrite(valvulaHorta, LOW);
      Blynk.virtualWrite(V1, 0);
      Blynk.virtualWrite(V5, 0);

      digitalWrite(valvulaSanitario, LOW);
      Blynk.virtualWrite(V2, 0);
      Blynk.virtualWrite(V6, 0);

      digitalWrite(valvulaLimpeza, LOW);
      Blynk.virtualWrite(V3, 0); 
      Blynk.virtualWrite(V7, 0);   

      digitalWrite(valvulaEsgotamento, LOW);
      Blynk.virtualWrite(V10, 0);
      Blynk.virtualWrite(V11, 0);

    }
    else*/
  
  if(seletoraManualApp == HIGH){              // Se a chave seletora comando manual estiver atuada no aplicativo
    Serial.print("MODO MANUAL APP BLYNK \n"); // Exibe a mensangem no monitor serial    
    comandoApp();                             // Executa função - Definição do funcionamento comandos manuais via App
    delay(1000);
  }
  else{ // Caso a chave seletora comando manual não esteja atuada no aplicativo, é executado o modo automatico
    modoAutomatico();
  }
}

// ==================================================================================================================
// --- Configurações Iniciais ---
void setup() {                   // Executa uma vez na inicialização do disposito
  Serial.begin(115200);          // Abre a porta serial, define a taxa de dados para 115200 bps
  Blynk.begin(auth, ssdi, pass); // Realiza conexão com a internet e acesso ao App Blynk  
  pinMode(nivelExtCax, INPUT);   // Defini o pino nivelExtCax como INPUT
  pinMode(nivelMaxCax, INPUT);   // Defini o pino nivelMaxCax como INPUT
  pinMode(nivelMinCax, INPUT);   // Defini o pino nivelMinCax como INPUT
  pinMode(nivelMedCax, INPUT);   // Defini o pino nivelMedCax como INPUT
  pinMode(nivelExtCis, INPUT);   // Defini o pino nivelExtCis como INPUT
  pinMode(nivelMaxCis, INPUT);   // Defini o pino nivelMaxCis como INPUT
  pinMode(nivelMedCis, INPUT);   // Defini o pino nivelMedCis como INPUT
  pinMode(nivelMinCis, INPUT);   // Defini o pino nivelMinCis como INPUT
  pinMode(sensorTds, INPUT);     // Defini o pino sensorTds como INPUT
  /*
  pinMode(seletoraManualHorta, INPUT);     // Defini o pino seletoraManualHorta como INPUT
  pinMode(seletoraManualSanitario, INPUT); // Defini o pino seletoraManualSanitario como INPUT
  pinMode(seletoraManualLimpeza, INPUT);   // Defini o pino seletoraManualLimpeza como INPUT
  pinMode(seletoraModoManual, INPUT);      // Defini o pino seletoraModoManual como INPUT
  */
  pinMode(valvulaHorta, OUTPUT);       // Defini o pino valvulaHorta como OUTPUT
  pinMode(valvulaSanitario, OUTPUT);   // Defini o pino valvulaSanitario como OUTPUT
  pinMode(valvulaLimpeza, OUTPUT);     // Defini o pino valvulaLimpeza como OUTPUT
  pinMode(valvulaEsgotamento, OUTPUT); // Defini o pino valvulaEsgotamento como OUTPUT 
  pinMode(bombaCaixa, OUTPUT);         // Defini o pino bombaCaixa como OUTPUT
  pinMode(bombaLadrao, OUTPUT);        // Defini o pino bombaLadrao como OUTPUT
  /*
  pinMode(ledHorta, OUTPUT);           // Defini o pino ledHorta como OUTPUT
  pinMode(ledSanitario, OUTPUT);       // Defini o pino ledSanitario como OUTPUT
  pinMode(ledLimpeza, OUTPUT);         // Defini o pino ledLimpeza como OUTPUT
  */
}
 
// ==================================================================================================================
// --- Loop Infinito ---
void loop(){
  Blynk.run();            // Processa os comandos de entrada e executa a manutenção da conexão com o servidor Blynk.
  funcionamentoSistema(); // Executa função - Definições do funcionamento do sistema
  delay(1000);
}