/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;


enum RadioButtonIds
{
    AlgorithmTypeButtons = 1001
};

class LadderFilterAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Button::Listener, juce::ChangeListener
{
public:
    LadderFilterAudioProcessorEditor (LadderFilterAudioProcessor&);
    ~LadderFilterAudioProcessorEditor() override;

    //==============================================================================
	void setPluginState(juce::AudioProcessorValueTreeState* p);
    void paint (juce::Graphics&) override;
    void resized() override;
	void buttonClicked(juce::Button* btn) override;
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;
    void updateToggleState (juce::Button* button, juce::String name);
    void saveScene();
    void loadScene();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
	
	juce::AudioProcessorValueTreeState* pluginState;
    
    int dimTextBox = 15;
    int distance = 10;
    int labelWidth = 80;
    int labelHeight = 35;
    int margin = 30;
    int xParamPos = 20;
    int dimSlider = 75;
    
	juce::OwnedArray<SliderAttachment> sliderAttachments;
    juce::OwnedArray<juce::Slider> paramSliders;
	juce::OwnedArray<juce::Label> paramLabels;

	LadderFilterAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LadderFilterAudioProcessorEditor)
};
