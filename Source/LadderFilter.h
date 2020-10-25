//
//  LadderFilter.h
//  ladderFIlter
//
//  Created by Rares on 07/10/2020.
//

#ifndef LadderFilter_h
#define LadderFilter_h

#include "FilterConsts.h"
#include <JuceHeader.h>
#include <math.h>


/*!
* \class LadderFilter
*
* \brief Physically-motivated model of a Moog lowpass ladder filter
*
* \author Rares
*/
template<class T>
class LadderFilter {
	
public:
	/*! constructor for LadderFilter class
	 \param osFactor Oversampling factor
	 */
	LadderFilter(int osFactor);
	
	/*! destructor for LadderFilter class
	 */
	virtual ~LadderFilter();
	
	/*! set sample rate for filter
	 \param sampleRate plugin host sample rate, in Hz
	 */
	void setSampleRate(T sampleRate);
	
	/*! set cutoff frequency for filter by calculating the bias control current
	 \param cutFreq desired cutoff frequency
	 */
	void setCutoffFreq(T cutFreq);
	
	/*! set resonance factor for filter
	 \param resonance desired resonance factor
	 */
	void setResonance(T resonance);
	
	/*! set gain level for input signal
	 \param dbLevel desired gain level for input signal in dB
	 */
	void setGain(T dbLevel);
	
	/*! process loop for filter
	 \param inputSample sample from input signal
	 \return filtered output sample
	 */
	T process(T inputSample);
	
private:
	
	// variables
	T mSampleRate{44100.f}; 			///--> filter sample rate
	T mResonance{0.f};					///--> resonance factor
	T mCutoffFreq{0.f};					///--> cutoff frequency
	T mBiasControl{0.f};				///--> bias control factor, which represents the control current source in the analog circuit
	T mGain{1.f};						///--> input signal gain level (converted from dB)
	int mOversampleFactor{1};			///--> oversampling factor
	T mvVolt[NUMBER_OF_FILTERS];		///--> array of voltage output at each filter stage at current time step, V[n]
	T mvVolt_1[NUMBER_OF_FILTERS];		///--> array of voltage output at each filter stage at previous time step, V[n-1]
	T mvDiffVolt[NUMBER_OF_FILTERS];	///--> array of voltage output differential at each filter stage at current time step, dV[n]
	T mvDiffVolt_1[NUMBER_OF_FILTERS];	///--> array of voltage output differential at each filter stage at previous time step, dV[n-1]
};

#endif /* LadderFilter_h */
