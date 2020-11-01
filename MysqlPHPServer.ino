#include<SPI.h>
#include<Ethernet.h>
#include<DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);


byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};
byte servidor[] = {192,168,1,3};
#define portaHTTP 80

EthernetClient clienteArduino;
// the setup routine runs once when you press reset:

//Переменные
unsigned long int timeConn = millis(); 
float h;
float t;


float sensor1 = 1;
float sensor2 = 3;
float sensor3 = 5;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Ethernet.begin(mac);
  dht.begin();
  

  if(Ethernet.begin(mac) == 0){
    Serial.println("Falha ao conectar a rede");
    Ethernet.begin(mac);
    
    }

  Serial.println("Conectado a read, no ip:");
  Serial.println(Ethernet.localIP());
  
}

// the loop routine runs over and over again forever:
void loop() {

  h = dht.readHumidity();
  t = dht.readTemperature();
  if(millis() - timeConn > 50000){
    sendData(t,h);
    timeConn = millis();
    Serial.println("CONNECT SERVER: Send temp/hum");
    
    }
  sensor1++; sensor2++; sensor3++;
  /*
  if(clienteArduino.available()){
    char dadosRetornados = clienteArduino.read();
    Serial.print(dadosRetornados);
    }
  if(!clienteArduino.connected()){
    clienteArduino.stop();
    char comando = Serial.read();
    if(comando == '1'){
      sensor1++;
      sensor2++;
      sensor3++;
      */

      Serial.println("Conectando ao servidor..");
      Serial.print("Sensor1:");
      Serial.println(sensor1);
      Serial.print("Sensor2:");
      Serial.println(sensor2);
      Serial.print("Sensor3");
      Serial.println(sensor3);
      
      if(clienteArduino.connect(servidor, portaHTTP)){

        //http://192.168.100.74/arduino/test.php?s1=5&s2=3&s3=7
        //clienteArduino.println("GET /arduino/teste.php HTTP/1.0")
        clienteArduino.print("GET /arduino/connect.php");
        clienteArduino.print("?s1=");
        clienteArduino.print(sensor1);
        clienteArduino.print("&s2=");
        clienteArduino.print(sensor2);
        clienteArduino.print("&s3=");
        clienteArduino.print(sensor3);
        clienteArduino.println(" HTTP/1.0");

        clienteArduino.println("Host: 192.168.1.3");
        clienteArduino.println("Connection: close");
        clienteArduino.println();
        
        clienteArduino.stop();
        
        //clienteArduino.println();
        }else {
          Serial.println("Falha na conexao com servidor");
          clienteArduino.stop();
          
          
          }
      delay(50000);
      }

 void sendData(float t, float h) {
  clienteArduino.connect(servidor, portaHTTP);
  clienteArduino.print( "GET /arduino/connect.php");
  //client.print("k=");  // Специальный код, например asREb25C
  //client.print("&");
  clienteArduino.print("?t=");
  clienteArduino.print(t);
  clienteArduino.print("&");
  clienteArduino.print("h=");
  clienteArduino.print(h);
  clienteArduino.println(" HTTP/1.0");
  clienteArduino.println( "Host: 192.168.1.3" );
  //clienteArduino.println(servidor);
  clienteArduino.println( "Connection: close" );
  clienteArduino.println();
  clienteArduino.println();
  clienteArduino.stop();
  clienteArduino.flush();
}
