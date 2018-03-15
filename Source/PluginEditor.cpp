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
    // openIR button init
    openFileButton.setButtonText("Load IR");
    openFileButton.addListener(this);
    
    playIRButton.setButtonText("Play IR");
    playIRButton.addListener(this);
    
    // gain sliders init
    dryGainSlider.setSliderStyle(Slider::LinearVertical);
    dryGainSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 30);
    dryGainSlider.setPopupDisplayEnabled(false, false, this);

    dryGainLabel.setText ("Dry Gain", dontSendNotification);
    addAndMakeVisible (dryGainLabel);
    addAndMakeVisible (dryGainSlider);
    dryGainAttachment = new SliderAttachment (valueTreeState, "dry_gain", dryGainSlider);
    
    wetGainSlider.setSliderStyle(Slider::LinearVertical);
    wetGainSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 30);
    wetGainSlider.setPopupDisplayEnabled(false, false, this);
    
    wetGainLabel.setText ("Wet Gain", dontSendNotification);
    addAndMakeVisible (wetGainLabel);
    addAndMakeVisible (wetGainSlider);
    wetGainAttachment = new SliderAttachment (valueTreeState, "wet_gain", wetGainSlider);
    
    //    dryGainSlider.setRange(-60.0, 0.0);
//    dryGainSlider.setTextValueSuffix(" dB");
//    dryGainSlider.setValue(-50.0);
    // dryGainSlider.addListener(this);
    
    // reverse button
    reverseIRButton.setButtonText("Reverse");
    addAndMakeVisible(reverseIRButton);
    reverseIRButton.addListener(this);
    
    //reverseIRAttachment = new ButtonAttachment(valueTreeState, "revrseIR", reverseIRButton);
    
    // add format manager
    formatManager.registerBasicFormats();
    thumbnail.addChangeListener(this);
    
    // display items
    // addAndMakeVisible(&dryGainSlider);
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
    if (button == &reverseIRButton) reverseIRButtonClicked();
}
void VacancyAudioProcessorEditor::sliderValueChanged(Slider* slider){
    // processor._dryGainSlider = dryGainSlider.getValue();
}

// this loads the file and passes it to the backend
// also sets the thumbnail
void VacancyAudioProcessorEditor::openFileButtonClicked(){
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
    changeState(Playing);
    processor.playIR();
}

void VacancyAudioProcessorEditor::reverseIRButtonClicked(){
    processor._useReverseIR = !processor._useReverseIR;
}

// callback for changes in a broadcaster
void VacancyAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* source){
    if(source == &thumbnail){
        repaint();
    }
}


VacancyAudioProcessorEditor::~VacancyAudioProcessorEditor()
{
    dryGainAttachment = nullptr;
    // reverseIRAttachment = nullptr;
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
    const Rectangle<int> thumbnailBounds (30, 105, getWidth() - 150, getHeight() - 180);
    g.fillAll(Colours::black);
    
    if(thumbnail.getNumChannels()==0) paintNoFileLoaded(g, thumbnailBounds);
    else paintWithFileLoaded(g, thumbnailBounds);
    
    // written text at top
//    g.setColour (Colours::white);
//    g.setFont (15.0f);
//    g.drawFittedText ("Convol. Reverb", 0 , 0, getWidth(), 30, Justification::centred, 1);
}

void VacancyAudioProcessorEditor::resized()
{
    // int center = getWidth()/2;
    // actually add our components to the window
    openFileButton.setBounds(115, 50, getWidth() - 300, 30);
    // playIRButton.setBounds(150, 70, getWidth()-300, 30);
    dryGainSlider.setBounds(getWidth()-115, 80, 60, 280);
    dryGainLabel.setBounds(getWidth()-115, 350, 60, 20);
    wetGainSlider.setBounds(getWidth()-60, 80, 60, 280);
    wetGainLabel.setBounds(getWidth()-60, 350, 60, 20);
    reverseIRButton.setBounds(30, 330, 80, 20);
}
