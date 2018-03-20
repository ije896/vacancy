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
    
    // filters
    LPFCutoffSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    LPFCutoffSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 30);
    LPFCutoffSlider.setPopupDisplayEnabled(true, true, this);
    
    LPFCutoffLabel.setText("LP Cutoff", dontSendNotification);
    addAndMakeVisible(LPFCutoffLabel);
    addAndMakeVisible(LPFCutoffSlider);
    LPFAttachment = new SliderAttachment (valueTreeState, "lpf_cutoff", LPFCutoffSlider);
    
    HPFCutoffSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    HPFCutoffSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 30);
    HPFCutoffSlider.setPopupDisplayEnabled(true, true, this);
    
    HPFCutoffLabel.setText("HP Cutoff", dontSendNotification);
    addAndMakeVisible(HPFCutoffLabel);
    addAndMakeVisible(HPFCutoffSlider);
    HPFAttachment = new SliderAttachment (valueTreeState, "hpf_cutoff", HPFCutoffSlider);
    
    // Volume Envelope Params
    InitialLevelSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    InitialLevelSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 30);
    InitialLevelSlider.setPopupDisplayEnabled(true, true, this);
    
    InitialLevelLabel.setText("Initial Level", dontSendNotification);
    addAndMakeVisible(InitialLevelLabel);
    addAndMakeVisible(InitialLevelSlider);
    InitialLevelAttachment = new SliderAttachment (valueTreeState, "initial_level", InitialLevelSlider);
    
    AttackTimeSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    AttackTimeSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 30);
    AttackTimeSlider.setPopupDisplayEnabled(true, true, this);
    AttackTimeSlider.addListener(this);
    
    AttackTimeLabel.setText("Attack Time", dontSendNotification);
    addAndMakeVisible(AttackTimeLabel);
    addAndMakeVisible(AttackTimeSlider);
    AttackTimeAttachement = new SliderAttachment (valueTreeState, "attack_time", AttackTimeSlider);
    
    FinalLevelSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    FinalLevelSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 30);
    FinalLevelSlider.setPopupDisplayEnabled(true, true, this);
    
    FinalLevelLabel.setText("Final Level", dontSendNotification);
    addAndMakeVisible(FinalLevelLabel);
    addAndMakeVisible(FinalLevelSlider);
    FinalLevelAttachement = new SliderAttachment (valueTreeState, "final_level", FinalLevelSlider);
    
    DecayTimeSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    DecayTimeSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 30);
    DecayTimeSlider.setPopupDisplayEnabled(true, true, this);
    DecayTimeSlider.addListener(this);
    
    DecayTimeLabel.setText("Decay Time", dontSendNotification);
    addAndMakeVisible(DecayTimeLabel);
    addAndMakeVisible(DecayTimeSlider);
    DecayTimeAttachement = new SliderAttachment (valueTreeState, "decay_time", DecayTimeSlider);
    
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


void VacancyAudioProcessorEditor::reverseIRButtonClicked(){
    processor._useReverseIR = !processor._useReverseIR;
}

// callback for changes in a broadcaster
void VacancyAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* source){
    if(source == &thumbnail){
        repaint();
    }
    // update parameters if attack or decay time sliders are updated
}


VacancyAudioProcessorEditor::~VacancyAudioProcessorEditor()
{
    dryGainAttachment = nullptr;
    LPFAttachment = nullptr;
    HPFAttachment = nullptr;
    AttackTimeAttachement = nullptr;
    InitialLevelAttachment = nullptr;
    FinalLevelAttachement = nullptr;
    DecayTimeAttachement = nullptr;
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
    const Rectangle<int> thumbnailBounds (30, 50, getWidth() - 150, getHeight() - 180);
    const Rectangle<int> envelopeBounds (175, 295, 295, 80);
    // draw bounds for filterBounds
    
    g.fillAll(Colours::black);
    
    g.setColour (Colours::grey);
    g.drawRect(envelopeBounds);
    
    if(thumbnail.getNumChannels()==0) paintNoFileLoaded(g, thumbnailBounds);
    else paintWithFileLoaded(g, thumbnailBounds);
}

void VacancyAudioProcessorEditor::resized()
{
    // actually add our components to the window
    openFileButton.setBounds(105, 10, getWidth() - 300, 30);
    dryGainSlider.setBounds(getWidth()-115, 0, 60, 280);
    dryGainLabel.setBounds(getWidth()-115, 280, 60, 20);
    wetGainSlider.setBounds(getWidth()-60, 0, 60, 280);
    wetGainLabel.setBounds(getWidth()-60, 280, 60, 20);
    reverseIRButton.setBounds(30, 275, 80, 20);
    
    HPFCutoffSlider.setBounds(15, 300, 60, 60);
    HPFCutoffLabel.setBounds(15, 350, 60, 20);
    LPFCutoffSlider.setBounds(70, 300, 60, 60);
    LPFCutoffLabel.setBounds(70, 350, 60, 20);
    
    InitialLevelSlider.setBounds(185, 300, 60, 60);
    InitialLevelLabel.setBounds(180, 330, 70, 60);
    AttackTimeSlider.setBounds(255, 300, 60, 60);
    AttackTimeLabel.setBounds(250, 330, 70, 60);
    FinalLevelSlider.setBounds(325, 300, 60, 60);
    FinalLevelLabel.setBounds(325, 330, 60, 60);
    DecayTimeSlider.setBounds(395, 300, 60, 60);
    DecayTimeLabel.setBounds(390, 330, 70, 60);

}
