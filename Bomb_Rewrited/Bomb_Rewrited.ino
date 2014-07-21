/**********************************************

  *Bomb V1.1
  *Author: Lingxi
  *Date: 2014/07/21
  *Build Version: 07211115-2
  *FOR TEST ONLY. ILLEGAL USE IS NOT ALLOWED.
  
***********************************************/

//Define Pins
int latchPin = 4;
int clockPin = 7;
int dataPin = 8;
int KEY1 = A1;
int KEY2 = A2;
int KEY3 = A3;
int sonic_inputPin = 5;
int sonic_outputPin = 6;
int buzzer = 3;

//Define Transistors' buffer
unsigned char Dis_table[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90};
unsigned char Dis_buf[]   = {0xF1,0xF2,0xF4,0xF8};
unsigned char disbuff[]  =  {0, 0, 0, 0};

//Define Button Module
int Flag_up_1 = 1;
int Flag_up_2 = 1;
int Flag_up_3 = 1;

//Define Counter Module
#define COUNTS 15
#define SECOND 100
#define BUZZER_TIME 10
int count = COUNTS;  //Set Counter's Second Here!
#define APPROCH 30  //Activate Counter When Extrasonic-monitored Data Lower Than This Value (cm)
//Define Extrasonic Module
long distance = 0;

void setup()
{

  Serial.begin(9600);  //Enable When Debugging
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(sonic_inputPin,INPUT);
  pinMode(sonic_outputPin,OUTPUT);
  
}

//Display Numbers In Transistors
void display()
{
  for(char i=0; i<=3; i++)  //Scan Transistors
  {
    digitalWrite(latchPin,LOW); 
    shiftOut(dataPin,clockPin,MSBFIRST,Dis_table[disbuff[i]]); 
    shiftOut(dataPin,clockPin,MSBFIRST,Dis_buf[i] ); 
    digitalWrite(latchPin,HIGH); 
    delay(2);
  }
  
}


//Obtain Distance From Extrasonic Module
unsigned long sonic()
{
    digitalWrite(sonic_outputPin, LOW); // Send Vil For 2us
    delayMicroseconds(2);
    digitalWrite(sonic_outputPin, HIGH); // Send Vih For 10us And At Least 10us
    delayMicroseconds(10);
    digitalWrite(sonic_outputPin, LOW);    // Keep Vil
    long distance = pulseIn(sonic_inputPin, HIGH);  // Read Pluse Time
    distance= distance/58;   // Convert Pluse Time Into Distance With CM
    Serial.println(distance);  //Enable When Debugging, Print Distance In Serial Interface
    return distance;
}

//Scan KEY1 When Pressed
unsigned char ScanKey1()
{
  if(Flag_up_1  && digitalRead(KEY1) == LOW)  //When Flag_up_1 = 1 And Pressed
  {
    Flag_up_1 = 0;   //Clear Flag_up_1
    display();
    display();  //Delay
    if(digitalRead(KEY1) == LOW)  //When Pressed
    {
      return 1;
    }
  }
  if(digitalRead(KEY1) == HIGH)	  //Unpressed
  {
    Flag_up_1 = 1;  //Reset Flag_up_1
  }
  return 0;
}

//Scan KEY2 When Pressed
unsigned char ScanKey2()
{
  if(Flag_up_2  && digitalRead(KEY2) == LOW)  //When Flag_up_2 = 1 And Pressed
  {
    Flag_up_2 = 0;   //Clear Flag_up_2
    display();
    display();  //Delay
    if(digitalRead(KEY2) == LOW)  //When Pressed
    {
      return 1;
    }
  }
  if(digitalRead(KEY2) == HIGH)	  //Unpressed
  {
    Flag_up_2 = 1;  //Reset Flag_up_2
  }
  return 0;
}

//Scan KEY3 When Pressed
unsigned char ScanKey3()
{
  if(Flag_up_3  && digitalRead(KEY3) == LOW)  //When Flag_up_3 = 1 And Pressed
  {
    Flag_up_3 = 0;   //Clear Flag_up_3
    display();
    display();  //Delay
    if(digitalRead(KEY3) == LOW)  //When Pressed
    {
      return 1;
    }
  }
  if(digitalRead(KEY3) == HIGH)	  //Unpressed
  {
    Flag_up_3 = 1;  //Reset Flag_up_3
  }
  return 0;
}

//Counters
void counts()
{
  count = COUNTS;
  while(1){
      //Set Buffer
      disbuff[0]=count/1000;
      disbuff[1]=count%1000/100;
      disbuff[2]=count%100/10;
      disbuff[3]=count%10;
      count--;      
      digitalWrite(buzzer,HIGH); //Buzzer NOT Work
      //Todo: Combine Extrasonic Module To Control Time Elapse
      //Todo: Deal With Count = 0
      for (char time=0;time<SECOND;time++)
          {
               display();
          }

       for(char t=0;t<BUZZER_TIME;t++)
           {
                  digitalWrite(buzzer,LOW);  //Buzzer WORKS
                  display();  //Keep Display
            }
                                
    if( ScanKey2() == 1)
        {
          uncounts();  //Cancel Counting When KEY2 Pressed
         }
  }
}

void uncounts()
{
    while(1){
    digitalWrite(buzzer,HIGH); //Buzzer NOT Work
    count = 0;  //Clear Count Number
    //Flush Buffer
    disbuff[0]=count/1000;
    disbuff[1]=count%1000/100;
    disbuff[2]=count%100/10;
    disbuff[3]=count%10;
    display();
    while(1)
        {
        sonic();  //Keep Extrasonic Monitoring
        if( ScanKey1() == 1)    //Activate Counting When KEY1 Pressed
              {
                  counts();
              }
        }
    }
}


void loop()
{
//display();
digitalWrite(buzzer,HIGH);
if( ScanKey1() == 1)  //Activate Counting When KEY1 Pressed
    {
        counts();
    }
sonic();  //Obtain Distance Data
if (sonic() <= APPROCH and sonic() !=0)  //Activate Counters When Object Approching and Data Vaild
    {
        counts();
    }
}
