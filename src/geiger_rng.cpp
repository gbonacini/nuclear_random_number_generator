// -----------------------------------------------------------------
// GeigerRng - A Random Number Generator using an Arduino Geiger Shield
// Copyright (C) 2022  Gabriele Bonacini
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
// -----------------------------------------------------------------

#include <geiger_rng.hpp>

namespace geigerrng {

    long           GeigerRng::count{0};
    long           GeigerRng::genCount{0};
    long           GeigerRng::lastCount{0};
    GeigerRng*     GeigerRng::instance{nullptr};
    unsigned int   GeigerRng::roulette{0};
    unsigned short GeigerRng::lastRnd{GeigerRng::INVALID_RESULT};
    const char     GeigerRng::VALUES[MAX_RESULT+1] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

    void GeigerRng::increaseCount(void) noexcept{
           detachInterrupt(0);
           GeigerRng::count++;
           GeigerRng::genCount++;
           GeigerRng::lastRnd  = GeigerRng::roulette % ( MAX_RESULT + 1 );
           while(digitalRead(GEIGER_PIN)==0);  
           attachInterrupt(0,GeigerRng::increaseCount,FALLING);
    }

    GeigerRng::GeigerRng(unsigned int speed)  noexcept 
        : ledArray{10,11,12,13,9},
          consoleSpeed{speed},
          geiger_input{GEIGER_PIN},
          countPerMinute{0},
          timePrevious{0},
          timePreviousMeasure{0},
          timePreviousPrint{0},
          time{0},
          countPrevious{0},
          radiationValue{0.0},
          lcd(3,4,5,6,7,8)
    {

          Serial.begin(consoleSpeed);
          pinMode(geiger_input, INPUT);
          digitalWrite(geiger_input,HIGH);
          for(int i{0};i<5;i++)
               pinMode(ledArray[i],OUTPUT);

          lcd.begin(16, 2);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("RNG Geiger App");
          lcd.setCursor(0,1);
          lcd.print("2022 - GBonacini");  
          delay(3000);

          lcd.clear();  
          lcd.setCursor(0, 0);
          lcd.print("Starting...");

          attachInterrupt(0, GeigerRng::increaseCount, FALLING);
    }

    void GeigerRng::ledVar(int value){
          if (value > 0){
            for(int i{0};i<=value;i++)
              digitalWrite(ledArray[i],HIGH);
            
            for(int i{5};i>value;i--)
              digitalWrite(ledArray[i],LOW);
          } else {
            for(int i{5};i>=0;i--)
              digitalWrite(ledArray[i],LOW);
          }
    }

    void GeigerRng::detect(void){
           GeigerRng::roulette++;

           if(millis() - timePreviousPrint > GeigerRng::PRINT_RATE){
               timePreviousPrint = millis();
               if(GeigerRng::lastCount !=  GeigerRng::genCount && 
                  GeigerRng::lastRnd   !=  GeigerRng::INVALID_RESULT)
                    Serial.println(GeigerRng::VALUES[GeigerRng::lastRnd]);
               
               GeigerRng::lastCount = GeigerRng::genCount;
           }

           if(millis() - timePreviousMeasure > GeigerRng::POLLING_RATE){
                 countPerMinute = GeigerRng::POLLING_SLOTS * GeigerRng::count;
                 radiationValue = countPerMinute * TUBE_CONV_FACTOR;
                 timePreviousMeasure = millis();
                 Serial.print("CPM = "); 
                 Serial.print(countPerMinute,DEC);
                 Serial.print(" - ");
                 Serial.print("uSv/h = ");
                 Serial.println(radiationValue,4);      
                 lcd.clear();    
                 lcd.setCursor(0, 0);
                 lcd.print("CPM=");
                 lcd.setCursor(4,0);
                 lcd.print(countPerMinute);
                 lcd.setCursor(8,0);
                 lcd.print("RND=");
                 lcd.setCursor(12,0);
                 if(GeigerRng::lastRnd != GeigerRng::INVALID_RESULT)
                     lcd.print(GeigerRng::VALUES[GeigerRng::lastRnd]);
                 else
                     lcd.print("NONE");
                 lcd.setCursor(0,1);
                 lcd.print(radiationValue,4);
                 lcd.setCursor(6,1);
                 lcd.print(" uSv/h");

                 // Led bar setting  
                 if(countPerMinute  <= THRESHOLD[0]) ledVar(0);
                 if((countPerMinute <= THRESHOLD[1]) && (countPerMinute>THRESHOLD[0])) ledVar(1);
                 if((countPerMinute <= THRESHOLD[2]) && (countPerMinute>THRESHOLD[1])) ledVar(2);
                 if((countPerMinute <= THRESHOLD[3]) && (countPerMinute>THRESHOLD[2])) ledVar(3);
                 if((countPerMinute <= THRESHOLD[4]) && (countPerMinute>THRESHOLD[3])) ledVar(4);
                 if(countPerMinute  >  THRESHOLD[4]) ledVar(5);

                 GeigerRng::count    = 0;
            }
    }

    GeigerRng* GeigerRng::getInstance(unsigned int speed){
        if(instance == nullptr)
            instance = new GeigerRng(speed);

        return instance;
    }
}