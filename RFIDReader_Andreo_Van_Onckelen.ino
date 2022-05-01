#include <SPI.h>
#include <MFRC522.h>


#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
int forward_button = A2;
int backward_button = A3;
int left_button = A4;
int right_button = A5;

int PWM_M1 = 5;
int PWM_M2 = 6; 
int Dir_M1 = 3; 
int Dir_M2 = 4; 

int pot = A0; // POTENTIOMETER
int input_val = 0;
int speed_val = 0;

int io = false; // AAN/UIT AUTO

void setup() {
  pinMode(forward_button, INPUT_PULLUP);
  pinMode(backward_button, INPUT_PULLUP);
  pinMode(left_button, INPUT_PULLUP);
  pinMode(right_button, INPUT_PULLUP);

  pinMode(pot, INPUT);

  pinMode(PWM_M1, OUTPUT);
  pinMode(PWM_M2, OUTPUT);
  pinMode(Dir_M1, OUTPUT);
  pinMode(Dir_M2, OUTPUT);
  Serial.begin(9600);   // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  delay(4);       // Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop() {

//KAARTLEZER

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( mfrc522.PICC_IsNewCardPresent()&& mfrc522.PICC_ReadCardSerial()) {
  // Show UID on serial monitor
  Serial.println("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.println(mfrc522.uid.uidByte[i]);
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " " ));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "86 7A A7 81" || content.substring(1) == "E6 71 A5 81") // Verander hier de UID die je toegang wilt geven
  {
    Serial.println("Authorized access");
    tone(8,800,200);
    delay(1000);
    io = ! io;
    
  }  

  else {
    Serial.println("Access Denied");
    tone(8,300,200);
    delay(1000);
    io = !io;
  }
  }





if(io){
  // AUTO UNLOCKED
  input_val = analogRead(pot);
  speed_val = map(input_val, 0, 1024, 150, 255);

   if (digitalRead(forward_button) == LOW) {  //VOORUIT
    Serial.println("vooruit");
    digitalWrite(Dir_M1, HIGH);
    digitalWrite(Dir_M2, HIGH);
    analogWrite(PWM_M1, 255 - speed_val);
    analogWrite(PWM_M2, 255 - speed_val);
  }
  else if (digitalRead(backward_button) == LOW) {  //ACHTERUIT
    Serial.println("achter");
    digitalWrite(Dir_M1, LOW);
    digitalWrite(Dir_M2, LOW);
    analogWrite(PWM_M1, speed_val);
    analogWrite(PWM_M2, speed_val);
  }
  else if (digitalRead(left_button) == LOW) {  //LINKS
    Serial.println("links");
    digitalWrite(Dir_M1, HIGH);
    digitalWrite(Dir_M2, LOW);
    analogWrite(PWM_M1, speed_val);
    analogWrite(PWM_M2, 255 - speed_val);
  }
  else if (digitalRead(right_button) == LOW) { //RECHTS
    Serial.println("rechts");
    digitalWrite(Dir_M1, LOW);
    digitalWrite(Dir_M2, HIGH);
    analogWrite(PWM_M1, 255 - speed_val);
    analogWrite(PWM_M2, speed_val);
  }
  else {  //STOP
    speed_val = 0;
    digitalWrite(Dir_M1, LOW);
    digitalWrite(Dir_M2, LOW);
    analogWrite(PWM_M1, speed_val);
    analogWrite(PWM_M2, speed_val);
  }
  }
  }
  
