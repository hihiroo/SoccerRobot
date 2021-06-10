#define KP 0.4

#define Line0_White 20
#define Line1_White 20
#define Line2_White 20

unsigned  char LineDetected=0;
unsigned  char LineDec[4];
//
//------------- 여기부터의 변수는 읽기만 하세요. ------------------------
//
// 아래 변수에 센서 값들이 들어옵니다.
// 읽기만 하시고 절대 쓰지 마세요.
//
unsigned int analog[16];   
// 0 : 원거리 적외선 0 값
// 1 : 근거리 적외선 0 값
// 2 : 원거리 적외선 1 값
// 3 : 근거리 적외선 1 값
// 4 : 원거리 적외선 2 값
// 5 : 근거리 적외선 2 값
// 6 : 원거리 적외선 3 값
// 7 : 근거리 적외선 3 값
// 8 : 원거리 적외선 4 값
// 9 : 근거리 적외선 4 값
// 10 : 원거리 적외선 5 값
// 11 : 근거리 적외선 5 값
// 12 : 배터리 모니터링
// 13 : 바닥감지 센서 0
// 14 : 바닥감지 센서 1
// 15 : 바닥감지 센서 2
//
//   초음파 센서 거리 측정 값(0~3번 초음파)
//
unsigned int ultra[4];

//
// 가공되어진 적외선 센서 값
// 원거리용 센서와 근거리용 센서를 조합하여 계산되어진 값.
//
unsigned int ir[12];      //시계방향으로 0시~11시방향의 값

int ball_dir;            //적외선 값이 가장 강하게 보이는 방향.
int max_ir;               //가장 강한 적외선 값.
int last_pos = 0;         //공이 마지막으로 있던 좌우 방향, 왼쪽 = 1, 오른쪽 = 0;
int long_pos=0;   //장거리 IR로 측정한 공의 위치
int short_pos=0;//단거리 IR로 측정한 공의 위치
int long_dir=0;
int short_dir=0;
int detect_light = 0;
// 컴파스 센서 값.
double compass;
int final_pos = 0;         //last_pos의 확장형 0이 전방 1이 우측 2가 후방 3은 왼쪽 
//11번의 가중치를 늘려서 11의 감지가 더 잘되게 함.
//------------- 여기까지의 변수는 읽기만 하세요. ------------------------


unsigned char menu = 0;
unsigned char def = 0;
unsigned char some = 0;
unsigned char comics = 0;
unsigned char ult = 0;
unsigned char iru = 0;
unsigned char lina = 0;
#include "SOCCER.h"

