

//This is the first menu that the user sees after switching on the device

void programMenu(){
  
  while (whileLoop == 0) {
    
    lcd.setCursor(0, 0);
    lcd.print(programMenuChoices[currProgramMenuSel]);  
    lcd.setCursor(0, 1);
    lcd.print(String(currProgramMenuSel + 1));
    lcd.print("/");
    lcd.print(String(programMenuSize + 1));
  
    actionBtnState = digitalRead(actionBtn);
    upBtnState = digitalRead(upBtn);
    downBtnState = digitalRead(downBtn);
  
    if (upBtnState == HIGH){
      while (upBtnState == HIGH){
        upBtnState = digitalRead(upBtn);
        delay(50);
      }
       if (currProgramMenuSel > 0){
          currProgramMenuSel --;
          lcd.clear();
        }
     }
  
    if (downBtnState == HIGH){
      while (downBtnState == HIGH){
        downBtnState = digitalRead(downBtn);
        delay(50);
      }
      if (currProgramMenuSel < programMenuSize){
        currProgramMenuSel ++;
        lcd.clear();
      } 
    }
  
    if (actionBtnState == HIGH){
      //There is a 500 milli sec delay in void loop to prevent actionBtn events being triggered
      //in subsequent menus unintentionally.
      break;
    }
    delay(10);
  }
}
