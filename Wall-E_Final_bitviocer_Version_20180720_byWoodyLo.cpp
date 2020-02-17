#include "Arduino.h"
#include <SoftwareSerial.h>
#include <Servo.h>
#include "DFRobotDFPlayerMini.h"

// Integer variable be a counter for Arduino main loop function.
int counter4MainLoop = 0;

/*----Bluetooth's Variables----*/
//Create a serial connection with TX and RX on these pins
SoftwareSerial BLE_serial_connection(A1, A0);  //Bluetooth's Rx->A0_Pin; Tx->A1_Pin

/*----DFPlayer's Variables----*/
//Create a serial connection with TX and RX on these pins
SoftwareSerial DFP_serial_connection(5, 6);   //Mini DFPlayer's Rx->6_Pin; Tx->5_Pin
DFRobotDFPlayerMini myDFPlayer;               //This will call in DFRobotDFPlayerMimi.cpp

/*----Upper Body's Variables----*/
Servo servo_right_Hand;
int pos_right_Hand;
Servo servo_left_Hand;
int pos_left_Hand;
Servo servo_Head;
int pos_Head;
int led_eyes = A2;
/*----Lowwer Body's Variables----*/
int en_right_DCMotor=11;
int right_DCMotor1=13;
int right_DCMotor2=12;
int en_left_DCMotor=10;
int left_DCMotor1=9;
int left_DCMotor2=8;


void setup(){
  //Bluetooth Setup via SoftwareSerial.h
  Serial.begin(9600);//Initialize communications to the serial monitor in the Arduino IDE
  BLE_serial_connection.begin(9600);//Initialize communications with the bluetooth module
  BLE_serial_connection.println("Ready!!!");//Send something to just start comms. This will never be seen.
  Serial.println("Started");//Tell the serial monitor that the sketch has started. 

  //DFPlay Setup via SoftwareSerial.h
  DFP_serial_connection.begin(9600);
  //Serial.begin(9600);
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  if (!myDFPlayer.begin(DFP_serial_connection)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));

  //Upper Body
  servo_Head.attach(A5);
  pinMode(led_eyes, OUTPUT);
  servo_right_Hand.attach(A3);
  servo_left_Hand.attach(A4);
  //Lower Body
  pinMode(en_right_DCMotor, OUTPUT);
  pinMode(right_DCMotor1, OUTPUT);
  pinMode(right_DCMotor2, OUTPUT);
  pinMode(en_left_DCMotor, OUTPUT);
  pinMode(left_DCMotor1, OUTPUT);
  pinMode(left_DCMotor2, OUTPUT);
  
}//END of setup function


/*****INITIALIZATION*****/
void initialization(){
  //Look Forward
  pos_Head = 100;
  servo_Head.write(pos_Head);
  //Both Hands Down
  pos_right_Hand=110;  pos_left_Hand=105;
  servo_right_Hand.write(pos_right_Hand);
  servo_left_Hand.write(pos_left_Hand);
  //Wink Three Times
  for(int i=0; i<1; i+=1){
    digitalWrite(led_eyes, HIGH);
    delay(200);
    digitalWrite(led_eyes, LOW);
    delay(200);
  }//for
}//END of initialization Function


/*****BLUETOOTH FUNCTION (communicate with laptop using PYTHON)*****/
char * dealing_bluetooth(){

  #define BUFFER_SIZE 64       //This will prevent buffer overruns.
  //Python script will sends datas into Arduino variable of inData[BUFFER_SIZE].
  char * inData[BUFFER_SIZE];  //This is a character buffer
  char inChar=-1;              //Initialie the first character as nothing
  int i=0;                     //Create the looping variable once
  //Gets the number of bytes that were sent by the python script
  byte byte_count = BLE_serial_connection.available(); 

  //If there are any bytes then deal with them
  if(byte_count){
    int first_bytes=byte_count;//Initialize the number of bytes that we might handle. 
    int remaining_bytes=0;//Initialize the bytes that we may have to burn off to prevent a buffer overrun

    //If the incoming byte count is more than our buffer...
    if(first_bytes >= BUFFER_SIZE -1){
      //Reduce the bytes that we plan on handleing to below the buffer size
      remaining_bytes = byte_count - (BUFFER_SIZE - 1);
    }//if
    
    //Handle each character which send by python script.
    for(i=0; i<first_bytes; i++){
      inChar = BLE_serial_connection.read();  //Read one byte
      *inData[i]=inChar;                   //Put it into a character(char(array))
    }//for

    //This ends the character array with a null character.
    inData[i] = '\0';  //This signals are the end of a character array

    //This burns off any remaining bytes that the buffer can't handle.
    for(i=0; i<remaining_bytes; i++){
      inChar = BLE_serial_connection.read();
    }//for

    //Print to the monitor what was detected
    Serial.println(*inData);
  }//if

  return *inData;
}//End of dealing_bluetooth Function


