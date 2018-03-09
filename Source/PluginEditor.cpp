/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
VacancyAudioProcessorEditor::VacancyAudioProcessorEditor (VacancyAudioProcessor& p)
: AudioProcessorEditor (&p), processor (p), thumbnailCache(5), thumbnail(512, formatManager, thumbnailCache)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(600, 400);
    
   
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
    level.addListener(this);
    
    // add format manager
    formatManager.registerBasicFormats();
//    transportSource->addChangeListener(this);
    
    thumbnail.addChangeListener(this);
    // processor._thumbnail.addChangeListener(this);
    
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
    processor.dry_gain = level.getValue();
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

// this is what gets called if there are changes in any broadcaster
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
    g.drawFittedText ("No File Loaded", thumbnailBounds, Justification::centred, 1.0f);
}

void VacancyAudioProcessorEditor::paintWithFileLoaded(Graphics& g, const Rectangle<int>& thumbnailBounds){
    g.setColour (Colours::white);
    g.fillRect (thumbnailBounds);
    g.setColour (Colours::black);
    thumbnail.drawChannels (g, thumbnailBounds, -0.2f, thumbnail.getTotalLength(),  1.0f);
}

void VacancyAudioProcessorEditor::paint (Graphics& g)
{
    const Rectangle<int> thumbnailBounds (80, 120, getWidth() - 100, getHeight() - 180);
    
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
    level.setBounds(40, 30, 40, getHeight() - 60);
}
