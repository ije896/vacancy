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
                                     public ChangeListener,
                                     public Slider::Listener
{
public:
    VacancyAudioProcessorEditor (VacancyAudioProcessor& p, AudioProcessorValueTreeState& vts);
    ~VacancyAudioProcessorEditor();
    //==============================================================================
    void paint (Graphics&) override;
    void paintNoFileLoaded(Graphics& g, const Rectangle<int>& thumbnailBounds);
    void paintWithFileLoaded(Graphics& g, const Rectangle<int>& thumbnailBounds);
    void resized() override;
    
    // to reduce how much we have to type
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    
    void buttonClicked (Button* button) override;
    void changeListenerCallback(ChangeBroadcaster* source) override;
    
private:
    void sliderValueChanged(Slider* slider) override;
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
    
    // for our plugin params
    AudioProcessorValueTreeState& valueTreeState;
    
    // physical objects
    Label dryGainLabel;
    Slider dryGainSlider;
    ScopedPointer<SliderAttachment> dryGainAttachment;
    
    TextButton openFileButton;
    TextButton playIRButton;
    
    AudioFormatManager formatManager;
    ScopedPointer<AudioFormatReaderSource> readerSource;
    // controls playback of AudioFormatReaderSource object
    // can also do SR conversions and buffer audio
    TransportState state;
    // thumbnail objects
    AudioThumbnailCache thumbnailCache;
    AudioThumbnail thumbnail;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VacancyAudioProcessorEditor)
};
