/*
----------------------------------------------------
Cablage:
*Pin Number 2  ==> Green LED  | long with signal and short with GND
*Pin Number 3  ==> Red Led    | long with signal and short with GND
*Pin Number 4  ==> Orange Led | long with signal and short with GND
*Pin Number 5  ==> Sensor     | Assimilate it with a push button
*Pin Number 6  ==> transistot | C=>Vcc E=>Servo(Vcc) B=>Command
*Pin Number 7  ==> Servo      | +&5V -&GND o&P7
*Pin Number 8  ==> Buzzer     | +&P8 -&GND
*Pin Number 9  ==> RST        | RFID PINs 
*Pin Number 10 ==> SDA        | RFID PINs
*Pin Number 11 ==> MOSI       | RFID PINs
*Pin Number 12 ==> MISO       | RFID PINs
*Pin Number 13 ==> SCK        | RFID PINs
*Pin GND       ==> RFID_GND + Buzzer + Servo + LEDresROG
*Pin 5V        ==> Servo
*Pin 3.3V      ==> RFID_VCC
------------------------------------------------------
*/

#include <MFRC522.h>
#include <Servo.h>

#define GREEN_LED_PIN 2
#define RED_LED_PIN 3
#define Orange_LED_PIN 4
#define Alarm_PIN 8
#define Trans 6

MFRC522 rfid(10, 9);
Servo myservo1; 

String users[] = {"c10a7d89"};
int usersSize = sizeof(users)/sizeof(String);
const int mySensor = 5;
bool userPresence = false;

void setup(){
  Serial.begin(9600);
  myservo1.attach(7); 
  SPI.begin();
  rfid.PCD_Init();
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(Trans, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(Orange_LED_PIN, OUTPUT);
  pinMode(mySensor,INPUT_PULLUP);
  pinMode(Alarm_PIN,OUTPUT); 
  digitalWrite(Trans, HIGH);
  myservo1.write (120);
  delay(100);
  digitalWrite(Trans, LOW);
}

void loop(){
  if ((digitalRead(mySensor)==HIGH))
  {
    triggerAlarm();
  }
  
  if(rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()){
    String uid = getUID();
    if(checkUID(uid)){
      userPresence = true;
      blinkLed(GREEN_LED_PIN, 2000, 1);
      digitalWrite(Trans, HIGH);
      myservo1.write (3++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++0);
      delay(10000);
      myservo1.write (120);
      delay(1000);
      digitalWrite(Trans, LOW);
      while (digitalRead(mySensor) == HIGH)
      {
        blinkLed(Orange_LED_PIN, 400, 4);
      }
      return;
    }else{
      blinkLed(RED_LED_PIN, 400, 2);
      digitalWrite(Trans, HIGH);
      myservo1.write (120);
      delay(1000);
      digitalWrite(Trans, LOW);
    }
  }
  delay(10);
}

String getUID(){
  String uid = "";
  for(int i = 0; i < rfid.uid.size; i++){
    uid += rfid.uid.uidByte[i] < 0x10 ? "0" : "";
    uid += String(rfid.uid.uidByte[i], HEX);
  }
  rfid.PICC_HaltA();
  return uid;
}

bool checkUID(String uid){
  for(int i = 0; i < usersSize; i++){
    if(users[i] == uid){
      return true;
    }
  }
  return false;
}
void blinkLed(int led, int duration, int repeat){
  int i = 0;
  while((i < repeat)){
    digitalWrite(led, HIGH);
    delay(duration/2);
    digitalWrite(led, LOW);
    delay(duration/2);
    i++;
  }
  exit;
}
void blinkAlarmLed(int led, int duration, int repeat){
  int i = 0;
  delay(1000);
  digitalWrite(Trans, HIGH);
  myservo1.write (120);
  delay(500);
  digitalWrite(Trans, LOW);
  while((i < repeat)&&(digitalRead(mySensor)==HIGH)){
    digitalWrite(led, HIGH);
    delay(duration/2);
    digitalWrite(led, LOW);
    delay(duration/2);
    i++;
  }
  exit;
}
void triggerAlarm(){
  blinkAlarmLed(RED_LED_PIN, 200, 20);
  blinkAlarmLed(Orange_LED_PIN, 200, 20);
  if (digitalRead(mySensor)==HIGH){
    int prev = millis(), act = 0;
    while (act < 6000 * 5)
    {
      noTone(Alarm_PIN);
      tone(Alarm_PIN,494,500);
      delay(300);
      noTone(Alarm_PIN);
      tone(Alarm_PIN,523,300);
      delay(200);
      noTone(Alarm_PIN);
      act = millis()- prev;
    }
  }  
}
