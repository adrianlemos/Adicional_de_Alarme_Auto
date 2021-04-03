/*
  Resumo: Firmware para comando o módulo que adiociona funções extras a qualquer alarme
          utilizando o sinal da buzzer do alarme como base.
  Board: Arduino Pro Mini Atmega328p 16MHz 5V
  Criador: Adrian Lemos




*/

/*-------------------------------------------------------------+
|                          BIBLIOTECAS                         |
+-------------------------------------------------------------*/
#include <Arduino.h>

/*-------------------------------------------------------------+
|                          CONSTANTES                          |
+-------------------------------------------------------------*/
const byte buzzerInterruptPin = 2;    //Config entrada da Buzzer
const byte setaAcionamento = 9;       //Config Acionamento da Seta
const byte sobeVidro = 3;             //Config do Pino que Aciona a Subida do Vidro
const byte fechaTravaEletrica = 5;    //Config do Pino que Fecha a Trava Elétrica
const byte abreTravaEletrica = 4;     //Config do Pino que Abre a Trava Elétrica
const byte botaoAbreFechaTrava = 13;  //Config do Botão que Abre ou Fecha a Trava Elterica
const byte buzzerAtencao = 11;        //Config do Buzzer que diz se o farol esta ligado
const byte posChave = 6;              //Config da dectação do 12V pós chave
const byte leitorFarolLigado = 7;     //Config do Pino que detecta se o Farol está ligado ou não

const byte LIGADO = 1;                // Constante utilizada para Ligado em nivel logico 1
const byte DESLIGADO = 0;             // Constante utilizada para Desligado em nivel logico 0
const byte LIGADOn = 0;               // Constante utilizada para Ligado em nivel logico 0
const byte DESLIGADOn = 1;            // Constante utilizada para desligado em nivel logico 0


/*-------------------------------------------------------------+
|                       VARIAVEIS GLOBAIS                      |
+-------------------------------------------------------------*/
int valorIterrupcao = 0;
int valorUnicoDeExucucao = 0;

/* Variaveis Globais referente ao Botão*/
int buttonState;             // Estado atual do pino de entrada
int lastButtonState = LOW;   // Ultimo estado do the previous reading from the input pin
int buttonValue = 0; // 

/* A variavel usada é um unsigned longs porque a medida do tempo em milisegundos
é um numero grande demais para ser guardado em apenas um int */
unsigned long lastDebounceTime = 0;  // o Ultimo tempo de quando o botão foi prescionado


/*-------------------------------------------------------------+
|                     PROTÓTIPO DAS FUNÇÕES                    |
+-------------------------------------------------------------*/
void interrupcaoBuzzer();
void tomaDecisaoInterrupcao(int valorLocalInterrupcao);
void sobeVidroEletrico(void);
void piscaSaida(int piscaPino, int piscaDelay1, int piscaDelay2);
void checaBotao(int pinoDoBotao, unsigned long debounceDelay);



/*-------------------------------------------------------------+
|                     LOOP DE CONFIGURAÇÃO                     |
+-------------------------------------------------------------*/
void setup(){
  // Configuração da Velocidade da Porta Serial
  Serial.begin(115200);
  
  //Configuração dos Pinos de IO's
  pinMode(buzzerInterruptPin, INPUT_PULLUP);
  pinMode(setaAcionamento,OUTPUT);
  pinMode(sobeVidro,OUTPUT);
  pinMode(fechaTravaEletrica,OUTPUT);
  pinMode(abreTravaEletrica,OUTPUT);
  pinMode(botaoAbreFechaTrava,INPUT_PULLUP);
  pinMode(buzzerAtencao,OUTPUT);
  pinMode(leitorFarolLigado, INPUT_PULLUP);
  pinMode(posChave, INPUT_PULLUP);
  
  //Configuração da Interrupção
  attachInterrupt(digitalPinToInterrupt(buzzerInterruptPin), interrupcaoBuzzer, FALLING);
  
  digitalWrite(buzzerAtencao, LOW);

  digitalWrite(abreTravaEletrica, LOW);
  digitalWrite(fechaTravaEletrica, LOW);
  digitalWrite(sobeVidro, LOW);

}


