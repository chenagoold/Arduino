const int LED=9;

void setup() { //конфигурация 
  // :
  pinMode(LED, OUTPUT); //Конфигурируем контакт как светодиод на выход
  //digitalWrite(LED, HIGH); //Устанавливаем значения HIGH на выход
  

}

void loop() {
  // Цикл в круговом режиме:
  for(int i = 0; i<=1000; i=i+100){
    digitalWrite(LED, HIGH);
    delay(i);
    digitalWrite(LED, LOW);
    delay(i);
    }
  

}
