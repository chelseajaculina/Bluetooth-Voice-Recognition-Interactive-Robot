#include <Servo.h>

Servo servo_right_Hand;
int pos_right_Hand;
Servo servo_left_Hand;
int pos_left_Hand;
Servo servo_Head;
int pos_Head;
int led_eyes = A2;


void setup(){
  //Upper Body
  servo_Head.attach(A5);
  pinMode(led_eyes, OUTPUT);
  servo_right_Hand.attach(A3);
  servo_left_Hand.attach(A4);
  //Lower Body
  
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
  delay(1500);
  pos_Head = 100;                // Look forward.
  servo_Head.write(pos_Head);
  digitalWrite(led_eyes, LOW);   // Close WALL-E's eyes.
}//END of turn_head_right Function


/*****NINTH FUNCTION - turn_head_left*****/
void turn_head_left(){
  /*Bluetooth Command is 'I'(char type)*/
  digitalWrite(led_eyes, HIGH);  // When turn head left WALL-E opens his eyes.
  pos_Head = 160;                 // Set the head position for turn head left.
  servo_Head.write(pos_Head);
  delay(1500);
  pos_Head = 100;                // Look forward.
  servo_Head.write(pos_Head);
  digitalWrite(led_eyes, LOW);   // Close WALL-E's eyes.
}//END of turn_head_left Function


/*---------------------------------START EXTRA FUNCTION-----------------------------------*/
/*****EXTRA FUNCTION 1 - shake_head*****/
void shake_head(){
  /*Bluetooth Command is NONE*/
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

/*---------------------------------END EXTRA FUNCTION-----------------------------------*/




void loop(){

  //servo_rightHand_finc();
  //servo_leftHand_finc();
  //servo_Head_finc();
  initialization();
  delay(3000);
  wave_right_hand();
  delay(3000);
  arms_up();
  delay(3000);
  shake_head();
  delay(3000);
  eyes_blink();
  delay(3000);
  turn_head_right(); 
  delay(3000);
  turn_head_left();
  delay(3000);
}//END of loop