int app(int a)//절댓값 도출함수
{
   if(a<0)   return -a;
   else return a;
}
void WarningDisplay()//배터리 없을 때의 경고 메세지 출력 함수
{
   OCR1A = 0;
   OCR1B = 0;
   OCR2B = 0;
   Lcd_Init();
   Lcd_Write_String(LINE1,"-WARNING-");
   Lcd_Write_String(LINE2,"  LOW BATTERY");
   while(1)
   Volt_Display();
}
void MOTORA(int ma)//모터A를 통제하는 함수
{
   int tmp = abs(ma);
   if((int)voltage < 95)
   {
      WarningDisplay();
   }
   else
   {
      tmp=tmp*255/100;
      if(tmp>255)   tmp=255;
      
      if(ma<0)   madirB;
      else      madirF;
      OCR1A = tmp;
   }
}
void MOTORB(int mb)//모터B를 통제하는 함수
{
   int tmp = abs(mb);
   
   tmp=tmp*255/100;
   if(tmp>255)   tmp=255;
   if((int)voltage < 95)
   {
      WarningDisplay();
   }
   else
   {
      if(mb<0)   mbdirB;
      else      mbdirF;
      OCR1B = tmp;
   }
}
void MOTORC(int mc)//모터C를 통제하는 함수
{
   int tmp = abs(mc);
   
   tmp=tmp*255/100;
   if(tmp>255)   tmp=255;
   if((int)voltage < 95)
   {
      WarningDisplay();
   }
   else
   {
      if(mc<0)   mcdirB;
      else      mcdirF;
      OCR2B = tmp;
   }
}
void move(int ma, int mb, int mc)//모든 모터를 통제하는 함수
{
   MOTORA(ma);
   MOTORB(mb);
   MOTORC(mc);
}
void compass_move(int ma, int mb, int mc)//컴파스 센서를 이용하여 로봇의 움직임을 보정하는 함수
{
   int comp;
   read_compass();
   comp = (int)compass / 10;
   comp = comp - 180;
   if (comp > 100)
   {
      move(90, 90, 90);
   }
   else if(comp < -100)
   {
      move(-90, -90, -90);
   }
   else
   {
      move(ma+comp*KP, mb+comp*KP, mc+comp*KP);
   }
}
void tvment(int n, int power, int time)//방향,힘,시간을 받아 움직임을 통제하는 함수
{
   switch(n)
   {
      case 0:
      move(power,0,-power);
      delay1ms(time);
      break;
      case 1:
      move(power/2,power/2,-power);
      delay1ms(time);
      break;      
      case 2:
      move(0,power,-power);
      delay1ms(time);
      break;
      case 3:
      move(-power/2,power,-power/2);
      delay1ms(time);
      break;
      case 4:
      move(-power,power,0);
      delay1ms(time);
      break;
      case 5:
      move(-power,power/2,power/2);
      delay1ms(time);
      break;
      case 6:
      move(-power,0,power);
      delay1ms(time);
      break;
      case 7:
      move(-power/2,-power/2,power);
      delay1ms(time);
      break;
      case 8:
      move(0,-power,power);
      delay1ms(time);
      break;
      case 9:
      move(power/2,-power,power/2);
      delay1ms(time);
      break;
      case 10:
      move(power, -power, 0);
      delay1ms(time);
      break;
      case 11:
      move(power, -power/2, -power/2);
      delay1ms(time);
      break;
   }
}
void stop(void)//일정기간 정지하는 함수
{
   move(0, 0, 0);
}
void movement(int n, int power)//컴파스를 이용한 움직임 보정이 아닌 절대적인 방향으로 모터를 제어하는 함수
{
   switch(n)
   {
      case 0:
      move(power,0,-power);
      break;
      case 1:
      move(power/2,power/2,-power);
      break;
      case 2:
      move(0,power,-power);
      break;
      case 3:
      move(-power/2,power,-power/2);
      break;
      case 4:
      move(-power,power,0);
      break;
      case 5:
      move(-power,power/2,power/2);
      break;
      case 6:
      move(-power,0,power);
      break;
      case 7:
      move(-power/2,-power/2,power);
      break;
      case 8:
      move(0,-power,power);
      break;
      case 9:
      move(power/2,-power,power/2);
      break;
      case 10:
      move(power, -power, 0);
      break;
      case 11:
      move(power, -power/2, -power/2);
      break;
   }
}
void dir_move(int input_ball, int power)//컴파스를 이용한 보정을 통해 로봇움직임을 제어하는 함수
{
   switch(input_ball)
   {
      case 0:
      compass_move(power, 0, -power);
      break;
      case 1:
      compass_move(power/2, power/2, -power);
      break;
      case 2:
      compass_move(0, power, -power);
      break;
      case 3:
      compass_move(-power/2, power, -power/2);
      break;
      case 4:
      compass_move(-power, power, 0);
      break;
      case 5:
      compass_move(-power, power/2, power/2);
      break;
      case 6:
      compass_move(-power, 0, power);
      break;
      case 7:
      compass_move(-power/2, -power/2, power);
      break;
      case 8:
      compass_move(0, -power, power);
      break;
      case 9:
      compass_move(power/2,- power, power/2);
      break;
      case 10:
      compass_move(power, -power, 0);
      break;
      case 11:
      compass_move(power, -power/2, -power/2);
      break;
   }
}
void no_ear(void)//골대 앞에서 공을 막는 함수. 이름의 유래는 축구선수 노이어
{
   if(final_pos == 0)   stop();
   else if(final_pos == 3)   movement(3, 60);
   else if(final_pos == 9)   movement(9, 60);
   else   stop();
   delay1ms(200);
}
void cpment(void)//경기 진행도중 로봇이 기울어져있는 문제로 제대로 골대를 찾아가지 않는 문제를 고치기 위해 고안해낸 함수.현재 제작중. compass_move를 바탕으로 확인함(민경코드)
{
   
   int comp;
   read_compass();
   comp = (int)compass / 10;
   int turned=app((int)(compass-1800)/10);//얼마나 회전되었는지 확인하는 변수. 양수면 오른쪽이다.
   
   if(turned<=5) move(0,0,0);
   
   else if (comp > 180)
   {
      move(turned*2, turned*2, turned*2);
   }
   else if(comp < 180)
   {
      move(-turned*2, -turned*2, -turned*2);
   }
}
void turnaro(void)
{
   move(40,40,40);
   delay1ms(500);
}
void ball_near(int dir, int power)//공이 가까이 있을 때, 공을 쫓아가는 함수
{
   switch(dir)
   {
      case 0:
      if(last_pos == 1)
      dir_move(11, power);           //dir_move말고 compass_move로 바꿔서 파워 낮추자 공잡았을때 옆으로 툭툭치는거 이거때문일수도있음 
      else
      dir_move(0, power);
      break;
      case 1:
      dir_move(3, power);
      break;
      case 2:
      dir_move(5, power);
      break;
      case 3:
      dir_move(6, power);
      break;
      case 4:
      dir_move(7, power);
      break;
      case 5:
      dir_move(8, power);
      break;
      case 6:
      dir_move(9, power);
      break;
      case 7:
      dir_move(4, power);
      break;
      case 8:
      dir_move(5, power);
      break;
      case 9:
      dir_move(6, power);
      break;
      case 10:
      dir_move(7, power);
      break;
      case 11:
      dir_move(9, power);
      break;
   }
}
void view_line(void)//Line센서를 보는 함수
{
   Lcd_Clear();
   Lcd_Write_String(LINE1,"LINE");
   while(1)
   {
      Volt_Display();
      Lcd_Cmd(LINE2);
      for(int i=13;i<16; i++)
      {
         DigitDisplay(analog[i]);
         Lcd_Data(' ');
      }
      if(SELECT)
      {
         while(!SELECT)   ;
         return;
      }
      delay1ms(200);
   }
}
void view_long_ir(void)//장거리 적외선 센서를 보는 함수
{
   Lcd_Clear();
   Lcd_Write_String(LINE1,"LONG IR");
   while(1)
   {
      Lcd_Move(0, 9);
      for(int i=0;i<2; i++)
      {
         DigitDisplay(analog[i*2]);
         Lcd_Data(' ');
      }
      Lcd_Cmd(LINE2);
      for(int i=2;i<6; i++)
      {
         DigitDisplay(analog[i*2]);
         Lcd_Data(' ');
      }
      if(SELECT)
      {
         while(!SELECT)   ;
         return;
      }
      delay1ms(200);
   }
}
void view_short_ir(void)//단거리 적외선 센서를 보는 함수
{
   Lcd_Clear();
   Lcd_Write_String(LINE1,"SHORT IR");
   while(1)
   {
      Lcd_Move(0, 9);
      for(int i=0;i<2; i++)
      {
         DigitDisplay(analog[i*2+1]);
         Lcd_Data(' ');
      }
      Lcd_Cmd(LINE2);
      for(int i=2;i<6; i++)
      {
         DigitDisplay(analog[i*2+1]);
         Lcd_Data(' ');
      }
      if(SELECT)
      {
         while(!SELECT)   ;
         return;
      }
      delay1ms(200);
   }
}
void view_ultra(void)//초음파 센서를 보는 함수
{
   Lcd_Clear();
   Lcd_Write_String(LINE1,"ULTRA");
   while(1)
   {
      Volt_Display();
      Lcd_Cmd(LINE2);
      for(int i=0;i<4; i++)
      {
         DigitDisplay((int)((float)ultra[i]*0.34));
         Lcd_Data(' ');
      }
      if(SELECT)
      {
         while(!SELECT)   ;
         return;
      }
      delay1ms(200);
   }
}
void new_ultra(void)
{
   Lcd_Clear();
   Lcd_Write_String(LINE1,"ULTRA");
   while(1)
   {
      Volt_Display();
      Lcd_Cmd(LINE2);
      for(int i=0;i<4; i++)
      {
         DigitDisplay((int)((float)ultra[i]*0.5));
         Lcd_Data(' ');
      }
      if(SELECT)
      {
         while(!SELECT)   ;
         return;
      }
      delay1ms(200);
   }
}
void view_compass(void)//콤파스 센서를 보는 함수
{
   Lcd_Clear();
   Lcd_Write_String(LINE1,"COMPASS");
   while(1)
   {
      read_compass();
      
      Lcd_Cmd(LINE2);
      AngleDisplay(compass);

      if(SELECT)
      {
         while(!SELECT)   ;
         return;
      }
      delay1ms(200);
   }
}
void menu_display(unsigned char no)//메뉴창을 보는 함수
{
   switch(no)
   {
      case 0: Lcd_Write_String(LINE2,"RUN DEFPROGRAM  ");
      break;
      case 1: Lcd_Write_String(LINE2,"VIEW SOMETHING  ");
      break;
   }
}
void find_ball(void)//공이 어디에 있는지 확인하는 함수
{
   ball_dir = 0;
   max_ir = 0;
   for(int i = 0; i < 6; i++)
   ir[i*2] = analog[i*2] > 240? 128 + analog[i*2+1]/2 : analog[i*2]/2;
   for(int i = 0; i < 5; i++)
   ir[i*2+1] = (int)((float)(ir[i*2] + ir[i*2+2])/2*1.20);
   ir[11] = (int)((float)(ir[10] + ir[0])/2*1.50);
   for(int i = 0; i < 12; i++)
   {
      if(max_ir < ir[i])
      {
         max_ir = ir[i];
         ball_dir = i;
      }
   }
   if(ball_dir== 0)   final_pos = 0;
   else if(ball_dir>=1 && ball_dir<=4)      final_pos = 3;
   else if(ball_dir>=8 && ball_dir<=11)   final_pos = 9;
}
void view_totalIR(void)//? 어디서 사용되는지 전혀 알수없는 함수
{
   Lcd_Clear();
   Lcd_Write_String(LINE1,"IR");
   while(1)
   {
      find_ball();
      Volt_Display();
      Lcd_Cmd(LINE2);
      DigitDisplay(max_ir);
      Lcd_Data(' ');
      DigitDisplay(ball_dir);
      delay1ms(200);
      if(SELECT)
      {
         while(!SELECT)   ;
         return;
      }
      delay1ms(500);
   }
}
void ultra_attack(void)// 초음파를 통해 우리 골대를 찾는 것을 모티브로 하여, 공격할 때, 초음파를 사용하는 함수. 상대 로봇들도 초음파에 걸리므로 컴파스로 보정이 필요하다.
{
   int goal = ((int)(float)ultra[0]*0.34);//얼마나 골대에 다가갔는지를 알려주는 변수.정확한 수치를 알기전엔 사용하지 않음.
   int tra = ((int)((float)ultra[1]*0.34) - (int)((float)ultra[3]*0.34));//좌우로 치우침 정도를 계산하는 변수. 양수는 오른쪽으로 치우침, 음수는 왼쪽으로 치우침.
   if(tra>=-5 && tra<=5)//가운데로 되어있다고 판단되어 직진함.
   {
      movement(0,100);
      delay1ms(250);
   }
   else if(tra > 10)//오른쪽으로 치우쳤으므로, 좌전방으로 진행한다.
   {
      movement(8,25);
      delay1ms(250);
   }
   else if(tra < -10)//왼쪽으로 치우쳤으므로, 우전방으로 진행한다.
   {
      movement(2,25);
      delay1ms(250);
   }
   else//이도저도 아니니 조심히 앞으로 간다.
   {
      movement(0,25);
      delay1ms(250);
   }
}
void comeback(void)//공격을 끝내고 원래 자리로 돌아오는 함수
{
   int tra = ((int)((float)ultra[1]*0.34) - (int)((float)ultra[3]*0.34));//좌우가 얼마나 치우쳐 있는 지 확인하는 변수. 양수면 오른쪽 음수면 왼쪽이다.
   if((int)(float)ultra[2]*0.34 > 50)//앞을 바라볼 때, 뒤로 돌아온다.
   {
      movement(6, 50);
      delay1ms(100);
   }
   else if((int)((float)ultra[2]*0.34) < 10)//너무 뒤로 가는 것을 방지한다.
   {
      movement(0, 45);
      delay1ms(50);
   }
   else if(tra > 15)//오른쪽으로 치우칠 때 가운데로 돌아간다.
   {
      movement(3, 55);
      delay1ms(100);
   }
   else if(tra < -15)//왼쪽으로 치우칠 때 가운데로 돌아간다.
   {
      movement(9, 55);
      delay1ms(100);
   }
   else//제자리를 찾았다고 판단되면 멈춘다.
   {
      stop();
      delay1ms(300);
   }
}
void lineback(void)//경기장 밖으로 나가지 않게 도와주는 함수
{
   
   if(LineDetected == 1)//정면에 흰색선이 있음.뒤로 물러남.
   {
      movement(0,50);
      delay1ms(500);
   }
   else if(LineDetected == 2)//오른쪽에 흰색선이 있음.왼쪽으로 물러남.
   {
      movement(9,50);
      delay1ms(500);
   }
   else if(LineDetected == 4)//왼쪽에 흰색선이 있음.오른쪽으로 물러남
   {
      movement(3,50);
      delay1ms(500);
   }
   LineDetected = 0;//흰색선에서 나왔다고 판단하고, 초기화
}
void view_imitate(void)
{
   Lcd_Clear();
   int turned=compass-1800;//얼마나 회전되었는지 확인하는 변수.양수면 오른쪽이다.
   Lcd_Write_String(LINE1,"Imitate number  ");
   while(1)
   {
      read_compass();
      turned=app(compass-1800);
      Lcd_Cmd(LINE2);
      AngleDisplay(turned);
   }
}
void leav_line(void)
{
   if(LineDec[1] != 0)//정면에 흰색선이 있음.뒤로 물러남.
   {
      movement(0,50);
      delay1ms(500);
      LineDec[1]=0;
   }
   else if(LineDec[2] != 0)//오른쪽에 흰색선이 있음.왼쪽으로 물러남.
   {
      movement(9,50);
      delay1ms(500);
      LineDec[2]=0;
   }
   else if(LineDec[3] != 0)//왼쪽에 흰색선이 있음.오른쪽으로 물러남
   {
      movement(3,50);
      delay1ms(500);
      LineDec[3]=0;
   }
   else if(LineDec[1] != 0 && LineDec[2] != 0)
   {
      movement(8,50);
      delay1ms(500);
      LineDec[1]=0;
      LineDec[2]=0;
   }
   else if(LineDec[1] != 0 && LineDec [3] != 0)
   {
      movement(4,50);
      delay1ms(500);
      LineDec[1]=0;
      LineDec[3]=0;
   }
   else if(LineDec[2] !=0 && LineDec [3] != 0)
   {
      movement(0,50);
      delay1ms(500);
      LineDec[2]=0;
      LineDec[3]=0;
   }
}
void allnew_comp(void)
{
   Lcd_Clear();
   int comp;
   Lcd_Write_String(LINE1,"allnew comp   ");
   while(1)
   {
      read_compass();
      comp = (int)compass;
      comp = (int)(comp/30);
      Lcd_Cmd(LINE2);
      compDisplay(comp);//디스플레이 새로 만들어야함
      if(SELECT)
      {
         while(!SELECT)   ;
         return;
      }
   }
}
void Lined(void)
{
   Lcd_Clear();
   Lcd_Write_String(LINE1,"LINED           ");
   while(1)
   {
      Lcd_Cmd(LINE2);
      DigitDisplay(LineDec[1]);
      Lcd_Data(' ');
      DigitDisplay(LineDec[2]);
      Lcd_Data(' ');
      DigitDisplay(LineDec[3]);      
      Lcd_Data(' ');   
      if(SELECT)
      {
         while(!SELECT)   ;
         return;
      }
      delay1ms(500);
   }
}
void ball_pos(void)//공의 정확한 위치 파악.
{
   short_pos=0;
   short_dir=0;
   long_pos=0;
   long_dir=0;
   for(int i=1; i<12;i+=2)
   {
      if(analog[i]>short_pos)
      {
         short_pos=analog[i];
         short_dir=i-1;
      }
   }
   for(int i=0;i<12;i+=2)
   {
      if(analog[i]>short_pos)
      {
         long_pos=analog[i];
         long_dir=i;
      }
   }
}
void view_pos()
{
   Lcd_Clear();
   Lcd_Write_String(LINE1,"view ball pos   ");
   while(1)
   {
      ball_pos();
      Lcd_Cmd(LINE2);
      DigitDisplay(long_pos);
      Lcd_Data(' ');
      DigitDisplay(long_dir);
      Lcd_Data(' ');
      DigitDisplay(short_pos);
      Lcd_Data(' ');
      DigitDisplay(short_dir);
      Lcd_Data(' ');
   }
   if(SELECT)
   {
      while(!SELECT)   ;
      return;
   }
   delay1ms(500);
}
//새로운 함수는 이 줄 바로 윗 줄에서 정의하는 것을 추천한다.
void def1(void)//골대 앞에서만 있는 수비수
{
   move(0,0,0);
   Lcd_Clear();
   Lcd_Write_String(LINE1,"def1 thes       ");
   Lcd_Write_String(LINE2,"This is keeper  ");
   while(1)
   {
      find_ball();
      if(LineDetected!=0)      lineback();
      if(max_ir>120)   no_ear();
      else   comeback();
   }
}
void def2(void)//공격까지 진행하는 함수
{
   Lcd_Clear();
   Lcd_Write_String(LINE1,"def2 othes      ");
   Lcd_Write_String(LINE2,"This is Midroad ");
   while (1)
   {
      allnew_comp();
   }
}
void def3(void)//아직 계획에 없음
{
   Lcd_Clear();
   Lcd_Write_String(LINE2,"def3 start      ");
   while (1)
   {
      view_imitate();
   }
}
void def4(void)//아직 계획에 없음
{
   Lcd_Clear();
   Lcd_Write_String(LINE2,"def4 start      ");
    while(1)
   {
      cpment();
   }
}
void DEFPROGRAM(void)//수비 프로그램을 호출하는 함수
{
   move(0,0,0);
   Lcd_Clear();
   Lcd_Write_String(LINE2,"RUN DEFPROGRAM 1");
   while(1)
   {
      delay1ms(200);
      Lcd_Write_String(LINE1,"DEFPROGRAM RAPID");
      if(SELECT)
      {
         while(SELECT)   ;
         def++;
         if (def>4)   def=0;
         switch(def)
         {
            case 0: Lcd_Write_String(LINE2,"RUN DEFPROGRAM 1");
            break;
            case 1: Lcd_Write_String(LINE2,"RUN DEFPROGRAM 2");
            break;
            case 2: Lcd_Write_String(LINE2,"RUN DEFPROGRAM 3");
            break;
            case 3: Lcd_Write_String(LINE2,"RUN DEFPROGRAM 4");
            break;
            case 4: Lcd_Write_String(LINE2,"BACKTRACK       ");
         }
      }
      if(ENTER)
      {
         while(ENTER)   ;
         switch(def)
         {
            case 0: def1();
            break;
            case 1: def2();
            break;
            case 2: def3();
            break;
            case 3:   def4();
            break;
            case 4: return;
         }
         Lcd_Clear();
         Lcd_Write_String(LINE1,"DEFPROGRAM      ");
         switch(def)
         {
            case 0: Lcd_Write_String(LINE2,"RUN DEFPROGRAM 1");
            break;
            case 1: Lcd_Write_String(LINE2,"RUN DEFPROGRAM 2");
            break;
            case 2: Lcd_Write_String(LINE2,"RUN DEFPROGRAM 3");
            break;
            case 3: Lcd_Write_String(LINE2,"RUN DEFPROGRAM 4");
            break;
            case 4: Lcd_Write_String(LINE2,"BACKTRACK       ");
         }
      }
   }
}   
void ultra_display(void)
{
   move(0,0,0);
   Lcd_Clear();
   Lcd_Write_String(LINE2,"view ultra      ");
   ult=0;
   while(1)
   {
      delay1ms(200);
      Lcd_Write_String(LINE1,"VIEW ULTRASOME  ");
      if(SELECT)
      {
         while(SELECT)   ;
         ult++;
         if (ult>2)   ult=0;
      }
      switch(ult)
      {
         case 0: Lcd_Write_String(LINE2,"view ultra      ");
         break;
         case 1: Lcd_Write_String(LINE2,"view new ultra  ");
         break;
         case 2:   Lcd_Write_String(LINE2,"BACKTRACK       "); 
         break;
      }
      if(ENTER)
      {
         while(ENTER)   ;
         switch(ult)
         {
            case 0: view_ultra();
            break;
            case 1: new_ultra();
            break;
            case 2: return;
            break;
         }
         Lcd_Clear();
         Lcd_Write_String(LINE1,"VIEW ULTRASOME  ");
         switch(ult)
         {
            case 0: Lcd_Write_String(LINE2,"view ultra      ");
            break;
            case 1: Lcd_Write_String(LINE2,"view new ultra  ");
            break;
            case 2:   Lcd_Write_String(LINE2,"BACKTRACK       ");
            break;
         }         
      }
   }
}
void IR_display(void)
{
   move(0,0,0);
   Lcd_Clear();
   Lcd_Write_String(LINE2,"view totalIR    ");
   iru=0;
   while(1)
   {
      delay1ms(200);
      Lcd_Write_String(LINE1,"VIEW IRSOME     ");
      if(SELECT)
      {
         while(SELECT)   ;
         iru++;
         if (iru>4)   iru=0;
      }
      switch(iru)
      {
         case 0: Lcd_Write_String(LINE2,"view totalIR    ");
         break;
         case 1: Lcd_Write_String(LINE2,"view short IR   ");
         break;
         case 2: Lcd_Write_String(LINE2,"view long IR    ");
         break;
         case 3: Lcd_Write_String(LINE2,"view ball pos   ");
         break;
         case 4: Lcd_Write_String(LINE2,"BACKTRACK       ");
      }
      if(ENTER)
      {
         while(ENTER)   ;
         switch(iru)
         {
            case 0: view_totalIR();
            break;
            case 1: view_short_ir();
            break;
            case 2: view_long_ir();
            break;
            case 3:   view_pos();
            break;
            case 4: return;
         }
         Lcd_Clear();
         Lcd_Write_String(LINE1,"VIEW IRSOME     ");
         switch(iru)
         {
            case 0: Lcd_Write_String(LINE2,"view totalIR    ");
            break;
            case 1: Lcd_Write_String(LINE2,"view short IR   ");
            break;
            case 2: Lcd_Write_String(LINE2,"view long IR    ");
            break;
            case 3: Lcd_Write_String(LINE2,"view ball pos   ");
            break;
            case 4: Lcd_Write_String(LINE2,"BACKTRACK       ");
         }
      }
   }
}
void line_display(void)
{
   move(0,0,0);
   Lcd_Clear();
   Lcd_Write_String(LINE2,"VIEW LINE       ");
   lina=0;
   while(1)
   {
      delay1ms(200);
      Lcd_Write_String(LINE1,"VIEW LINESOME   ");
      if(SELECT)
      {
         while(SELECT)   ;
         lina++;
         if (lina>2)   lina=0;
      }
      switch(lina)
      {
         case 0: Lcd_Write_String(LINE2,"VIEW LINE       ");
         break;
         case 1: Lcd_Write_String(LINE2,"VIEW LINED      ");
         break;
         case 2: Lcd_Write_String(LINE2,"BACKTRACK       ");
      }
      if(ENTER)
      {
         while(ENTER)   ;
         switch(lina)
         {
            case 0: view_line();
            break;
            case 1: Lined();
            break;
            case 2: return;
         }
         Lcd_Clear();
         Lcd_Write_String(LINE1,"VIEW LINESOME   ");
         switch(lina)
         {
            case 0: Lcd_Write_String(LINE2,"VIEW LINE       ");
            break;
            case 1: Lcd_Write_String(LINE2,"VIEW LINED      ");
            break;
            case 2: Lcd_Write_String(LINE2,"BACKTRACK       ");
         }
      }
   }
}
void compass_display(void)
{
   move(0,0,0);
   Lcd_Clear();
   Lcd_Write_String(LINE2,"VIEW COMPASS    ");
   comics=0;
   while(1)
   {
      delay1ms(200);
      Lcd_Write_String(LINE1,"VIEW COMPASSSOME");
      if(SELECT)
      {
         while(SELECT)   ;
         comics++;
         if (comics>1)   comics=0;
      }
      switch(comics)
      {
         case 0: Lcd_Write_String(LINE2,"VIEW COMPASS    ");
         break;
         case 1: Lcd_Write_String(LINE2,"BACKTRACK       ");
      }
      if(ENTER)
      {
         while(ENTER)   ;
         switch(comics)
         {
            case 0: view_compass();
            break;
            case 1: return;
         }
         Lcd_Clear();
         Lcd_Write_String(LINE1,"VIEW COMPASSSOME");
         switch(comics)
         {
            case 0: Lcd_Write_String(LINE2,"VIEW COMPASS    ");
            break;
            case 1: Lcd_Write_String(LINE2,"BACKTRACK       ");
         }
      }
   }
}
void VIEW_SOME(void)
{
   move(0,0,0);
   Lcd_Clear();
   Lcd_Write_String(LINE2,"VIEW COMPASS    ");
   some=0;
   while(1)
   {
      delay1ms(200);
      Lcd_Write_String(LINE1,"VIEW SENSER     ");
      if(SELECT)
      {
         while(SELECT)   ;
         some++;
         if (some>4)   some=0;
         switch(some)
         {
            case 0: Lcd_Write_String(LINE2,"VIEW COMPASS    ");
            break;
            case 1: Lcd_Write_String(LINE2,"VIEW ULTRA      ");
            break;
            case 2: Lcd_Write_String(LINE2,"VIEW LINE       ");
            break;
            case 3: Lcd_Write_String(LINE2,"VIEW IR         ");
            break;
            case 4: Lcd_Write_String(LINE2,"BACKTRACK       ");
         }
      }
      if(ENTER)
      {
         while(ENTER)   ;
         switch(some)
         {
            case 0: compass_display();
            break;
            case 1: ultra_display();
            break;
            case 2: line_display();
            break;
            case 3:   IR_display();
            break;
            case 4: return;
         }
         Lcd_Clear();
         Lcd_Write_String(LINE1,"VIEW SENSER     ");
         switch(some)
         {
            case 0: Lcd_Write_String(LINE2,"VIEW COMPASS    ");
            break;
            case 1: Lcd_Write_String(LINE2,"VIEW ULTRA      ");
            break;
            case 2: Lcd_Write_String(LINE2,"VIEW LINE       ");
            break;
            case 3: Lcd_Write_String(LINE2,"VIEW IR         ");
            break;
            case 4: Lcd_Write_String(LINE2,"BACKTRACK       ");
         }
      }
   }
}
int main()//메인함수.주석이 필요없음
{
   init_devices();
   Lcd_Init();
   
   Lcd_Write_String(LINE1,"RAPID       V3.2");
   Lcd_Write_String(LINE2," ROBOCUP KOREA");
   
   while(!ENTER)   ;
   while(ENTER)   ;

   IIC_WRITE(SLA_W,0x02,0x00);
   delay1ms(200);
   IIC_WRITE(SLA_W,0x00,0x58);
   delay1ms(200);
   IIC_WRITE(SLA_W,0x02,0x00);
   delay1ms(200);
   
   
   
   Lcd_Clear();
   Lcd_Write_String(LINE1,"RAPID       V3.2");
   Lcd_Write_String(LINE2,"RUN DEFPROGRAM  ");
   while(1)
   {
      delay1ms(200);

      Volt_Display();
      
      if(SELECT)
      {
         while(SELECT)   ;
         menu++;
         if (menu>1)   menu=0;
         menu_display(menu);
      }
      if(ENTER)
      {
         while(ENTER)   ;
         switch(menu)
         {
            case 0: DEFPROGRAM();
            break;
            case 1: VIEW_SOME();
            break;
         }
         Lcd_Clear();
         Lcd_Write_String(LINE1,"RCKA");
         menu_display(menu);
      }
   }
   return 0;
}

