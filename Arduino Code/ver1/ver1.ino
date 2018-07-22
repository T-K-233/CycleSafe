//right turn signal variables
const int rightSignal = 5;
const int rightPushButton = 6;
int rightState = LOW;
int rightReading;
int rightPrevious = LOW;

//left turn signal variables
const int leftSignal = 4;
const int leftPushButton = 7;
int leftState = LOW;
int leftReading;
int leftPrevious = LOW;


void setup() {
  //right turn signal pin setups
  pinMode(rightSignal, OUTPUT);
  pinMode(rightPushButton, INPUT);

  //left turn signal pin setups
  pinMode(leftSignal, OUTPUT);
  pinMode(leftPushButton, INPUT);

}

void loop() {
  rightReading = digitalRead(rightPushButton);
  leftReading = digitalRead(leftPushButton);
  
  if(rightReading == HIGH  && rightPrevious == LOW) //Button pressed 
  {  
    if(rightState == HIGH)
    {  //change to opposite
      rightState = LOW;
    }
    else
    {
      rightState = HIGH;
    }


    rightPrevious = rightReading;
  }
    digitalWrite(rightSignal, rightState);  //blink right turn signal
    delay(250);
    digitalWrite(rightSignal, LOW);
    delay(250);

  if(leftReading == HIGH && leftPrevious == LOW){
    if(leftState == HIGH){
      leftState = LOW;
    }
    else{
      leftState = HIGH;
    }
  

    leftPrevious = leftReading;
  }
    digitalWrite(leftSignal, leftState);
    delay(250);
    digitalWrite(leftSignal, LOW);
    delay(250);

}
/*
for(i=1;i<250,i++)
{
  
  delay(1);
}
}
*/

int i=0;
void loop(){
  if(i > 500)i=0;
  
  if(rightReading == HIGH  && rightPrevious == LOW) //Button pressed 
  {  
    if(rightState == HIGH)
    {  //change to opposite
      rightState = LOW;
    }
    else
    {
      rightState = HIGH;
    }


    rightPrevious = rightReading;
  }

  if(i<250 && is_on){
    digitalWrite(rightSignal, HIGH);
  }else{
    digitalWrite(rightSignal, LOW);
  }
  delay(1);
}



