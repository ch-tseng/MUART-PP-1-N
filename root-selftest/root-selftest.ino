#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//LiquidCrystal_I2C lcd(0x27, 16, 2);
LiquidCrystal_I2C lcd(0x3f, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

byte ledGreen = 10;
byte ledBlue = 9;
byte ledRed = 8;
byte pinBtnStart = 5;
byte pinBtnUp = 6;
byte pinBtnDown = 7;
byte pinOutIO1 = 11;
byte pinOutIO2 = 12;
byte pinInIO1 = A0;
byte pinInIO2 = A1;

//分隔字元, 接收後只取中間部份
char firstFrame = '@';
char lastFrame = '@';
//實際傳送的data
String dataSend = "012345";
//分隔判斷傳送結束的字元
char linebreakChar = '#';
byte cmdTarget[4];
byte cmdStandby[4] = { 0x55, 0x02, 0x00, 0X0d };
byte cmdTarget1[4] = { 0x55, 0x01, 0x01, 0X0d };
byte cmdTarget2[4] = { 0x55, 0x01, 0x02, 0X02 };
byte cmdTarget3[4] = { 0x55, 0x01, 0x03, 0X03 };
byte cmdTarget4[4] = { 0x55, 0x01, 0x04, 0X00 };
byte cmdTarget5[4] = { 0x55, 0x01, 0x05, 0X01 };
byte cmdTarget6[4] = { 0x55, 0x01, 0x06, 0X06 };
byte cmdTarget7[4] = { 0x55, 0x01, 0x07, 0X07 };
byte cmdTarget8[4] = { 0x55, 0x01, 0x08, 0X04 };

int deviceID = 0;
boolean uartTestResult = 0;
boolean ioTestResult = 0;
String LCD_1 = "[1]";
String LCD_2 = "";
String receiveString = "";

void ledState(String statusCMD) {
  if (statusCMD == "send") {
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledBlue, HIGH);
    digitalWrite(ledRed, HIGH);
  } else if (statusCMD == "recv") {
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledBlue, LOW);
    digitalWrite(ledRed, HIGH);
  } else if (statusCMD == "all") {
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledBlue, HIGH);
    digitalWrite(ledRed, HIGH);
  } else if (statusCMD == "error") {
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledBlue, LOW);
    digitalWrite(ledRed, HIGH);
  } else if (statusCMD == "dark") {
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledBlue, LOW);
    digitalWrite(ledRed, LOW);
  } else if (statusCMD == "ok") {
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledBlue, LOW);
    digitalWrite(ledRed, LOW);
  } else if (statusCMD == "click") {
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledBlue, HIGH);
    digitalWrite(ledRed, LOW);
  }
}

void displayLCD(int yPosition, int xPosition, String txtMSG) {
  lcd.setCursor(xPosition, yPosition);
  lcd.print(txtMSG);
}

void sendMSG(String txtSend) {
  mySerial.print(txtSend + linebreakChar);
}

void serialFlush() {
  while (mySerial.available() > 0) {
    char t = mySerial.read();
  }
}

String listenMSG() {
  boolean lineBreak = 0;
  String rcvmsg = "";
  byte inByte = 0;
  unsigned long timeStart = millis();

  mySerial.listen();
  //delay(100);

  Serial.println("Start waiting for UART.....");
  while (char(inByte) != linebreakChar and (millis() - timeStart < 3000) ) {
    while (mySerial.available() > 0) {
      inByte = mySerial.read();
      Serial.print(char(inByte));
      if (char(inByte) == linebreakChar) {
        Serial.println("Get ascii # for stop!");
      } else {
        rcvmsg += char(inByte);
      }
      if (char(inByte) == '#') break;
    }
  }
  rcvmsg.trim();
  Serial.print("Received: ");
  Serial.println(rcvmsg);
  return rcvmsg;
}

void cmdTargetCmd(int ii) {
  int i = 4;

  switch (ii) {
    case 1:
      Serial.print("i = 1");
      while ( i-- ) ( cmdTarget[i] ) = ( cmdTarget1[i] );
      break;
    case 2:
      Serial.print("i = 2");
      while ( i-- ) ( cmdTarget[i] ) = ( cmdTarget2[i] );
      break;
    case 3:
      Serial.print("i = 3");
      //while ( i-- ) *( cmdTarget + i ) = *( cmdTarget3 + i );
      while ( i-- ) ( cmdTarget[i] ) = ( cmdTarget3[i] );
      break;
    case 4:
      Serial.print("i = 4");
      //while ( i-- ) *( cmdTarget + i ) = *( cmdTarget4 + i );
      while ( i-- ) ( cmdTarget[i] ) = ( cmdTarget4[i] );
      break;
    case 5:
      while ( i-- ) ( cmdTarget[i] ) = ( cmdTarget5[i] );      
      break;
    case 6:
      Serial.print("i = 6");
      //while ( i-- ) *( cmdTarget + i ) = *( cmdTarget6 + i );
      while ( i-- ) ( cmdTarget[i] ) = ( cmdTarget6[i] );
      break;
    case 7:
      Serial.print("i = 7");
      //while ( i-- ) *( cmdTarget + i ) = *( cmdTarget7 + i );
      while ( i-- ) ( cmdTarget[i] ) = ( cmdTarget7[i] );
      break;
    case 8:
      Serial.print("i = 8");
      //while ( i-- ) *( cmdTarget + i ) = *( cmdTarget8 + i );
      while ( i-- ) ( cmdTarget[i] ) = ( cmdTarget8[i] );
      break;
  }

}

