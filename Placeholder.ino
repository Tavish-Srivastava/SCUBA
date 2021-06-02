/*
 * Tavish Srivastava 
 * tsrivas1@binghamton.edu
 * tavishsrivastava87@gmail.com
 * Scuba placeholder code
 * 
 * This code has 'simulated' aspects in it and must
 * be filled in as the project progresses;
 * 
 * List of problems:
 * - Pressure sensor is being simulated due to experimental limitations --> 1 ----!!!!!!
 * - Data must be sent to the buddy diver in certain places, example; when diver fails test --> 2 -----!!!!!!
 *    We have not been able to perfect underwater wireless communication, 
 *    please add sending the message in the appropirate spots.
 * - For debugging purposes only the diver-diver, Intermediate mode prints to the LCD and administers the actual test ---> 3 -----!!!!!
 *    Easy fix, just change all serial prints to lcd prints.
 *    There is a function at the called CFFF(), call that in places instead of simulation tests.
 * - 
*/
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
int i = 0;
long randNumber;
int buttonState = 0;
int results[5];
float baseline = -1;
unsigned long time;
unsigned long prev_time;
int mode_flag = -1;// 0 -> Inst-Stud & 1-> Diver-Diver
int self_flag = -1;//0 -> Instructor & 1-> Student
int sim_CFFF = -1;
int sim_score = -1;
int diff_flag = -1;
int BI_flag = -1;
LiquidCrystal_I2C lcd(0x27, 16, 2);
int sda = 4;
int scl = 3;