/*****FIRST FUNCTION - hey_walle*****/
void hey_walle(){
  /*Bluetooth Command is 'A'(chat type)*/
  digitalWrite(led_eyes, HIGH);  // When hey Wall-E then WALL-E opens his eyes.
  myDFPlayer.volume(28);         // Set volume value 28 (0~30).
  myDFPlayer.playMp3Folder(1);   // Play specific mp3 in SD:/MP3/0001.mp3; File Name(0~65535)
  eyes_blink();                  // Call eyes_blink function.
  delay(1050);                   // Delay for playing music.
}//END of hey_walle Function


/*****SECOND FUNCTION - say_eva*****/
void say_eva(){
  /*Bluetooth Command is 'B'(cahr type)*/
  digitalWrite(led_eyes, HIGH);  // When saying eva WALL-E opens his eyes.
  pos_right_Hand=0;              // Set right hand position to the top.
  servo_right_Hand.write(pos_right_Hand);
  delay(100);
  myDFPlayer.volume(28);         // Set volume value 28 (0~30).
  myDFPlayer.playMp3Folder(4);   // Play specific mp3 in SD:/MP3/0004.mp3; File Name(0~65535)
  delay(1100);                   // Delay for playing music.
  pos_right_Hand=110;            // Set right hand position to the down.
  servo_right_Hand.write(pos_right_Hand);
  digitalWrite(led_eyes, LOW);   // Closes his eyes.
  delay(100);
}//END of say_eva Function


/*****THIRD FUNCTION - wave_right_hand*****/
void wave_right_hand(){
  /*Bluetooth Command is 'C'(char type)*/
  // This for loop is how many times that we want to WALL-E wave his right hand.
  for(int t=0; t<2; t+=1){
    digitalWrite(led_eyes, HIGH);  // When hands up WALL-E opens his eyes.
    for(pos_right_Hand=110; pos_right_Hand>0; pos_right_Hand-=1){
      servo_right_Hand.write(pos_right_Hand);
      delay(10);
    }// for
    digitalWrite(led_eyes, LOW);   // When hands up WALL-E closes his eyes.
    for(pos_right_Hand=0; pos_right_Hand<110; pos_right_Hand+=1){
      
      servo_right_Hand.write(pos_right_Hand);
      delay(10);
    }// for
  }// for
}//END of wave_right_hand Function


/*****FOURTH FUNCTION - arms_up*****/
void arms_up(){
  /*Bluetooth Command is 'D'(chat type)*/
  pos_right_Hand=30;  pos_left_Hand=180;
  servo_right_Hand.write(pos_right_Hand);
  servo_left_Hand.write(pos_left_Hand);
  digitalWrite(led_eyes, HIGH);
  // Raise WALL-E's hands for two seconds. Then put hands down.
  delay(2000);
  digitalWrite(led_eyes, LOW); 
  pos_right_Hand=110;  pos_left_Hand=105;
  servo_right_Hand.write(pos_right_Hand);
  servo_left_Hand.write(pos_left_Hand);
}//END of arms_up Function


/*****FOURTH FUNCTION - arms_up*****/
void move_forward(){
  /**Bluetooth Command is 'E'(char type)*/
  // Carrying armature conducotrs experience force due to the magnetic field.
  digitalWrite(right_DCMotor1, HIGH);   // Set one side of commutator of right motor high level.
  digitalWrite(right_DCMotor2, LOW);    // Set one side of commutator of right motor low level.
  digitalWrite(left_DCMotor1, HIGH);    // Set one side of commutator of left motor high level.
  digitalWrite(left_DCMotor2, LOW);     // Set one side of commutator of left motor low level.
  digitalWrite(en_right_DCMotor, 190);  // Control the right motor speed to 190(0~255).
  digitalWrite(en_left_DCMotor, 190);   // Control the left motor speed to 190(0~255).
  delay(5000);                          // Let WALL-E go forward for five second.
  digitalWrite(en_right_DCMotor, 0);    // Turn off right CD motor enable pin and set to no input power. 
  digitalWrite(en_left_DCMotor, 0);     // Turn off left CD motor enable pin and set to no input power. 
  delay(100);
}//END of move_forward Function


