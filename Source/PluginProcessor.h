/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#define NUM_OF_PARAMS 4


#include <JuceHeader.h>
#include <vector>
#include "LadderFilter.h"
#include "LadderFilter.cpp"

//using namespace juce;

//==============================================================================
// list parameters for XML interface
enum Params_Idx
{
	kCutoffFreq,
	kResonance,
	kDrive,
	kOutLevel
};

static const juce::String Params_Name[] =
{
	"CutoffFreq",
	"Resonance",
	"Drive",
	"OutputLevel"
};
//==============================================================================
/**
*/
class LadderFilterAudioProcessor  : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener, public juce::ChangeBroadcaster
{
public:
    //==============================================================================
    LadderFilterAudioProcessor();
    ~LadderFilterAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
	
	void parameterChanged (const juce::String& parameterID, float newValue) override;
    float getParameter (int index) override;
	const juce::String getParameterName (int index) override;
    void initParameters();
    void setParam(int parameterIndex, float newValue);

	std::unique_ptr<juce::XmlElement> getStateXml();
	void setStateXml(juce::XmlElement* xml);
	std::unique_ptr<juce::UndoManager> undoManager;
	std::unique_ptr<juce::AudioProcessorValueTreeState> pluginState;
	juce::AudioProcessorValueTreeState* getPluginState()
    {
        return pluginState.get();
    }
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LadderFilterAudioProcessor)
	
	juce::OwnedArray<juce::LinearSmoothedValue<float>> paramValues; ///--> array of parameter values
	float mSampleRate{0.f}; 										///--> host sample rate
	std::vector<LadderFilter<float>*> mvFilters;					///--> vector of channel ladder filters
	int mOversampleFactor{2};										///--> oversampling factor
};
