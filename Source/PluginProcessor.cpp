/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/


/********************TODO************************
 gain in dB (not necessary)

 Add level meters? Not much of a need to
 
 Change Length/Start of IR

 
 Volume envelope
    - initial level
    - attack (start point for envelope)
    - end level
    - decay (end point for envelope)
 
 Line 306 for applying filter func
 
 Variable SR for the IR
 Change the thumbnail to show reversed IR when selected
 Predelay
 Check for mono vs stereo IR
 Maybe accept different types of IR's? Like mp3?...
 Accept MIDI input?
 Image parsing and convolution!!!!!
 Make all buttons APVTS params
 
 for debugging AU:
 https://forum.juce.com/t/au-plugin-install-working-for-ableton-but-not-logic-pro-x/24211/2
 *************************************************/
//#define cimg_use_magick
//#define cimg_use_jpg
//#define cimg_use_png
//#include "cimg/CImg.h"

#include "PluginProcessor.h"
#include "PluginEditor.h"


//using namespace cimg_library;

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
        _lowPassFilter  (dsp::IIR::Coefficients<float>::makeFirstOrderLowPass  (44100.0, 21000.0f)),
        _highPassFilter (dsp::IIR::Coefficients<float>::makeFirstOrderHighPass (44100.0, 20.0f)),
        _parameters(*this, nullptr)
#endif
{
    _parameters.createAndAddParameter("dry_gain", "Dry Gain", String(), NormalisableRange<float> (0.0f, 1.0f), 0.7f, nullptr, nullptr);
    _parameters.createAndAddParameter("wet_gain", "Wet Gain", String(), NormalisableRange<float> (0.0f, 1.0f), 0.7f, nullptr, nullptr);
    _parameters.createAndAddParameter("lpf_cutoff", "LP Cutoff", String(), NormalisableRange<float> (500.0f, 21000.0f), 21000.0f, nullptr, nullptr);
    _parameters.createAndAddParameter("hpf_cutoff", "HP Cutoff", String(), NormalisableRange<float> (20.0f, 700.0f), 20.0f, nullptr, nullptr);
    
    _parameters.createAndAddParameter("initial_level", "Initial Level", String(), NormalisableRange<float> (0.0f, 1.0f), 1.0f, nullptr, nullptr);
    _parameters.createAndAddParameter("attack_time", "Attack Time", String(), NormalisableRange<float> (0.0f, 1.0f), 1.0f, nullptr, nullptr);
//    _parameters.createAndAddParameter ("reverseIR", "Reverse IR", String(),
//                                       NormalisableRange<float> (0.0f, 1.0f, 1.0f), 0.0f,
//                                       reverseToText,    // value to text function
//                                       textToReverse);
    
    _parameters.state = ValueTree(Identifier("VacancyParams"));
    
    _formatManager.registerBasicFormats();
    
//    do_image_stuff();
    
}

//void VacancyAudioProcessor::do_image_stuff(){
//    
//    CImg<float> defimg("ripples.jpg");
//    CImgDisplay main_disp(defimg,"Original");
//    
//}
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
            
                break;
            case Stopped:
            
                break;
            case Playing:
                
            default:
                break;
        }
    }
}

