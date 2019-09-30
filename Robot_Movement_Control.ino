//receiver pin
#define CH1 2 //
#define CH2 3
#define CH5 6

//motor pin
#define MI1 8
#define MI2 11
#define MI3 12
#define MI4 13
#define EN1 9
#define EN2 10

//Misc
#define EROSIG 10
#define UPDATETIME 100
#define PWMCONST 0.8
unsigned long ch1, ch2, ch5, previousMillis;

void setup() {
pinMode(2, INPUT);
pinMode(3, INPUT);
pinMode(4, INPUT);
pinMode(5, INPUT);
pinMode(6, INPUT);
pinMode(7, INPUT);
pinMode(8, OUTPUT);
pinMode(9, OUTPUT);
pinMode(10, OUTPUT);
pinMode(11, OUTPUT);
pinMode(12, OUTPUT);
pinMode(13, OUTPUT);

OCR0A = 0xAF;
TIMSK0 |= _BV(OCIE0A);
}

SIGNAL(TIMER0_COMPA_vect){
unsigned long currentMillis = millis();
if (currentMillis - previousMillis == UPDATETIME){
ch1 = sigCompute(pulseIn(CH1, HIGH));
ch2 = sigCompute(pulseIn(CH2, HIGH));
ch5 = pulseIn(CH5, HIGH);
Serial.begin(115200);

motorControl(ch1, ch2, ch5);
previousMillis = currentMillis;
}
}

void loop() {
}

void motorCommand (int pw1, int pw2, int c5){
if (c5 > 1500){
Serial.println("Motor Online");
analogWrite(EN1, pw1);
analogWrite(EN2, pw2);
Serial.println(pw1);
Serial.println(pw2);
}
else if (c5 < 1500){
Serial.println("Motor Offline");
analogWrite(EN1,0);
analogWrite(EN2,0);
}
}

void motorControl(int c1,int c2, int cf)
{
int op1,op2;
op1 = constrain(PWMCONST(c1+c2),-255,255); //left motor output pwm
op2 = constrain(PWMCONST(c2 - c1),-255,255); //right motor output pwm

directionControl(op1,MI1,MI2);
directionControl(op2,MI3,MI4);
motorCommand(op1,op2,cf);
op1 = 0;
op2 = 0;
}

void directionControl(int OP,int DR1, int DR2){
if(OP >= 0){
digitalWrite(DR1,HIGH);
digitalWrite(DR2,LOW);
}
else if (OP < 0){
digitalWrite(DR1,LOW);
digitalWrite(DR2,HIGH);
}
}

int sigCompute(int sig){
int PWM = map(sig,900, 2000, -255, 255);
if (PWM < EROSIG && PWM > - EROSIG){
PWM = 0;
}
return PWM;
}
