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

  pinMode(D8,OUTPUT); //--> LED port Direction output
  digitalWrite(D8, LOW); //--> Turn off Led

  //----------------------------------------Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    //----------------------------------------Make the On Board Flashing LED on the process of connecting to the wifi router.
    digitalWrite(ON_Board_LED, LOW);
    delay(250);
    digitalWrite(ON_Board_LED, HIGH);
    delay(250);
    //----------------------------------------
  }
  //----------------------------------------
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off the On Board LED when it is connected to the wifi router.
  //----------------------------------------If successfully connected to the wifi router, the IP Address that will be visited is displayed in the serial monitor
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  //----------------------------------------
}

void loop() {
  // put your main code here, to run repeatedly:
  HTTPClient http; //--> Declare object of class HTTPClient

  //----------------------------------------Getting Data from MySQL Database
  String GetAddress, LinkGet, getData;
  String LEDStatResultSend;
  int id = 0; //--> ID in Database
  GetAddress = "led/GetData.php"; 
  LinkGet = host + GetAddress; //--> Make a Specify request destination
  getData = "id=" + String(id);
  Serial.println("----------------Connect to Server-----------------");
  Serial.println("Get LED Status from Server or Database");
  Serial.print("Request Link : ");
  Serial.println(LinkGet);
  http.begin(LinkGet); //--> Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
  int httpCodeGet = http.POST(getData); //--> Send the request
  String payloadGet = http.getString(); //--> Get the response payload from server
  Serial.print("Response Code : "); //--> If Response Code = 200 means Successful connection, if -1 means connection failed. For more information see here : https://en.wikipedia.org/wiki/List_of_HTTP_status_codes
  Serial.println(httpCodeGet); //--> Print HTTP return code
  Serial.print("Returned data from Server : ");
  Serial.println(payloadGet); //--> Print request response payload

  if (payloadGet == "1") {
    digitalWrite(D8, HIGH); //--> Turn off Led
    LEDStatResultSend = payloadGet;
  }
  if (payloadGet == "0") {
    digitalWrite(D8, LOW); //--> Turn off Led
    LEDStatResultSend = payloadGet;
  }

  //----------------------------------------Sends LED status feedback data to server
Serial.println();
Serial.println("Sending LED Status to Server");
String postData, LinkSend, SendAddress;
SendAddress = "led/getLEDStatFromNodeMCU.php";
LinkSend = host + SendAddress;
postData = "getLEDStatusFromNodeMCU=" + LEDStatResultSend;
Serial.print("Request Link : ");
Serial.println(LinkSend);
http.begin(LinkSend); //--> Specify request destination
http.addHeader("Content-Type", "application/x-www-form-urlencoded"); //--> Specify content-type header
int httpCodeSend = http.POST(postData); //--> Send the request
String payloadSend = http.getString(); //--> Get the response payload
Serial.print("Response Code : "); //--> If Response Code = 200 means Successful connection, if -1 means connection failed
Serial.println(httpCodeSend); //--> Print HTTP return code
Serial.print("Returned data from Server : ");
Serial.println(payloadSend); //--> Print request response payload
//----------------------------------------
  //----------------------------------------
  
  Serial.println("----------------Closing Connection----------------");
  http.end(); //--> Close connection
  Serial.println();
  Serial.println("Please wait 5 seconds for the next connection.");
  Serial.println();
  delay(5000); //--> GET Data at every 5 seconds
}
