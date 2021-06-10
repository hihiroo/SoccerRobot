#include <AFMotor.h>
#include <Wire.h>
#include <NewPing.h>
#define address 0x1E
#define KP 0.4

AF_DCMotor MOTORA(1);
AF_DCMotor MOTORB(2);
AF_DCMotor MOTORC(3);
AF_DCMotor MOTORD(4);

typedef struct
{
  int echo;
  int trig;
}Nping;

typedef struct
{
  int pin;
  int num;
  int stat;
}Lig;

Nping ult[4];
double compass;
Lig cds[4];
int ball_dir;
int max_ir;
float ultra[4];

void setup(){
    Serial.begin(9600);
    Wire.begin();
    MOTORA.setSpeed(255);
    MOTORB.setSpeed(255);
    MOTORC.setSpeed(255);
    MOTORD.setSpeed(255);
    Wire.beginTransmission(address);
    Wire.write(0x02);
    Wire.write(0x00);
    Wire.endTransmission();

    ult[0].echo=1;
    ult[0].trig=2;
    ult[1].echo=3;
    ult[1].trig=4;
    ult[2].echo=5;
    ult[2].trig=6;
    ult[3].echo=7;
    ult[3].trig=8;
    
    cds[0].pin=9;
    cds[1].pin=10;
    cds[2].pin=11;
    cds[3].pin=12;
    
    pinMode(ult[0].trig,OUTPUT);
    pinMode(ult[0].echo,INPUT);
    pinMode(ult[1].trig,OUTPUT);
    pinMode(ult[1].echo,INPUT);
    pinMode(ult[2].trig,OUTPUT);
    pinMode(ult[2].echo,INPUT);
    pinMode(ult[3].trig,OUTPUT);
    pinMode(ult[3].echo,INPUT);

    pinMode(cds[0].pin,INPUT);
    pinMode(cds[1].pin,INPUT);
    pinMode(cds[2].pin,INPUT);
    pinMode(cds[3].pin,INPUT);
}

void read_compass(void)
{
    int x,y,z;
    Wire.beginTransmission(address);
    Wire.write(0x03);
    Wire.endTransmission();
    Wire.requestFrom(address, 6);

    if(6<=Wire.available())
    {
        x = Wire.read()<<8;
        x |= Wire.read();
        z = Wire.read()<<8;
        z |= Wire.read();
        y = Wire.read()<<8;
        y |= Wire.read();
    }
    compass = (atan2((double)y,(double)x) * (180 / 3.14159265) + 180);
}

void read_ultra(void)
{
  for(int i=0;i<=3;i++)
  {
    float cm;
    digitalWrite(ult[i].trig,LOW);
    delayMicroseconds(2);
    digitalWrite(ult[i].trig,HIGH);
    delayMicroseconds(10);
    digitalWrite(ult[i].trig,LOW);

    cm=pulseIn(ult[i].echo,HIGH)/58.0;
    ultra[i]=(int(cm*100.0))/100.0;
  }
}


void madirF(int ma){
  MOTORA.run(FORWARD);
  MOTORA.setSpeed(ma);
}
void madirB(int ma){
  MOTORA.run(BACKWARD);
  MOTORA.setSpeed(ma);
}

void mbdirF(int mb){
  MOTORB.run(FORWARD);
  MOTORB.setSpeed(mb);
}
void mbdirB(int mb){
  MOTORB.run(BACKWARD);
  MOTORB.setSpeed(mb);
}

void mcdirF(int mc){
  MOTORC.run(FORWARD);
  MOTORC.setSpeed(mc);
}
void mcdirB(int mc){
  MOTORC.run(BACKWARD);
  MOTORC.setSpeed(mc);
}

void mddirF(int md){
  MOTORD.run(FORWARD);
  MOTORD.setSpeed(md);
}
void mddirB(int md){
  MOTORD.run(BACKWARD);
  MOTORD.setSpeed(md);
}

