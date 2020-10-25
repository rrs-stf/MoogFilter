/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LadderFilterAudioProcessorEditor::LadderFilterAudioProcessorEditor (LadderFilterAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
	for(int i=0; i<NUM_OF_PARAMS; i++)
    {
		paramSliders.add(new juce::Slider());
		paramLabels.add(new juce::Label());
        addAndMakeVisible(paramSliders[i]);
        addAndMakeVisible(paramLabels[i]);
    }
    
    setPluginState(audioProcessor.pluginState.get());
	//processor.addChangeListener(this);
    setSize (450, 180);
}

LadderFilterAudioProcessorEditor::~LadderFilterAudioProcessorEditor()
{
	audioProcessor.removeChangeListener(this);
    sliderAttachments.clear();
}

void LadderFilterAudioProcessorEditor::setPluginState(juce::AudioProcessorValueTreeState *p)
{
    pluginState = p;
    for(int i=0; i<NUM_OF_PARAMS; i++)
    {
		sliderAttachments.add(new SliderAttachment (*pluginState, Params_Name[i], *paramSliders[i]));
    }
    
}

//==============================================================================
void LadderFilterAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (17.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
	g.drawText ("Moog Ladder Filter", juce::Rectangle<int>(0,10,getWidth(),40), juce::Justification::centredTop, true);
}

void LadderFilterAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	for(int i = 0; i < NUM_OF_PARAMS; i++)
    {
		paramSliders[i]->setSliderStyle(juce::Slider::Rotary);
        paramSliders[i]->setSize(dimSlider, dimSlider);
		paramSliders[i]->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, dimSlider, dimTextBox);
        paramLabels[i]->setFont(juce::Font(12.0f));
		paramLabels[i]->setJustificationType(juce::Justification::centred);
        paramLabels[i]->setText(pluginState->getParameter(Params_Name[i])->name + " " + pluginState->getParameter(Params_Name[i])->label, juce::dontSendNotification);
    }
	
	paramSliders[kCutoffFreq]->setBounds (xParamPos, 0.125*getHeight(), dimSlider, dimSlider);
    paramLabels[kCutoffFreq]->setBounds(xParamPos, 0.125*getHeight() + 2.5*margin, labelWidth, labelHeight);
	
	paramSliders[kResonance]->setBounds (xParamPos + dimSlider + margin, 0.125*getHeight(), dimSlider, dimSlider);
    paramLabels[kResonance]->setBounds(xParamPos + dimSlider + margin, 0.125*getHeight() + 2.5*margin, labelWidth, labelHeight);
	
	paramSliders[kDrive]->setBounds (xParamPos + 2*dimSlider + 2*margin, 0.125*getHeight(), dimSlider, dimSlider);
    paramLabels[kDrive]->setBounds(xParamPos + 2*dimSlider + 2*margin, 0.125*getHeight() + 2.5*margin, labelWidth, labelHeight);
	
	paramSliders[kOutLevel]->setBounds (xParamPos + 3*dimSlider + 3*margin, 0.125*getHeight(), dimSlider, dimSlider);
    paramLabels[kOutLevel]->setBounds(xParamPos + 3*dimSlider + 3*margin, 0.125*getHeight() + 2.5*margin, labelWidth, labelHeight);
}

void LadderFilterAudioProcessorEditor::buttonClicked(juce::Button * btn)
{
    
}

void LadderFilterAudioProcessorEditor::updateToggleState (juce::Button* button, juce::String name)
{
    
}

void LadderFilterAudioProcessorEditor::saveScene()
{
    
	std::unique_ptr<juce::XmlElement> xml = audioProcessor.getStateXml();
    
    juce::File presetsLocation = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDesktopDirectory);
    
    juce::FileChooser myChooser ("Save Preset",
                           presetsLocation.getChildFile("AllPassSettings.xml"),
                           "*.*");
    if (myChooser.browseForFileToSave(true))
    {
		juce::File outFile (myChooser.getResult());
        xml->writeTo(outFile);
    }
    
}

void LadderFilterAudioProcessorEditor::loadScene()
{
	juce::File presetsLocation = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDesktopDirectory);
    
    juce::FileChooser myChooser ("Load Preset",
                           presetsLocation,
                           "*.*");
    if (myChooser.browseForFileToOpen())
    {
        juce::File inFile (myChooser.getResult());
		std::unique_ptr<juce::XmlElement> xml (juce::XmlDocument::parse (inFile));
        if(xml != nullptr){
            audioProcessor.setStateXml(xml.get());
        }
    }
}

void LadderFilterAudioProcessorEditor::changeListenerCallback (juce::ChangeBroadcaster* source){}
