/*
 * —————————————————————————— *
 *                   ---- A0     VIN --- 
 *                   ---- A1     VCC ---
 *                   ---- A2     GND ---
 *                   ---- A3     RES --- 
 *                   ---- A4     14 ----  
 *             Acc   ---- A5     13 ---- Buzzer
 *                   ---- A6     12 ---- PING_R_TRIG   green
 *                   ----  0     11 ---- PING_R_ECHO   yel
 *                   ----  1     10 ---- PING_L_TRIG   green
 *             Laser ---- ~2      9 ---- PING_L_ECHO   yel
 *    purple   LED_L ---- ~3      8 ---- BT_L
 *    blue     LED_B ---- ~4      7 ---- BT_R
 *    orange   LED_R ---- ~5      6 ---- BT_L
 *    
 * —————————————————————————— *      
 *    battery 4V
 *    laser 2.3V 
 * —————————————————————————— *
 */

const int LASER = 2, LED_L = 3, LED_B = 4, LED_R = 5;
const int BT_L = 6, BT_R = 7, BT_HEAD = 8;
const int PING_L_ECHO = 9, PING_L_TRIG = 10, PING_R_ECHO = 11, PING_R_TRIG = 12;
const int ACCEL = 3, BUZZ = 13;
const int DIS_THRES = 30;
int ACCEL_THRES = 2, CORRECTION_VAL = 0;

int timer = 0;
bool l_push_down = false, l_prev_down = false, r_push_down = false, r_prev_down = false;
int turning = 0;
int l_push_time = 0, r_push_time = 0;
int blink_rate = 500;
int distance = 0;

bool laser_on = false, hazard_on = false;

void setup() {
  Serial.begin(9600);
  pinMode(LASER, OUTPUT); pinMode(BUZZ, OUTPUT); pinMode(LED_L, OUTPUT); pinMode(LED_R, OUTPUT); pinMode(LED_B, OUTPUT);
  pinMode(BT_L, INPUT); pinMode(BT_L, INPUT); pinMode(BT_HEAD, INPUT);
  pinMode(PING_L_ECHO, INPUT); pinMode(PING_L_TRIG, OUTPUT); pinMode(PING_R_ECHO, INPUT); pinMode(PING_R_TRIG, OUTPUT);
  
  ACCEL_THRES += analogRead(ACCEL);
}


int get_distance(bool left){
  int trig, echo;
  if(left) {
    trig = PING_L_TRIG; echo = PING_L_ECHO;
  } else {
    trig = PING_R_TRIG; echo = PING_R_ECHO;
  }
  digitalWrite(trig, LOW);    delayMicroseconds(2);
  digitalWrite(trig, HIGH);   delayMicroseconds(10);
  digitalWrite(trig, LOW);
  int dis = pulseIn(echo, HIGH, 100000) / 58;
  if(left){
    // Serial.println(distance);
  }
  return dis;
}

void loop() {
  
  if(timer % 100 == 0){
    distance = get_distance(true);
    int r_dis = get_distance(false);
    if(r_dis && r_dis < distance) distance = r_dis;
  }
  if(distance && distance < DIS_THRES) {
    tone(BUZZ, 2000, 1);
  }
  
  if(timer % 100 == 0){
    int acc = analogRead(3);
    Serial.println(acc - ACCEL_THRES);
    if(acc - ACCEL_THRES > 20){
      digitalWrite(LED_B, HIGH);
    }else{
      digitalWrite(LED_B, LOW);
    }
  }

  
  l_push_down = digitalRead(BT_L); r_push_down = digitalRead(BT_R);
  if(l_push_down && !l_prev_down){
    l_push_time = 0;
  }else if(l_push_down && l_prev_down){
    l_push_time ++;
  }
  if(r_push_down && !r_prev_down){
    r_push_time = 0;
  }else if(r_push_down && r_prev_down){
    r_push_time ++;
  }
  if(!l_push_down && !r_push_down && l_push_time > 500 && r_push_time > 500) {
    hazard_on = !hazard_on;
    if(hazard_on){
      digitalWrite(LED_L, HIGH); digitalWrite(LED_R, HIGH); digitalWrite(LED_B, HIGH);
    }
    Serial.println(hazard_on);
    l_push_time = 0;
    r_push_time = 0;
  }
  if(!l_push_down && l_push_time > 0 && l_push_time < 200){
    if(turning != -1){
      turning = -1;
    } else {
      turning = 0;
    }
    l_push_time = 0;
  } else if(!r_push_down && r_push_time > 0 && r_push_time < 200) {
    if(turning != 1){
      turning = 1;
    } else {
      turning = 0;
    }
    r_push_time = 0;
  }
  
  l_prev_down = l_push_down; r_prev_down = r_push_down;
  if(!hazard_on && turning == 1){
    if(timer < blink_rate / 2){
      digitalWrite(LED_L, HIGH);
    } else {
      digitalWrite(LED_L, LOW);
    }
    digitalWrite(LED_R, LOW);
  } else if(!hazard_on && turning == -1){
    if(timer < blink_rate / 2){
      digitalWrite(LED_R, HIGH);
    } else {
      digitalWrite(LED_R, LOW);
    }
    digitalWrite(LED_L, LOW);
  } else if(!hazard_on){
    digitalWrite(LED_L, LOW);
    digitalWrite(LED_R, LOW);
  }
  timer ++;
  if(timer > blink_rate){
    timer = 0;
  }
  analogWrite(LASER, 20);
  // Serial.println(turning);
  delay(1);
}