/*-------------------------------------------------------------+
|                 LOOP PRINCIPAL DO PROGRAMA                   |
+-------------------------------------------------------------*/
void loop() {
  
  /* Executa funções se o PÓS Chave esta ligado*/  
  if (digitalRead(posChave)== LIGADOn ){

    checaBotao(botaoAbreFechaTrava, 50);
    
  }
  
  /* Executa funções se o Pós Chave esta Desligado */
  if (digitalRead(posChave)== DESLIGADOn){

    /* VErifica se o Farol está ligado e aciona a Buzzer Interna*/
    if (digitalRead(leitorFarolLigado)==LIGADOn){
      Serial.println("Farol: Ligado");
      piscaSaida(buzzerAtencao, 150, 50);
      piscaSaida(buzzerAtencao, 150, 150);
    }
        
    /* Encrementa +1 se Detecta que o Pino da Buzzer do Alarme esta desligado*/
    if (digitalRead(buzzerInterruptPin) == DESLIGADOn){
      valorUnicoDeExucucao++;
      delay(1);
    }

    /* Verifica se o valor lido peça Buzzer do Alarme é maior que zero e se já passou pelo loop
    *  mais de 4 vezes */
    if (valorIterrupcao != 0 && valorUnicoDeExucucao >= 4 ){
    tomaDecisaoInterrupcao(valorIterrupcao);
    }

    /* Ajuda a Limpar o valor da variavel para que não estoure a pilha de memória.*/
    if (valorUnicoDeExucucao >= 1000){
      valorUnicoDeExucucao = 0;
    }
  
    checaBotao(botaoAbreFechaTrava, 50);
    delay(50); // Tempo de espera para executar novamente o Loop
  }

}//FIM DO LOOP

/*-------------------------------------------------------------+
|                            FUNÇÕES                           |
+-------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------+
|  Função que toma decisão após ser detectado a interrupção                          |
|                                                                                    |
|  Resumo: O que esta função faz é utilizar de um debounce por software para que não |
|          aja erro no momento de decisão. É sempre feito o incremento de mais um na |
|          variavel state para q o programa possa tomar suas decisões.               |
+-----------------------------------------------------------------------------------*/
void interrupcaoBuzzer(){

  Serial.println("Entrou na interrupcao");

  static unsigned long last_interrupt_time = 0; // Cria a variavel e atribui o valor 0 a ela
  unsigned long interrupt_time = millis(); // Cria a variavel e atribui o tempo da função millis
  
  // Se a interrupção for detectada com menos de 300ms ele ignora
  if (interrupt_time - last_interrupt_time > 30 && digitalRead(buzzerInterruptPin) == LOW){ 
  
    valorIterrupcao++; //Soma +1 na Variavel
    valorUnicoDeExucucao = 0; //Zera o valor da variavel que ajuda na tomada de decisão
  }
  
  // seta o valor da variavel interrupt_time na variavel last_interrupt_time
  last_interrupt_time = interrupt_time;

}

/*-----------------------------------------------------------------------------------+
|  Toma decisão com base no valor lido na Interrupção
|  Resumo: Caso 1 - Aciona a Seta uma vez, Trava as portas e sobe o Vidro
|          Caso 2 - Aciona a Seta duas vezes, Destrava as portas 
+-----------------------------------------------------------------------------------*/
void tomaDecisaoInterrupcao(int valorLocalInterrupcao){
    
  switch (valorLocalInterrupcao)
  {
    case 1:
      valorIterrupcao = 0; //Zera o valor da interrupção
      
      digitalWrite(fechaTravaEletrica, LIGADO);
      piscaSaida(setaAcionamento, 500, 500);
      digitalWrite(fechaTravaEletrica, DESLIGADO);
    
      piscaSaida(sobeVidro, 1000, 10);
      buttonValue = 1;

      //Serial.println("Passou dentro da execução 1");
      break;
  
    case 2:
      valorIterrupcao = 0; //Zera o valor da interrupção
      
      digitalWrite(abreTravaEletrica, LIGADO);
      piscaSaida(setaAcionamento, 500, 250);
      digitalWrite(abreTravaEletrica, DESLIGADO);
      piscaSaida(setaAcionamento, 500, 250);

      buttonValue = 0;
      //Serial.println("Passou dentro da execução 2");
      break;

    case 3 :
      valorIterrupcao = 0;//Zera o valor da interrupção
      break;

    default:
      break;
  }

}


/*-----------------------------------------------------------------------------------+
| Função que permite Ligar e desligar qualquer pino somente passando os Parametros
| piscaPino = Pino de saida do Arduino
| piscaDelay1 = Valor de Delay do Pino Ligado
| piscaDelay2 = Valor de Delay do Pino Desligado
+-----------------------------------------------------------------------------------*/
void piscaSaida(int piscaPino, int piscaDelay1, int piscaDelay2){

  digitalWrite(piscaPino, LIGADO);
  delay(piscaDelay1);
  digitalWrite(piscaPino, DESLIGADO);
  delay(piscaDelay2);

}



/*-----------------------------------------------------------------------------------+
| Função que checa alteração no estado do botão e abre ou fecha a trava elétrica
| pinoDoBotão = Pino de leitura do Botão
| deboounceDelay = Tempo de espera contra ruidos
+-----------------------------------------------------------------------------------*/
void checaBotao(int pinoDoBotao, unsigned long debounceDelay){

    // read the state of the switch into a local variable:
  int reading = digitalRead(pinoDoBotao);

  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay){

    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH){
        buttonValue = !buttonValue;

        switch (buttonValue){
          case 0:
            piscaSaida(abreTravaEletrica, 250, 50);
            break;

          case 1:
            piscaSaida(fechaTravaEletrica, 250, 50);
            break;

          default:
          break;
        }
      }
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
}