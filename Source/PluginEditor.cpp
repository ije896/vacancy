/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
// copy constructor
VacancyAudioProcessorEditor::VacancyAudioProcessorEditor (VacancyAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
   
    // sample open button params
    openFileButton.setButtonText("Load IR");
    openFileButton.addListener(this);
    
    playIRButton.setButtonText("Play IR");
    playIRButton.addListener(this);
    
    // volume slider parameters
    level.setSliderStyle(Slider::LinearVertical);
    level.setRange(-60.0, 0.0);
    level.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    level.setPopupDisplayEnabled(true, false, this);
    level.setTextValueSuffix(" dB");
    level.setValue(-50.0);
    
    // add format manager
    formatManager.registerBasicFormats();
    transportSource.addChangeListener(this);
    
    // display items
    addAndMakeVisible(&level);
    addAndMakeVisible(&openFileButton);
    addAndMakeVisible(&playIRButton);
    
}

void VacancyAudioProcessorEditor::changeState(TransportState nextState){
    if (state!=nextState){
        state = nextState;
        switch(state){
            case Stopped:
                break;
            case Starting:
                break;
            case Playing:
                transportSource.start();
                break;
            case Stopping:
                break;
        }
    }
}

void VacancyAudioProcessorEditor::buttonClicked (Button* button)
{
    if (button == &openFileButton) openFileButtonClicked();
    if (button == &playIRButton) playIRButtonClicked();
}

void VacancyAudioProcessorEditor::openFileButtonClicked(){
    DBG("open file clicked");
    
    FileChooser chooser ("Select a Wave file to play...",
                         File::nonexistent,
                         "*.wav");
    
    if (chooser.browseForFileToOpen())
    {
        File file (chooser.getResult());
        AudioFormatReader* reader = formatManager.createReaderFor (file);
        
        if (reader != nullptr)
        {
            ScopedPointer<AudioFormatReaderSource> newSource = new AudioFormatReaderSource (reader, true);
            transportSource.setSource (newSource, 0, nullptr, reader->sampleRate);
            // playButton.setEnabled (true);
            readerSource = newSource.release();
        }
    }
}

void VacancyAudioProcessorEditor::playIRButtonClicked(){
    DBG("play ir clicked");
    changeState(Playing);
}

void VacancyAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* source){}


VacancyAudioProcessorEditor::~VacancyAudioProcessorEditor()
{
    VacancyAudioProcessorEditor::transportSource.releaseResources();
}

//==============================================================================
void VacancyAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.fillAll(Colours::black);
    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Convol. Reverb", 0 , 0, getWidth(), 30, Justification::centred, 1);
}

void VacancyAudioProcessorEditor::resized()
{
    // actually add our components to the window
    openFileButton.setBounds(100, 30, getWidth() - 200, 30);
    playIRButton.setBounds(100, 70, getWidth()-200, 30);
    level.setBounds(40, 30, 40, getHeight() - 60);
}
