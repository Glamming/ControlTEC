
void fetCalibMenu() {
  
  byte yesNo = 0; //Boolean to check if the user wanted to proceed with the MOSFET calibration on breaking from
                 //the initial while loop.
  
  while (whileLoop == 0) {
   
    lcd.setCursor(0, 0);
    lcd.print("    Proceed?    "); 
    lcd.setCursor(0, 1);
    if (yesNo == 0){
      lcd.print("       No       ");
    } else {
      lcd.print("      Yes        ");
    }
    
    actionBtnState = digitalRead(actionBtn);
    upBtnState = digitalRead(upBtn);
    downBtnState = digitalRead(downBtn);
    
    if (upBtnState == HIGH){
      while (upBtnState == HIGH){
        upBtnState = digitalRead(upBtn);
        delay(50);
      }
      yesNo = 0;
    }
     
  
    if (downBtnState == HIGH){
      while (downBtnState == HIGH){
        downBtnState = digitalRead(downBtn);
        delay(50);
      }
      yesNo = 1;
    }
  
    if (actionBtnState == HIGH){
      //Need a delay to stop double presses.
      delay(500);
      break;
    }
    
    delay(10);
  }
  
  //if the response is yes, proceed to the calibration, if no, just let the funtion come to end
  //and let the void loop, loop to bring back the programMenu.
  
  if ( yesNo == 1) {
    
    //Calibration will be achieved by going from cold to hot
    
    lcd.clear();
    
    mosfetPower = 0; //just use the pre-existing float
    int currentSetPower = -100; //may be useful to have an integer value of current power.
    byte powerIncrements = 10;
    
    byte changePowerLevel = 1; //boolean to check if MOSFET power needs changing on while loop
    whileLoopCounter = 0; //used to count the time between temperature checks - looking for temp stability to take reading.
    
    //We need two arrays to store our recorded data and an int to remember the current read/write location
    byte arrayLocation = 0;
    int extTempArray[((100/powerIncrements)*2)]; //array size = 100 % / power increment % * 2 (we're covering -100 % to 100 %).
    int intTempArray[((100/powerIncrements)*2)];
    
    externalTemp = externalThermistor()+(EEPROM_External_Temp_Calib);           //These are 10 times bigger.
    internalTemp = Thermistor(analogRead(INTERNAL_THERM_PIN))+(EEPROM_Internal_Temp_Calib*10);  //These are 10 times bigger.
    int previousExternalTemp = int(externalTemp);
    
    analogWrite(FAN_PIN, 255);
    

    while (whileLoop == 0) {
      
      if (currentSetPower > 100) {
        analogWrite(MOSFET_PIN, 0);
        delay(10);
        digitalWrite(RELAY_PIN, LOW); //Set to cool the plate.
        analogWrite(FAN_PIN, 0);
        break;
      }
      
      externalTemp = externalThermistor()+(EEPROM_External_Temp_Calib*10);
      internalTemp = Thermistor(analogRead(INTERNAL_THERM_PIN))+(EEPROM_Internal_Temp_Calib*10);
      
      lcd.setCursor(0, 0);
      lcd.print("E:");
      lcd.print(externalTemp, 1); //Print our 10*int as a float value to 1 dp adding the calibration value.
      lcd.print(" C ");
      lcd.print("I:");
      lcd.print(internalTemp/10, 1);
      lcd.print(" C  ");
      lcd.setCursor(0, 1);
      lcd.print("Power: ");
      lcd.print(currentSetPower);
      lcd.print(" T: ");
      lcd.print(whileLoopCounter/2);
      lcd.print("  ");
      
      //If statement to change MOSFET power if needed.
      if (changePowerLevel == 1) {     
        
        if (currentSetPower <= 0){
          digitalWrite(RELAY_PIN, LOW); //Set to cool the plate.
          mosfetPower = currentSetPower * -2.55;
        } else {
          digitalWrite(RELAY_PIN, HIGH); //Set to heat the plate.
          mosfetPower = currentSetPower * 2.55;
        }
        analogWrite(MOSFET_PIN, mosfetPower);
        changePowerLevel = 0;
      }
      
      
      
      if (whileLoopCounter >= 59){ //was 59    
        //20 seconds has passed
        //Has temperature been stable for 20 seconds?
        
        //check with +/- 0.1 degrees
        if (externalTemp <= (previousExternalTemp+2) && externalTemp >= (previousExternalTemp-2)) {
          //Temperature is stable
          //Store temperature values in int array (10 times larger to store 1 d.p.)
          extTempArray[arrayLocation] = externalTemp;
          intTempArray[arrayLocation] = internalTemp;
          
          //Increment 
          arrayLocation ++;
          changePowerLevel = 1;
          currentSetPower += powerIncrements;
        }

        whileLoopCounter = 0;
        previousExternalTemp = int(externalTemp);
      }
      
      //A 500 milli second delay means that: time passed = whileLoopCounter/2 
      whileLoopCounter ++;
      delay(500);
    }   
    
    
    //Report results
    //Maximum array value = ((100/powerIncrements)*2)
    arrayLocation = (((100/powerIncrements)*2) + 1);
    lcd.clear();
    
    while (whileLoop == 0) {
    
      if (arrayLocation == (((100/powerIncrements)*2) + 1)) {
        lcd.setCursor(0, 0);
        lcd.print(" Please review  ");
        lcd.setCursor(0, 1);
        lcd.print("calibration data");
      } else {
        lcd.setCursor(0, 0);
        lcd.print("Power: ");
        lcd.print((arrayLocation - (100/powerIncrements))*10);
        lcd.print(" %  ");
        lcd.setCursor(0, 1);
        lcd.print("E:");
        lcd.print(float(extTempArray[arrayLocation])/10, 1);  
        lcd.print(" C I: ");
        lcd.print(intTempArray[arrayLocation]/10); 
        lcd.print(" C ");
      }
  
      actionBtnState = digitalRead(actionBtn);
      upBtnState = digitalRead(upBtn);
      downBtnState = digitalRead(downBtn);
  
      if (downBtnState == HIGH){
        while (downBtnState == HIGH){
          downBtnState = digitalRead(downBtn);
          delay(50);
        }
         if (arrayLocation > 0){
            arrayLocation --;
            lcd.clear();
          }
       }
  
      if (upBtnState == HIGH){
        while (upBtnState == HIGH){
          upBtnState = digitalRead(upBtn);
          delay(50);
        }
        if (arrayLocation < (((100/powerIncrements)*2))+1){
          arrayLocation ++;
          lcd.clear();
        } 
      }
  
      if (actionBtnState == HIGH){
        delay(500);
        //There is a 500 milli sec delay in void loop to prevent actionBtn events being triggered
        //in subsequent menus unintentionally.
        break;
      }
      delay(10);
    }
    
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Saving data ...");
    lcd.setCursor(0, 1);
    arrayLocation = 0;
    
    //MOSFET calibration data will be stored in EEPROM banks from +120 
    
    while (arrayLocation != (((100/powerIncrements)*2))) {
      
      //EEPROM.write((arrayLocation + 120), (byte(extTempArray[arrayLocation]/10)+128));
      lcd.print("."); 
      arrayLocation ++;
    }
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Data saved!");
    delay(1500);
  }
}
