int thermostatCounter = 0;
int stabilityCount = 0;
//As coefficient goes down, power goes up

void thermostatControl(float thermoTemp) {
  
  analogWrite(FAN_PIN, 255);
  
  //for >20 degC, approx. 0.70 degC/%
  
   thermostatCounter ++;

  if (thermoTemp >= (internalTemp/10)) {
    digitalWrite(RELAY_PIN, HIGH); //Set to heat the plate.
    
    if (thermostatCounter >= 280) {   
      thermostatCounter = 0; //reset
        
      if (abs(externalTemp - thermoTemp) > 0.04) {  //Only fudge if thermostat <> externalTemp
        if (externalTemp < thermoTemp) {
          //Not hot enough
          if ((externalTemp - lastExternalTemp) < 0.05) {  //fudge it even if zero difference
            //And temp not rising towards thermoTemp
            if (tempCoeffFudge > -0.740) {
              tempCoeffFudge = tempCoeffFudge - (0.2*abs(1-(externalTemp/thermoTemp)));
              if (tempCoeffFudge < -0.740) {
                tempCoeffFudge = -0.740;
              }
              //thermostatCounter = -500;  //Don't make a change for a while
            }
          }
        } else if (externalTemp > thermoTemp) {
          //Too hot
          if ((externalTemp - lastExternalTemp) > -0.05) {  //fudge it even if zero difference
            //And temp not falling towards thermoTemp
            if (tempCoeffFudge < 0.740) {
              tempCoeffFudge = tempCoeffFudge + (0.2*abs(1-(externalTemp/thermoTemp)));
              if (tempCoeffFudge > 0.740) {
                tempCoeffFudge = 0.740;
              }
              //thermostatCounter = -500; //Don't make a change for a while
            }   
          }
        }
      }
      lastExternalTemp = externalTemp;
    }
    tempCoeff = (0.745 + tempCoeffFudge) - ((1-(externalTemp/thermoTemp))*(0.745 + tempCoeffFudge));
    mosfetPower = ((thermoTemp/tempCoeff)-34) * 2.55;  //Scale to 256 bits
     
  } else if (thermoTemp < (internalTemp/10)) {
    
    digitalWrite(RELAY_PIN, LOW); //Set to cool the plate.
    
    if (thermostatCounter >= 280) {   
      thermostatCounter = 0; //reset
        
      if (abs(externalTemp - thermoTemp) > 0.04) {  //Only fudge if thermostat <> externalTemp
        if (externalTemp > thermoTemp) {
          //Not cold enough
          if ((externalTemp - lastExternalTemp) < 0.05) {  //fudge it even if zero difference
            //And temp not rising towards thermoTemp
            if (tempCoeffFudge > -0.740) {
              tempCoeffFudge = tempCoeffFudge - (0.2*abs(1-(externalTemp/thermoTemp)));
              if (tempCoeffFudge < -0.740) {
                tempCoeffFudge = -0.740;
              }
              //thermostatCounter = -500;  //Don't make a change for a while
            }
          }
        } else if (externalTemp < thermoTemp) {
          //Too cold
          if ((externalTemp - lastExternalTemp) > -0.05) {  //fudge it even if zero difference
            //And temp not falling towards thermoTemp
            if (tempCoeffFudge < 0.740) {
              tempCoeffFudge = tempCoeffFudge + (0.2*abs(1-(externalTemp/thermoTemp)));
              if (tempCoeffFudge > 0.740) {
                tempCoeffFudge = 0.740;
              }
              //thermostatCounter = -500; //Don't make a change for a while
            } 
          }
        }
      }
      lastExternalTemp = externalTemp;
    }
     tempCoeff = (-0.2 + tempCoeffFudge) - ((1-(externalTemp/thermoTemp))*(-0.2 + tempCoeffFudge));
     mosfetPower = abs((thermoTemp/tempCoeff)-32) * 2.55;  //Scale to 256 bits
  }
  
  if (abs(externalTemp - thermoTemp) < 0.04) {  //Only consider beginning ramp if thermostat == externalTemp
    if ((externalTemp - lastExternalTemp) < 0.05) {  //and temperature is stable
      stabilityCount ++;
      if (stabilityCount >= 2000){
        stabilityCount = 0;
        readyToBeginRamp = 1;
      }
    } else {
      stabilityCount = 0;  
    }    
  }
 
  
  if (mosfetPower > 255) {
    mosfetPower = 255;
  } else if (mosfetPower < 0) {
    mosfetPower = 0;
  }
  analogWrite(MOSFET_PIN, mosfetPower);        
}  
        

