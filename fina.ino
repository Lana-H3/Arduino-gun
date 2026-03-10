#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
int shot = 0; //how many bullets shot

const int poten = A3; 

const int button = 2;

const int x_axis = A0;
const int y_axis = A1;

const int in1 = 8;
const int in2 = 9;

bool lastTriggerState = false;
bool lastReloadState = false;
bool recoilActive = false;
unsigned long recoilStartTime = 0;
int recoilDuration = 50;

unsigned long lastShotTime = 0;
int fireDelay = 300;

void setup() {
  Serial.begin(9600);
  pinMode(button, INPUT);

  lcd.init();
  lcd.backlight();
  updateLCD();

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
}

void loop() {
  //poten == SHOOT
  bool triggerState = analogRead(poten) > 1000;

  unsigned long currentTime = millis();

  if (triggerState && currentTime - lastShotTime >= fireDelay){
    shot ++;
    Serial.println("SHOOT");
    lastShotTime = currentTime;

    startRecoil();
    updateLCD();
  }

  //DC == recoil
  if (recoilActive && currentTime - recoilStartTime >= recoilDuration) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  recoilActive = false;
  }

  //button == RELOAD
  bool reloadState = digitalRead(button);

  if (reloadState && !lastReloadState) {
    shot = 0;
    Serial.println("RELOAD");
    updateLCD();
    delay(300);
  }
  lastReloadState = reloadState;

  //joystuck == MOVE
  int x = analogRead(x_axis);
  int y = analogRead(y_axis);

  int moveX = map(x, 0, 1023, -10, 10);
  int moveY = map(y, 0, 1023, -10, 10);

  if (abs(moveX) > 2 || abs(moveY) > 2) {
    Serial.print("MOVE ");
    Serial.print(moveX);
    Serial.print(" ");
    Serial.println(moveY);
  }

  delay(10);
}

void updateLCD(){
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Shots fired: ");
  lcd.print(shot);
}

void startRecoil(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  recoilActive = true;
  recoilStartTime = millis();
}