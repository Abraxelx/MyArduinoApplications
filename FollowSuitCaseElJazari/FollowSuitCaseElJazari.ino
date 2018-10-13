#include <Wire.h>
#include <Pixy.h>
#include <SPI.h> 

int enA = 6;
int enB=5;
int RF= 7;
int RB = 4;
int LF= 2;
int LB= 3;



Pixy pixy;
char value;

#define X_CENTER        ((PIXY_MAX_X-PIXY_MIN_X)/2)       
#define Y_CENTER        ((PIXY_MAX_Y-PIXY_MIN_Y)/2)
class ServoLoop
{
public:
  ServoLoop(int32_t pgain, int32_t dgain);

  void update(int32_t error);
   
  int32_t m_pos;
  int32_t m_prevError;
  int32_t m_pgain;
  int32_t m_dgain;
};


ServoLoop panLoop(300, 500);
ServoLoop tiltLoop(500, 700);

ServoLoop::ServoLoop(int32_t pgain, int32_t dgain)
{
  m_pos = PIXY_RCS_CENTER_POS;
  m_pgain = pgain;
  m_dgain = dgain;
  m_prevError = 0x80000000L;
}

void ServoLoop::update(int32_t error)
{
  long int vel;
  char buf[32];
  if (m_prevError!=0x80000000)
  {  
    vel = (error*m_pgain + (error - m_prevError)*m_dgain)>>10;
    //sprintf(buf, "%ld\n", vel);
    //Serial.print(buf);
    m_pos += vel;
    if (m_pos>PIXY_RCS_MAX_POS) 
      m_pos = PIXY_RCS_MAX_POS; 
    else if (m_pos<PIXY_RCS_MIN_POS) 
      m_pos = PIXY_RCS_MIN_POS;
  }
  m_prevError = error;
}

void setup() {
   pinMode(enA, OUTPUT);
   pinMode(enB, OUTPUT);
   pinMode(RF, OUTPUT);
   pinMode(RB, OUTPUT);
   pinMode(LF, OUTPUT);
   pinMode(LB, OUTPUT);
 Serial.begin(9600);
  Serial.print("Starting...\n");
  
  pixy.init();

}



void loop() { 

static int i = 0;
  int j;
  uint16_t blocks;
  char buf[32]; 
    int32_t panError, tiltError;

if(Serial.available()>0)
 {
  value = Serial.read();
  //Serial.println(value);
  if(value=='a')
  {
    go();
  }
   else if(value=='d')
   {   
    gauche();

  }
  else if(value=='b')
  {
    droite();
  }
  else if(value=='e')
  {
    o9if();
  } 
    else if(value=='c')
  {
    back();
  } 
      else if(value=='f')
  {
    FollowMode();
  } 
  
 }
 else{
  blocks = pixy.getBlocks();
  
  if (blocks)
  {
        panError = X_CENTER-pixy.blocks[0].x;
    tiltError = pixy.blocks[0].y-Y_CENTER;
    
    panLoop.update(panError);
    tiltLoop.update(tiltError);
    
    pixy.setServos(panLoop.m_pos, tiltLoop.m_pos);
    
    i++;
    
    // do this (print) every 50 frames because printing every
    // frame would bog down the Arduino
    if (i%50==0)
    {
      sprintf(buf, "Detected %d:\n", blocks);
      Serial.print(buf);
      for (j=0; j<blocks; j++)
      {
        sprintf(buf, "  block %d: ", j);
        Serial.print(buf); 
        pixy.blocks[j].print();
 
      }
  
    }
           if((pixy.blocks[0].x>120)&&(pixy.blocks[0].x<190)) {
  go();}
 if((pixy.blocks[0].x<120) && (pixy.blocks[0].x>10)){
  gauche();
  }
  if(pixy.blocks[0].x>190) {
  droite();}
  
  } 
else{o9if();}

}
}
 

 


  
/*  else{o9if();
  delay(2000);}
  */




void droite(){
          analogWrite(enA,210);
          analogWrite(RF,255);
          analogWrite(LF,0);
          analogWrite(RB,0);
          analogWrite(LB,0);  
            delay(30);}
void gauche(){
          analogWrite(enB,210);
          analogWrite(LF,255);
          analogWrite(LB,0);
          analogWrite(RF,0);
          analogWrite(RB,0);
            delay(30);}
void o9if(){
          analogWrite(RF,0);
          analogWrite(RB,0);
          analogWrite(LF,0);
          analogWrite(LB,0);
         delay(30); }
void go(){
          analogWrite(enA,210);
          analogWrite(enB,210);
          analogWrite(LF,255);
          analogWrite(LB,0);
          analogWrite(RF,255);
          analogWrite(RB,0);
         delay(30); }
		 
void back (){
          analogWrite(enA,210);
          analogWrite(enB,210);
          analogWrite(LF,0);
          analogWrite(LB,255);
          analogWrite(RF,0);
          analogWrite(RB,255);
         delay(30); }
		 
void FollowMode()
{
	static int i = 0;
  int j;
  uint16_t blocks;
  char buf[32]; 
    int32_t panError, tiltError;
	  blocks = pixy.getBlocks();
  
  if (blocks)
  {
        panError = X_CENTER-pixy.blocks[0].x;
    tiltError = pixy.blocks[0].y-Y_CENTER;
    
    panLoop.update(panError);
    tiltLoop.update(tiltError);
    
    pixy.setServos(panLoop.m_pos, tiltLoop.m_pos);
    
    i++;
    
    // do this (print) every 50 frames because printing every
    // frame would bog down the Arduino
    if (i%50==0)
    {
      sprintf(buf, "Detected %d:\n", blocks);
      Serial.print(buf);
      for (j=0; j<blocks; j++)
      {
        sprintf(buf, "  block %d: ", j);
        Serial.print(buf); 
        pixy.blocks[j].print();
 
      }
  
    }
   if((pixy.blocks[0].x>120)&&(pixy.blocks[0].x<190)) {
  go();}
 if((pixy.blocks[0].x<120) && (pixy.blocks[0].x>10)){
  gauche();
  }
  if(pixy.blocks[0].x>190) {
  droite();}
  
  } 
else{o9if();}

}


