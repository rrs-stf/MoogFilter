//
//  FilterConsts.h
//  ladderFIlter
//
//  Created by Rares on 07/10/2020.
//

#ifndef FilterConsts_h
#define FilterConsts_h

// constants for ladder filter processing
static const float TEMP = 30; 										///--> temperature in degrees Celsius
static const float THERMAL_VOLT = 8.617f*1e-5*(273.15f + TEMP); 	///--> thermal voltage, Vt
static const float OVER_TWO_THERMAL_VOLT = 1/(THERMAL_VOLT * 2.f); 	///--> precompute division to 2 * Vt
static const int NUMBER_OF_FILTERS = 4; 							///--> number of filter stages
static const float PI =  3.14159265358979323846f;					
static const float TWO_PI = 2*PI;

#endif /* FilterConsts_h */
