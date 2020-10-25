/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LadderFilterAudioProcessor::LadderFilterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
	undoManager = std::make_unique<juce::UndoManager>();
	pluginState = std::make_unique<juce::AudioProcessorValueTreeState>(*this, undoManager.get());
	
	using Parameter = juce::AudioProcessorValueTreeState::Parameter;
	
	// create and add plugin parameters
	pluginState->createAndAddParameter(std::make_unique<Parameter>(
										Params_Name[kCutoffFreq], "Cutoff Freq.", "[Hz]",
										juce::NormalisableRange<float>(10.0, 24000.0, 0.1, 0.5), 10.f,
									    [](float v) -> juce::String { return juce::String(v,1); },
										[](const juce::String& s) -> float { return s.getFloatValue(); }
										));
	
	
	pluginState->createAndAddParameter(std::make_unique<Parameter>(
										Params_Name[kResonance], "Resonance", "",
										juce::NormalisableRange<float>(0.0, 10.0, 0.1, 0.5), 0.f,
									    [](float v) -> juce::String { return juce::String(v,1); },
										[](const juce::String& s) -> float { return s.getFloatValue(); }
										));
	
	
	pluginState->createAndAddParameter(std::make_unique<Parameter>(
										Params_Name[kDrive], "Drive", "[dB]",
										juce::NormalisableRange<float>(-20.0, 20.0, 0.01, 1), 0.f,
									    [](float v) -> juce::String { return juce::String(v,2); },
										[](const juce::String& s) -> float { return s.getFloatValue(); }
										));
	
	
	pluginState->createAndAddParameter(std::make_unique<Parameter>(
										Params_Name[kOutLevel], "Output Level", "[dB]",
										juce::NormalisableRange<float>(-20.0, 20.0, 0.01, 1), 0.f,
									    [](float v) -> juce::String { return juce::String(v,2); },
										[](const juce::String& s) -> float { return s.getFloatValue(); }
										));
	
	pluginState->state = juce::ValueTree("LadderFilter");
	
	for(int i = 0; i < NUM_OF_PARAMS; i++)
    {
		paramValues.add(new juce::LinearSmoothedValue<float>());
		pluginState->addParameterListener(Params_Name[i], this);
    }
	
    initParameters();
}

LadderFilterAudioProcessor::~LadderFilterAudioProcessor()
{
	pluginState.release();
}

//==============================================================================
void LadderFilterAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    for(int i=0; i < NUM_OF_PARAMS; i++)
    {
        if(parameterID == Params_Name[i])
        {
            paramValues[i]->setTargetValue(newValue);
            setParam(i, paramValues[i]->getTargetValue());
            
            break;
        }
    }
}

float LadderFilterAudioProcessor::getParameter (int index)
{
    float v = 0.0f;
    
    if( index >= 0 && index < NUM_OF_PARAMS)
    {
        v = paramValues[index]->getCurrentValue();
    }
    
    return v;
}


const juce::String LadderFilterAudioProcessor::getParameterName (int index)
{
    if( index < 0 || index > NUM_OF_PARAMS-1) return juce::String();
    return Params_Name[index];
}

void LadderFilterAudioProcessor::initParameters()
{
    for(int i=0; i < NUM_OF_PARAMS; i++)
    {
        paramValues[i]->setTargetValue(*pluginState->getRawParameterValue(Params_Name[i]));
        setParam(i, paramValues[i]->getTargetValue());
    }
	
}


void LadderFilterAudioProcessor::setParam(int parameterIndex, float newValue)
{
	switch(parameterIndex) {
		
	}
	//paramValues[parameterIndex]->setTargetValue(getParameter(parameterIndex));
}

//==============================================================================
const juce::String LadderFilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LadderFilterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LadderFilterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LadderFilterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LadderFilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LadderFilterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LadderFilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LadderFilterAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String LadderFilterAudioProcessor::getProgramName (int index)
{
    return {};
}

void LadderFilterAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void LadderFilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	
	if(mSampleRate != sampleRate)
	{
		auto totalNumInputChannels  = getTotalNumInputChannels();
		mSampleRate = sampleRate;
		// initialize filters for each input channel
		for (int channel = 0; channel < totalNumInputChannels; ++channel)
		{
			mvFilters.push_back(new LadderFilter<float>(mOversampleFactor));
			mvFilters[channel]->setSampleRate(sampleRate);
		}
	}
}

void LadderFilterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LadderFilterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void LadderFilterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    //juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
	
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
		
		for(int sample = 0; sample < buffer.getNumSamples(); ++sample)
		{
			// assign the values from the interface parameters
			mvFilters[channel]->setCutoffFreq(paramValues[kCutoffFreq]->getNextValue());
			mvFilters[channel]->setResonance(paramValues[kResonance]->getNextValue());
			mvFilters[channel]->setGain(paramValues[kDrive]->getNextValue());
			// process the filtered output
			auto outputLevel = juce::Decibels::decibelsToGain(paramValues[kOutLevel]->getNextValue());
			buffer.getWritePointer(channel) [sample] = outputLevel *  mvFilters[channel]->process(buffer.getReadPointer(channel)[sample]);
		}

    }
}

//==============================================================================
bool LadderFilterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* LadderFilterAudioProcessor::createEditor()
{
    return new LadderFilterAudioProcessorEditor (*this);
}

//==============================================================================
std::unique_ptr<juce::XmlElement> LadderFilterAudioProcessor::getStateXml()
{
    
    auto state = pluginState->copyState();
    auto xml = state.createXml();
    
    return xml;
}

void LadderFilterAudioProcessor::setStateXml(juce::XmlElement *xml)
{
    
    if(xml != nullptr){
		juce::MemoryBlock mb;
        copyXmlToBinary(*xml,mb);
        setStateInformation(mb.getData(), (int)mb.getSize());
    }
}

//==============================================================================
void LadderFilterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
	std::unique_ptr<juce::XmlElement> xml (getStateXml());
    copyXmlToBinary (*xml, destData);
}

void LadderFilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
	const juce::ScopedLock sl(getCallbackLock());
    
	std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState != nullptr)
        if (xmlState->hasTagName (pluginState->state.getType()))
			pluginState->state = juce::ValueTree::fromXml (*xmlState);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LadderFilterAudioProcessor();
}
