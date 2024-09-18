#include <Sim800L.h>
#include <SoftwareSerial.h>
// #include <ESP8266WiFi.h>
#include <EEPROM.h>

// Prototyping
// void sensorSetup();
// int sensorLoop();
// void writeEEPROM(String number, uint8_t adminNo);
// String readEEPROM(uint8_t adminNo);
// void phoneNumberRegistration();

Sim800L Sim800l(4, 5);  //TX & RX pin D5 & D6
// SoftwareSerial Sender(4, 5);



uint8_t led = LED_BUILTIN, siren = 12;  //Output pin D2

int wait = 80;

int indexOfPhoneNumberString1 = 5;  //assigning random values just to not get garbage value when a error occurs
int indexOfPhoneNumberString2 = 5;  //assigning random values just to not get garbage value when a error occurs
int indexOfPhoneNumberString3 = 5;  //assigning random values just to not get garbage value when a error occurs

String phoneNumber1 = "0";
String phoneNumber2 = "0";
String phoneNumber3 = "0";
String response;

char tempConvertedCharNumber1[15];
char tempConvertedCharNumber2[15];
char tempConvertedCharNumber3[15];
char reader;

//Constants for SENSOR
const int buttonPin = 8;  // the number of the pushbutton pin D7
int buttonState = 0;       // variable for reading the pushbutton status
int dlay = 10;
String textNew, textOld = "";

void setup() {
  // WiFi.mode(WIFI_OFF);
  pinMode(led, OUTPUT);
  pinMode(siren, OUTPUT);
  digitalWrite(led, !HIGH);
  digitalWrite(siren, LOW);
  sensorSetup();

  Serial.begin(57600);
  Sim800l.begin(57600);
  // EEPROM.begin(512);
  Serial.println("Initializing...");
  while (wait--) {
    delay(100);
    digitalWrite(led, !LOW);
    delay(100);
    digitalWrite(led, !HIGH);
  }

  while (!Sim800l.prepareForSmsReceive()) {
    Serial.println("SIM is not ready");
    delay(1000);
  }

  Serial.println("SIM is ready!");

    // Configure SIM800L to text mode
  Sim800l.write("AT\r\n");
  Sim800l.write("AT+CMGF=1\r\n");
  delay(100);
    // Configure SIM800L to show new incoming messages
  Sim800l.write("AT+CNMI=2,2,0,0,0\r\n");
  delay(100);

  phoneNumber1 = readEEPROM(1);
  Serial.print("admin1 is ");
  Serial.println(phoneNumber1);
  phoneNumber2 = readEEPROM(2);
  Serial.print("admin2 is ");
  Serial.println(phoneNumber2);
  phoneNumber3 = readEEPROM(3);
  Serial.print("admin3 is ");
  Serial.println(phoneNumber3);
  digitalWrite(led, !LOW);
}

void loop() {
  delay(1000);
  // while(Sender.available()) {
        
  // }
  if (!dlay--) {
    Serial.print("Reading SMS..");
    dlay = 20;

    //MMMMMMMMMMMMMMM***************MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM**************MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM**************MMMMMMMMMMMMMMMMMMM
    //MMMMMMMMMMMMMMM***************MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM**************MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM**************MMMMMMMMMMMMMMMMMMM
    //MMMMMMMMMMMMMMM***************MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM**************MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM**************MMMMMMMMMMMMMMMMMMM
    //MMMMMMMMMMMMMMM***************MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM**************MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM**************MMMMMMMMMMMMMMMMMMM

    
    while(Sim800l.available()) {
      // Serial.println("Serial available");
      reader = Sim800l.read();
      response += reader;
      // Serial.write(Sim800l.read());
    }
    Serial.println(response);
    // textNew = "";
    // // response = Sim800l.readStringUntil('\n');

    if (response.indexOf("+CMT:") != -1) {
      textNew = "";

      int crlf = response.indexOf("\r\n");
      textNew = response.substring(crlf + 2);
      Serial.println(textNew);
    }
    response = "";

    //MMMMMMMMMMMMMMMM**************MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM**************MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM**************MMMMMMMMMMMMMMMMMMM
    //MMMMMMMMMMMMMMM***************MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM**************MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM**************MMMMMMMMMMMMMMMMMMM
    //MMMMMMMMMMMMMMM***************MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM**************MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM**************MMMMMMMMMMMMMMMMMMM
    //MMMMMMMMMMMMMMM***************MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM**************MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM**************MMMMMMMMMMMMMMMMMMM
    //MMMMMMMMMMMMMMM***************MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM**************MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM**************MMMMMMMMMMMMMMMMMMM




    
    //textNew = "admin101710635707 admin201961042756";
    if (textNew != "" && textNew != textOld) {
      textOld = textNew;
      Serial.println("New SMS Received!");
      Serial.print(">>");
      Serial.println(textNew);
      phoneNumberRegistration();
    } else {
      Serial.println("No new SMS");
    }
  }
  //Taking sensor data
  int sensorState = sensorLoop();
  if (sensorState) {
    digitalWrite(siren, HIGH);
    phoneNumber1.toCharArray(tempConvertedCharNumber1, 15);
    phoneNumber2.toCharArray(tempConvertedCharNumber2, 15);
    phoneNumber3.toCharArray(tempConvertedCharNumber3, 15);

    delay(1000);
    Sim800l.callNumber(tempConvertedCharNumber1);
    Serial.print("Calling at: ");
    Serial.println(tempConvertedCharNumber1);
    delay(15000);
    digitalWrite(siren, LOW);
    digitalWrite(led, LOW);
    Sim800l.hangoffCall();
    Sim800l.callNumber(tempConvertedCharNumber2);
    Serial.print("Calling at: ");
    Serial.println(tempConvertedCharNumber2);
    delay(15000);
    Sim800l.hangoffCall();
    Sim800l.callNumber(tempConvertedCharNumber3);
    Serial.print("Calling at: ");
    Serial.println(tempConvertedCharNumber3);
    delay(15000);
    Sim800l.hangoffCall();
    delay(15000);
  }
}

