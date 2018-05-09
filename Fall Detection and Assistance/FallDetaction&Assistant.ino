// defines pins numbers
const int stepPin = 5; //CLK+
const int dirPin = 2; //CW+ Directin of the motor
const int enPin = 8;

// rope motor
const int stepPin2 = 6; //CLK+
const int dirPin2 = 4; //CW+ Directin of the motor
const int enPin2 = 3;

const int ledPin = 13;

const int blue = 23;
const int green = 25;
const int yellow = 27;

const int interruptPin_CW = 18;
const int interruptPin_CCW = 19;
long X = 0;
long Y = 0;
long Z = 0;
bool is_reset = 0;
int position_var;
long target_step = 0;
long vertical_step = 0;


//small linear actuator
//Declare pin functions on Redboard
#define stp 22
#define dir 12
#define EN  11

// states variables that will be used in the program
long numberOfStep = 0;
int flag = 0;

//for pressure sensor
int fsrPin = 0;     // the FSR and 10K pulldown are connected to a0
int fsrPin1 = 1;
int fsrPin2 = 2;
int fsrPin3 = 3;
int fsrPin4 = 4;
int fsrPin5 = 5;

int fsrReading;     // the analog reading from the FSR resistor divider
int fsrReading1; 
int fsrReading2;
int fsrReading3;
int fsrReading4;
int fsrReading5;



