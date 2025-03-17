/****************************************************************
 Arduino Pin  APDS-9960 Board  Function
 
 3.3V         VCC              Power
 GND          GND              Ground
 A4           SDA              I2C Data
 A5           SCL              I2C Clock
 2            INT              Interrupt

****************************************************************/

#include <Wire.h>
#include <SparkFun_APDS9960.h>
#include <LiquidCrystal.h>
#define APDS9960_INT    2 // Needs to be an interrupt pin
const int rs = 12, en = 11, d4 = 6, d5 = 5, d6 = 4, d7 = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
SparkFun_APDS9960 apds = SparkFun_APDS9960();
int isr_flag = 0;
char* myMenu[]={"BULB","TV  ","FAN "};
int BULB = 8;
int TV = 9;
int FAN = 10;
int a=0,b=0,c=0,d=0,f=0;
int i=2,j=0,k=0,pos=1;
int SW = 7;
byte left[8] = {
                  0b10000,
                  0b11000,
                  0b11100,
                  0b11110,
                  0b11110,
                  0b11100,
                  0b11000,
                  0b10000
                };
                
byte right[8] = {
                  0b00001,
                  0b00011,
                  0b00111,
                  0b01111,
                  0b01111,
                  0b00111,
                  0b00011,
                  0b00001
                };

void setup() 
{
  pinMode(BULB, OUTPUT);
  pinMode(TV, OUTPUT);
  pinMode(FAN, OUTPUT);
  pinMode(APDS9960_INT, INPUT);
  pinMode(SW, INPUT);
  digitalWrite(BULB, LOW);
  digitalWrite(TV, LOW);
  digitalWrite(FAN, LOW);
  
  lcd.begin(16,2);
  lcd.createChar(0, left);
  lcd.createChar(1, right);
  lcd.setCursor(0,0);
  lcd.print("Project For YOU");
  lcd.clear();
  Serial.begin(9600);
  Serial.println();
  Serial.println(F("--------------------------------"));
  Serial.println(F("SparkFun APDS-9960 - GestureTest"));
  Serial.println(F("--------------------------------"));
  
  // Initialize interrupt service routine
  attachInterrupt(0, interruptRoutine, FALLING);

  // Initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    Serial.println(F("APDS-9960 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }
  
  // Start running the APDS-9960 gesture sensor engine
  if ( apds.enableGestureSensor(true) ) {
    Serial.println(F("Gesture sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during gesture sensor init!"));
  }
}

void loop()
{
  while(pos==1)
  {
    lcd.setCursor(0,0);
    lcd.print("Project For YOU");
    lcd.setCursor(0,1);
    lcd.print("     WEL-COME    ");
    if(digitalRead(SW)==HIGH)
          {
            lcd.clear();
            delay(500);
            pos=2;
            break;
          }
          else
          {
            pos=1;
          }
   }

    lcd.setCursor(0,0);
    lcd.print("Project For YOU");
    
    if( isr_flag == 1 ) 
      {
         detachInterrupt(0);
         handleGesture();
         isr_flag = 0;
         attachInterrupt(0, interruptRoutine, FALLING);
         controlAppl();
      }
      i=2;
      k=0;
      lcd.setCursor(6,1);
      lcd.print(myMenu[j]);
    // if(digitalRead(swtch)==LOW)
     //{
      //pos=1;
     //}
            
      if(j<2)
      {
        lcd.setCursor(11,1);
        lcd.write(byte(0));
      }
      if(j>0)
      {
        lcd.setCursor(5,1);
        lcd.write(byte(1));
      }
      if(j<=0)
      {
        lcd.setCursor(5,1);
        lcd.print(" ");
      }
       if(j>=2)
      {
        lcd.setCursor(11,1);
        lcd.print(" ");
      }
}

void interruptRoutine() 
{
  isr_flag = 1;
}



void controlAppl()
{
  if(j==0 && i==1)
    {
      if(a==0)
      {
        digitalWrite(BULB, LOW);
        a=1;
      }
    }
    
  if(j==0 && i==0)
    {
      if(a==1)
      {
        digitalWrite(BULB, HIGH);
        a=0;
      }
    } 
    
  if(j==1 && i==1)
    {
      if(b==0)
      {
        digitalWrite(TV, LOW);
        b=1;
      }
    } 
    
  if(j==1 && i==0)
    {
      if(b==1)
      {
        digitalWrite(TV, HIGH);
        b=0;
      }
    } 
    
  if(j==2 && i==1)
    {
      if(c==0)
      {
        digitalWrite(FAN, LOW);
        c=1;
      }
    } 
    
  if(j==2 && i==0)
    {
      if(c==1)
      {
        digitalWrite(FAN, HIGH);
        c=0;
      }
    } 
 /******
 if((k==1 || k==3) && f==0)
  {
      digitalWrite(BULB1, HIGH);
     digitalWrite(BULB2, HIGH);
     digitalWrite(FAN, HIGH);
    }  
       
  if((k==2 || k==3) && f==0)
   {
      digitalWrite(BULB1, LOW);
      digitalWrite(BULB2, LOW);
      digitalWrite(FAN, LOW);
    }  
   *******/   
}

void handleGesture() 
{
    lcd.setCursor(6,1);
    lcd.print("    ");
    
    if ( apds.isGestureAvailable() ) 
    {
    switch ( apds.readGesture() )
    {
      case DIR_UP:
        Serial.println("UP");
        i=0;
        break;
      case DIR_DOWN:
        Serial.println("DOWN");
        i=1;
        break;
      case DIR_LEFT:
        Serial.println("LEFT");
        if(j>0)
        {
          j--;
          delay(200);
        }
        break;
      case DIR_RIGHT:
        Serial.println("RIGHT");
        if(j<2)
        {
          j++;
          delay(200);
        }
        break;
      case DIR_NEAR:
        k=1;
        Serial.println("NEAR");
        break;
      case DIR_FAR:
        k=2;
        Serial.println("FAR");
        break;
      default:
        Serial.println("NONE");  
        k=3;
    }
  }
}
