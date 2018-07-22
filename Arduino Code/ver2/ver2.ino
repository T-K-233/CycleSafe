/*
 * —————————————————————————— *
 *                   ---- A0     VIN --- 
 *                   ---- A1     VCC ---
 *                   ---- A2     GND ---
 *                   ---- A3     RES --- 
 *                   ---- A4     14 ---- 
 *                   ---- A5     13 ---- 
 *                   ---- A6     12 ---- PING_L_ECHO   green
 *                   ----  0     11 ---- PING_L_TRIG   yel
 *                   ----  1     10 ---- PING_R_ECHO   green
 *             Laser ---- ~2      9 ---- PING_R_TRIG   yel
 *    purple   LED_L ---- ~3      8 ---- BT_L
 *    blue     LED_R ---- ~4      7 ---- BT_R
 *    orange   LED_B ---- ~5      6 ---- BT_HEAD
 *    
 * —————————————————————————— *      
 *    battery 4V
 *    laser 2.3V 
 * —————————————————————————— *
 */

const int LASER = 2, LED_L = 3, LED_R = 4, LED_B = 5;
const int BT_L = 8, BT_R = 7, BT_HEAD = 6;
const int PING_L_ECHO = 12, PING_L_TRIG = 11, PING_R_ECHO = 10, PING_R_TRIG = 9;

const bool LEFT = false, RIGHT = true;

class TempName{
  public:
    bool rval = false, rprev_val = false, rled_on = false;
    bool lval = false, lprev_val = false, lled_on = false;
    int timer = 0;

    int ping(bool side){
      int trig, echo;
      if(side == LEFT) {
        trig = PING_L_TRIG; echo = PING_L_ECHO;
        Serial.print("Distance @ left: ");
      } else {
        trig = PING_R_TRIG; echo = PING_R_ECHO;
        Serial.print("Distance @ right: ");
      }
      digitalWrite(trig, LOW);    delayMicroseconds(2);
      digitalWrite(trig, HIGH);   delayMicroseconds(10);
      digitalWrite(trig, LOW);
      int distance = pulseIn(echo, HIGH) / 58;
      Serial.println(distance);
      return distance;
    }

    void emergency(){

    }

    void led(){
      rval = digitalRead(BT_R);
      lval = digitalRead(BT_L);

      
      if(rval && !rprev_val) {
        rled_on = !rled_on;
        lled_on = false;
        delay(10);
      }
      if(!rval && rprev_val) {
        delay(10);
      }
      rprev_val = rval;
      if(rled_on == true) {
        if(timer < 500) digitalWrite(LED_R, true);
        else digitalWrite(LED_R, false);
      } else {
        digitalWrite(LED_R, false);
      }
      
      if(lval && !lprev_val) {
        lled_on = !lled_on;
        rled_on = false;
        delay(10);
      }
      if(!lval && lprev_val) {
        delay(10);
      }
      lprev_val = lval;
      if(lled_on == true){
        if(timer<50) digitalWrite(LED_L, true);
        else digitalWrite(LED_L, false);
      } else {
        digitalWrite(LED_L, false);
      }
      timer ++;
      if(timer > 100) timer = 0;
    }
};

TempName board;
void setup(){
  Serial.begin(9600);
  pinMode(PING_L_TRIG, OUTPUT);
  pinMode(PING_L_ECHO, INPUT);
  pinMode(PING_R_TRIG, OUTPUT);
  pinMode(PING_R_ECHO, INPUT);
  pinMode(LED_L, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(BT_L, INPUT);
  pinMode(BT_R, INPUT);
}

void loop(){
  //board.ping(LEFT);
  //board.ping(RIGHT);
  board.led();
  analogWrite(LASER, 0);
  delay(1);
}