void VacancyAudioProcessor::loadIR(File file){
    _IRFile = file;
    AudioFormatReader* reader = _formatManager.createReaderFor(file);
    
    if (reader != nullptr)
    {
        // load IR into buffer for reversal
        reversedIRBuffer.setSize (reader->numChannels, int(reader->lengthInSamples));
        reader->read (&reversedIRBuffer,
                      0,
                      int(reader->lengthInSamples),
                      0,
                      true,
                      true);
        
        // copy reversedIR again for volume envelope
        
        forwardIRBuffer = AudioSampleBuffer(reversedIRBuffer);
        envelopedIRBuffer = AudioSampleBuffer(reversedIRBuffer);
//        DBG(reversedIRBuffer.getNumChannels());
//        for (int channel = 0; channel< reversedIRBuffer.getNumChannels(); channel++){
//            DBG("Channel");
//            DBG(channel);
//            DBG("LoadIR forwardBuffer copy");
//            forwardIRBuffer.copyFrom(
//                                     2, // destChannel
//                                     0, //destStartSample
//                                     reversedIRBuffer, // sourceBuffer
//                                     2, // sourceChannel
//                                     0, // sourceStartSample,
//                                     reversedIRBuffer.getNumSamples()
//                                     );
//            DBG("LoadIR envelopedBuffer copy");
//            envelopedIRBuffer.copyFrom(
//                                       2, // destChannel
//                                       0, //destStartSample
//                                       reversedIRBuffer, // sourceBuffer
//                                       2, // sourceChannel
//                                       0, // sourceStartSample,
//                                       reversedIRBuffer.getNumSamples()
//                                       );
//        }
        
        // standard, load regular IR
        _convolution.copyAndLoadImpulseResponseFromBuffer(envelopedIRBuffer, getSampleRate(), true, true, true, 0);
        
        reverseIR(reversedIRBuffer);
        
        // set volume envelope params
        IRVolumeEnvelope.setInitialValue(1.0f);
        float IRLengthInSeconds = forwardIRBuffer.getNumSamples() / getSampleRate();
        // attack, decaytime, slevel, rtime
        IRVolumeEnvelope.setAllTimes(0.0f, IRLengthInSeconds, 1.0f, 0.001f);
    }
}

void VacancyAudioProcessor::reverseIR(AudioSampleBuffer& inBuffer){
    auto totalNumInputChannels = getTotalNumInputChannels();
    for (int channel = 0; channel < inBuffer.getNumChannels(); ++channel)
    {
        const int actualInputChannel = channel % totalNumInputChannels;
        
        auto* inputData = inBuffer.getReadPointer(actualInputChannel);
        auto* reverseData = inBuffer.getWritePointer(actualInputChannel);
        
        float tmp;
        const int end = inBuffer.getNumSamples();
        for (int sample = 0; sample < end/2; ++sample){
            tmp = inputData[sample];
            reverseData[sample] = inputData[end-sample-1];
            reverseData[end - sample - 1] = tmp;
        }
    }
}

void VacancyAudioProcessor::playIR(){
    changeState(Starting);
}

