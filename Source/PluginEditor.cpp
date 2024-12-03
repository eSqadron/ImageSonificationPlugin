/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

enum RadioButtonIds
{
    SynthAlgorithmButtonId = 1001,
    PixelByPixelDirectionButtonId = 1002
};






//==============================================================================
ImageSonificationProcessorEditor::ImageSonificationProcessorEditor(ImageSonificationProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    windowSizeSlider.addListener (this);

    
    // BUTTONS FOR ALGORITHM CHOOSING
    for (auto& button: AlgorithmsButtons) {
        button.button->setRadioGroupId(SynthAlgorithmButtonId);
        auto alg = button.alg;
        button.button->onClick = [this, alg]() {
            *audioProcessor.algorithmParam = static_cast<float>(alg);
        };

        button.button->setToggleState(*audioProcessor.algorithmParam == static_cast<float>(alg), juce::NotificationType::dontSendNotification);
        button.button->setButtonText(button.name);
        addAndMakeVisible(button.button);
    }


    // BUTTONS FOR CRAWLING DIRECTIONS
    for (auto& button : PixelByPixelDirectionsButtons) {
        button.button->setRadioGroupId(PixelByPixelDirectionButtonId);
        auto dir = button.alg;
        button.button->onClick = [this, dir]() {
            *audioProcessor.pixelByPixelDirectionParam = static_cast<float>(dir);
            audioProcessor.directionOfPixelByPixelPlay = static_cast<PixelByPixelDirection>(audioProcessor.pixelByPixelDirectionParam->load());
        };

        button.button->setToggleState(*audioProcessor.pixelByPixelDirectionParam == static_cast<float>(dir), juce::NotificationType::dontSendNotification);
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
               
            // TODO - maybe cap window slider max value at height or width?
            //int newSliderMaxValue = (audioProcessor.imageBitmapPtr->height > audioProcessor.imageBitmapPtr->width) ? audioProcessor.imageBitmapPtr->width : audioProcessor.imageBitmapPtr->height;
            //
            //windowSizeSlider.setRange(1, newSliderMaxValue, 1);
        }
    };

    if (AlgorithmBase::imageBitmapPtr == nullptr) {
        imagePathText.setText("Enter path to image");
    }
    else {
        imageComponent.setImage(audioProcessor.image);
    }

    addAndMakeVisible(imagePathText);
    addAndMakeVisible(&imageComponent);

    setSize(800, 700);
    
    windowSizeSlider.setSliderStyle(juce::Slider::LinearBar);
    windowSizeSlider.setRange(1, MAX_WINDOW_SIZE_FOR_PIXEL_BY_PIXEL, 1); // Range from 1 to MAX_WINDOW_SIZE_FOR_PIXEL_BY_PIXEL with step 1
    windowSizeSlider.setTextBoxStyle (juce::Slider::TextBoxAbove, false, 90, 0);
    windowSizeSlider.setPopupDisplayEnabled (true, false, this);
    windowSizeSlider.setValue(1);
    windowSizeSlider.setTextValueSuffix(": is the current window size");
    addAndMakeVisible (&windowSizeSlider);
}


ImageSonificationProcessorEditor::~ImageSonificationProcessorEditor()
{

}

void ImageSonificationProcessorEditor::sliderValueChanged(juce::Slider*)
{
    audioProcessor.WindowSizeSliderValue = static_cast<int>(windowSizeSlider.getValue());
}


//==============================================================================
void ImageSonificationProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
}

void ImageSonificationProcessorEditor::resized()
{
    // Here frontend is defined

    // TODO - make it prettier!
    
    imagePathText.setBounds(10, 10, getWidth() - 20, 20);

    int it = 170;
    for (auto& button : PixelByPixelDirectionsButtons) {
        button.button->setBounds(it, 40, 150, 25);
        it += 150;
    }

    it = 40;
    for (auto& button : AlgorithmsButtons) {
        button.button->setBounds(10, it, 150, 25);
        it += 25;
    }

    imageComponent.setBounds(10, getHeight()-425, getWidth() - 20, 400);
    
    windowSizeSlider.setBounds (150, 110, getWidth()-170, 20);
}
