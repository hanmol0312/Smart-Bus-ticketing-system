#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <LiquidCrystal.h>
#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above
#define Buzzer          2          //Buzzer pin 
#define button1         8          //button1 pin
#define button2         6          //button2 pin
#define red_led_pin     8          //led pin
#define green_led_pin   4
LiquidCrystal lcd(A0 ,A1 ,A2 ,A3, A4, A5);
bool recharge=false;
int A[9];
int nw;
int fare=25;~
int stops=0;
bool isConductor = false;
int delaytime=5000;
String trainStops[9]={"START","TAMBARAM","CHROMEPET","PALLAVARAM","MINAMBAKKAM","GUINDY","KODAMBAKKAM","CHENNAI EGMORE","STOP"};
int dt=70;
bool bus;
int tags = 5;
int pos;
struct passenger{
  bool flag=false;
  int bal=200;
  int stop_count;
  String RF_id="";
  };
  passenger p[10];
int counter=3;
bool seats=true;
bool addUser = false;
String User = "C3 6003 A6";
String conductor ="AD 9F B0 79";
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
Servo myservo1 ;  //servo name
Servo myservo2;
void setup() {
  lcd.begin(16,2);
  Serial.begin(9600);   // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
 myservo1.attach(5);
 myservo1.write(45);
 myservo2.attach(7);
 myservo2.write(0);
 pinMode(button1,INPUT);
 pinMode(button2,INPUT);
 pinMode(Buzzer, OUTPUT);
 pinMode(red_led_pin,OUTPUT);
 pinMode(green_led_pin,OUTPUT);
 digitalWrite(red_led_pin,LOW);
 digitalWrite(green_led_pin,HIGH);
 pinMode(A0,INPUT);
 digitalWrite(A0,HIGH);
  pinMode(A1,INPUT);
 digitalWrite(A1,HIGH);
  pinMode(A2,INPUT);
 digitalWrite(A2,HIGH);
  pinMode(A3,INPUT);
 digitalWrite(A3,HIGH);
  pinMode(A4,INPUT);
 digitalWrite(A4,HIGH);
 pinMode(A5,INPUT);
 digitalWrite(A5,HIGH);
 digitalWrite(button2,HIGH);
 noTone(Buzzer);
  mfrc522.PCD_Init();   // Init MFRC522
  delay(4);       // Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
    Serial.println("Bring Your card near the reader...");
   
}

