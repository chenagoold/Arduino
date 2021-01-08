#include<ESP8266WiFi.h>
#include<WiFiClient.h>
#include<ESP8266HTTPClient.h>

#define ON_Board_LED 2 // Константа на D4 вход  
//define LED_D8 15



const char* ssid="Okt30"; //указатель на неизменную переменную с именем точки доступа Wi-fi
const char* password ="2"; //пароль точки доступа
const char* host = "http://192.168.1.238/"; // адресс отправки запроса 


void setup() {
  // put your setup code here, to run once:
  //pinMode(D8, OUTPUT);
  //digitalWrite(D8, 0);
  Serial.begin(115200); //задает скорость передачи данных 
  delay(500); //выставляем задержку пол секунды

  WiFi.mode(WIFI_STA);//выбор режима WIFI_AP точка доступа и WIFI_STA клиент 
  WiFi.begin(ssid, password); // функция инициализируется подключения к роутеру 
  Serial.println("");//выводит
    
  pinMode(ON_Board_LED,OUTPUT); //--> Устанавливаем режим работы выход(OUTPUT)
  digitalWrite(ON_Board_LED, HIGH); //--> Функция подает высокий сигнал на вход D4(Включить его) 

  pinMode(D8,OUTPUT); //--> Устанвливает режим работы на выход(OUTPUT)
  digitalWrite(D8, LOW); //--> Функция подает низкий сигнал на вход D8(выключить его)

  //----------------------------------------Для подключения
  Serial.print("Connecting");// передаст "Connection"
  while (WiFi.status() != WL_CONNECTED) { //Возращает статус соединения если нет соидинения != (WL_CONNECTED-соединени с Wi-fi)
    Serial.print(".");//вывод точки 
    //----------------------------------------На устройстве мерцает диод пока не установит соединение с маршрутизатором .
    digitalWrite(ON_Board_LED, LOW);// Диод выкл
    delay(250); // задержка 250млс
    digitalWrite(ON_Board_LED, HIGH); // Диод вкл
    delay(250); // задержка 250млс
    //----------------------------------------
  }
  //----------------------------------------
  digitalWrite(ON_Board_LED, HIGH); //--> Установлино соединение установлено
  //----------------------------------------Для Успешное подключение выводим следующие данные  
  Serial.println("");//Вывод
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());//Возращает локайльный адресс 
  Serial.println();
  //----------------------------------------
}

void loop() {
  // put your main code here, to run repeatedly:
  HTTPClient http; //объект класса HTTPClient 

  //---------------------------------------- from MySQL Database
  String GetAddress, LinkGet, getData; //строковые переменные 
  String LEDStatResultSend;//строковые переменные 
  int id = 0; // 
  GetAddress = "led/GetData.php"; //Адресс запроса  
  LinkGet = host + GetAddress; //http://192.168.1.238/ + led/GetData.php
  getData = "id=" + String(id);
  Serial.println("----------------Connect to Server-----------------");
  Serial.println("Get LED Status from Server or Database");
  Serial.print("Request Link : ");
  Serial.println(LinkGet);//Вывод  http://192.168.1.238/led/GetData.php
  http.begin(LinkGet); //Выполняем http запрос http://192.168.1.238/led/GetData.php
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
  int httpCodeGet = http.POST(getData); //--> Запрос на сервер есть в базе один или ноль 
  String payloadGet = http.getString(); //--> Ответ на запрос и записываем в переменную payloadGet=1
  Serial.print("Response Code : "); //--> If Response Code = 200 means Successful connection, if -1 means connection failed. For more information see here : https://en.wikipedia.org/wiki/List_of_HTTP_status_codes
  Serial.println(httpCodeGet); //--> Print HTTP return Возращает вывод HTTP 
  Serial.print("Returned data from Server : ");
  Serial.println(payloadGet); //--> Print request response payload Ответ на запрос  выводим 

  if (payloadGet == "1") { // Проверяем ответ если еденица зажигаем 
    digitalWrite(D8, HIGH); //--> Turn off Led
    LEDStatResultSend = payloadGet;
  }
  if (payloadGet == "0") { // Ноль отключаем
    digitalWrite(D8, LOW); //--> Turn off Led
    LEDStatResultSend = payloadGet;
  }

  //----------------------------------------Запускает скрипт вывод статуса 
Serial.println();//вывод
Serial.println("Sending LED Status to Server");//вывод
String postData, LinkSend, SendAddress;
SendAddress = "led/getLEDStatFromNodeMCU.php";// присвоим строковое значение 
LinkSend = host + SendAddress;// http://192.168.1.238/ + led/getLEDStatFromNodeMCU.php
postData = "getLEDStatusFromNodeMCU=" + LEDStatResultSend; //getLEDStatusFromNodeMCU=1 
Serial.print("Request Link : ");
Serial.println(LinkSend);//http://192.168.1.238/ + led/getLEDStatFromNodeMCU.php
http.begin(LinkSend); //--> Запуск http://192.168.1.238/ + led/getLEDStatFromNodeMCU.php
http.addHeader("Content-Type", "application/x-www-form-urlencoded"); //--> 
int httpCodeSend = http.POST(postData); //--> getLEDStatusFromNodeMCU=1
String payloadSend = http.getString(); //--> Get the response payload получение полезной нагрузки 
Serial.print("Response Code : "); //--> If Response Code = 200 means Successful connection, if -1 means connection failed
Serial.println(httpCodeSend); //--> Print HTTP return code вывод запроса 
Serial.print("Returned data from Server : "); // 
Serial.println(payloadSend); //--> Print request response payload Вывести ответ на запрос
//----------------------------------------
  //----------------------------------------
  
  Serial.println("----------------Closing Connection----------------");//Вывод 
  http.end(); //--> Close connection закрыть подключение http
  Serial.println();
  Serial.println("Please wait 5 seconds for the next connection.");
  Serial.println();
  delay(5000); //--> GET Data at every 5 seconds следующие подключение 
}
