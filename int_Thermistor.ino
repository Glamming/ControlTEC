
//I'm not to worried about the accuracy of the internal thermometer, however the external thermometer
//picks up a bit of noise, a running median should solve this, I've implemented a 4 Hz interupt
//to collect an external thermistor reading and add it to the running median.

//<data type, running window size, initial value, name>
FastRunningMedian<unsigned int,7, 0> newMedian;

float Temp;
int temporary;


int Thermistor(int RawADC) {
  Temp = log(10000.0*((1024.0/RawADC-1))); 
  //         =log(10000.0/(1024.0/RawADC-1)) // for pull-up configuration
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
  Temp = (Temp - 273.15)*10;            // Convert Kelvin to Celcius  

  return Temp;
}


float externalThermistor() {
  
  //get the median raw analog reading and do the math :)
  Temp = log(2200.0*((8184.0/(newMedian.getMedian())-1))); //adc.getMaxPossibleReading()
  //Temp = log(10000.0*(4092.0/(newMedian.getMedian()))-1); 
  //Temp = log(10000.0*((adc.getMaxPossibleReading()/newMedian.getMedian()-1))); 
  //         =log(10000.0/(1024.0/RawADC-1)) // for pull-up configuration
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
  Temp = (Temp - 273.15);            // Convert Kelvin to Celcius 
  return Temp;
}

//Timer interupt at 4 Hz to add a reading to the running

ISR(TIMER1_COMPA_vect) {
  fourHertzCounter ++;
  //newMedian.addValue(analogRead(THERM_PIN)); //We're saving the raw analog reading so we only have to do float calculations once.
  newMedian.addValue(adc.newAnalogRead(THERM_PIN));
  
}