void loop() {
    p[0].RF_id= "30 E1 97 A3";
    p[1].RF_id= "90 CF 67 A4";
    p[2].RF_id= "C7 3D C3 1F";
    p[3].RF_id= "60 E9 8E A3";
   // p[5].RF_id = "E3 84 D1 95";
    p[4].RF_id = "1020304";
    delay(dt);
    nw=digitalRead(button2);
    delay(dt);
    if(nw==0){
      bus = !bus;
      stops+=1;
    }
    if(stops==18){
      Serial.println("End of Journey");
      stops =0;
      return ;
    }
    if(isConductor == true){
      Serial.println("Refill of tickets active");
    }
//    emer=digitalRead(button1);
//    delay(dt);
//    if(emer==0){
//      emergency = !emergency;
//    }
//    delay(dt);
//    if(emergency){
//      Serial.println("Emergency");
//       myservo1.write(90);
//       myservo2.write(90);
//       delay(500);
//          tone(Buzzer,500);
//          delay(300);
//          noTone(Buzzer);
//       return;
//    }
//    else{
//      
//    }
    if(bus){
      
      Serial.println("Bus is at ");
      Serial.println(trainStops[stops/2]);
      Serial.println("Stop: ");
      Serial.println(stops/2);
      digitalWrite(red_led_pin,HIGH);
      digitalWrite(green_led_pin,LOW);
      delay(500);
      for(int i=0;i<9;i++){
        int count=0;
        int j=0;
        if(p[i].flag==true && p[i].bal<= fare *((stops-(p[i].stop_count)))){
          Serial.print("User:");
          Serial.print(i);
          Serial.print("cannot travel further");
          delay(500);
          tone(Buzzer,500);
          delay(300);
          noTone(Buzzer);
          recharge=true;
          A[j]=i;
          count+=1;
          j+=1;
        }
        }
//        digitalWrite(led_pin,HIGH);
      }
    else{
      Serial.println("Bus is moving");
      digitalWrite(red_led_pin,LOW);  
      
      digitalWrite(green_led_pin,HIGH);
      delay(500);
      Serial.println("Next Stop : ");
      Serial.println(trainStops[(stops/2)]);
//      digitalWrite(led_pin,LOW);
      return;
    }
    
    
    
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  
  Serial.print("UID tag: ");
  String content="";
 
  byte letter;
    for(byte i = 0;i < mfrc522.uid.size;i++)
    {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0":" ");
      Serial.print(mfrc522.uid.uidByte[i],HEX);
      content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0": " "));
      content.concat(String(mfrc522.uid.uidByte[i],HEX));
    }
    Serial.println(content.substring(1));
    Serial.print("MEssage:  ");
    content.toUpperCase();
//    if(recharge && content.substring(1)== p[2].RF_id){
//                recharge =false;
//                int j=0;
//                for(int i=0;i<9;i++){
//                        if(A[j]==i){
//                            p[i].bal=200;
//                            j+=1;
//                          }
//                    }
//                 Serial.println("Recharge successful for passengers");
//                 return;
//                  
//      }
   if(content.substring(1) == "AD 9F B0 79"){
          isConductor = !isConductor;
          tone(Buzzer,500);
          delay(300);
          noTone(Buzzer);
          delay(1000);
        }
  if(isConductor){
    Serial.println("Refill of Card Active");
    for(int i=0;i<9;i++){
      if(content.substring(1) == p[i].RF_id){
         p[i].bal+=100;
         Serial.println("Filled for user:");
         Serial.print(i+1);
         Serial.println("New Balance" + p[i].bal);
         tone(Buzzer,500);
          delay(300);
          noTone(Buzzer);
      }
   }
   return; 
  }
  //
  if(content.substring(1) == User){
          addUser = !addUser;
          tone(Buzzer,500);
          delay(300);
          noTone(Buzzer);
          delay(1000);
        }
  if(addUser){
    Serial.println("Add User");
    if(content.substring(1) != User){
      delay(1000);
      p[tags].RF_id = content.substring(1);
      Serial.println("User Added : ");
      Serial.print(content.substring(1));
    tags++;
      }
    delay(1000);
   return; 
  }


    if(counter == 0){
      for(int i=0;i<9;i++){
      if(content.substring(1) == p[i].RF_id && p[i].flag==true){
          p[i].bal -= fare * (stops-p[i].stop_count);
          Serial.println(fare * (stops-p[i].stop_count));
          Serial.println("Balance: ");
          Serial.print(p[i].bal);
          tone(Buzzer,500);
          delay(300);
          noTone(Buzzer);
          seats=false;
       for(pos=0;pos<=90;pos+=1)
  {
      myservo2.write(pos);
      delay(15);
  }
  for(pos=90;pos>=0;pos-=1)
  {
      myservo2.write(pos);
      delay(15);
  }
          p[i].flag=false;
          counter+=1;
          return;
      }

   }

    if(seats)
      {
        Serial.println("No Seats Available");
        tone(Buzzer,500);
        delay(300);
        noTone(Buzzer);
        delay(200);
        tone(Buzzer,500);
        delay(300);
        noTone(Buzzer);
        return;
  }

}

 

    for(int i=0;i<9;i++){
      if(content.substring(1) ==p[i].RF_id){
      Serial.println("Authorised Access");
      Serial.println();
      }
    }
    for(int i=0;i<9;i++){
      if(content.substring(1)== p[i].RF_id ){
        if(p[i].flag == true){
          p[i].bal -= fare*(stops-p[i].stop_count);   
          Serial.println("Fare:");
          Serial.println((fare)*(stops-p[i].stop_count));  
          Serial.println(p[i].bal);  
          delay(500);
          tone(Buzzer,500);
          delay(300);
          noTone(Buzzer);
   for(pos=0;pos<=90;pos+=1)
  {
      myservo2.write(pos);
      delay(15);
  }
  for(pos=90;pos>=0;pos-=1)
  {
      myservo2.write(pos);
      delay(15);
  }
          p[i].flag=false;
          counter+=1;
          content.substring(1)=="";
          return;
        }
        else{
          if(p[i].bal==0){
                Serial.println("NO balance");
                recharge=true;
                return;
            }
          p[i].stop_count=stops;
          delay(500);
          tone(Buzzer,500);
          delay(300);
          noTone(Buzzer);
  for(pos=45;pos<=135;pos+=1)
  {
      myservo1.write(pos);
      delay(15);
  }
  for(pos=135;pos>=45;pos-=1)
  {
      myservo1.write(pos);
      delay(15);
  }
          p[i].flag=true;
          counter-=1;
          content.substring(1)=="";
        }
      }
    }

}