void VacancyAudioProcessor::changeListenerCallback(ChangeBroadcaster* source){
    if(source){
        if(/* DISABLES CODE */ (true)){
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
    // in here, multiply the irfile or the buffer by the envelope
    
    //applyIREnvelope();
    
    if (isUsingReversed != _useReverseIR){
        if (_useReverseIR){
            _convolution.copyAndLoadImpulseResponseFromBuffer(reversedIRBuffer, getSampleRate(), true, true, true, 0);
            isUsingReversed = true;
        }
        else{
            _convolution.loadImpulseResponse(_IRFile, true, false, 0);
            isUsingReversed = false;
        }
    }
    *_lowPassFilter.state  = *dsp::IIR::Coefficients<float>::makeLowPass  (getSampleRate(), *_parameters.getRawParameterValue("lpf_cutoff"));
    *_highPassFilter.state = *dsp::IIR::Coefficients<float>::makeHighPass (getSampleRate(), *_parameters.getRawParameterValue("hpf_cutoff"));
}

void VacancyAudioProcessor::applyIREnvelope(){
    // first, clear buffer, then copy correct buffer into envelope buffer
    envelopedIRBuffer.clear();
    if(isUsingReversed){
        for (int channel = 0; channel< reversedIRBuffer.getNumChannels(); channel++){
            DBG("apply envelope reverse copy");
            envelopedIRBuffer.copyFrom(
                                     channel, // destChannel
                                     0, //destStartSample
                                     reversedIRBuffer, // sourceBuffer
                                     channel, // sourceChannel
                                     0, // sourceStartSample,
                                     reversedIRBuffer.getNumSamples()
                                     );
        }
    }
    else{
        for (int channel = 0; channel< forwardIRBuffer.getNumChannels(); channel++){
            DBG("apply envelope forwardBuffer copy");
            envelopedIRBuffer.copyFrom(
                                              channel, // destChannel
                                              0, //destStartSample
                                              forwardIRBuffer, // sourceBuffer
                                              channel, // sourceChannel
                                              0, // sourceStartSample,
                                              forwardIRBuffer.getNumSamples()
                                              );
        }
    }

    auto totalNumInputChannels = getTotalNumInputChannels();
    for (int channel = 0; channel < envelopedIRBuffer.getNumChannels(); ++channel)
    {
        const int actualInputChannel = channel % totalNumInputChannels;
        
        // auto* readData = envelopedIRBuffer.getReadPointer(actualInputChannel);
        auto* writeData = envelopedIRBuffer.getWritePointer(actualInputChannel);
        
        for (int sample = 0; sample<envelopedIRBuffer.getNumSamples(); sample++){
            writeData[sample] *= IRVolumeEnvelope.tick();
        }
    }
    
}
void VacancyAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    prev_dry_gain = *_parameters.getRawParameterValue("dry_gain");
    prev_wet_gain = *_parameters.getRawParameterValue("wet_gain");
    setPlayConfigDetails(2, 2, sampleRate, samplesPerBlock);
    
    // setup convolution
    auto channels = static_cast<uint32> (jmin (getMainBusNumInputChannels(), getMainBusNumOutputChannels()));
    dsp::ProcessSpec spec { sampleRate, static_cast<uint32> (samplesPerBlock), channels };
    _convolution.prepare(spec);
    _lowPassFilter.prepare(spec);
    _highPassFilter.prepare(spec);
}

void VacancyAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    _readerSource.release();
    
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
    updateParams();
    
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    dsp::AudioBlock<float> block (buffer);
    if (block.getNumChannels() > 2)
        block = block.getSubsetChannelBlock (0, 2);
    
    _highPassFilter.process(dsp::ProcessContextReplacing<float> (block));
    
    const float curr_dry_gain = *_parameters.getRawParameterValue("dry_gain");
    const float curr_wet_gain = *_parameters.getRawParameterValue("wet_gain");
    
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
    
    AudioSampleBuffer dryBuffer(buffer.getNumChannels(), buffer.getNumSamples());
    
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel){
    dryBuffer.copyFrom(
                       channel, // destChannel
                       0, //destStartSample
                       buffer, // sourceBuffer
                       channel, // sourceChannel
                       0, // sourceStartSample,
                       buffer.getNumSamples()
                       );
    }
    
   
    _lowPassFilter.process(dsp::ProcessContextReplacing<float> (block));
    
    _convolution.process(dsp::ProcessContextReplacing<float> (block));
    
    

    if(curr_wet_gain == prev_wet_gain){
        buffer.applyGain(curr_wet_gain);
    }
    else{
        buffer.applyGainRamp(0, buffer.getNumSamples(), prev_wet_gain, curr_wet_gain);
        prev_wet_gain = curr_wet_gain;
    }
    
    if(curr_dry_gain == prev_dry_gain){
        dryBuffer.applyGain(curr_dry_gain);
    }
    else{
        dryBuffer.applyGainRamp(0, buffer.getNumSamples(), prev_dry_gain, curr_dry_gain);
        prev_dry_gain = curr_dry_gain;
    }
    
    // combine the wet and dry buffers
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel){
        buffer.addFrom(
                           channel, // destChannel
                           0, // destStartSample
                           dryBuffer, // sourceBuffer
                           channel, // sourceChannel
                           0, // soucreStartSample,
                           buffer.getNumSamples(),
                           1.0 // gain
                           );
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
