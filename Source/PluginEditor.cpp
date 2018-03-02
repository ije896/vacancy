/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
VacancyAudioProcessorEditor::VacancyAudioProcessorEditor (VacancyAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
   
    // sample open params
    openFileButton.setButtonText("Open IR");
//    openFileButton.addListener(this);
    
    // volume slider parameters
    level.setSliderStyle(Slider::LinearVertical);
    level.setRange(-60.0, 0.0);
    level.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    level.setPopupDisplayEnabled(true, false, this);
    level.setTextValueSuffix(" dB");
    level.setValue(-50.0);
    
    
    // display items
    addAndMakeVisible(&level);
    addAndMakeVisible(&openFileButton);
}

VacancyAudioProcessorEditor::~VacancyAudioProcessorEditor()
{
}

//==============================================================================
void VacancyAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.fillAll(Colours::white);
    g.setColour (Colours::black);
    g.setFont (15.0f);
    g.drawFittedText ("Slider test", 0 , 0, getWidth(), 30, Justification::centred, 1);
}

void VacancyAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    // level.setBounds(<#int x#>, <#int y#>, <#int width#>, <#int height#>)
    openFileButton.setBounds(100, 30, getWidth() - 200, 30);
    level.setBounds(40, 30, 40, getHeight() - 60);
}
