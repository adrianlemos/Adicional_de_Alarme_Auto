/*
  Resumo: Firmware para comando o módulo que adiociona funções extras a qualquer alarme
          utilizando o sinal da buzzer do alarme como base.
  Board: Arduino Pro Mini Atmega328p 16MHz 5V
  Criador: Adrian Lemos




*/

/*Bibliotecas*/
#include <Arduino.h>

/*Constantes*/
const byte buzzerInterruptPin = 2;  //Config entrada da Buzzer
const byte setaAcionamento = 9;     //Config Acionamento da Seta
const byte sobeVidro = 3;           //Config do Pino que Aciona a Subida do Vidro
const byte fechaTravaEletrica = 5;  //Config do Pino que Fecha a Trava Elétrica
const byte abreTravaEletrica = 4;   //Config do Pino que Abre a Trava Elétrica
const byte botaoAbreFechaTrava = 13;//Config do Botão que Abre ou Fecha a Trava Elterica
const byte buzzerAtencao = 0;       //Config do Buzzer que diz se o farol esta ligado
const byte posChave = 6;            //Config da dectação do 12V pós chave
const byte leitorFarolLigado = 7;   //Config do Pino que detecta se o Farol está ligado ou não

/*Variaveis Globais*/
int valorIterrupcao = 0;

/*Protótipo das Funções*/
void interrupcaoBuzzer();

/*Função de Configuração*/
void setup() {
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
  pinMode(leitorFarolLigado, INPUT);
  
  //Configuração da Interrupção
  attachInterrupt(digitalPinToInterrupt(buzzerInterruptPin), interrupcaoBuzzer, FALLING);
  
}

/*Loop principal do programa*/
void loop() {



}

/*FUNÇÕES*/

/* 
  Função que toma decisão após ser detectado a interrupção

  Resumo: O que esta função faz é utilizar de um debounce por software para que não 
          aja erro no momento de decisão. É sempre feito o incremento de mais um na
          variavel state para q o programa possa tomar suas decisões.

*/
void interrupcaoBuzzer(){
  static unsigned long last_interrupt_time = 0; // Cria a variavel e atribui o valor 0 a ela
  unsigned long interrupt_time = millis(); // Cria a variavel e atribui o tempo da função millis
  
  if (interrupt_time - last_interrupt_time > 300) // Se a interrupção for detectada com menos de 300ms ele ignora
  {
    valorIterrupcao++; //Soma +1 na Variavel
  }
  last_interrupt_time = interrupt_time;// seta o valor da variavel interrupt_time na variavel last_interrupt_time
}

/*
  Toma decisão com base no valor lido na Interrupção
  Resumo: Caso 1 - Aciona a Seta uma vez, Trava as portas e sobe o Vidro
          Caso 2 - Aciona a Seta duas vezes, Destrava as portas 

*/
void tomaDecisaoInterrupcao(int valorLocalInterrupcao){
    
  switch (valorLocalInterrupcao)
  {
    case 1:
      /* code */
      break;
  
    case 2:
      /* code */
      break;
  
    default:
      break;
  }

}