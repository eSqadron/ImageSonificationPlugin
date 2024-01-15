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
            std::make_unique<juce::AudioParameterInt>("crawling_direction",            // parameterID
                                                         "CrawlingDirection",            // parameter name
                                                         0,              // minimum value
                                                         100,              // maximum value
                                                         0)            // default value
        }),
    imageAsNoiseAlg(widthIt, heightIt, imageBitmapPtr, directionOfPixelByPixelPlay),
    eecs351wn22Alg(widthIt, heightIt, imageBitmapPtr),
    windowingAlg(widthIt, heightIt, imageBitmapPtr),
    landscapeAlg(widthIt, heightIt, imageBitmapPtr)

#endif
{
    imageBitmapPtr.reset();
    m_flogger = std::unique_ptr<juce::FileLogger>(juce::FileLogger::createDateStampedLogger("Juce", "visualiser", ".txt", "Welcome to plugin"));

    algorithmParam = parameters.getRawParameterValue("algorithm");
    crawlingDirectionParam = parameters.getRawParameterValue("crawling_direction");
}

ImageSonificationProcessor::~ImageSonificationProcessor()
{
    imageBitmapPtr.reset();
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

void ImageSonificationProcessor::setCurrentProgram(int index)
{
}

const juce::String ImageSonificationProcessor::getProgramName(int index)
{
    return {};
}

void ImageSonificationProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void ImageSonificationProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
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


void ImageSonificationProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    directionOfPixelByPixelPlay = static_cast<CrawlingDirection>(crawlingDirectionParam->load());

    juce::ScopedNoDenormals noDenormals;
    int totalNumInputChannels = getTotalNumInputChannels();
    int totalNumOutputChannels = getTotalNumOutputChannels();
    int sample_len = buffer.getNumSamples();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Do NOT play anythong when image is~not loaded or being loaded!
    if (imageBitmapPtr == nullptr || imageIsBeingLoaded) {
        return;
    }

    // mono sine - for mono signal generation
    float* mono_signal = new float[sample_len];

    // Depending on chosen algorithm, generate new buffer of samples
    if (*algorithmParam == static_cast<float>(NoiseCrawler)) {
        imageAsNoiseAlg.generateNextSamples(mono_signal, sample_len);
    }

    
    else if (*algorithmParam == static_cast<float>(Windowing)) {
        windowingAlg.generate_next_samples(mono_signal, sample_len, valueOfSlider);
    }
    
    else if (*algorithmParam == static_cast<float>(Landscape)) {
        landscapeAlg.generate_next_samples(mono_signal, sample_len);
    }
    
    
    else if (*algorithmParam == static_cast<float>(SineChordCrawler)) { //https://sites.google.com/umich.edu/eecs351-project-sonify/how-we-sonify?authuser=0
        this->eecs351wn22Alg.generate_next_samples(mono_signal, sample_len);
    }

    // rewrite mono signal into all (both) channels
    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        auto* channelData = buffer.getWritePointer(channel);
        for (int i = 0; i < sample_len; ++i) {
            channelData[i] = mono_signal[i];
        }
        
    }
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
    this->imageBitmapPtr.reset(new juce::Image::BitmapData(this->image, juce::Image::BitmapData::readOnly));

    this->imageAsNoiseAlg.imageBitmapPtr = this->imageBitmapPtr;
    this->eecs351wn22Alg.imageBitmapPtr = this->imageBitmapPtr;
    this->windowingAlg.imageBitmapPtr = this->imageBitmapPtr;
    this->landscapeAlg.imageBitmapPtr = this->imageBitmapPtr;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ImageSonificationProcessor();
}
