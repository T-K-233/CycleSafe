  /*
   * —————————————————————————— *
   *   accel_x         ---- A0     VIN --- 
   *   accel_z         ---- A1     VCC ---
   *                   ---- A2     GND ---
   *                   ---- A3     RES --- 
   *                   ---- A4     14 ----  
   *                   ---- A5     13 ---- Buzzer
   *                   ---- A6     12 ---- PING_R_TRIG   green
   *                   ----  0     11 ---- PING_R_ECHO   yel
   *                   ----  1     10 ---- PING_L_TRIG   green
   *             Laser ---- ~2      9 ---- PING_L_ECHO   yel
   *    purple   LED_L ---- ~3      8 ---- BT_L
   *    blue     LED_B ---- ~4      7 ---- BT_R
   *    orange   LED_R ---- ~5      6 ---- BT_HEAD
   *    
   * —————————————————————————— *      
   *    battery 4V
   *    laser 2.3V 
   * —————————————————————————— *
   */

const int LASER = 2, LED_L = 3, LED_R = 4, LED_B = 5;
const int BT_L = 8, BT_R = 7, BT_HEAD = 6;
const int PING_L_ECHO = 9, PING_L_TRIG = 10, PING_R_ECHO = 11, PING_R_TRIG = 12;
const int ACCEL = 3, ACCEL_THRES=30, CORRECTION_VAL = 0;
const int buzzPin=13, distanceThres=10;

const bool LEFT = false, RIGHT = true;

int turning_time = 500;

int turning_l = false, turning_r = false;
int timer = 0, l_bt_timer = 0, r_bt_timer = 0, head_bt_timer = 0;
bool head_bt_released = false;
int accel;
int Ldistance,Rdistance;
void setup() {
  pinMode(LASER, OUTPUT); pinMode(LED_L, OUTPUT); pinMode(LED_R, OUTPUT); pinMode(LED_B, OUTPUT);
  pinMode(BT_L, INPUT); pinMode(BT_L, INPUT); pinMode(BT_HEAD, INPUT);
  pinMode(PING_L_ECHO, INPUT); pinMode(PING_L_TRIG, OUTPUT); pinMode(PING_R_ECHO, INPUT); pinMode(PING_R_TRIG, OUTPUT);
}

void loop() {
  //Proximity alarm
  if(timer % 100 == 0){
      int trig, echo;
      trig = PING_L_TRIG; echo = PING_L_ECHO;
      // Serial.print("Distance @ left: ");
      digitalWrite(trig, LOW);    delayMicroseconds(2);
      digitalWrite(trig, HIGH);   delayMicroseconds(10);
      digitalWrite(trig, LOW);
      Ldistance = pulseIn(echo, HIGH) / 58;
      // Serial.println(Ldistance);
      trig = PING_R_TRIG; echo = PING_R_ECHO;
      // Serial.print("Distance @ right: ");
      digitalWrite(trig, LOW);    delayMicroseconds(2);
      digitalWrite(trig, HIGH);   delayMicroseconds(10);
      digitalWrite(trig, LOW);
      Rdistance = pulseIn(echo, HIGH) / 58;
      // Serial.println(Rdistance);

  }

  if(Rdistance<distanceThres || Ldistance <distanceThres) {
    tone(buzzPin, 2000, 1);
  }

  //Turn Signals
  if(digitalRead(BT_L)){
    l_bt_timer ++;
    Serial.println("button pressed");
  } else {
    l_bt_timer = 0;
  }
  
  if(digitalRead(BT_R)){
    r_bt_timer ++;
  } else {
    r_bt_timer = 0;
  }
  if(digitalRead(BT_HEAD)){
    head_bt_timer ++;
    head_bt_released = false;
  } else {
    head_bt_released = true;
  }
  
  if(l_bt_timer > 200 && r_bt_timer == 0){
    turning_l = !turning_l;
    turning_r = false;
    l_bt_timer = 0;
  }
  if(r_bt_timer > 200 && l_bt_timer == 0){
    turning_r = !turning_r;
    turning_l = false;
    r_bt_timer = 0;
    Serial.println("Turning right");
  }
  if(l_bt_timer > 1000 && r_bt_timer > 1000){
    // TODO: emergency
  }
  
  if(head_bt_released) {
    if(head_bt_timer > 1000) {
      // TODO: toggle laser
    } else {
      // TODO: toggle headlight
    }
    head_bt_timer = 0;
  }
  if(turning_l){
    if(timer < turning_time / 2){
      analogWrite(LED_L, 150);
      Serial.println("tuning time");
      Serial.println(turning_time / 2);
    } else {
      digitalWrite(LED_L, LOW);
    }
  }else{
    digitalWrite(LED_L, LOW);
  }
  if(turning_r){
    if(timer < turning_time / 2){
      analogWrite(LED_R, 150);
    } else {
      digitalWrite(LED_R, LOW);
    }
  }else{
    digitalWrite(LED_R, LOW);
  }
  timer ++;
  if(timer > turning_time){
    timer = 0;
  }
  analogWrite(LASER, 20);
  delay(1);

  //Brake Lights
  if(timer % 250==0)
  {
    accel = analogRead(ACCEL) - CORRECTION_VAL;
    Serial.println(accel);
    if(accel>ACCEL_THRES)
    {
      digitalWrite(LED_B,HIGH);
    }else{
      digitalWrite(LED_B, LOW);
    }
  }
}