void phoneNumberRegistration() {
  if (textNew.indexOf("admin1") != -1) {
    indexOfPhoneNumberString1 = textNew.indexOf("admin1");
    phoneNumber1 = "";
    Serial.print("admin1 is modified to ");
    phoneNumber1 = String("+88" + textNew.substring(indexOfPhoneNumberString1 + 6, indexOfPhoneNumberString1 + 17));
    Serial.println(phoneNumber1);
    writeEEPROM(phoneNumber1, 1);
  }
  if (textNew.indexOf("admin2") != -1) {
    indexOfPhoneNumberString2 = textNew.indexOf("admin2");
    phoneNumber2 = "";
    Serial.print("admin2 is modified to ");
    phoneNumber2 = String("+88" + textNew.substring(indexOfPhoneNumberString2 + 6, indexOfPhoneNumberString2 + 17));
    Serial.println(phoneNumber2);
    writeEEPROM(phoneNumber2, 2);
  }
  if (textNew.indexOf("admin3") != -1) {
    indexOfPhoneNumberString3 = textNew.indexOf("admin3");
    phoneNumber3 = "";
    Serial.print("admin3 is modified to");
    phoneNumber3 = String("+88" + textNew.substring(indexOfPhoneNumberString3 + 6, indexOfPhoneNumberString3 + 17));
    Serial.println(phoneNumber3);
    writeEEPROM(phoneNumber3, 3);
  }
}

void sensorSetup() {
  pinMode(buttonPin, INPUT);
}

int sensorLoop() {
  buttonState = digitalRead(buttonPin);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    Serial.println("Sensor on");
    digitalWrite(led, !HIGH);
    return 1;
  } else {
    Serial.println("Sensor off");
    // turn LED off:
    digitalWrite(led, !LOW);
    return 0;
  }
}

String readEEPROM(uint8_t adminNo) {
  String output;
  if (adminNo == 1) {
    for (int i = 0; i < 14; i++) {
      output += (char)EEPROM.read(i);
    }
  } else if (adminNo == 2) {
    for (int i = 14; i < 28; i++) {
      output += (char)EEPROM.read(i);
    }
  } else if (adminNo == 3) {
    for (int i = 28; i < 42; i++) {
      output += (char)EEPROM.read(i);
    }
  }
  return output;
}

void writeEEPROM(String number, uint8_t adminNo) {
  if (adminNo == 1) {
    for (int i = 0, j = 0; i < 14; i++, j++) {
      EEPROM.write(i, (int)number[j]);
    }
  } else if (adminNo == 2) {
    for (int i = 14, j = 0; i < 28; i++, j++) {
      EEPROM.write(i, (int)number[j]);
    }
  } else if (adminNo == 3) {
    for (int i = 28, j = 0; i < 42; i++, j++) {
      EEPROM.write(i, (int)number[j]);
    }
  }
  // if (EEPROM.commit()) Serial.println("Saved to EEPROM");
  for (int i = 0; i < 14; i++) {
    Serial.print(EEPROM.read(i));
  }
}
