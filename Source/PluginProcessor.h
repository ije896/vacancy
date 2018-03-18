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
    void do_image_stuff();
    void loadIR(File file);
    void reverseIR(AudioSampleBuffer& inBuffer);
    void playIR();
    static String reverseToText(float value){
            return value < 0.5 ? "Normal" : "Reversed";
    };
    static float textToReverse(const String& text){
        if (text == "Normal")    return 0.0f;
        if (text == "Reversed")  return 1.0f;
        return 0.0f;
    };
    bool _useReverseIR = false;
    bool isUsingReversed = false;
    //==============================================================================
    void updateParams();
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void changeListenerCallback(ChangeBroadcaster* source) override;
    
   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

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
    dsp::Convolution _convolution;
    float prev_dry_gain;
    float prev_wet_gain;
    enum TransportState {
        Stopped,
        Starting,
        Playing,
        Pausing,
        Paused,
        Stopping
    };
    void changeState(TransportState newState);
    //==============================================================================
    TransportState transport_state;
    AudioFormatManager _formatManager;
    ScopedPointer<AudioFormatReaderSource> _readerSource;
    AudioProcessorValueTreeState _parameters;
    AudioSampleBuffer reversedIRBuffer;
//    AudioBuffer<float> reversedIR;
    File _IRFile;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VacancyAudioProcessor)
};
