/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/**
*/
class VacancyAudioProcessor  : public AudioProcessor,
                               public ChangeListener
{
public:
    //==============================================================================
    VacancyAudioProcessor();
    ~VacancyAudioProcessor();
    //==============================================================================
    void loadIR(File file);
    void playIR();
    float dry_gain;
    bool guiShouldRepaint = false;
    
    //Convolution convolver;
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void changeListenerCallback(ChangeBroadcaster* source) override;
    
   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    enum TransportState {
        Stopped,
        Starting,
        Playing,
        Pausing,
        Paused,
        Stopping
    };
    //==============================================================================
    AudioTransportSource _transportSource;
    AudioFormatManager _formatManager;
    ScopedPointer<AudioFormatReaderSource> _readerSource;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VacancyAudioProcessor)
};
