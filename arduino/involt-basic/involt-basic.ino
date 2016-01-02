/*
  INVOLT BASIC ARDUINO SKETCH
  by Ernest Warzocha 2015
  ------------------------------------------------------
  This file can be used for serial communication
  between Arduino Uno and Involt Chrome App. It can be 
  used with HC-05 Bluetooth device connected via 
  hardware serial.
*/

/*
  involtDigital array contains values received from app.
  Each UI element refers to pin number which is index of
  this array. involtString is array for values received
  with "S" pin. You can increase the length of array to
  store more values then arduino total pins. Use them 
  in sketch.
*/
int    involtDigital[14] = {};
String involtString[2] ={};

/*
  Buffer for received data. If you plan to receive more 
  at once just increase the array length.
*/
char involt[16];

/*
  String for function received from app.
*/
String fname;

int previousValue = LOW;

void setup() {
 //Connection speed must be same as app.
 Serial.begin(57600);
  pinMode(13, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(4,INPUT_PULLUP);
  //delay(2000);
}

String isButton = "click";

void loop() {
  involtReceive();
  int buttonValue = digitalRead(4);

  if(buttonValue == LOW && previousValue == HIGH) {
        delay(30);
        involtSendString(4, isButton);
    };
  previousValue = buttonValue;
  //ADD YOUR CODE HERE
  delay(4);
  involtSend(0,analogRead(A0));
  delay(4);
  involtSend(1,analogRead(A0)/2);
  delay(4);
  involtSend(2,analogRead(A0)/4);
 
 

  analogWrite(9,involtDigital[9]);
  analogWrite(10,involtDigital[10]);
  analogWrite(11,involtDigital[11]);
  
  digitalWrite(13,involtDigital[13]); //Serial.println(involtDigital[13]);
  //Clear the function to trigger once.
  fname = "";
}


/*
  INVOLT FUNCTIONS
  ------------------------------------------------------
  You don't have to look below. Especially if you don't
  want to complicate everything. 

  involtReceive
  ------------------------------------------------------ 
  read the data from app and parse the values received 
  into proper array. The read until is used due to
  issues with missing chars when reading direct strings.
  
  involtSend, involtSendString
  ------------------------------------------------------
  send int or string to app. Multiple prints are to
  reduce the sketch size (compared to sprintf()).
*/

void involtReceive(){
  if(Serial.available()>0) {
    Serial.readBytesUntil('\n',involt,sizeof(involt));
    int pin;
    if (involt[0] == 'P'){
      int value;
      sscanf(involt, "P%dV%d", &pin, &value);
      involtDigital[pin] = value;
    }
    else if (involt[0] == 'S'){
      String value;
      sscanf(involt, "S%dV%s", &pin, &value);
      involtString[pin] = value;
    }
    memset(involt,0,sizeof(involt));
  };
};

void involtSend(int pinNumber, int sendValue){
  Serial.print('A'); 
  Serial.print(pinNumber); 
  Serial.print('V'); 
  Serial.print(sendValue); 
  Serial.println('E');
};

void involtSendString(int pinNumber, String sendString){
  Serial.print('A'); 
  Serial.print(pinNumber); 
  Serial.print('V'); 
  Serial.print(sendString); 
  Serial.println('E'); 
};
