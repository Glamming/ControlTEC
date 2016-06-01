void holdTempMenu(){
  
  while (whileLoop == 0) {
    
    //Get thermistor readings.
    externalTemp = externalThermistor()+(EEPROM_External_Temp_Calib);
    internalTemp = Thermistor(analogRead(INTERNAL_THERM_PIN))+(EEPROM_Internal_Temp_Calib*10);
    
    lcd.setCursor(0, 0);
    lcd.print("Set temp: ");
    lcd.print(String(holdTempSetTemp));
    lcd.print(" C  ");
    
    lcd.setCursor(0, 1);
    lcd.print("E:");
    lcd.print(externalTemp, 1);
    lcd.print(" C ");
    lcd.print("I:");
    lcd.print(internalTemp/10);
    lcd.print(" C  ");
   
   //Set button states to determine what is being pressed.
    actionBtnState = digitalRead(actionBtn);
    upBtnState = digitalRead(upBtn);
    downBtnState = digitalRead(downBtn);
    
    if (upBtnState == HIGH){
      btnHoldTime = 0;
      while (upBtnState == HIGH){
        upBtnState = digitalRead(upBtn);
        delay(50);
        btnHoldTime ++;
        if (btnHoldTime >= 20) {
          if(holdTempSetTemp < maxSafeTemp){
            holdTempSetTemp ++;
            lcd.setCursor(0, 0);
            lcd.print("Set temp: ");
            lcd.print(String(holdTempSetTemp));
            lcd.print(" C  ");
          }
        }
      }
      btnHoldTime = 0;
      if(holdTempSetTemp < maxSafeTemp){
        holdTempSetTemp ++;
      }
    }
    
    if (downBtnState == HIGH){
      btnHoldTime = 0;
      while (downBtnState == HIGH){
        downBtnState = digitalRead(downBtn);
        delay(50);
        btnHoldTime ++;
        if (btnHoldTime >= 20) {
          if(holdTempSetTemp > minSafeTemp){
            holdTempSetTemp --;
            lcd.setCursor(0, 0);
            lcd.print("Set temp: ");
            lcd.print(String(holdTempSetTemp));
            lcd.print(" C  ");
          }
        }
      }
      btnHoldTime = 0;
      if(holdTempSetTemp > minSafeTemp){
        holdTempSetTemp --;
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
  
        
        //Set thermostat  
        thermostatActive = 1;
        lastExternalTemp = externalTemp;
        thermostatSetTemperature = holdTempSetTemp;

        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Temperature set");
        lcd.setCursor(6, 1);
        lcd.print(holdTempSetTemp);
        lcd.print(" C     ");
        delay(1400);
      }
    }
    
    if (thermostatActive == 1) {
      thermostatControl(thermostatSetTemperature);
    }
    
    delay(1);
  }
}
