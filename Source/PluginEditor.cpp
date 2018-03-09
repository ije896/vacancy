/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
VacancyAudioProcessorEditor::VacancyAudioProcessorEditor (VacancyAudioProcessor& p, AudioProcessorValueTreeState& vts)
: AudioProcessorEditor (&p), processor (p), valueTreeState(vts),
thumbnailCache(5), thumbnail(512, formatManager, thumbnailCache)

{
    // sample open button params
    openFileButton.setButtonText("Load IR");
    openFileButton.addListener(this);
    
    playIRButton.setButtonText("Play IR");
    playIRButton.addListener(this);
    
    // volume slider parameters
    dryGainSlider.setSliderStyle(Slider::LinearVertical);

    dryGainSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 30);
    dryGainSlider.setPopupDisplayEnabled(true, false, this);
    //    dryGainSlider.setRange(-60.0, 0.0);
//    dryGainSlider.setTextValueSuffix(" dB");
//    dryGainSlider.setValue(-50.0);
    // dryGainSlider.addListener(this);
    
    dryGainLabel.setText ("Dry Gain", dontSendNotification);
    addAndMakeVisible (dryGainLabel);
    addAndMakeVisible (dryGainSlider);
    dryGainAttachment = new SliderAttachment (valueTreeState, "dry_gain", dryGainSlider);
    
    // add format manager
    formatManager.registerBasicFormats();
    thumbnail.addChangeListener(this);
    
    // display items
    addAndMakeVisible(&dryGainSlider);
    addAndMakeVisible(&openFileButton);
    addAndMakeVisible(&playIRButton);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(600, 400);
    
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
                // transportSource->start();
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
void VacancyAudioProcessorEditor::sliderValueChanged(Slider* slider){
    // processor._dryGainSlider = dryGainSlider.getValue();
}

// this loads the file and passes it to the backend
// also sets the thumbnail
void VacancyAudioProcessorEditor::openFileButtonClicked(){
    DBG("open file clicked");
    FileChooser chooser ("Select a Wave file to play...",
                         File::nonexistent,
                         "*.wav");
    
    if (chooser.browseForFileToOpen())
    {
        File file (chooser.getResult());
        processor.loadIR(file);
        thumbnail.setSource (new FileInputSource (file));
    }
}

void VacancyAudioProcessorEditor::playIRButtonClicked(){
    DBG("play ir clicked");
    changeState(Playing);
    processor.playIR();
}

// callback for changes in a broadcaster
void VacancyAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* source){
    if(source == &thumbnail){
        repaint();
    }
}


VacancyAudioProcessorEditor::~VacancyAudioProcessorEditor()
{
    
}

//==============================================================================
void VacancyAudioProcessorEditor::paintNoFileLoaded(Graphics& g, const Rectangle<int>& thumbnailBounds){
    g.setColour (Colours::darkgrey);
    g.fillRect (thumbnailBounds);
    g.setColour (Colours::white);
    g.drawFittedText ("No IR Loaded", thumbnailBounds, Justification::centred, 1.0f);
}

void VacancyAudioProcessorEditor::paintWithFileLoaded(Graphics& g, const Rectangle<int>& thumbnailBounds){
    g.setColour (Colours::white);
    g.fillRect (thumbnailBounds);
    g.setColour (Colours::black);
    thumbnail.drawChannels (g, thumbnailBounds, 0.0f, thumbnail.getTotalLength(),  1.0f);
}

void VacancyAudioProcessorEditor::paint (Graphics& g)
{
    const Rectangle<int> thumbnailBounds (80, 120, getWidth() - 150, getHeight() - 180);
    g.fillAll(Colours::black);
    
    if(thumbnail.getNumChannels()==0) paintNoFileLoaded(g, thumbnailBounds);
    else paintWithFileLoaded(g, thumbnailBounds);
    
    // written text at top
    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Convol. Reverb", 0 , 0, getWidth(), 30, Justification::centred, 1);
}

void VacancyAudioProcessorEditor::resized()
{
    // int center = getWidth()/2;
    // actually add our components to the window
    openFileButton.setBounds(150, 30, getWidth() - 300, 30);
    playIRButton.setBounds(150, 70, getWidth()-300, 30);
    dryGainSlider.setBounds(getWidth()-70, 80, 60, 280);
    dryGainLabel.setBounds(getWidth()-70, 350, 60, 20);
}
