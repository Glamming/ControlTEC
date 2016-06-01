
void rampTempMenu(){
  
  byte rampModeSelect = 0;
  byte rampModeSelected = 0;
  byte parameterOptionNumber = 0;
  int tempTempInt = 20;
  
  rampTempStartTemp = 20; //Contains the desired ramp start temperature
  rampTempEndTemp = 20; //Contains the desired ramp end temperature
  rampTime = 30;
  numberOfCycles = 1;
  tempTempInt = rampTempStartTemp; //Set this so the tempTempInt begins with the correct default value
  
  while (whileLoop == 0) {
    
    if (rampModeSelected == 0) {
    
      //Get thermistor readings.
      externalTemp = externalThermistor()+(EEPROM_External_Temp_Calib);
      internalTemp = Thermistor(analogRead(INTERNAL_THERM_PIN))+(EEPROM_Internal_Temp_Calib*10);
    
      lcd.setCursor(0, 0);
      lcd.print("  Select mode:  ");
      lcd.setCursor(0, 1);
    
      if (rampModeSelect == 0) {
        lcd.print("   Ramp temp    ");
      } else {
        lcd.print("   Cycle temp   ");
      }
    
   
     //Set button states to determine what is being pressed.
      actionBtnState = digitalRead(actionBtn);
      upBtnState = digitalRead(upBtn);
      downBtnState = digitalRead(downBtn);
    
      if (upBtnState == HIGH){
        btnHoldTime = 0;
        while (upBtnState == HIGH){
          upBtnState = digitalRead(upBtn);
          delay(50);
        }
        rampModeSelect = 0;
      }
    
      if (downBtnState == HIGH){
        btnHoldTime = 0;
        while (downBtnState == HIGH){
          downBtnState = digitalRead(downBtn);
          delay(50);
      
        }
        rampModeSelect = 1;
      }
    
      if (actionBtnState == HIGH){
        btnHoldTime = 0;
        while (actionBtnState == HIGH){
          actionBtnState = digitalRead(actionBtn);
          delay(50);
          btnHoldTime ++;
          if (btnHoldTime == 20) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Release to exit");
          }
        }
        if (btnHoldTime >= 20) {
          btnHoldTime = 0;
          rampModeSelected = 0; //reset this just in case.
          break;
        } else {
         rampModeSelected = 1; 
        }
      }
      
      
    } else if (rampModeSelected == 1) {
      
      //
      //
      //
      //################################  The user has selected the mode, now get the paramaeters. ###################################
      //
      //
      //parameterOptionNumber == 0 --> rampTempStartTemp
      //parameterOptionNumber == 1 --> rampTempEndTemp
      //parameterOptionNumber == 2 --> rampTime
      //parameterOptionNumber == 2 --> numberOfCycles
      //
      //
      
      
    
      lcd.setCursor(0, 0);     
      
      if (parameterOptionNumber == 0) {
        lcd.print("> Start T: ");
        lcd.print(tempTempInt);
        lcd.print(" C   ");
        
        lcd.setCursor(0, 1);    
        lcd.print("End T: ");
        lcd.print(rampTempEndTemp);
        lcd.print(" C   ");
      } else if (parameterOptionNumber == 1) {
        lcd.print("Start T: ");
        lcd.print(rampTempStartTemp);
        lcd.print(" C   ");
        
        lcd.setCursor(0, 1);    
        lcd.print("> End T: ");
        lcd.print(tempTempInt);
        lcd.print(" C   ");
      } else if (parameterOptionNumber == 2) {
        lcd.print("> Time: ");
        lcd.print(tempTempInt);
        lcd.print(" min   ");
        
        lcd.setCursor(0, 1);    
        lcd.print("# cylces: ");
        lcd.print(numberOfCycles);
        lcd.print("     ");
      } else if (parameterOptionNumber == 3) {
        lcd.print("Time: ");
        lcd.print(rampTime);
        lcd.print(" min   ");
        
        lcd.setCursor(0, 1);    
        lcd.print("> # cylces: ");
        lcd.print(tempTempInt);
        lcd.print("     ");
      }
      
      //Set button states to determine what is being pressed.
      actionBtnState = digitalRead(actionBtn);
      upBtnState = digitalRead(upBtn);
      downBtnState = digitalRead(downBtn);
    
    
      if (upBtnState == HIGH){
        btnHoldTime = 0;
        while (upBtnState == HIGH){
          upBtnState = digitalRead(upBtn);
          lcd.setCursor(0, 0); 
          delay(50);
          btnHoldTime ++;
          if (btnHoldTime >= 20) {
            if (parameterOptionNumber == 0) {  
              
              if(tempTempInt < maxSafeTemp){
                tempTempInt ++;
              }
              
              lcd.print("> Start T: ");
              lcd.print(tempTempInt);
              lcd.print(" C   ");
            
              lcd.setCursor(0, 1);    
              lcd.print("End T: ");
              lcd.print(rampTempEndTemp);
              lcd.print(" C   ");
              
            } else if (parameterOptionNumber == 1) {
             
              if(tempTempInt < maxSafeTemp){
                tempTempInt ++;
              }
             
              lcd.print("Start T: ");
              lcd.print(rampTempStartTemp);
              lcd.print(" C   ");
        
              lcd.setCursor(0, 1);    
              lcd.print("> End T: ");
              lcd.print(tempTempInt);
              lcd.print(" C   ");
              
            } else if (parameterOptionNumber == 2) {
            
              if(tempTempInt < maxRampTime){
                tempTempInt ++;
              }
          
              lcd.print("> Time: ");
              lcd.print(tempTempInt);
              lcd.print(" min   ");
            
        
              lcd.setCursor(0, 1);    
              lcd.print("# cylces: ");
              lcd.print(numberOfCycles);
              lcd.print("     ");
              
            } else if (parameterOptionNumber == 3) {
            
              if(tempTempInt < maxNumberOfCycles){
                tempTempInt ++;
              }
          
              lcd.print("Time: ");
              lcd.print(rampTime);
              lcd.print(" min   ");
        
              lcd.setCursor(0, 1);    
              lcd.print("> # cylces: ");
              lcd.print(tempTempInt);
              lcd.print("     ");
            }
          }
        }
        btnHoldTime = 0;
        if (parameterOptionNumber == 0) {
          if(tempTempInt < maxSafeTemp){
            tempTempInt ++;
          }
        } else if (parameterOptionNumber == 1) {
          if(tempTempInt < maxSafeTemp){
            tempTempInt ++;
          }
         } else if (parameterOptionNumber == 2) {
           if(tempTempInt < maxRampTime){
             tempTempInt ++;
           }
         } else if (parameterOptionNumber == 3) {
           if(tempTempInt < maxNumberOfCycles){
             tempTempInt ++;
           }
         }
       }
      
      
      if (downBtnState == HIGH){
        btnHoldTime = 0;
        while (downBtnState == HIGH){
          downBtnState = digitalRead(downBtn);
          lcd.setCursor(0, 0); 
          delay(50);
          btnHoldTime ++;
          if (btnHoldTime >= 20) {
            if (parameterOptionNumber == 0) {   
              
              if(tempTempInt > minSafeTemp){
                tempTempInt --;
              }
                
              lcd.print("> Start T: ");
              lcd.print(tempTempInt);
              lcd.print(" C   ");
      
              lcd.setCursor(0, 1);    
              lcd.print("End T: ");
              lcd.print(rampTempEndTemp);
              lcd.print(" C   ");
              
            } else if (parameterOptionNumber == 1) {
             
              if(tempTempInt > minSafeTemp){
                tempTempInt --;
              }
               
              lcd.print("Start T: ");
              lcd.print(rampTempStartTemp);
              lcd.print(" C   ");
              
              lcd.setCursor(0, 1);    
              lcd.print("> End T: ");
              lcd.print(tempTempInt);
              lcd.print(" C   ");
              
            } else if (parameterOptionNumber == 2) {
            
              if(tempTempInt > minRampTime){
                tempTempInt --;
              }
              
              lcd.print("> Time: ");
              lcd.print(tempTempInt);
              lcd.print(" min   ");
              
              lcd.setCursor(0, 1);    
              lcd.print("# cylces: ");
              lcd.print(numberOfCycles);
              lcd.print("     ");
            } else if (parameterOptionNumber == 3) {
              
              if(tempTempInt > 1){
                tempTempInt --;
              }
             
              lcd.print("Time: ");
              lcd.print(rampTime);
              lcd.print(" min   ");
        
              lcd.setCursor(0, 1);    
              lcd.print("> # cylces: ");
              lcd.print(tempTempInt);
              lcd.print("     ");
            }
          }
        }
        
        btnHoldTime = 0;
        if (parameterOptionNumber == 0) {
          if(tempTempInt > minSafeTemp){
            tempTempInt --;
          }
        } else if (parameterOptionNumber == 1) {
          if(tempTempInt > minSafeTemp){
            tempTempInt --;
          }
         } else if (parameterOptionNumber == 2) {
           if(tempTempInt > minRampTime){
             tempTempInt --;
           }
         } else if (parameterOptionNumber == 3) {
           if(tempTempInt > 1){
             tempTempInt --;
           }
         }
      }
          
          
      if (actionBtnState == HIGH){
        btnHoldTime = 0;
        while (actionBtnState == HIGH){
          actionBtnState = digitalRead(actionBtn);
          delay(50);
          btnHoldTime ++;
          if (btnHoldTime == 20) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Release to exit");
          }
        }
        if (btnHoldTime >= 20) {
          btnHoldTime = 0;
          parameterOptionNumber = 0; //reset this just in case.
          break;
        } else {
          
          if (parameterOptionNumber == 0) {
            rampTempStartTemp = tempTempInt;
            tempTempInt = rampTempEndTemp;
          } else if (parameterOptionNumber == 1) {
            rampTempEndTemp = tempTempInt;
            tempTempInt = rampTime;
          } else if (parameterOptionNumber == 2) {
            rampTime = tempTempInt;
            tempTempInt = 1;
          } else if (parameterOptionNumber == 3) {
            numberOfCycles = tempTempInt;
          }
          parameterOptionNumber ++;
          
          
          //Make it so
          if (parameterOptionNumber >= 4) {
            //thermostatActive = 1;
            lastExternalTemp = externalTemp;
            thermostatSetTemperature = float(rampTempStartTemp);
            thermostatControl(thermostatSetTemperature);
            
            //A few calculations before we begin
            float degreesPerMinute = (abs(float(rampTempStartTemp) - float(rampTempEndTemp))/float(rampTime));
            
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Rate: ");
            lcd.print(degreesPerMinute, 2);
            lcd.print(" C/min");
            delay(1200);

//          byte numberOfCycles = 1; //Contains the max number of temperature cycles

            while (readyToBeginRamp == 0) {
              //Not ready to go yet
              thermostatControl(thermostatSetTemperature);
              
              externalTemp = externalThermistor()+(EEPROM_External_Temp_Calib);
              internalTemp = Thermistor(analogRead(INTERNAL_THERM_PIN))+(EEPROM_Internal_Temp_Calib*10);
        
              lcd.setCursor(0, 0);
              lcd.print("Set: ");
              lcd.print(thermostatSetTemperature, 1);
              lcd.print(" C  ");
              lcd.setCursor(0, 1);
              lcd.print("Cur: ");
              lcd.print(externalTemp, 1);
              lcd.print(" C ");
              delay(1);
            }
            
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Program started  ");
            
            //Should default to not ready
            readyToBeginRamp = 0;
            delay(1000);
            lcd.clear();
            
            while (whileLoop == 0) {
              
              externalTemp = externalThermistor()+(EEPROM_External_Temp_Calib);
              internalTemp = Thermistor(analogRead(INTERNAL_THERM_PIN))+(EEPROM_Internal_Temp_Calib*10);
              
              lcd.setCursor(0, 0);
              lcd.print("Set: ");
              lcd.print(thermostatSetTemperature, 1);
              lcd.print(" C    ");
              lcd.setCursor(0, 1);
              lcd.print("Cur:");
              lcd.print(externalTemp, 1);
              lcd.print(" C    ");
              
              
              if (fourHertzCounter >= 240) {
                if (rampTempStartTemp > rampTempEndTemp) {
                  //we need to ramp the plate to cooler temperatures
                  thermostatSetTemperature -= degreesPerMinute;
                } else {
                  //we need to ramp the plate to hotter temperatures
                  thermostatSetTemperature += degreesPerMinute;
                }
                //1 minute has passed
                fourHertzCounter = 0;
              }
              
              if (thermostatSetTemperature < rampTempEndTemp) {
                //Program ended
                break; 
              }
              
              thermostatControl(thermostatSetTemperature);
             
            }
            
          }
        }
      }
    }
    
    if (thermostatSetTemperature < rampTempEndTemp) {
      //Program ended
      break;
    }
  }
  
  
  while (whileLoop == 0) {
    //Just hold temperature at the end
    externalTemp = externalThermistor()+(EEPROM_External_Temp_Calib);
    thermostatControl(rampTempEndTemp);
    
    upBtnState = digitalRead(upBtn);
    
    lcd.setCursor(0, 0);
    lcd.print("Program ended   ");
    lcd.setCursor(0, 1);
    lcd.print("Cur:");
    lcd.print(externalTemp, 1);
    lcd.print(" C   ");
   
    if (actionBtnState == HIGH){
      btnHoldTime = 0;
      while (actionBtnState == HIGH) {
        actionBtnState = digitalRead(actionBtn);
        delay(50);
        btnHoldTime ++;
        if (btnHoldTime == 20) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Release to exit");
        }
      }
      if (btnHoldTime >= 20) {
        btnHoldTime = 0;
        parameterOptionNumber = 0; //reset this just in case.
        break;
      }
    }
    delay(10);
  } 
  delay(10);
}
