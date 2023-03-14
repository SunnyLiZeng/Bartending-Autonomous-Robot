#include <SD.h>

#define ledPin 13

#include <SoftwareSerial.h>

SoftwareSerial BTSerial(A2, A3); // RX | TX

int pump_one = 5;
int pump_two = 6;
int pump_three = 7;
int pump_four = 8;



void setup() {
  // put your setup code here, to run once:
  pinMode(pump_one, OUTPUT);
  pinMode(pump_two, OUTPUT);
  pinMode(pump_three, OUTPUT);
  pinMode(pump_four, OUTPUT);
  pinMode(ledPin, OUTPUT);

   Serial.begin(9600); // start serial communication at 9600 baud
  BTSerial.begin(9600); // start Bluetooth communication at 9600 baud
}

void loop() {
   // Checks if the data is coming from the serial port
   
if (BTSerial.available()) { // check if there is data coming from the Bluetooth module
    String data = BTSerial.readStringUntil(']'); // read the incoming data until ']' is received
    data = data.substring(1); // remove the first '[' character from the string
    data.trim(); // remove any leading/trailing whitespace
    if (data.length() > 0) { // check if there is any data left after removing the '[' character
      String numbers[5];
      int i = 0;
      while (data.indexOf(',') != -1 && i < 5) { // loop until there are no more commas in the string or the numbers array is full
        numbers[i] = data.substring(0, data.indexOf(',')); // extract the number before the comma
        data = data.substring(data.indexOf(',') + 1); // remove the extracted number and the comma from the string
        numbers[i].trim(); // remove any leading/trailing whitespace from the extracted number
        i++; // move to the next element in the array
      }
      numbers[i] = data; // extract the last number in the string (after the last comma)
      numbers[i].trim(); // remove any leading/trailing whitespace from the extracted number
      /*
      for (int j = 0; j < 5; j++) {
        // Serial.print("Number ");
        // Serial.print(j + 1);
        // Serial.print(": ");
        Serial.println(numbers[j]); // print each number in the array to the serial monitor
      }
      */
      
      double one = numbers[0].toDouble();
      double two = numbers[1].toDouble();
      double three = numbers[2].toDouble();
      double four = numbers[3].toDouble();
      
//Serial.println(one);
//Serial.println(two);
//Serial.println(three);
//Serial.println(four);

      Serial.println("20,20,0");
      delay(10000);
      Serial.println("0,0,190");
      delay(10000);



      
      if(one > 0){
        Serial.println("15,3,210");
        delay(5000);
        Serial.println("27,3,210");
        delay(5000);

        digitalWrite(pump_one, HIGH);
        delay(one * 60);
        digitalWrite(pump_one, LOW);
        
        Serial.println("15,3,210");
        delay(5000);
        Serial.println("0,0,190");
        delay(5000);
      }

      if(two > 0){
        Serial.println("20,5,190");
        delay(5000);
        Serial.println("31,5,190");
        delay(5000);
        
        digitalWrite(pump_two, HIGH);
        delay(two * 60 );
        digitalWrite(pump_two, LOW);

        Serial.println("20,5,190");
        delay(5000);
        Serial.println("0,0,190");
        delay(5000);
      }

      if(three > 0){
        Serial.println("20,5,175");
        delay(5000);
        Serial.println("36,5,175");
        delay(5000);
        
        digitalWrite(pump_three, HIGH);
        delay(three * 60 );
        digitalWrite(pump_three, LOW);

        if(four > 0){
          
        }
        else{
          Serial.println("20,5,175");
          delay(5000);
          Serial.println("0,0,190");
          delay(5000);
        }
      }
      
      if(four > 0){
        /*
        Serial.println("20,5,170");
        delay(5000);
        Serial.println("40,5,165");
        delay(5000);
        */
        if(three > 0){
          digitalWrite(pump_four, HIGH);
          delay(four * 60 );
          digitalWrite(pump_four, LOW);
        
          Serial.println("20,5,175");
          delay(5000);
          Serial.println("0,0,190");
          delay(5000);
        }
        else{
          Serial.println("20,5,175");
          delay(5000);
          Serial.println("36,5,175");
          delay(5000);

          digitalWrite(pump_four, HIGH);
          delay(four * 60 );
          digitalWrite(pump_four, LOW);
        
          Serial.println("20,5,175");
          delay(5000);
          Serial.println("0,0,190");
          delay(5000);
        }
        /*
        Serial.println("20,5,170");
        delay(5000);
        Serial.println("0,0,190");
        delay(5000);
        */
      }

      Serial.println("20,20,0");
      
    }
}


}
