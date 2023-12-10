/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

enum RadioButtonIds
{
    SynthAlgorithmID = 1001
};

//class ToggleButtonWrapper {
//public:
//    ToggleButtonWrapper(std::string name, SythesiserAlgorithm algNum, ImageAudialisationAudioProcessor& processor) {
//        Button.setRadioGroupId(SynthAlgorithmID);
//        Button.onClick = [this, processor]() {
//            *processor.algorithmParam = static_cast<float>(Noise);
//            };
//        Button.setToggleState(*processor.algorithmParam == static_cast<float>(Noise), juce::NotificationType::dontSendNotification);
//        Button.setButtonText("Image as noise");
//    }
//
//private:
//    juce::ToggleButton Button;
//};

//==============================================================================
ImageSonificationProcessorEditor::ImageSonificationProcessorEditor(ImageSonificationProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    

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

    /*noiseToggle.setRadioGroupId(SynthAlgorithmID);
    noiseToggle.onClick = [this]() {
        *audioProcessor.algorithmParam = static_cast<float>(Noise);
    };
    noiseToggle.setToggleState(*audioProcessor.algorithmParam == static_cast<float>(Noise), juce::NotificationType::dontSendNotification);
    noiseToggle.setButtonText("Image as noise");*/
    

    //toggle2.setRadioGroupId(SynthAlgorithmID);
    //toggle2.onClick = [this]() {
    //    *audioProcessor.algorithmParam = static_cast<float>(alg2);
    //};
    //toggle2.setToggleState(*audioProcessor.algorithmParam == static_cast<float>(alg2), juce::NotificationType::dontSendNotification);
    //noiseToggle.setButtonText("Algorithm 2");


    //addAndMakeVisible(&noiseToggle);
    //addAndMakeVisible(&toggle2);

    setSize(500, 400);
    //imagePathText.setEditable(true);
    imagePathText.onTextChange = [this] {
        auto tempImage = juce::JPEGImageFormat::loadFrom(juce::File::File(imagePathText.getText()));
        if (tempImage.isValid()) {
            audioProcessor.imageBitmapPtr.reset(nullptr);
            audioProcessor.image = tempImage;
            imageComponent.setImage(audioProcessor.image);
            auto temp = juce::Image::BitmapData(audioProcessor.image, juce::Image::BitmapData::readOnly);
            audioProcessor.resetImageData(temp.width, temp.height);
            audioProcessor.imageBitmapPtr.reset(new juce::Image::BitmapData(audioProcessor.image, juce::Image::BitmapData::readOnly));
        }
    };

    if (audioProcessor.imageBitmapPtr == nullptr) {
        imagePathText.setText("C:/Users/OEM/Documents/wtyczki/ImageAudialisation/Jan_Matejko_Stanczyk.jpg");
    }
    else {
        imageComponent.setImage(audioProcessor.image);
    }

    addAndMakeVisible(imagePathText);

    addAndMakeVisible(&imageComponent);
}

ImageSonificationProcessorEditor::~ImageSonificationProcessorEditor()
{

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

    int it = 40;
    for (auto button : buttons) {
        button.button->setBounds(10, it, 60, 25);
        it += 25;
    }

    /*noiseToggle.setBounds(10, 40, 60, 25);
    toggle2.setBounds(10, 65, 60, 25);*/

    imageComponent.setBounds(10, 90, getWidth() - 20, getHeight() - 100);
}