void setup() {
 lcd.begin();
 lcd.backlight();
 pinMode(9, OUTPUT);//LED
 pinMode(11, INPUT);//Button #1
 pinMode(7,INPUT);//Button #2
 pinMode(13,INPUT);//Button #3
 Serial.begin(9600);
}
void loop() {
  for(int k=0;k<5;k++){
    unsigned long del=0;
    unsigned long test  = 1000;
    float HZ = 55 ;
    if(k%2==0){
      HZ=85;
    }
//Select Mode:
//--------------------------------------------------------------------------------------------------------------------------------
    lcd.setCursor(0,0);
    lcd.print(F("Please select"));
    lcd.setCursor(0,1);
    lcd.print(F("desired mode"));
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(F("1-> Inst-Student"));
    lcd.setCursor(0,1);
    lcd.print(F("2-> Diver-Diver"));
    while(digitalRead(11)==LOW && digitalRead(12)==LOW){
      //waiting for input  
    }
  
    if(digitalRead(11)==HIGH){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print( F("Mode selected:"));
      lcd.setCursor(0,1);
      lcd.print( F("Inst-stud mode"));
      delay(1000);
      mode_flag = 0;
    }
    if(digitalRead(12)==HIGH){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print( F("Mode selected:"));
      lcd.setCursor(0,1);
      lcd.print( F("Diver-Diver mode"));
      delay(1000);
      mode_flag = 1;
    }
    delay(500);
//-------------------------------------------------------------------------------------------------------------------------------
//MODE==Inst-Stud 

//Select self:
    //--------------------------------------------------------------------------------------
    if(mode_flag == 0){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print( F("Next,please"));
      lcd.setCursor(0,1);
      lcd.print( F("Identify yourself"));
      delay(2000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print( F("1-> Instructor"));
      lcd.setCursor(0,1);
      lcd.print( F("3-> Student"));
      while(digitalRead(11)==LOW && digitalRead(12)==LOW){
        //waiting for input  
      }
      if(digitalRead(11)==HIGH){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print( F("Self: Instuctor\n"));
        self_flag = 0;
      }
      if(digitalRead(12)==HIGH){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print( F("Self: Student\n"));
        self_flag = 1;
      }
    }
    delay(1000);
    //--------------------------------------------------------------------------------------
    //SELF == INSTRUCTOR
    if(self_flag == 0){
    while(1){
      Serial.print( F("Please press button 3 to deploy test to student\n"));
      while(digitalRead(12)==LOW){
        //waiting for input 
      }
      Serial.print( F("Test has been deployed to student -- awaiting response\n"));
      delay(5000);
      sim_CFFF = random(65,77);
      sim_score = random(0,2);
      if(sim_score==0){// 0 -> Student passed CFFF test
        Serial.print( F("Student CFFF test score: "));
        Serial.print(sim_CFFF);
        Serial.print( F("\nStudent has passed the test\n"));
      }
      else if(sim_score==1){// 0 -> Student passed CFFF test but not with good results
        Serial.print( F("Student CFFF test score: "));
        Serial.print(sim_CFFF);
        Serial.print( F("\nStudent has passed the test but has a low score, please wait a few minutes before descending furthur\n"));
      }
      else{// 2 -> Student failed CFFF test 
        Serial.print( F("Student CFFF test score: "));
        Serial.print(sim_CFFF);
        Serial.print( F("\nStudent has passed NOT passed test -- Please check on student\n"));
      }
    }
    }
    //-----------------------------------------------------------------------------------------
    //SELF == STUDENT
      if(self_flag == 1){
      Serial.print( F("Please take your baseline test -- press button 3 when ready\n"));
      while(digitalRead(12)==LOW){
        //waiting for input 
      }
      Serial.print( F("Simulating test score:\n"));
      delay(5000);
      sim_CFFF = random(65,77);
      Serial.print( F("Baseline CFFF test score: "));
      Serial.print(sim_CFFF);
      Serial.print( F("\nStand-by till input recieved from instructor---(3)\n"));
      while(digitalRead(12)==LOW){
        //waiting for input 
      }
      while(1){
        delay(2000);
        Serial.print( F("---------- Instructor input recieved ----------------\n"));
        Serial.print( F("Please take the test -- press button 3 when ready\n"));
        while(digitalRead(12)==LOW){
          //waiting for input 
        }
        float RED_checker = sim_CFFF - (sim_CFFF*0.1); //baseline - 10%
        float YELLOW_checker = sim_CFFF - (sim_CFFF*0.05);//baseline - 5%
        Serial.print( F("Red cuttoff = "));
        Serial.print(RED_checker);
        Serial.print( F("\nYellow cuttoff = "));
        Serial.print(YELLOW_checker);
        Serial.print( F("\n"));
        int new_sim_CFFF = random(58,80);
        if(new_sim_CFFF >= (int) YELLOW_checker){// Student passed CFFF test
          Serial.print( F("new CFFF test score: "));
          Serial.print(new_sim_CFFF);
          Serial.print( F("\nStudent has passed the test -- sending results to Instructor\n"));
        }
        else if((new_sim_CFFF< (int) YELLOW_checker) && (new_sim_CFFF >= (int) RED_checker) ){//Student passed CFFF test but not with good results
          Serial.print( F("new CFFF test score: "));
          Serial.print(new_sim_CFFF);
          Serial.print( F("\nStudent has passed the test but has a low score -- sending results to Instructor\n"));
        }
        else{//Student failed CFFF test 
          Serial.print( F("new CFFF test score: "));
          Serial.print(new_sim_CFFF);
          Serial.print( F("\nStudent has passed NOT passed test -- sending results to Instructor\n"));
          }
          Serial.print( F("******* Please wait for Instructor input **********\n"));
          delay(3000);
        }
        //-------------------------------------------------------------------------------------------------
      }
 //MODE==Instructor-Student complete --------------------------------------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------------------------------
//MODE==Diver-Diver

  if(mode_flag == 1){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print( F("Next, take"));
    lcd.setCursor(0,1);
    lcd.print( F("basline test"));
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print( F("baseline test --"));
    lcd.setCursor(0,1);
    lcd.print( F("->1 when ready"));
    /*
    while(digitalRead(6)==LOW){
        //waiting for input 
      }
    
    lcd.print( F("Simulating test score:\n"));
    delay(3000);
    sim_CFFF = random(65,77);
    */
    baseline = CFFF();
    lcd.clear();
    delay(1000);
    lcd.print( F("Base CFFF: "));
    lcd.setCursor(0,1);
    lcd.print(baseline);
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print( F("select exp level:"));
    delay(1500);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print( F("1-> Beg/Interm"));
    lcd.setCursor(0,1);
    lcd.print( F("3-> Advanced"));
    while(digitalRead(11)==LOW && digitalRead(12)==LOW){
      //waiting for input  
    }
    if(digitalRead(11)==HIGH){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print( F("level selected:"));
      lcd.setCursor(0,1);
      lcd.print( F("Begin/Intermed"));
      diff_flag = 0;
    }
    if(digitalRead(12)==HIGH){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print( F("level selected:"));
      lcd.setCursor(0,1);
      lcd.print( F("Advanced"));
      diff_flag = 1;
    }
    delay(1000);
  }
  
//---------------------------------------------------------------------------------------------
  //DIFFICULTY == BEG/INTER
  if(diff_flag == 0){ //Diff == Beg/Inter
    delay(1000);
    //Serial.print( F("Next, please input the depth-interval at which tests to occur:\n"));
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print( F("1 -> Beginner"));
    lcd.setCursor(0,1);
    lcd.print( F("2 -> Intermediate"));
    while(digitalRead(11)==LOW && digitalRead(12)==LOW){
      //waiting for input  
    }
     if(digitalRead(11)==HIGH){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print( F("Difficulty:"));
      lcd.setCursor(0,1);
      lcd.print( F("Beginner"));
      BI_flag = 0;
      delay(1500);
    }
    if(digitalRead(12)==HIGH){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print( F("Difficulty:"));
      lcd.setCursor(0,1);
      lcd.print( F("Intermediate"));
      BI_flag = 1;
      delay(1500);
    }
   //Beginner--------------------------------------------------------------------------------------------------------------
      if(BI_flag == 0){
        for(int j = 0; j<2; j++){
          Serial.print( F("\nPlease descend X feet.\n")); 
          delay(1500);
          for(int i =0; i<3; i++){
            Serial.print( F("descending.........\n"));
            delay(1050);
          }
          Serial.print( F("--------!!!!ALERT!!!!---------\n"));
          Serial.print( F("You have reached X feet -- Please take your test -- press button 3 when ready\n"));
          while(digitalRead(12)==LOW){
            //waiting for input 
          }
          float RED_checker2 = sim_CFFF - (sim_CFFF*0.1); //baseline - 10%
          float YELLOW_checker2 = sim_CFFF - (sim_CFFF*0.05);//baseline - 5%
          Serial.print( F("Simulating test score:\n"));
          delay(3000);
          Serial.print( F("Red cuttoff = "));
          Serial.print(RED_checker2);
          Serial.print( F("\nYellow cuttoff = "));
          Serial.print(YELLOW_checker2);
          Serial.print( F("\n"));
          int new2_sim_CFFF = random(58,80);
          if(new2_sim_CFFF >= (int) YELLOW_checker2){// Diver passed CFFF test
            Serial.print( F("new CFFF test score: "));
            Serial.print(new2_sim_CFFF);
            Serial.print( F("\nDiver1 passed the test -- sending results to Diver2\n"));
            Serial.print( F("...Continue descend\n"));
            delay(2000);
          }
          else if((new2_sim_CFFF< (int) YELLOW_checker2) && (new2_sim_CFFF >= (int) RED_checker2) ){//Diver passed CFFF test but not with good results
            Serial.print( F("new CFFF test score: "));
            Serial.print(new2_sim_CFFF);
            Serial.print( F("\nDiver1 passed the test but has a low score -- sending results to Diver2\n"));
            Serial.print( F("...Continue descend\n"));
            delay(2000);
          }
          else{//Diver failed CFFF test 
            Serial.print( F("new CFFF test score: "));
            Serial.print(new2_sim_CFFF);
            Serial.print( F("\nDiver1 has passed NOT passed test -- sending results to Diver2\n"));
            Serial.print( F("---------!!!!!STOP DESCEND!!!!---------\n"));
            Serial.print( F("Please ascend X feet.\n"));
            Serial.print( F("Press button 3 to continue:\n"));
            while(digitalRead(12)==LOW){
              //waiting for input 
            }
            }
        }
        delay(1000);
        Serial.print( F("---------------------------------------------------\n"));
        Serial.print( F("MAX DEPTH REACHED -- PRESS RESET TO RESTART PROGRAM\n"));
    }
    //----------------------------------------------------------------------------------------------------------------
    //Intermediate
    if(BI_flag == 1){
        for(int j = 0; j<2; j++){
          int depth = 0;
          if (j==0){
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print( F("Start your")); 
              lcd.setCursor(0,1);
              lcd.print( F("Dive"));
              delay(1500);
            for(int i =0; i<8; i++){
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print( F("depth: "));
              lcd.setCursor(0,1);
              lcd.print(depth);
              lcd.print(" feet.");
              delay(1700);
              depth +=10;
          }
          }
          delay(2000);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print( F("!!!!!ALERT!!!!!!"));
          lcd.setCursor(0,1);
          lcd.print( F("!Depth Reached!"));
          delay(3000);
           lcd.clear();
           lcd.setCursor(0,0);
           lcd.print( F("take test --"));
           lcd.setCursor(0,1);
           lcd.print( F("->1 when ready"));
          float test_CFFF = CFFF();
          float RED_checker2 = baseline - (baseline*0.1); //baseline - 10%
          float YELLOW_checker2 = baseline - (baseline*0.06);//baseline - 5%
          //Serial.print( F("Simulating test score:\n"));
          //delay(3000);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print( F("Red= "));
          lcd.print(RED_checker2);
          lcd.setCursor(0,1);
          lcd.print( F("Yellow= "));
          lcd.print(YELLOW_checker2);
          delay(2000);
          if(test_CFFF >= YELLOW_checker2){// Diver passed CFFF test
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print( F("CFFF score:"));
            lcd.setCursor(0,1);
            lcd.print(test_CFFF);
            delay(2000);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print( F("Diver-passed"));
            lcd.setCursor(0,1);
            lcd.print( F("Continue descend"));
            delay(2000);
          }
          else if((test_CFFF< YELLOW_checker2) && (test_CFFF >= RED_checker2) ){//Diver passed CFFF test but not with good results
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print( F("CFFF score:"));
            lcd.setCursor(0,1);
            lcd.print(test_CFFF);
            delay(2000);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print( F("Diver-Low pass"));
            lcd.setCursor(0,1);
            lcd.print( F("Continue descend"));
            delay(2000);
          }
          else{//Diver failed CFFF test 
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print( F("CFFF score:"));
            lcd.setCursor(0,1);
            lcd.print(test_CFFF);
            delay(2000);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print( F("!!!!!STOP!!!!!!"));
            lcd.setCursor(0,1);
            lcd.print( F("!TEST FAILED!"));
            delay(3000);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print( F("Please ascend"));
            lcd.setCursor(0,1);
            lcd.print( F("10 feet"));
            delay(2000);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print( F("depth: "));
            lcd.setCursor(0,1);
            lcd.print("70 feet.");
            lcd.setCursor(0,0);
            delay(1700);
            lcd.print( F("depth: "));
            lcd.setCursor(0,1);
            lcd.print("60 feet.");
            delay(2000);
            }
        }
        delay(3000);
        //Serial.print( F("---------------------------------------------------\n"));
        //Serial.print( F("MAX DEPTH REACHED -- PRESS RESET TO RESTART PROGRAM\n"));
        int depth2 = 70;
        for(int i =0; i<7; i++){
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print( F("depth: "));
              lcd.setCursor(0,1);
              lcd.print(depth2);
              lcd.print(" feet.");
              delay(1700);
              depth2 +=10;
          }
        delay(2000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Max-depth hit:");
        lcd.setCursor(0,1);
        lcd.print("130 feet.");
        
    }
    
  }
//-----------------------------------------------------------------------------------------------------------------------------------
  //DIFFICULTY == ADVANCED
  if(diff_flag == 1){
    Serial.print( F("Please take your baseline test -- press button 3 when ready\n"));
    while(digitalRead(12)==LOW){
        //waiting for input 
      }
    Serial.print( F("Simulating test score:\n"));
    delay(3000);
    sim_CFFF = random(65,77);
    Serial.print( F("Baseline CFFF test score: "));
    Serial.print(sim_CFFF);
    delay(1500);
    Serial.print( F("\nPlease descend to Max Depth M\n"));
    delay(2000);
    for(int i =0; i<3; i++){
      Serial.print( F("descending.........\n"));
      delay(1050);
    }
    Serial.print( F("---------------------------------------------------\n"));
    Serial.print( F("MAX DEPTH REACHED -- Please press button 3 to deploy test\n"));
    while(digitalRead(12)==LOW){
        //waiting for input 
      }
    
    float RED_checker2 = sim_CFFF - (sim_CFFF*0.1); //baseline - 10%
    float YELLOW_checker2 = sim_CFFF - (sim_CFFF*0.05);//baseline - 5%
    Serial.print( F("Simulating test score:\n"));
    delay(3000);
    Serial.print( F("Red cuttoff = "));
    Serial.print(RED_checker2);
    Serial.print( F("\nYellow cuttoff = "));
    Serial.print(YELLOW_checker2);
    Serial.print( F("\n"));
    int new2_sim_CFFF = random(58,80);
    if(new2_sim_CFFF >= (int) YELLOW_checker2){// Diver passed CFFF test
      Serial.print( F("new CFFF test score: "));
      Serial.print(new2_sim_CFFF);
      Serial.print( F("\nDiver1 passed the test -- sending results to Diver2\n"));
      Serial.print( F("Please press press button 3 to take the test again.\n"));
       while(digitalRead(12)==LOW){
        //waiting for input 
      }
      delay(2000);
     }
    else if((new2_sim_CFFF< (int) YELLOW_checker2) && (new2_sim_CFFF >= (int) RED_checker2) ){//Diver passed CFFF test but not with good results
      Serial.print( F("new CFFF test score: "));
      Serial.print(new2_sim_CFFF);
      Serial.print( F("\nDiver1 passed the test but has a low score -- sending results to Diver2\n"));
      Serial.print( F("Please press press button 3 to take the test again.\n"));
       while(digitalRead(12)==LOW){
        //waiting for input 
      }
      delay(2000);
     }
     else{//Diver failed CFFF test 
       Serial.print( F("new CFFF test score: "));
       Serial.print(new2_sim_CFFF);
       Serial.print( F("\nDiver1 has passed NOT passed test -- sending results to Diver2\n"));
       Serial.print( F("---------!!!!!STOP DESCEND!!!!---------\n"));
       Serial.print( F("Please ascend\n"));
       Serial.print( F("Press button 3 to continue:\n"));
       while(digitalRead(12)==LOW){
              //waiting for input 
       }
     }
  } 
//--------------------------------------------------------------------------------------------------------------------------------    
     while(digitalRead(11)==LOW){
          //if(HZ==55){exit(-1);}
          digitalWrite(9, HIGH);   
          delay(1000/HZ);
          del= del + (1000/HZ);                  
          digitalWrite(9, LOW); 
          delay(1000/HZ);
          del= del + (1000/HZ);
          if(del>test){
            if(k%2!=0){
              HZ = HZ + 1;
            }
            else{
              HZ = HZ - 1;  
            }
            del = 0;  
          }
     }
     while(digitalRead(11)==HIGH){
          if(k==0){Serial.print( F("TEST DEPLOYED: \n"));}
          
          else{
          Serial.print( F("Reading: "));
        
          Serial.print( F("\n"));
          //results[k]=HZ;
          }
          delay(1000);
     }
 }
 digitalWrite(9, LOW); 
}

float CFFF(){
  for(int k=0;k<5;k++){
    unsigned long del=0;
    unsigned long test  = 500;
    float HZ = 55 ;
    if(k%2==0){
      HZ=85;
    }
    
     while(digitalRead(11)==LOW){
          digitalWrite(9, HIGH);   
          delay(1000/HZ);
          del= del + (1000/HZ);                  
          digitalWrite(9, LOW); 
          delay(1000/HZ);
          del= del + (1000/HZ);
          if(del>test){
            if(k%2!=0){
              HZ = HZ + 0.5;
            }
            else{
              HZ = HZ - 0.5;  
            }
            del = 0;  
          }
     }
     while(digitalRead(11)==HIGH){
          if(k==0){lcd.clear();lcd.setCursor(0,0);lcd.print("TEST DEPLOYED:");}
          else{
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Reading ");
              lcd.print(k);
              lcd.setCursor(0,1);
              lcd.print(HZ);
              results[k]=HZ;
          }
            digitalWrite(9, LOW);
            delay(2000);
            lcd.clear();
     }
 }
 float ret = (results[1]+results[2]+results[3]+results[4])/4;
 return ret;
}
//Alert demo
    /*
    lcd.setCursor(0,0);
    lcd.print(F("Alert!-Buddy"));
    lcd.setCursor(0,1);
    lcd.print(F("failed test"));
    while(digitalRead(11)==LOW && digitalRead(10)==LOW){
      //waiting for input 
      digitalWrite(13, HIGH);
      delay(200);
      digitalWrite(13, LOW);
      delay(200); 
    }
    */
