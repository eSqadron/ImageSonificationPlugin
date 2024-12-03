/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "CommonTypeDefs.h"

//==============================================================================
ImageSonificationProcessor::ImageSonificationProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
    parameters(*this, nullptr, juce::Identifier("ImageSonification"),
        {
            std::make_unique<juce::AudioParameterInt>("algorithm",            // parameterID
                                                         "Algorithm",            // parameter name
                                                         0,              // minimum value
                                                         100,              // maximum value
                                                         0),            // default value
            std::make_unique<juce::AudioParameterInt>("pixelByPixel_direction",            // parameterID
                                                         "PixelByPixelDirectionButtonId",            // parameter name
                                                         0,              // minimum value
                                                         100,              // maximum value
                                                         0)            // default value
        }),
    // Initialize algorithms
    imageAsNoiseAlg(directionOfPixelByPixelPlay, WindowSizeSliderValue),
    eecs351wn22Alg(directionOfPixelByPixelPlay, WindowSizeSliderValue),
    // windowingAlg(WindowSizeSliderValue), OBSOLETE
    landscapeAlg()

#endif
{
    AlgorithmBase::imageBitmapPtr = nullptr;

    // TODO - maybe actually use this logger
    m_flogger = std::unique_ptr<juce::FileLogger>(juce::FileLogger::createDateStampedLogger("Juce", "visualiser", ".txt", "Welcome to plugin"));

    // DAW parameters
    // TODO - make them actually save values!
    algorithmParam = parameters.getRawParameterValue("algorithm");
    pixelByPixelDirectionParam = parameters.getRawParameterValue("pixelByPixel_direction");

    // TODO - maybe it is possible to immediately initialize array of objects, instead of creating array of pointers to objects?
    AlgorithmsArray[NoiseCrawler] = &imageAsNoiseAlg;
    AlgorithmsArray[SineChordCrawler] = &eecs351wn22Alg;
    // AlgorithmsArray[Windowing] = &windowingAlg; OBSOLETE
    AlgorithmsArray[Landscape] = &landscapeAlg;
}

ImageSonificationProcessor::~ImageSonificationProcessor()
{
    AlgorithmBase::imageBitmapPtr = nullptr;
}

//==============================================================================
const juce::String ImageSonificationProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ImageSonificationProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool ImageSonificationProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool ImageSonificationProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double ImageSonificationProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ImageSonificationProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int ImageSonificationProcessor::getCurrentProgram()
{
    return 0;
}

void ImageSonificationProcessor::setCurrentProgram(int)
{
}

const juce::String ImageSonificationProcessor::getProgramName(int)
{
    return {};
}

void ImageSonificationProcessor::changeProgramName(int, const juce::String&)
{
}

//==============================================================================
void ImageSonificationProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Call prepare to play on all agarithms that implement it!
    this->eecs351wn22Alg.prepareToPlay(sampleRate, samplesPerBlock);
}

void ImageSonificationProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ImageSonificationProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

// This function will be called in a loop, to process audio in real time!
// It will be called sample_rate/buffer_size times a second - both values defined in DAWs!
// where buffer size is amount of samples processed during one function call.
void ImageSonificationProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    int totalNumOutputChannels = getTotalNumOutputChannels();
    int sample_len = buffer.getNumSamples();

    // Do NOT play anything when image is not loaded or being loaded!
    if (AlgorithmBase::imageBitmapPtr == nullptr || imageIsBeingLoaded) {
        // Clear all channels:
        for (auto i = 0; i < totalNumOutputChannels; ++i)
            buffer.clear(i, 0, buffer.getNumSamples());
        return;
    }

    // mono sine - for mono signal generation
    float* mono_signal = new float[sample_len];

    // Depending on chosen algorithm, generate new buffer of samples
    // mono_signal - output
    // sample_len - amount of samples to generate
    AlgorithmsArray[static_cast<int>(*algorithmParam)]->generateNextSamples(mono_signal, sample_len);

    // rewrite mono signal into all (usually both - stereo) channels
    for (int channel = 0; channel < totalNumOutputChannels; ++channel) {
        float* channelBufferPtr = buffer.getWritePointer(channel);
        for (int i = 0; i < sample_len; ++i) {
            channelBufferPtr[i] = mono_signal[i];
        }
    }

    delete[] mono_signal;
}

//==============================================================================
bool ImageSonificationProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ImageSonificationProcessor::createEditor()
{
    return new ImageSonificationProcessorEditor(*this);
}

//==============================================================================
void ImageSonificationProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void ImageSonificationProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}


void ImageSonificationProcessor::resetBitmap()
{
    AlgorithmBase::imageBitmapPtr = new juce::Image::BitmapData(this->image, juce::Image::BitmapData::readOnly);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ImageSonificationProcessor();
}
