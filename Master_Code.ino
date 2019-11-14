//Ping Sensor Set up start
const int trigPin = 35;
const int echoPin = 36;
int ledPins[] = {30,31,32,33,34};
const int SIZE = 5;
long duration;
int distance;
//ping Senor set up end

// Neopixel Setup start
#include <Adafruit_NeoPixel.h>
#define PIN 7
#define NUMPIXELS 10 // Popular NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500
// Neopixel Setup end

//LCD Setup Start
#include <LiquidCrystal.h>
const int rs = 13, en = 12, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//LCD Setup End


//Temperature Sensor Setup Start
int sensorPin = A1;
//Temperature Sensor Setup End

//KEYPAD setup start
#include <Keypad.h>
const byte ROWS = 4; 
const byte COLS = 4; 
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {29,28,27,26}; 
byte colPins[COLS] = {25,24,23,22}; 
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
//Keypad setup end

//Start STEPPER MOTOR setup
#include <Stepper.h>
#define STEPS 200
Stepper stepper(STEPS, 8, 9, 10, 11);
int previous = 0;
//End Stepper setup


//Start MOTION SENSOR setup
//int ledPin = 7;                // LED 
int pirPin = 37;                 // PIR Out pin 
int pirStat = 0;                   // PIR status
//End MOTION SENSOR setup


void setup(){
 int index;
  for(index = 0; index < SIZE; index++)
    {
      pinMode(ledPins[index],OUTPUT);
    }

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication

  pixels.begin();
  pixels.show();

  stepper.setSpeed(30);

   // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  // Print a message to the LCD.
  //lcd.print("hello, world!");
  Serial.begin(9600);
  
  //Motion Sensor
  //pinMode(ledPin, OUTPUT);     
  pinMode(pirPin, INPUT); 
  
}



//This function takes in a value in between 0 and (# of LEDS - 1)
//which will set how many LEDs will be on or off
//LEDs to the left of 'n' will be on 
//LEDs to the right of 'n' will be off
void setLeds(int n)
{
  int i;
  for(i = 0; i < SIZE; i++)
  {
    if ( i <= n)
      digitalWrite(ledPins[i], HIGH);
    else
      digitalWrite(ledPins[i], LOW);
  }

}



  
void loop(){
  
  pirStat = digitalRead(pirPin); 
  Serial.print(pirStat);
   
  if (pirStat == HIGH) {            // if motion detected
    uint32_t low = pixels.Color(0, 0, 0);
    uint32_t high = pixels.Color(30, 60, 255);
    // Turn them off
    for( int i = 0; i<NUMPIXELS; i++){
        pixels.setPixelColor(i, high);
        pixels.show();
        delay(100);
        pixels.setPixelColor(i, low);
        pixels.show();
    }

  } 
  else {
    uint32_t low = pixels.Color(0, 0, 0);
    uint32_t high = pixels.Color(30, 60, 255);
    for( int i = 0; i<NUMPIXELS; i++){
        pixels.setPixelColor(i, low);
        pixels.show();
    }   
  }
  //End Motion Sensor
  
  char key = customKeypad.getKey(); //read key from keypad
  if (key != NO_KEY){
    
    Serial.println(key);
  }
 while (key == '1'){ //Ping Sensor
  char newkey = customKeypad.getKey();
  digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH); // Sets the trigPin on HIGH state for 10 micro seconds
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
    distance = (duration * 0.034 / 2) / 2.54; // Calculating the distance

    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("Ping Sensor");
    lcd.setCursor(0,1);
    lcd.print("Distance: ");
    lcd.setCursor(6,2);
    lcd.print(distance);
    lcd.print(" inches");
    
  
    if (distance >= 0 && distance < 3)
      setLeds(0);
    else if (distance >= 3 && distance < 6){
      setLeds(1);
    }
    else if (distance >= 6 && distance < 9){
      setLeds(2);
    } 
    else if (distance >= 9 && distance < 12){
      setLeds(3);
    } 
    else if (distance >= 12){
      setLeds(4);
    }
    delay(100);
    // Prints the distance on the Serial Monitor
    //Serial.print("Distance: ");
    //Serial.println(distance);
    if(newkey == '*'){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Please pick a sensor");
      lcd.setCursor(6,1);
      lcd.print("by using");
      lcd.setCursor(3,2);
      lcd.print("the number pad");
      int i;
      for(i = 0; i < SIZE; i++)
    {
        digitalWrite(ledPins[i], LOW);
    }
        break;}
  }

  
while (key == '2'){ //Temp Sensor
      lcd.clear();
      int reading = analogRead(sensorPin);  
      float voltage = reading * 2.4;
      voltage /= 1024.0; 
      float temperatureF = (voltage - 0.09) * 100 ; 
      float temperatureC = (temperatureF - 32) * 5/9 ;
      float temperatureK = (temperatureC + 273.15) ;
     
      lcd.setCursor(1,0);
      lcd.print("Temperature Sensor");
      lcd.setCursor(6,2);
      lcd.print(temperatureF);
      lcd.print(" ");
      lcd.print((char)223);
      lcd.print("F");
      lcd.setCursor(6,1);
      lcd.print(temperatureC);
      lcd.print(" ");
      lcd.print((char)223);
      lcd.print("C");
      lcd.setCursor(5,3);
      lcd.print(temperatureK);
      //lcd.print((char)223);
      lcd.print("  K");
      
      delay(250);
    char newkey = customKeypad.getKey();
    if(newkey == '*'){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Please pick a sensor");
      lcd.setCursor(6,1);
      lcd.print("by using");
      lcd.setCursor(3,2);
      lcd.print("the number pad");
      
    break;}
}


  


while (key == '3'){ //Stepper Motor
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Stepper Motor");
    delay(250);
   // float rad;
    int val = analogRead(0);
    stepper.step(val - previous);
    previous = val;
    val = map(val, 0, 1023, 0, 1800);
    //rad = val * 3.141592;
     
     /* lcd.setCursor(0,0);
      lcd.print(val);
      lcd.print((char)223);
      lcd.setCursor(1,0);
      lcd.print(rad);
      lcd.print(" rad");
      delay(100);
*/
    
    char newkey = customKeypad.getKey();
    if(newkey == '*'){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Please pick a sensor");
      lcd.setCursor(6,1);
      lcd.print("by using");
      lcd.setCursor(3,2);
      lcd.print("the number pad");
    break;}
}


while (key == '4'){ //Neopixels
    uint32_t low = pixels.Color(0, 0, 0);
    uint32_t high = pixels.Color(30, 60, 255);
    // Turn them off
    for( int i = 0; i<NUMPIXELS; i++){
        pixels.setPixelColor(i, high);
        pixels.show();
    }
    char newkey = customKeypad.getKey();
    if(newkey == '*'){
    for( int i = 0; i<NUMPIXELS; i++){
        pixels.setPixelColor(i,low);
        pixels.show();
        continue;
    }
    break;}
}



while (key == '5'){ //Sensor 5

    char newkey = customKeypad.getKey();
    if(newkey == '*'){
    break;}
}
}
