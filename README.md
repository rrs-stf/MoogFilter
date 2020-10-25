# Moog Ladder Filter 

The following implementation of a ladder filter was inspired from [1], which is a white-box modelling approach of the Moog lowpass ladder filter.

## Goals of the project

- design a ladder filter audio plugin which processes in real-time
- control the filter's cutoff frequency and resonance parameters in real-time
- control the saturation behaviour of the filter
- achieve the self-oscillation behaviour specific to the Moog ladder structure

## Implementation

The project takes the classic four-pole design of the Moog ladder filter (shown in Fig. 1 of [1]) and discretizes the voltage output at each filter stage within the circuit using the bilinear transform. 

The filter's characteristics are physically-motivated by the structure of the ladder filter and its presumed environmental conditions (fx. cutoff frequency is translated as the bias control current source found within the real-life circuit or that temperature influences the behaviour of the model).

The resulting audio plugin is developed in C++, using the JUCE audio framework. The available plugin parameters are described as follows:
- *Cutoff Freq.* [Hz] - defines the desired cutoff frequency for the filter
- *Resonance* - controls the resonance amount around the cutoff frequency
- *Drive* [dB]- defines the gain level of the input audio signal, which allows control of the saturation amount within the filtered sound
- *Output Level* [dB] - modifies the output level of the filtered sound

## Verification

With the given time frame, the model's behaviour was checked through real-time debugging, which consisted of mainly filtering white noise or sound samples, and observing the correlation between parametric modifications and timbre changes. The plugin host of choice was REAPER.

The performance of the algorithm was also checked through CPU and memory usage monitoring within XCode and REAPER.

## Results and Issues

For the limited time available, the model presents a reasonably-good approximation of a real-time Moog ladder filter.

Additionally, the model manages to offer novel behaviours typical to the classic Moog circuit. This includes its saturation behaviour and its ability to self-oscillate, if the resonance factor is set high enough.

The main issue with the model's output is its frequency tuning errors, which increase with the cutoff frequency. Additionally, phase shifting issues occur due to the addition of fictitious unit delays within the discretized circuit, which eliminates any self-dependency issues during computation. Aliasing also occurs during self oscillation, especially when cutoff frequency approaches the Nyquist limit.

## ToDos

A list of probable future work topics:

- Reduce frequency tuning error by empirically designing a polynomial interpolation process which follows the expected cutoff frequency variation
- A more thorough analysis of the filtered output, especially in regard to its saturation behaviour and the harmonics generated.
- Comparison of the model's output either with a real-life circuit or with a simulated ladder structure (fx. through SPICE)


### References
[1] - D'Angelo, Stefano, and Vesa Välimäki. "An improved virtual analog model of the Moog ladder filter." 2013 IEEE International Conference on Acoustics, Speech and Signal Processing. IEEE, 2013. (*DAngeloImproved.pdf* + *ErrataImproved.pdf*)