void PROGRAM1(void)//공격
{
   move(0,0,0);
   Lcd_Clear();
   int ultra_gap = 0;
   LineDetected = 0;
   while(1)
   {
      find_ball();
      if(ball_dir > 6)
      last_pos = 1;
      else if(ball_dir >= 1 && ball_dir < 6)
      last_pos = 0;
      if(LineDetected != 0)
      {
         if(LineDetected == 2 || LineDetected == 4)
         {
            
            if((int)((float)ultra[2]*0.34) < 20)
            dir_move(0, 50);
            else if(LineDetected == 2)
            dir_move(9, 50);
            else if(LineDetected == 4)
            dir_move(3, 50);
         }
         else if(LineDetected == 1)
         dir_move(6, 50);
         delay1ms(500);
         LineDetected = 0;
      }
      if(max_ir > 165 && (ball_dir == 0 || ball_dir == 1 || ball_dir == 11))         //공이 가까이 있을 때
      {
         if((int)((float)ultra[1]*0.34) + (int)((float)ultra[3]*0.34) > 100)
         {
            ultra_gap = (int)((float)ultra[1]*0.34) - (int)((float)ultra[3]*0.34);
            int comp;
            read_compass();
            comp = (int)compass / 10;
            comp = comp - 180 - ultra_gap*KP;

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
void PROGRAM2(void)//수비
{
   move(0,0,0);
   Lcd_Clear();
   int ultra_gap = 0;
   while(1)
   {
      find_ball();
      if(ball_dir > 6)
      last_pos = 1;
      else if(ball_dir > 1 && ball_dir < 6)
      last_pos = 0;
      if(LineDetected != 0) //선이 감지되었을 떄
      {
         if(LineDetected == 1)
         dir_move(6, 50);
         if(LineDetected == 2)   //오른쪽 선
         dir_move(9, 50);      //왼쪽으로 이동
         else if(LineDetected == 4)   //왼쪽 선
         dir_move(3, 50);         //오른쪽으로 이동
         delay1ms(500);            //딜레이
         LineDetected = 0;
      }
      if(max_ir > 165 && (ball_dir == 0 || ball_dir == 1 || ball_dir == 11))         //공이 가까이 있을 때
      {
         ultra_gap = (int)((float)ultra[1]*0.34) - (int)((float)ultra[3]*0.34);
         int comp;
         read_compass();
         comp = (int)compass / 10;
         comp = comp - 180 - ultra_gap;
         
         move(100+comp*KP, comp*KP, -100+comp*KP);
         
         dir_move(0, 100);
         compass_move(100, -ultra_gap*2, -100);
      }
      else if(max_ir > 130)
      {
         dir_move(ball_dir, 100);
      }
      else
      {
         if((int)((float)ultra[1]*0.34) + (int)((float)ultra[3]*0.34) > 40 && (int)((float)ultra[1]*0.34) - (int)((float)ultra[3]*0.34) < 20 && (int)ultra[1] - (int)ultra[3] > -20 && (int)((float)ultra[2]*0.34) > 30)
         dir_move(6, 100);
         else if((int)((float)ultra[2]*0.34) < 20)
         dir_move(0, 80);
         else if((int)((float)ultra[1]*0.34) - (int)((float)ultra[3]*0.34) > 10)
         dir_move(3, 80);
         else if((int)((float)ultra[1]*0.34) - (int)((float)ultra[3]*0.34) < -10)
         dir_move(9, 80);
         else
         dir_move(0, 0);
      }
   }
}
void PROGRAM3(void)
{
   move(0,0,0);
   Lcd_Clear();
   Lcd_Write_String(LINE1,"Soso");
   Lcd_Write_String(LINE2,"Made by end");
   int ultra_gap=0;
   LineDetected = 0;
   while(1)
   {
      find_ball();
      if(ball_dir > 6)
      last_pos = 1;
      else if(ball_dir > 1 && ball_dir < 6)
      last_pos = 0;
      
      if(ball_dir== 0)   final_pos = 0;
      else if(ball_dir>=1 && ball_dir<=4)      final_pos = 3; 
      else if(ball_dir>=8 && ball_dir<=11)   final_pos = 9;
      
      if(max_ir>120)   no_ear();
      else   comeback();
   }
}
void PROGRAM4(void)
{
   move(0,0,0);
   Lcd_Clear();
   LineDetected = 0;
   while(1)
   {
      find_ball();//last_pos의 확장형 4방으로 하지만 실제로는 아무짝에도 쓸모없으니 지워도 됨
      if(ball_dir > 6)
         last_pos = 1;
      else if(ball_dir > 1 && ball_dir < 6)
         last_pos = 0;
      lineback();//라인 밖으로 나가면 돌아가게 됨
      int ultra_gap =0;
      if(max_ir>165 && (ball_dir == 0 || ball_dir == 1 || ball_dir == 11))//공을 잡았을 경우 골대 찾아서 앞으로 나감 
      {
       ultra_gap = (int)((float)ultra[1]*0.34) - (int)((float)ultra[3]*0.34);
      int comp;
      read_compass();
      comp = (int)compass / 10;
      comp = comp - 180 - ultra_gap;
      
      move(100+comp*KP, comp*KP, -100+comp*KP);
      
      dir_move(0, 100);
      compass_move(100, -ultra_gap*2, -100);
      }
      
      else if(ultra[2]<10) // 공오는 방향으로 골대에서만 움직이게&&울트라랑 골대사이 센서값 측정해야됨
      {
         if(ball_dir== 0) move(0,0,0);
         else if(ball_dir==1||ball_dir==2||ball_dir==3||ball_dir==4|| ball_dir==5) dir_move(3,100);
         else if(ball_dir==7||ball_dir==8||ball_dir==9||ball_dir==10||ball_dir==11) dir_move(9,100);
      }
      /*else if(ultra[2]>50 && max_ir>=140)//ir값 다시 측정 공이 완전 가까이 있을때
      {
         ball_near(ball_dir, 100);
      }
      else
      {
         comeback();
      }
       else // 골대 한 번 벗어나면 공격형 프로그램으로 전환
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