int Led_internsity = 64;
int reset2 = 0;
int reset3 = 0;
void setup() { 
  //position_var = 1;
  Serial.begin(2000000);
  //LEDS
  pinMode(ledPin,OUTPUT);
  pinMode(blue,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(yellow,OUTPUT);
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(enPin,OUTPUT);

  pinMode(stepPin2,OUTPUT); 
  pinMode(dirPin2,OUTPUT);
  pinMode(enPin2,OUTPUT);


  pinMode(interruptPin_CW, INPUT);
  pinMode(interruptPin_CCW, INPUT);
  digitalWrite(enPin,LOW);  
  digitalWrite(enPin2,LOW);
  //attachInterrupt(digitalPinToInterrupt(interruptPin_CW), VMotor_CW, RISING);
  //attachInterrupt(digitalPinToInterrupt(interruptPin_CCW), VMotor_CCW, RISING);

  //small linear actuator
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(EN, OUTPUT);
  resetEDPins(); //Set step, direction, microstep and enable pins to default states
  position_var = 1;
}
void loop(){

  //for pressure sensor//
  fsrReading = analogRead(fsrPin); 
  fsrReading1 = analogRead(fsrPin1);
  fsrReading2 = analogRead(fsrPin2);
  fsrReading3 = analogRead(fsrPin3);
  fsrReading4 = analogRead(fsrPin4);
  fsrReading5 = analogRead(fsrPin5);
//
//  // LED = HIGH
    analogWrite(ledPin, Led_internsity); 
    
  
    digitalWrite(EN, LOW); //Pull enable pin low to allow motor contro
      //StepForwardDefault();
      //VMotor_CW1(100000);
      //vertical_step = 1800;
      //reverse_vertical_motor();
      //if(position_var = 1) {

    
   if(fsrReading > 1 && fsrReading1 > 1) {
    reset2 = 0;
    reset3 = 0;
    vertical_step = 5900;
    Led_internsity = 255;
    analogWrite(ledPin, 255); 
    Serial.println("Someone fell at location 0 & 1");
    target_step = 30667;
    Serial.print("target step = ");
    Serial.println(target_step);
    VMotor_CW1(target_step); 
    position_var = 0;
    }

   else if(fsrReading2 > 1 && fsrReading3 > 1) {
    reset2 = 0;
    reset3 = 0;
    vertical_step = 5900;
    Led_internsity = 255;
    analogWrite(ledPin, 255); 
    Serial.println("Someone fell at location 2 & 3");
    target_step = 105333;
    Serial.print("target step = ");
    Serial.println(target_step);
    VMotor_CW1(target_step); 
    position_var = 0;
    }

   else if(fsrReading4 > 1 && fsrReading5 > 1) {
    reset2 = 0;
    reset3 = 0;
    vertical_step = 5900;
    Led_internsity = 255;
    analogWrite(ledPin, 255); 
    Serial.println("Someone fell at location 4 & 5");
    target_step = 105333;
    Serial.print("target step = ");
    Serial.println(target_step);
    StepForwardDefault(); 
      if(reset3 != 1){
        VMotor_CW1(target_step);
        position_var = 0;
      }
    }
  
  else if(digitalRead(interruptPin_CCW) == 1 ){ // reset
    reset2 = 0;
    reset(); 
  }
  while(digitalRead(interruptPin_CW) == 1){ // reset
     reset2 = 0;
     digitalWrite(dirPin2,LOW); // MOVE TO THE LEFT SIDE. 
        if(vertical_step <= 0 ){
          break;
        }
        Serial.print("Vertical steps ******** = ");
        Serial.println(vertical_step);
        digitalWrite(stepPin2,HIGH); 
        delayMicroseconds(1500); 
        digitalWrite(stepPin2,LOW); 
        delayMicroseconds(1500); 
        //Serial.print("current steps = ");
        //Serial.println(vertical_step);
        vertical_step--;
  }
  resetEDPins();
  delay (1000);
}



/*-----------Function Calling-----------*/


void VMotor_CW1(long target_step) 
{
  digitalWrite(green, HIGH); 
  Serial.print("target step = ");
  Serial.println(target_step);
  digitalWrite(dirPin,LOW ); // MOVE TO THE LEFT SIDE.
  // Makes 200 pulses for making one full cycle rotation
  for (X = X; X <= target_step; X++) 
  {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(1); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(1); 
    Serial.print("current steps = ");
    Serial.println(X);
    
    if(digitalRead(interruptPin_CCW) == 1){
    reset2 = 1;
    reset();
    break;
   }
  }
  digitalWrite(green, LOW);
  if(reset2 != 1){
    vertical_motor();
  }
}

void reset() // to go to 
{
  if(reset2 != 1){
     reverse_vertical_motor();
  }
  digitalWrite(dirPin,HIGH); //MOVE TO THE RIGHT SIDE.
  // Makes 400 pulses for making two full cycle rotation
  digitalWrite(green, HIGH); 
  for (X = X-1; X >= 0; X--) 
  {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(1);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(1);
    Serial.println(X);
  }
  digitalWrite(green, LOW); 
  Led_internsity = 64;
  X = 0;
  
  ReverseStepDefault();
}

//small linear actuator
void resetEDPins()
{
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(EN, HIGH);
}

//Default microstep mode function
void StepForwardDefault()
{
 
  Serial.println("Moving forward at default step mode.");
  digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
  while(numberOfStep < 78000)  //Loop the forward stepping enough times for motion to be visible
  { 
    digitalWrite(blue, HIGH);
    digitalWrite(stp,HIGH); //Trigger one step forward
    delayMicroseconds(350);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delayMicroseconds(350);
   Serial.print("Invalid option entered: ");
   Serial.println(numberOfStep);
    numberOfStep++;
    if(digitalRead(interruptPin_CCW) == 1){
      reset3 = 1;
      ReverseStepDefault();
      break;
    }
  }
   digitalWrite(blue, LOW); 
}
//Reverse default microstep mode function
void ReverseStepDefault()
{
  Serial.println("Moving in reverse at default step mode.");
//Reset Easy Driver pins to default states
  digitalWrite(dir, HIGH); //Pull direction pin high to move in "reverse"
  while(numberOfStep > 0)//Loop the stepping enough times for motion to be visible
  {  
    digitalWrite(blue, HIGH); 
    digitalWrite(stp,HIGH); //Trigger one step
    delayMicroseconds(350);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delayMicroseconds(350);
    numberOfStep--;
  }
    digitalWrite(blue, LOW); 
}


void vertical_motor()
{
  
  Serial.print("Vertical step = ");
  Serial.println(vertical_step);
  digitalWrite(dirPin2,HIGH); // MOVE TO THE LEFT SIDE.
  // Makes 200 pulses for making one full cycle rotation
  for (int x = 0; x <= vertical_step; x++) 
  {
    digitalWrite(yellow, HIGH); 
    digitalWrite(stepPin2,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(stepPin2,LOW); 
    delayMicroseconds(500); 
    Serial.print("Vertical steps = ");
    Serial.println(x);
    if(digitalRead(interruptPin_CCW) == 1){
      vertical_step = x;
      reset();
      break;
    }
  }
  digitalWrite(yellow, LOW); 
}
void reverse_vertical_motor() 
{
  
  Serial.print("Vertical step = ");
  Serial.println(vertical_step);
  digitalWrite(dirPin2,LOW); // MOVE TO THE LEFT SIDE.
  // Makes 200 pulses for making one full cycle rotation
  for (int x = vertical_step; x > 0; x--) 
  {
    digitalWrite(yellow, HIGH); 
    digitalWrite(stepPin2,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(stepPin2,LOW); 
    delayMicroseconds(500); 
    Serial.print("Vertical steps = ");
    Serial.println(x);
    if(digitalRead(interruptPin_CCW) == 1){
      reset();
      break;
   }
  }
   digitalWrite(yellow, LOW); 
}
