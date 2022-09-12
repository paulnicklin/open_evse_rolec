// Copyright (C) 2016 Sam C. Lin
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// See LICENSE for a copy of the GNU General Public License or see
// it online at <http://www.gnu.org/licenses/>.


//Really this just reads and interprets PP Pin


#include "open_evse.h"
int cntr = 0; // loop counter

#ifdef PP_AUTO_AMPACITY

/*
// This is the table for OpenEVSE
static PP_AMPS s_ppAmps[] = {
  {0,0},
  {93,63},  // 100 = 93
  {185,32}, // 220 = 185
  {415,20}, // 680 = 415
  {615,13}, // 1.5K = 615
  {1023,0}
};
*/

// This is the empirical table for Rolec & my additions. 
// ADC = 0.0823R + 3.1279
// Struct   uint16_t adcVal;  uint8_t amps;
static PP_AMPS s_ppAmps[] = {
  {0,0},
  {11,63},  // 100 = 11
  {18,32}, // 220 = 18
  {60,20}, // 680 = 60
  {126,13}, // 1.5K = 126
  #ifdef PP_AUTO_AMPACITY_PLUS_SOLAR
  {184,6}, // no standard for this, but I picked a common resistor 2k2 = 6A 
  {389,-1}, // no standard for this, but I picked a common resistor 4k7 = 0A + whatever from solar. Magic return value -1
  #endif
  {1023,0}
};

AutoCurrentCapacityController::AutoCurrentCapacityController() :
  adcPP(PP_PIN)
{
}

//This is a hack - we use PP line which normally tells it the cable capacity to choose our charge speed.
//Fine, but now we're using it as a minimum, with solar top-up. so it's quite different semantically
//In my controller this knob does something else, it sets the MINIMUM current.
//with a socketed type wallbox it sets the cable capacity.
uint8_t AutoCurrentCapacityController::ReadPPMaxAmps()
{
    // n.b. sample a few times and average it
  uint16_t adcval = 0;
  for (cntr=0; cntr<10; cntr++ ) {
    adcval = adcval + adcPP.read();
    }
  adcval = (adcval/10);

  uint8_t amps = 0;
  //scan down the amps table looking for the first one too big. Add in half the "gap"to the next one to find "nearest"
  //
  for (uint8_t i=1;i < sizeof(s_ppAmps)/sizeof(s_ppAmps[0]);i++) {
    if (adcval <= (s_ppAmps[i].adcVal - (s_ppAmps[i].adcVal - s_ppAmps[i-1].adcVal)/2)) {
      amps = s_ppAmps[i-1].amps;
      break;
    }
  }

#ifdef SERDBG
    Serial.print("pp: ");Serial.print(adcval);Serial.print(" amps: ");Serial.println(amps);
#endif //SERDBG

  return amps;
}


// uint8_t AutoCurrentCapacityController::AutoSetCurrentCapacity()
// {
//   uint8_t amps = ReadPPMaxAmps();

//   if (amps) {
//     g_EvseController.SetCurrentCapacity(amps,0,1);
//     return 0;
//   }
//   else {
//     return 1;
//   }
// }

#endif //PP_AUTO_AMPACITY


