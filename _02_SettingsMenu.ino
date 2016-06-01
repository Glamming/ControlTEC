void settingsMenu() {
  
  //Re-read EEPROM memory, values can range from 0 - 255
  EEPROM_Internal_Temp_Calib = EEPROM.read(0) - 100; //I'm giving zero a value of 100, normalising this.
  int old_EEPROM_Internal_Temp_Calib = EEPROM_Internal_Temp_Calib;
  
  EEPROM_External_Temp_Calib = EEPROM.read(1) - 100;
  int old_EEPROM_External_Temp_Calib = EEPROM_External_Temp_Calib;
    
  //Position in the settings menu: 0=Internal Temp   1=External Temp
  int settingsMenuPosition = 0;
  int valueToStore = EEPROM_Internal_Temp_Calib;
    
  while (whileLoop == 0) {
    
    if (settingsMenuPosition == 0) {
      //Draw LCD
      lcd.setCursor(0, 0);
      lcd.print("Internal Temp");
      lcd.setCursor(0, 1);
      lcd.print("Calibrate: ");
      lcd.print(valueToStore);
      lcd.print(" C   ");
      
    } else if (settingsMenuPosition == 1) {
      //Draw LCD
      lcd.setCursor(0, 0);
      lcd.print("External Temp");
      lcd.setCursor(0, 1);
      lcd.print("Calibrate: ");
      lcd.print(valueToStore);
      lcd.print(" C   ");
      
    } else {
      //Just in case.
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Error. Overflow.");
      delay(1500);
      break;
    }
    
   //Set button states to determine what is being pressed.
    actionBtnState = digitalRead(actionBtn);
    upBtnState = digitalRead(upBtn);
    downBtnState = digitalRead(downBtn);
    
    if (upBtnState == HIGH){
      while (upBtnState == HIGH){
        upBtnState = digitalRead(upBtn);
        delay(50);
      }
      if(valueToStore < 20){
        valueToStore ++;
      }
    }
    
    if (downBtnState == HIGH){
      while (downBtnState == HIGH){
        downBtnState = digitalRead(downBtn);
        delay(50);
      }
      if(valueToStore > -20){
        valueToStore --;
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
        break;
      } else {
        
        //Set EEPROM Values
        if(settingsMenuPosition == 0) {
          if (old_EEPROM_Internal_Temp_Calib != valueToStore) {
            EEPROM.write(0, int(valueToStore + 100)); //Don't forget to convert back
            EEPROM_Internal_Temp_Calib = valueToStore;
          }
          //Prepare temp variable for next setting
          valueToStore = EEPROM_External_Temp_Calib;
        }
        if(settingsMenuPosition == 1 && old_EEPROM_External_Temp_Calib != valueToStore) {
          EEPROM.write(1, int(valueToStore + 100)); //Don't forget to convert back
          EEPROM_External_Temp_Calib = valueToStore;
        }
        
        //Move through the settings
        settingsMenuPosition ++;
        
        //Have we reached the last setting?
        if (settingsMenuPosition > 1) {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Settings saved.");
          delay(1500);
          break;
        }
      }
    }
    delay(10);
  }
}