String testIO() {
  String testingResult = "";
  boolean result1 = 0;
  boolean result2 = 0;
  unsigned long timeStart;

  //Test IO #1
  ledState("send");
  timeStart = millis();
  digitalWrite(pinOutIO1, LOW);  //IO1 送出到device
  ledState("dark");
  delay(50);
  ledState("recv");
  while (millis() - timeStart < 2000) {
    //Serial.print("Read IO #1:");
    //Serial.println(digitalRead(pinInIO1));
    if (digitalRead(pinInIO1) == 0) {
      result1 = 1;
      break;
    }
  }
  ledState("dark");
  digitalWrite(pinOutIO1, HIGH);  //還原
  //delay(50);

  //Test IO #2
  ledState("send");
  timeStart = millis();
  digitalWrite(pinOutIO2, LOW);  //IO1 送出到device
  ledState("dark");
  delay(50);
  ledState("recv");
  while (millis() - timeStart < 2000) {
    //Serial.print("Read IO #2:");
    //Serial.println(digitalRead(pinInIO2));
    if (digitalRead(pinInIO2) == 0) {
      result2 = 1;
      break;
    }
  }
  ledState("dark");
  digitalWrite(pinOutIO2, HIGH);  //還原

  if ( result1 == 1) {
    testingResult = "Y";
  } else {
    testingResult = "N";
  }

  if ( result2 == 1) {
    testingResult = testingResult + " Y";
  } else {
    testingResult = testingResult + " N";
  }

  ioTestResult = 1;
  if (result1 == 0 or result2 == 0) ioTestResult = 0;

  return testingResult;
}

String testUART() {
  ledState("click");
  delay(500);
  ledState("dark");
  String testingResult = "";
  //modeTesting = (modeTesting == 0) ? 1 : 0;

  Serial.print("Button start clicked...start test!");
  serialFlush();

  //Step 1 --> switch to the target device
  if (deviceID > 0) {
    ledState("send");
    cmdTargetCmd(deviceID);  //get the device coding for the #id
    //.println("TEST --> deviceID=" + String(deviceID));
    Serial.println(" ");
    for(int i=0;i< sizeof(cmdTarget);i++){
      Serial.print("0x");  
      Serial.print(cmdTarget[i], HEX);    
      Serial.print(" ");
    }
    Serial.println(" ");
    mySerial.write(cmdStandby, sizeof(cmdStandby));  //set root standby mode so changing IO state will be no effect
    ledState("dark");
    delay(50); // wait  IO debunce
    serialFlush();
    ledState("send");
    mySerial.write(cmdTarget, sizeof(cmdTarget)); // change to Device
    ledState("dark");
    delay(20); // wait cmd be done
  }

  //Step 2 --> send data
  ledState("send");
  Serial.println("    send message: " + dataSend);

  ledState("recv");
  sendMSG(dataSend);

  //Step 3 --> receive data
  delay(500);

  receiveString = listenMSG();
  Serial.println("UART test: " + receiveString);
  ledState("dark");

  if (receiveString == dataSend) {
    testingResult = "Y";
    uartTestResult = 1;
  } else {
    testingResult = "N";
    uartTestResult = 0;
  }
  Serial.println(testingResult);

  return testingResult;
}

void clickBtnUp() {
  ledState("click");
  delay(500);
  deviceID += 1;
  if (deviceID > 8) deviceID = 0;
  ledState("dark");
}

void clickBtnDown() {
  ledState("click");
  delay(500);
  deviceID += -1;
  if (deviceID < 0) deviceID = 8;
  ledState("dark");
}

String deviceModel() {
  String model = "";
  if (deviceID == 0) {
    model = "UART 1:1 1:2";
  } else {
    model = "UART 1:N N>2";
  }

  return model;
}

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  lcd.begin(16, 2);
  lcd.backlight();

  pinMode(pinBtnStart, INPUT_PULLUP);
  pinMode(pinBtnUp, INPUT_PULLUP);
  pinMode(pinBtnDown, INPUT_PULLUP);

  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(ledRed, OUTPUT);

  pinMode(pinOutIO1, OUTPUT);
  pinMode(pinOutIO2, OUTPUT);
  digitalWrite(pinOutIO1, HIGH);
  digitalWrite(pinOutIO2, HIGH);

  pinMode(pinInIO1, INPUT);
  pinMode(pinInIO2, INPUT);

  String model = deviceModel();
  LCD_1 = "[" + String(deviceID) + "] " + model;

}

void loop() {
  String uartTestDisplay = "";
  String IOTestDisplay = "";

  int btnStartStaus = digitalRead(pinBtnStart);
  int btnUpStaus = digitalRead(pinBtnUp);
  int btnDownStaus = digitalRead(pinBtnDown);

  if (btnUpStaus == LOW) {
    clickBtnUp();
    String model = deviceModel();
    LCD_1 = "[" + String(deviceID) + "] " + model;
  }

  if (btnDownStaus == LOW) {
    clickBtnDown();
    String model = deviceModel();
    LCD_1 = "[" + String(deviceID) + "] " + model;
  }

  if (btnStartStaus == LOW) {
    displayLCD(0, 0, "                ");
    displayLCD(1, 0, "                ");

    uartTestDisplay = testUART();
    IOTestDisplay = testIO();

    Serial.print("UART: "); Serial.println(uartTestDisplay);
    Serial.print("I/O: "); Serial.println(IOTestDisplay);

    if (ioTestResult == 1 && uartTestResult == 1) {
      ledState("ok");
    } else {
      ledState("error");
    }
    LCD_1 = "[" + String(deviceID) + "] " + receiveString;
    LCD_2 = "  UART:" + uartTestDisplay + " IO:" + IOTestDisplay;
  }


  displayLCD(0, 0, LCD_1);
  displayLCD(1, 0, LCD_2);


}
