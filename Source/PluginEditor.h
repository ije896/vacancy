/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class VacancyAudioProcessorEditor  : public AudioProcessorEditor,
                                    public Button::Listener,
                                    public ChangeListener
//public Slider::Listener
{
public:
    VacancyAudioProcessorEditor (VacancyAudioProcessor&);
    ~VacancyAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void buttonClicked (Button* button) override;
    void changeListenerCallback(ChangeBroadcaster* source) override;

private:
    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Stopping
    };
   
    void changeState(TransportState nextState);
    void openFileButtonClicked();
    void playIRButtonClicked();
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    VacancyAudioProcessor& processor;
    
    // physical objects
    Slider level;
    TextButton openFileButton;
    TextButton playIRButton;
    
    AudioFormatManager formatManager;
    ScopedPointer<AudioFormatReaderSource> readerSource;
    // controls playback of AudioFormatReaderSource object
    // can also do SR conversions and buffer audio
    AudioTransportSource transportSource;
    TransportState state;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VacancyAudioProcessorEditor)
};