void MA(int ma)
{
  int tmp=abs(ma);
  tmp=tmp*255/100;
  if(tmp>255) tmp=255;
  if(ma>0) madirF(tmp);
  else madirB(tmp);
}
void MB(int mb)
{
  int tmp=abs(mb);
  tmp=tmp*255/100;
  if(tmp>255) tmp=255;
  if(mb>0) mbdirF(tmp);
  else mbdirB(tmp);
}
void MC(int mc)
{
  int tmp=abs(mc);
  tmp=tmp*255/100;
  if(tmp>255) tmp=255;
  if(mc>0) mcdirF(tmp);
  else mcdirB(tmp);
}

void move(int ma,int mb,int mc,int md)
{
  MA(ma);
  MB(mb);
  MC(mc);
}

void compass_move(int ma,int mb,int mc,int md)
{
  read_compass();
  int comp;
  comp=compass-180;

  if(comp>90) move(90,90,90,90);
  else if(comp<-90) move(-90,-90,-90,-90);
  else move(ma+comp*KP,mb+comp*KP,mc+comp*KP,md+comp*KP);
}

void dir_move(int input_ball,int power)
{
    switch(input_ball)
    {
    case 0;
    compass_move(,,,);
    break;
    case 1;
    comapass_move(power,power,power,power);
    break;
    case 2;
    compass_move(-power,-power,-power,-power);
    break
    }  
}
  
void ball_near(int dir, int power)
{
  switch(dir)
  {
    case 0:
    dir_move(0, power);
    break;
    case 1:
    dir_move(1, power);
    break;
    case 2:
    dir_move(1, power);
    break;
    case 3:
    dir_move(1, power);
    break;
    case 4:
    dir_move(1, power);
    break;
    case 5:
    dir_move(1, power);
    break;
    case 6:
    dir_move(1, power);
    break;
    case 7:
    dir_move(1, power);
    break;
    case 8:
    dir_move(1, power);
    break;
    case 9:
    dir_move(1, power);
    break;
    case 10:
    dir_move(1, power);
    break;
    case 11:
    dir_move(1, power);
    break;
    case 12:
    dir_move(1, power);
    break;
    case 13:
    dir_move(2, power);
    break;
    case 14:
    dir_move(2, power);
    break;
    case 15:
    dir_move(2, power);
    break;
    case 16:
    dir_move(2, power);
    break;
    case 17:
    dir_move(2, power);
    break;
    case 18:
    dir_move(2, power);
    break;
    case 19:
    dir_move(2, power);
    break;
    case 20:
    dir_move(2, power);
    break;
    case 21:
    dir_move(2, power);
    break;
    case 22:
    dir_move(2, power);
    break;
    case 23:
    dir_move(2, power);
    break;
  }
}
void find_ball(void)
{
    ball_dir=0;
    max_ir=0;

    for(int i=0;i<6;i++)  
        ir[i*2+1]=(int)((float)(ir[i*2]+ir[i*2+2])/2*1.20);

    for(int i=0;i<12;i++)
    {
        if(max_ir<ir[i])
        {
        max_ir=ir[i];
        ball_dir=i;
        } 
    }
}
void loop(){
    move(0,0,0);
    int ultra_gap=0;
    LineDetected=0;

    while(1)
    {
        find_ball();
        read_line();
        if(LineDetected != 0)
        {
            if(ir[0]>165)
            {
                read_ultra();
                if(ultra[1] + ultra[3] > 100)
                {
                    ultra_gap = ultra[1] - ultra[3];
                    int comp;
                    read_compass();
                    comp = compass - 180 - ultra_gap*KP;
                    move(100+comp, comp, -100+comp);
                }
                else
                {
                    dir_move(0, 100);
                }
            }
            else
            {
                if (max_ir < 120)
                {
                    dir_move(ball_dir, 100);
                }
                else
                {
                    ball_near(ball_dir, 80);
                }
            }
        }
    }
}
