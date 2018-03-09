/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/


/* TODO
 True dry/wet gain (mix input signals)
 Wet gain slider
 gain in dB
 Reverse IR
 Reverse IR Button
 Reverse IR button currently doesn't change values
 Remove the play IR button
 Add level meters?
     I guess you can load IR into a readerSource, and then manually reverse it into
     another buffer and convolve with that or save to tmp file worst case?
 Change Length/Start of IR
 Filtering, whether it knobs or visual points like SD
 Volume envelope
 Variable SR
 Predelay
 Check for mono vs stereo IR
 Maybe accept different types of IR's? Like mp3?...
 Image parsing and convolution!!!!!
 Make all buttons APVTS params
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
VacancyAudioProcessor::VacancyAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
        _parameters(*this, nullptr)
#endif
{
    _parameters.createAndAddParameter("dry_gain", "Dry Gain", String(), NormalisableRange<float> (0.0f, 1.0f), 0.7f, nullptr, nullptr);
    _parameters.createAndAddParameter("reverseIR", "Reverse IR", String(), NormalisableRange<float> (0.0f, 1.0f, 1.0f), 0.0f,
                                      reverseToText, textToReverse);
    
    _parameters.state = ValueTree(Identifier("VacancyParams"));
    _formatManager.registerBasicFormats();
    _transportSource.addChangeListener(this);
    
}

VacancyAudioProcessor::~VacancyAudioProcessor()
{
    releaseResources();
}

//==============================================================================
void VacancyAudioProcessor::changeState(TransportState newState){
    if(transport_state != newState){
        transport_state = newState;
        switch (transport_state) {
            case Starting:
                _transportSource.start();
                break;
            case Stopped:
                _transportSource.stop();
                _transportSource.setPosition(0.0);
                break;
            case Playing:
                _transportSource.stop();
                _transportSource.setPosition(0.0);
                _transportSource.start();
            default:
                break;
        }
    }
}

void VacancyAudioProcessor::loadIR(File file){
    // somewhere in here, we could pre-emptively reverse the file
    AudioFormatReader* reader = _formatManager.createReaderFor(file);
    
    if (reader != nullptr)
    {
        _convolution.loadImpulseResponse(file, true, false, 0);
        
        // sample player
        ScopedPointer<AudioFormatReaderSource> newSource = new AudioFormatReaderSource (reader, true);
        _transportSource.setSource (newSource, 0, nullptr, reader->sampleRate);
        _readerSource = newSource.release();
    }
}

void VacancyAudioProcessor::playIR(){
    changeState(Starting);
}

void VacancyAudioProcessor::changeListenerCallback(ChangeBroadcaster* source){
    if(source==&_transportSource){
        if(_transportSource.isPlaying()){
            changeState(Playing);
        }
        else if (transport_state==Playing){
            changeState(Stopped);
        }
    }
}

//==============================================================================
const String VacancyAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool VacancyAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool VacancyAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool VacancyAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double VacancyAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int VacancyAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int VacancyAudioProcessor::getCurrentProgram()
{
    return 0;
}

void VacancyAudioProcessor::setCurrentProgram (int index)
{
}

const String VacancyAudioProcessor::getProgramName (int index)
{
    return {};
}

void VacancyAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void VacancyAudioProcessor::updateParams(){
    
}
void VacancyAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    prev_dry_gain = *_parameters.getRawParameterValue("dry_gain");
    setPlayConfigDetails(2, 2, sampleRate, samplesPerBlock);
    _transportSource.prepareToPlay(samplesPerBlock, sampleRate);
    
    // setup convolution
    auto channels = static_cast<uint32> (jmin (getMainBusNumInputChannels(), getMainBusNumOutputChannels()));
    dsp::ProcessSpec spec { sampleRate, static_cast<uint32> (samplesPerBlock), channels };
    _convolution.prepare(spec);
}

void VacancyAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    _transportSource.releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VacancyAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void VacancyAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    const bool val = *_parameters.getRawParameterValue("reverseIR") < 0.5f ? false : true;
    //DBG(val);
    if(val){
        DBG("hit");
    }
    else if (!val){
        DBG("miss");
    }
    const float curr_dry_gain = *_parameters.getRawParameterValue("dry_gain");
    DBG(curr_dry_gain);
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
//        for (int channel = 0; channel < totalNumInputChannels; ++channel)
//        {
//            const int actualInputChannel = channel % totalNumInputChannels;
//
//            auto* inputData = buffer.getReadPointer(actualInputChannel);
//            auto* channelData = buffer.getWritePointer (channel);
//            // ..do something to the data...
//            // _transportSource.getNextAudioBlock(channelData);
//            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
//                channelData[sample] = inputData[sample];
//        }
    
    dsp::AudioBlock<float> block (buffer);
    
    if (block.getNumChannels() > 2)
        block = block.getSubsetChannelBlock (0, 2);
    
    _convolution.process(dsp::ProcessContextReplacing<float> (block));

//    AudioSourceChannelInfo bufferToFill;
//    bufferToFill.buffer = &buffer;
//    bufferToFill.startSample = 0;
//    bufferToFill.numSamples = buffer.getNumSamples();
//
//    _transportSource.getNextAudioBlock(bufferToFill);
//
    // this is really just overall gain rn
    if(curr_dry_gain == prev_dry_gain){
        buffer.applyGain(curr_dry_gain);
    }
    else{
        buffer.applyGainRamp(0, buffer.getNumSamples(), prev_dry_gain, curr_dry_gain);
        prev_dry_gain = curr_dry_gain;
    }
    
}

//==============================================================================
bool VacancyAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* VacancyAudioProcessor::createEditor()
{
    return new VacancyAudioProcessorEditor (*this, _parameters);
}

//==============================================================================
void VacancyAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    ScopedPointer<XmlElement> xml (_parameters.state.createXml());
    copyXmlToBinary (*xml, destData);
}

void VacancyAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState != nullptr)
        if (xmlState->hasTagName (_parameters.state.getType()))
            _parameters.state = ValueTree::fromXml (*xmlState);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VacancyAudioProcessor();
}
