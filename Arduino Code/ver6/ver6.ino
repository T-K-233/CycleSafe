class CycleSafeBoard{
  /*
   * —————————————————————————— *
   *              Acc     ---- A0     VIN --- 
   *                   ---- A1     VCC ---
   *                   ---- A2     GND ---
   *                   ---- A3     RES --- 
   *                   ---- A4     14 ----  
   *                ---- A5     13 ---- Buzzer
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
  public:
    static const int DIS_THRES = 60, MIC_THRES = 90, ACCEL_THRES = 24, BLINK_RATE = 250, LASER_BRIGHTNESS = 200;
  
    static const int LASER = 2, LED_L = 3, LED_B = 4, LED_R = 5;
    static const int MIC = 4, ACCEL = 5, BUZZ = 13;
    static const int BT_L = 6, BT_R = 7, BT_HEAD = 8;
    static const int PING_L_ECHO = 9, PING_L_TRIG = 10, PING_R_ECHO = 11, PING_R_TRIG = 12;
    int MIC_REF = 0, ACCEL_REF = 0;
    int timer = 0;
    bool l_push_down = false, l_prev_down = false, r_push_down = false, r_prev_down = false;
    int turning = 0;
    int l_push_time = 0, r_push_time = 0, break_timer = 0;
    int distance = 0;
    int acc = 0;
    bool laser_on = false, hazard_on = false;

    void set_pin_mode(){
      pinMode(LASER, OUTPUT); pinMode(BUZZ, OUTPUT); pinMode(LED_L, OUTPUT); pinMode(LED_R, OUTPUT); pinMode(LED_B, OUTPUT);
      pinMode(BT_L, INPUT); pinMode(BT_L, INPUT); pinMode(BT_HEAD, INPUT);
      pinMode(PING_L_ECHO, INPUT); pinMode(PING_L_TRIG, OUTPUT); pinMode(PING_R_ECHO, INPUT); pinMode(PING_R_TRIG, OUTPUT);
    }
    
    void calibrate(){
      delay(1000);
      int mic_ = 0, acc_ = 0;
      for(int i=0; i<100; i++){
        mic_ += analogRead(MIC);
        acc_ += analogRead(ACCEL);
        delay(10);
      }
      mic_ /= 100;
      acc_ /= 100;
      MIC_REF = mic_;
      ACCEL_REF = acc_;
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
        Serial.println(distance);
      }
      return dis;
    }

    void run(){
      if(timer % 100 == 0){
        distance = get_distance(true);
        int r_dis = get_distance(false);
        if(r_dis && r_dis < distance) distance = r_dis;
      }
      if(distance && distance < DIS_THRES){tone(BUZZ, 2000, 1);}
      
      if(timer % 100 == 0){
        acc = analogRead(5);
        if(-(acc - ACCEL_REF) > ACCEL_THRES){
          break_timer = 500;
        }
        
      }
      if(break_timer > 0) {
        digitalWrite(LED_B, HIGH);
        break_timer --;
      } else {
        digitalWrite(LED_B, LOW);
      }
    
      l_push_down = digitalRead(BT_L); r_push_down = digitalRead(BT_R);
      if(l_push_down && !l_prev_down) l_push_time = 0;
      else if(l_push_down && l_prev_down) l_push_time ++;
      
      if(r_push_down && !r_prev_down) r_push_time = 0; 
      else if(r_push_down && r_prev_down) r_push_time ++;
      
      if(!l_push_down && !r_push_down && l_push_time > 500 && r_push_time > 500) {
        hazard_on = !hazard_on;
        if(hazard_on) digitalWrite(LED_L, HIGH); digitalWrite(LED_R, HIGH); digitalWrite(LED_B, HIGH);
        l_push_time = 0; r_push_time = 0;
      }
      
      if(!l_push_down && l_push_time > 0 && l_push_time < 200){
        if(turning != -1) turning = -1;
        else turning = 0;
        l_push_time = 0;
      } else if(!r_push_down && r_push_time > 0 && r_push_time < 200) {
        if(turning != 1) turning = 1; 
        else turning = 0;
        r_push_time = 0;
      }
      
      l_prev_down = l_push_down; r_prev_down = r_push_down;
      if(!hazard_on && turning == 1){
        digitalWrite(LED_L, timer < BLINK_RATE / 2);
        digitalWrite(LED_R, LOW);
      } else if(!hazard_on && turning == -1){
        digitalWrite(LED_R, timer < BLINK_RATE / 2);
        digitalWrite(LED_L, LOW);
      } else if(!hazard_on){
        digitalWrite(LED_L, LOW);
        digitalWrite(LED_R, LOW);
      }
      
      timer ++;
      if(timer > BLINK_RATE) timer = 0;

      analogWrite(LASER, LASER_BRIGHTNESS);
      delay(1);
    }

    void log(){
      // Serial.println(turning);
      // Serial.println(hazard_on);
      // Serial.println(acc - ACCEL_REF);
    }
};

CycleSafeBoard board;

void setup() {
  Serial.begin(9600);
  board = CycleSafeBoard();
  board.set_pin_mode();
  board.calibrate();
}

void loop() {
  board.run();
  board.log();
}
