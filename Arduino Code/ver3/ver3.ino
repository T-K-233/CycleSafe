/*
 * —————————————————————————— *
 *                   ---- A0     VIN --- 
 *                   ---- A1     VCC ---
 *                   ---- A2     GND ---
 *                   ---- A3     RES --- 
 *                   ---- A4     14 ---- 
 *                   ---- A5     13 ---- 
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

const bool LEFT = false, RIGHT = true;

int turning_l = false, turning_r = false;
int timer = 0, l_bt_timer = 0, r_bt_timer = 0, head_bt_timer = 0;
bool head_bt_released = false;

void setup() {
  pinMode(LASER, OUTPUT); pinMode(LED_L, OUTPUT); pinMode(LED_R, OUTPUT); pinMode(LED_B, OUTPUT);
  pinMode(BT_L, INPUT); pinMode(BT_L, INPUT); pinMode(BT_HEAD, INPUT);
  pinMode(PING_L_ECHO, INPUT); pinMode(PING_L_TRIG, OUTPUT); pinMode(PING_R_ECHO, INPUT); pinMode(PING_R_TRIG, OUTPUT);
}

void loop() {
  if(timer % 100 == 0){
      int trig, echo;
      trig = PING_L_TRIG; echo = PING_L_ECHO;
      Serial.print("Distance @ left: ");
      digitalWrite(trig, LOW);    delayMicroseconds(2);
      digitalWrite(trig, HIGH);   delayMicroseconds(10);
      digitalWrite(trig, LOW);
      int distance = pulseIn(echo, HIGH) / 58;
      Serial.println(distance);
      trig = PING_R_TRIG; echo = PING_R_ECHO;
      Serial.print("Distance @ right: ");
      digitalWrite(trig, LOW);    delayMicroseconds(2);
      digitalWrite(trig, HIGH);   delayMicroseconds(10);
      digitalWrite(trig, LOW);
      distance = pulseIn(echo, HIGH) / 58;
      Serial.println(distance);

  }
  if(digitalRead(BT_L)){
    l_bt_timer ++;
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
  
  if(l_bt_timer > 100 && r_bt_timer == 0){
    turning_l = !turning_l;
    turning_r = false;
    Serial.println("button pressed");
    l_bt_timer = 0;
  }
  if(r_bt_timer > 100 && l_bt_timer == 0){
    turning_r = !turning_r;
    turning_l = false;
    r_bt_timer = 0;
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
    if(timer < 500){
      digitalWrite(LED_L, HIGH);
    } else {
      digitalWrite(LED_L, LOW);
    }
  }else{
    digitalWrite(LED_L, LOW);
  }
  if(turning_r){
    if(timer < 500){
      digitalWrite(LED_R, HIGH);
    } else {
      digitalWrite(LED_R, LOW);
    }
  }else{
    digitalWrite(LED_R, LOW);
  }
  timer ++;
  if(timer > 1000){
    timer = 0;
  }
  analogWrite(LASER, 20);
  delay(1);
}