/*****FOURTH FUNCTION - arms_up*****/
void move_backward(){
  /**Bluetooth Command is 'E'(char type)*/
  // Carrying armature conducotrs experience force due to the magnetic field.
  digitalWrite(right_DCMotor1, LOW);    // Set one side of commutator of right motor low level.
  digitalWrite(right_DCMotor2, HIGH);   // Set one side of commutator of right motor high level.
  digitalWrite(left_DCMotor1, LOW);     // Set one side of commutator of left motor low level.
  digitalWrite(left_DCMotor2, HIGH);    // Set one side of commutator of left motor high level.
  digitalWrite(en_right_DCMotor, 190);  // Control the right motor speed to 190(0~255).
  digitalWrite(en_left_DCMotor, 190);   // Control the left motor speed to 190(0~255).
  delay(5000);                          // Let WALL-E go forward for five second.
  digitalWrite(en_right_DCMotor, 0);    // Turn off right CD motor enable pin and set to no input power.
  digitalWrite(en_left_DCMotor, 0);     // Turn off left CD motor enable pin and set to no input power. 
  delay(100);
}//END of move_forward Function


/*****SEVENTH FUNCTION - eyes_blink*****/
void eyes_blink(){
  /*Bluetooth Command is 'G'(char type)*/
  // This for loop is how many times that we want to WALL-E blink.
  for(int blink_times=0; blink_times<3; blink_times+=1){
    digitalWrite(led_eyes, HIGH);
    delay(180);
    digitalWrite(led_eyes, LOW);
    delay(180);
  }// for
}//END of eyes_blink Function


/*****EIGHTH FUNCTION - turn_head_right*****/
void turn_head_right(){
  /*Bluetooth Command is 'H'(char type)*/
  digitalWrite(led_eyes, HIGH);  // When turn head right WALL-E opens his eyes.
  pos_Head = 40;                 // Set the head position for turn head right.
  servo_Head.write(pos_Head);
  delay(1500);                   // Look right for one and half senconds.
  pos_Head = 100;                // Look forward.
  servo_Head.write(pos_Head);
  digitalWrite(led_eyes, LOW);   // Close WALL-E's eyes.
}//END of turn_head_right Function


/*****NINTH FUNCTION - turn_head_left*****/
void turn_head_left(){
  /*Bluetooth Command is 'I'(char type)*/
  digitalWrite(led_eyes, HIGH);  // When turn head left WALL-E opens his eyes.
  pos_Head = 160;                // Set the head position for turn head left.
  servo_Head.write(pos_Head);
  delay(1500);                   // Look left for one and half senconds.
  pos_Head = 100;                // Look forward.
  servo_Head.write(pos_Head);
  digitalWrite(led_eyes, LOW);   // Close WALL-E's eyes.
}//END of turn_head_left Function


/*****TENTH FUNCTION - say_name*****/
void say_name(){
  /*Bluetooth Command is 'J'(char type)*/
  digitalWrite(led_eyes, HIGH);  // When hey Wall-E then WALL-E opens his eyes.
  myDFPlayer.volume(28);         // Set volume value 28 (0~30).
  myDFPlayer.playMp3Folder(1);   // Play specific mp3 in SD:/MP3/0001.mp3; File Name(0~65535)
  digitalWrite(led_eyes, LOW);   // Closes his eyes.
  delay(2100);                   // Delay for playing music.
}//END of say_name Function


/*****ELEVENTH FUNTION - play_music*****/
void play_music(){
  /*Bluetooth Command is 'K'(char type)*/
  digitalWrite(led_eyes, HIGH);  // When play music then WALL-E opens his eyes.
  myDFPlayer.volume(28);         // Set volume value 28 (0~30).
  myDFPlayer.playMp3Folder(7);   // Play specific mp3 in SD:/MP3/0007.mp3; File Name(0~65535)
  /* When Wall-E plays music he also do Cha-Cha */
  for(int t =0; t<3; t+=1){
    // Right hand up and left hand down for one half seconds.
    pos_right_Hand=30;  pos_left_Hand=105;  // Set right and left hands position.
    servo_right_Hand.write(pos_right_Hand);
    servo_left_Hand.write(pos_left_Hand);
    // Right hand down and left hand up for one half seconds. 
    delay(1500);
    pos_right_Hand=110;  pos_left_Hand=180; // Set right and left hands position.
    servo_right_Hand.write(pos_right_Hand);
    servo_left_Hand.write(pos_left_Hand);
    delay(1500);
  }// for
  pos_right_Hand=110;  pos_left_Hand=105;   // Put both hands down.
  servo_right_Hand.write(pos_right_Hand);
  servo_left_Hand.write(pos_left_Hand);
  delay(1000);                   // Delay for playing music.
  myDFPlayer.volume(30);         // Set volume value 30 (0~30).
  myDFPlayer.playMp3Folder(3);   // Play specific mp3 in SD:/MP3/0003.mp3; File Name(0~65535)
  eyes_blink();                  // Call eyes_blink function when play ta-da also do blink.
  delay(80);
}//END of play_music Function


