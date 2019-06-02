/*

AUTOR: Rafael Gustavo da Cruz
DATA: 25/10/2018
ATUALIZADO: 01/06/2019

DISPOSITIVO: ESP-01 (Pode ser facilmente adpatado para NodeMCU)
SENSOR: DHT22

DESCRIÇÃO:

O seguinte software faz a leitura de umidade do ar e temperatura atraves de um DHT22 e envia para um canal no Thingspeak ( https://thingspeak.com/channels/596198 )
Além disso ainda registra o tempo de operação do dispositivo

VARIAVES DE LEITURAS:
umidade (Recebe a umidade do ar do DHT22)
temperatura (Recebe a temperatura do DHT22)

*/


#include <ThingSpeak.h> //Biblioteca ThingSpeak
#include <ESP8266WiFi.h> //Biblioteca ESP

#include "DHT.h" //Inclusão da biblioteca do sensor DHT11 e DHT22
 
#define DHT_DATA_PIN 2 //Definicao da GPIO2 no ESP-01
#define DHTTYPE DHT22 //Define DHT do tipo 22 (Alterar se outro for usado)
DHT dht(DHT_DATA_PIN, DHTTYPE);

// Parametros da REDE
const char* ssid = "SSSSSSS"; // PREENCHER DE ACORDO
const char* password = "PPPPPPPPPP"; // PREENCHER DE ACORDO

// Informacoes do ThingSpeak
char thingSpeakAddress[] = "api.thingspeak.com";
unsigned long channelID = NNNNNN; //ID do Canal do ThingSpeak
char* writeAPIKey = "WWWWWWWWWWWWWWWW"; //Chave de Escrita
char* readAPIKey = "RRRRRRRRRRRRRRRR"; //Chave de Leitura
const unsigned long Intervalo = 10L * 1000L; //NAO SEI O PQ DESSE MALTIDO "L" MAS SEI QUE ESSE É O TEMPO DE Intervalo de leituras (10x1000=10s)
unsigned int dataFieldOne = 1;                       // Nome_Variavel = Campo 1 do Canal no ThingSpeak
unsigned int dataFieldTwo = 2;                       // Nome_Variavel = Campo 2 do Canal no ThingSpeak
unsigned int dataFieldThree = 3;                     // Nome_Variavel = Campo 3 do Canal no ThingSpeak
//unsigned int aField = 6;                           // RESERVA              
//unsigned int bField = 7;                           // RESERVA
//unsigned int cField = 8;                           // RESERVA

//unsigned long lastConnectionTime = 0;//NAO SEI QUE POHA E ESSA

long lastUpdateTime = 0; //Declarando e iniciando a variavel de contagem dos milisegundos em zero
int minutos = 0; //Declarando e iniciando em zero a variavel que vai converter o valor para minutos

WiFiClient client;//Inicia o WiFi                                  

void setup() {

Serial.begin(9600);//Inicia e configura o Serial
delay(15000);
Serial.println("INICIANDO...");
delay(500);

CONECTA_WiFi(); //Chama a função CONECTA_WiFi escrita no fim do programa 

// Read the constants at startup.
//aConst = readTSData( channelID, aField );
//bConst = readTSData( channelID, bField );
//cConst = readTSData( channelID, cField );
}

void loop() {
    
    // Este IF faz com que o a leitura seja atualizada somente quando o intervalo for excedido
    if (millis() - lastUpdateTime >=  Intervalo) {

        float umidade = dht.readHumidity(); //Faz a leitura de umidade no DHT e coloca na variavel umidade
        float temperatura = dht.readTemperature(); //Faz a leitura da temperatura no DHT e coloca na variavel temperatura
        
        lastUpdateTime = millis();//tempo atual
        minutos = lastUpdateTime/60000;
        
        //Logs na porta serial
        Serial.print("Temperatura: ");
        Serial.println(temperatura);
        Serial.print("Umidade do ar: ");
        Serial.println(umidade);
        Serial.print("Tempo de Operacao: ");
        Serial.print(minutos);
        Serial.println(" Minutos");
        Serial.println();
                
        write2TSData( channelID, dataFieldOne, temperatura, dataFieldTwo, umidade, dataFieldThree, minutos);      // Escreve no Canal, Temperatura no Campo 1 ; Umidade no Campo 2 ; Tempo decorrido no Campo 3
        
        
    }
}

//INICIO DA FUNÇÃO DO WIFI
int CONECTA_WiFi(){

    WiFi.begin(ssid, password);

    delay(500);
    Serial.print("Conectar em: ");
    Serial.println(ssid);
    Serial.print("Usando a senha: ");
    Serial.println(password);
    delay(500);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Conectando...");
    }
    
    Serial.println( "CONECTADO COM SUCESSO!!!" );
    Serial.print("IP do ESP-01: ");
    Serial.println(WiFi.localIP());
    Serial.println("");
    ThingSpeak.begin( client );
}
//FIM DA FUNÇÃO DO WIFI

//NAO FACO IDEIA DESSAS MERDAS DAQUI PARA BAIXO !!!
/*
  float readTSData( long TSChannel,unsigned int TSField ){
    
  float data =  ThingSpeak.readFloatField( TSChannel, TSField, readAPIKey );
  Serial.println( " Data read from ThingSpeak: " + String( data, 9 ) );
  return data;
  
}

/* Use this function if you want to write a single field
  int writeTSData( long TSChannel, unsigned int TSField, float data ){
  int  writeSuccess = ThingSpeak.writeField( TSChannel, TSField, data, writeAPIKey ); // Write the data to the channel
  if ( writeSuccess ){
    
    Serial.println( String(data) + " written to Thingspeak." );
    }
    
    return writeSuccess;
}*/

//use this function if you want multiple fields simultaneously
int write2TSData( long TSChannel, unsigned int TSField1, float field1Data, unsigned int TSField2, long field2Data, unsigned int TSField3, long field3Data ){

  ThingSpeak.setField( TSField1, field1Data );
  ThingSpeak.setField( TSField2, field2Data );
  ThingSpeak.setField( TSField3, field3Data );
   
  int writeSuccess = ThingSpeak.writeFields( TSChannel, writeAPIKey );
  return writeSuccess;
}
