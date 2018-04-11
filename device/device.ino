#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//LiquidCrystal_I2C lcd(0x27, 16, 2);
//LiquidCrystal_I2C lcd(0x27, 16, 2);
LiquidCrystal_I2C lcd(0x3f, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

char linebreakChar = '#';
byte pinOutIO1 = 11;
byte pinOutIO2 = 12;
byte pinInIO1 = A0;
byte pinInIO2 = A1;

byte ledGreen = 10;
byte ledBlue = 9;
byte ledRed = 8;


boolean lineBreak = 0;

void displayLCD(int yPosition, int xPosition, String txtMSG) {
  lcd.setCursor(xPosition, yPosition);
  lcd.print(txtMSG);
}

void sendMSG(String txtSend) {
  mySerial.print(txtSend);
}

void ledState(String statusCMD) {
  if (statusCMD == "send") {
    digitalWrite(ledGreen, LOW); 
    digitalWrite(ledBlue, HIGH);
    digitalWrite(ledRed, HIGH);    
  }else if (statusCMD == "recv") {
    digitalWrite(ledGreen, HIGH); 
    digitalWrite(ledBlue, LOW);
    digitalWrite(ledRed, HIGH);  
  }else if (statusCMD == "all") {
    digitalWrite(ledGreen, HIGH); 
    digitalWrite(ledBlue, HIGH);
    digitalWrite(ledRed, HIGH);  
  }else if (statusCMD == "error") {
    digitalWrite(ledGreen, LOW); 
    digitalWrite(ledBlue, LOW);
    digitalWrite(ledRed, HIGH); 
  }else if (statusCMD == "dark") {
    digitalWrite(ledGreen, LOW); 
    digitalWrite(ledBlue, LOW);
    digitalWrite(ledRed, LOW); 
  }else if (statusCMD == "ok") {
    digitalWrite(ledGreen, HIGH); 
    digitalWrite(ledBlue, LOW);
    digitalWrite(ledRed, LOW); 
  }else if (statusCMD == "click") {
    digitalWrite(ledGreen, LOW); 
    digitalWrite(ledBlue, HIGH);
    digitalWrite(ledRed, LOW);  
  }
}

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);  

  pinMode(pinOutIO1, OUTPUT);
  pinMode(pinOutIO2, OUTPUT);
  digitalWrite(pinOutIO1, HIGH); 
  digitalWrite(pinOutIO2, HIGH); 

  pinMode(pinInIO1, INPUT);
  pinMode(pinInIO2, INPUT);

  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(ledRed, OUTPUT);

  lcd.begin(16, 2);
  lcd.backlight();   
}

void serialFlush(){
  while(mySerial.available() > 0) {
    char t = mySerial.read();
  }
}

void loop() {
  
  mySerial.listen();
  String rcvmsg = "";

  if(mySerial.available()) ledState("recv");
  while (mySerial.available() > 0) {
    byte inByte = mySerial.read();
    Serial.print(char(inByte));
    rcvmsg += char(inByte);
    rcvmsg.trim();
    if(char(inByte)==linebreakChar) Serial.println("Get ascii # for linebreak!");
    lineBreak = 1;
  }  
  

  if(lineBreak==1) {
    Serial.print("Received: ");
    Serial.println(rcvmsg);
    
    ledState("dark");
    Serial.println();
    lineBreak = 0;

    lcd.clear(); 
    //delay(300);
    displayLCD(1, 0, "                ");
    displayLCD(0, 0, "                ");
    displayLCD(0, 0, "RCVE: "+ rcvmsg);
    
    ledState("send");
    mySerial.print(rcvmsg + "#");
    displayLCD(1, 0, "SEND: "+rcvmsg);    
    Serial.print("Send: ");
    Serial.println(rcvmsg + "#");
    
    //delay(50);
    ledState("dark");
    //lcd.noBacklight();
    serialFlush();
  }

  if(digitalRead(pinInIO1)==0) {
    ledState("recv");
    Serial.println("Receive IO #1 ");
    ledState("dark");
    ledState("send");
    digitalWrite(pinOutIO1, LOW);
    delay(300); 
    digitalWrite(pinOutIO1, HIGH); 
    ledState("dark");
  }

  if(digitalRead(pinInIO2)==0) {
    ledState("recv");
    Serial.println("Receive IO #2 ");
    ledState("dark");
    ledState("send");
    digitalWrite(pinOutIO2, LOW);
    delay(300);  
    digitalWrite(pinOutIO2, HIGH); 
    ledState("dark");
  }  

  //Serial.println(String(digitalRead(pinInIO1)) + " / " + String(digitalRead(pinInIO2)) );
  delay(250);  
}