/*****TWELFTH FUNCTION - laugh*****/
void laugh(){
  /*Bluetooth Command is 'L'(char type)*/
  myDFPlayer.volume(30);         // Set volume value 30 (0~30).
  myDFPlayer.playMp3Folder(5);   // Play specific mp3 in SD:/MP3/0005.mp3; File Name(0~65535)
  eyes_blink();                  // Call eyes_blink function when he is laughing also do blink.
  delay(50);
}//END of laugh Function


/*****FOURTEENTH FUNCTION - goodbye_walle*****/
void goodbye_walle(){
  /*Bluetooth Command is 'M'(char type)*/
  myDFPlayer.volume(30);         // Set volume value 30 (0~30).
  myDFPlayer.playMp3Folder(1);   // Play specific mp3 in SD:/MP3/0001.mp3; File Name(0~65535)
  wave_right_hand();             // Call wave_right_hand function.
  delay(0);
}//END of goodbye_walle Function


/*---------------------------------START EXTRA FUNCTION-----------------------------------*/
/*****EXTRA FUNCTION 1 - shake_head*****/
void shake_head(){
  /*Bluetooth Command is 'N'(char type)*/
  for(pos_Head=40; pos_Head<160 ; pos_Head+=1){
    servo_Head.write(pos_Head);
    if(pos_Head%10==0){
    digitalWrite(led_eyes, LOW);  
    }else{
    digitalWrite(led_eyes, HIGH);  
    }// if...else...
    delay(22);
  }// for
  for(pos_Head=160; pos_Head>40; pos_Head-=1){
    servo_Head.write(pos_Head);
    if(pos_Head%10==0){
    digitalWrite(led_eyes, LOW);  
    }else{
    digitalWrite(led_eyes, HIGH);  
    }// if...else...
    delay(22);
  }// for
  // Turn head back to forward.
  pos_Head = 100;              // Look Forward.
  servo_Head.write(pos_Head);
  digitalWrite(led_eyes, LOW); // Turn off eyes's led.
}//END of shake_head Function


/*****EXTRA FUNCTION 2 - warm_up*****/
void warm_up(){
  //pos_right_Hand;  pos_left_Hand;  pos_Head;
  //servo_right_Hand;  servo_left_Hand;  servo_Head;
  digitalWrite(led_eyes, HIGH);
  pos_right_Hand=30;  pos_Head=160;
  servo_right_Hand.write(pos_right_Hand);
  servo_Head.write(pos_Head);
  delay(500);
  pos_right_Hand=110;  pos_left_Hand=80;  pos_Head=40;
  servo_right_Hand.write(pos_right_Hand);
  servo_left_Hand.write(pos_right_Hand);
  servo_Head.write(pos_Head);
  delay(500);
  //Look Forward
  pos_Head = 100;
  servo_Head.write(pos_Head);
  //Both Hands Down
  pos_right_Hand=110;  pos_left_Hand=105;
  servo_right_Hand.write(pos_right_Hand);
  servo_left_Hand.write(pos_left_Hand);
  arms_up();
  digitalWrite(led_eyes, LOW);
  
}//END of warm_up Function
/*---------------------------------END EXTRA FUNCTION-----------------------------------*/




void loop(){
  //move_forward();
  if(counter4MainLoop==0){
    initialization();
  }// if

    BLE_serial_connection.listen();
  //char * inData = dealing_bluetooth();
  char inData = BLE_serial_connection.read();
  
  //Pause for a moment
  
  if(inData=='A'){
    DFP_serial_connection.listen();
    hey_walle();
    inData='0';
  }// if
  if(inData=='B'){
    DFP_serial_connection.listen();
    say_eva();
    inData='0';
  }// if
  if(inData=='C'){
    wave_right_hand();
    inData='0';
  }// if
  if(inData=='D'){
    arms_up();
    inData='0';  
  }// if
  if(inData=='E'){
    move_forward();
    inData='0';
  }
  if(inData=='F'){
    move_backward();
    inData='0';
  }// if
  if(inData=='G'){
    eyes_blink();
    inData='0'; 
  }// if
  if(inData=='H'){
    turn_head_right();
    inData='0';
  }// if
  if(inData=='I'){
    turn_head_left();
    inData='0';
  }// if
  if(inData=='J'){
    DFP_serial_connection.listen();
    say_name();
    inData='0';
  }// if
  if(inData=='K'){
    DFP_serial_connection.listen();
    play_music();
    inData='0';
  }// if
  if(inData=='L'){
    DFP_serial_connection.listen();
    laugh();
    inData='0';
  }// if
  if(inData=='M'){
    DFP_serial_connection.listen();
    goodbye_walle();
    inData='0';
  }// if
  if(inData=='N'){
    shake_head();
    inData='0';
  }// if
  if(inData=='O'){
    warm_up();
    inData='0';
  }// if
  delay(200);
  counter4MainLoop += 1;


}//END of loop
