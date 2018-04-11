#include <Stepper.h>

#define LIGHT_SENSOR A0                 //Grove - Light Sensor is connected to A0 of Arduino
#define MANUAL_PIN 2
#define AUTO_MODE_PIN 3
#define LED_PIN 12           //Connect the LED Grove module to Pin12, Digital 12
#define STEPS 2038

Stepper stepper(STEPS,8,10,9,11);  
const int thresholdvalue = 120;         //The treshold for which the LED should turn on. Setting it lower will make it go on at more light, higher for more darkness

// states variables that will be used in the program
int numberOfStep = 0;

volatile int mode = 0; // mode 0 -> light sensor // mode 1 -> manual button
volatile int windowState = 1; // window state 0 -> closed // window state 1 -> open
volatile bool toggle = true; // true -> to close // false -> to open 

void setup() 
{
    Serial.begin(9600);                //Start the Serial connection
    pinMode(LED_PIN,OUTPUT);            //Set the LED on Digital 12 as an OUTPUT
    pinMode(MANUAL_PIN, INPUT);  // initialize the pushbutton pin as an input
    pinMode(AUTO_MODE_PIN, INPUT); // initialize the pushbutton pin as an input
    attachInterrupt(digitalPinToInterrupt(MANUAL_PIN), manualButtonController, RISING);
    attachInterrupt(digitalPinToInterrupt(AUTO_MODE_PIN), lightSensorMode, RISING);
    stepper.setSpeed(15);
}

void loop() 
{
    
//    buttonValueOfMannaul = digitalRead(MANUAL_PIN);
//    buttonValueOfAuto = digitalRead(auto_button);
    Serial.print("Mode: ");
    Serial.print(mode);  
    Serial.println();
    Serial.println(analogRead(LIGHT_SENSOR));
    /* Mannaul Button is pressed, meaaning toggle between open and close */
   
    if( mode == 1){ // mode = 1 -> using pushbutton
      if(mode == 1 && toggle == false && windowState == 0){ // push to open window
      Serial.println("open!!!!!!!!!!!!!!!!!!");
      open_shade();
    }
      if(mode == 1 && toggle == true && windowState == 1){ // push to close window
        Serial.println("close-----------------");
        close_shade();
      }
    }
    else{ // mode = 0 -> using light sensor
      if(mode == 0 && analogRead(LIGHT_SENSOR) < thresholdvalue && windowState == 0 ){ // no light -> open window
      Serial.println("open!!!!!!!!!!!!!!!!!!");
      open_shade();
      }
      if(mode == 0 && analogRead(LIGHT_SENSOR) >= thresholdvalue && windowState == 1){ // light -> close window
      Serial.println("close-----------------");
      close_shade();
      }
    }
}

void lightSensorMode(){
  mode = 0;
}
void manualButtonController(){
   mode = 1; // change to manual mode
   toggle = !toggle;
}

void open_shade(){
    digitalWrite(LED_PIN,HIGH);
    while(numberOfStep < 3000){
        stepper.step(1);
        numberOfStep++;       
        if( (mode == 1 && toggle == true) || (mode == 0 && analogRead(LIGHT_SENSOR) > thresholdvalue) ){
           break;
        }
    }
    digitalWrite(LED_PIN,LOW);
    windowState = 1; // 
}

void close_shade(){
    digitalWrite(LED_PIN,HIGH);
    while(numberOfStep > 0){
        stepper.step(-1);
        numberOfStep--;
         if( (mode == 1 && toggle == false) || (mode == 0 && analogRead(LIGHT_SENSOR) < thresholdvalue)){
           break;
        }
    }
    digitalWrite(LED_PIN,LOW);
    windowState = 0;
}

