/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

enum RadioButtonIds
{
    SynthAlgorithmID = 1001,
    CrawlingDirection = 1002
};






//==============================================================================
ImageSonificationProcessorEditor::ImageSonificationProcessorEditor(ImageSonificationProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    windowSizeSlider.addListener (this);

    
    // BUTTONS FOR ALGORITHM CHOOSING
    for (auto button: buttons) {
        button.button->setRadioGroupId(SynthAlgorithmID);
        auto alg = button.alg;
        button.button->onClick = [this, alg]() {
            *audioProcessor.algorithmParam = static_cast<float>(alg);
        };

        button.button->setToggleState(*audioProcessor.algorithmParam == static_cast<float>(alg), juce::NotificationType::dontSendNotification);
        button.button->setButtonText(button.name);
        addAndMakeVisible(button.button);
    }


    // BUTTONS FOR CRAWLING DIRECTIONS
    for (auto button : crawl_direction_buttons) {
        button.button->setRadioGroupId(CrawlingDirection);
        auto dir = button.alg;
        button.button->onClick = [this, dir]() {
            *audioProcessor.crawlingDirectionParam = static_cast<float>(dir);
        };

        button.button->setToggleState(*audioProcessor.crawlingDirectionParam == static_cast<float>(dir), juce::NotificationType::dontSendNotification);
        button.button->setButtonText(button.name);
        addAndMakeVisible(button.button);
    }

    // IMAGE DISPLAY
    imagePathText.onTextChange = [this] {
        auto tempImage = juce::JPEGImageFormat::loadFrom(juce::File(imagePathText.getText()));
        if (tempImage.isValid()) {
            audioProcessor.imageIsBeingLoaded = true;
            audioProcessor.image = tempImage;
            audioProcessor.resetBitmap();
            imageComponent.setImage(audioProcessor.image);
            audioProcessor.resetBitmap();
            audioProcessor.imageIsBeingLoaded = false;
               
            // TODO - maybe cap slider max value at height or width?
            //int newSliderMaxValue = (audioProcessor.imageBitmapPtr->height > audioProcessor.imageBitmapPtr->width) ? audioProcessor.imageBitmapPtr->width : audioProcessor.imageBitmapPtr->height;
            //
            //windowSizeSlider.setRange(1, newSliderMaxValue, 1);
        }
    };

    if (audioProcessor.imageBitmapPtr == nullptr) {
        imagePathText.setText("Enter path to image");
    }
    else {
        imageComponent.setImage(audioProcessor.image);
    }

    addAndMakeVisible(imagePathText);
    addAndMakeVisible(&imageComponent);

    setSize(800, 700);
    
    windowSizeSlider.setSliderStyle (juce::Slider::LinearBar);
    windowSizeSlider.setRange(1, 100, 1); // Zakres od 1 do 100 z krokiem 1
    windowSizeSlider.setTextBoxStyle (juce::Slider::TextBoxAbove, false, 90, 0);
    windowSizeSlider.setPopupDisplayEnabled (true, false, this);
//    windowSize.setTextValueSuffix ("Window size");
    windowSizeSlider.setValue(1);
//    windowSize.setTitle("Set window size as percent of image size(only for windowing algorithm)");
    windowSizeSlider.setTextValueSuffix(": is the current window size (only for windowing algorithm)");
    // MOD 4 dodanie suwaka do edytora
    addAndMakeVisible (&windowSizeSlider);
    
}






ImageSonificationProcessorEditor::~ImageSonificationProcessorEditor()
{

}

// MOD 13 definicja funkcji do obslugi
void ImageSonificationProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
audioProcessor.valueOfSlider = windowSizeSlider.getValue();
}


//==============================================================================
void ImageSonificationProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
}

void ImageSonificationProcessorEditor::resized()
{
    
    
    imagePathText.setBounds(10, 10, getWidth() - 20, 20);

    int it = 170;
    for (auto button : crawl_direction_buttons) {
        button.button->setBounds(it, 40, 150, 25);
        it += 150;
    }

    it = 40;
    for (auto button : buttons) {
        button.button->setBounds(10, it, 150, 25);
        it += 25;
    }

    imageComponent.setBounds(10, getHeight()-425, getWidth() - 20, 400);
    
    windowSizeSlider.setBounds (150, 110, getWidth()-170, 20);
}
