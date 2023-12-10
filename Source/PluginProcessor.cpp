/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/
#include "PluginProcessor.h"
#include "PluginEditor.h"

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
        })
#endif
{
    imageBitmapPtr.reset(nullptr);
    m_flogger = std::unique_ptr<juce::FileLogger>(juce::FileLogger::createDateStampedLogger("Juce", "visualiser", ".txt", "Welcome to plugin"));

    algorithmParam = parameters.getRawParameterValue("algorithm");
    crawlingDirectionParam = parameters.getRawParameterValue("crawling_direction");
}

ImageSonificationProcessor::~ImageSonificationProcessor()
{
    imageBitmapPtr.reset(nullptr);
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
    currentSampleRate = sampleRate;
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

    if (imageBitmapPtr == nullptr) {
        return;
    }

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    float* mono_sine = new float[sample_len];

    for (float* i = mono_sine; i < mono_sine + sample_len; ++i) {
        //if (m_flogger) {
        //    std::stringstream s;
        //    s << pixel.R << " " << pixel.G << " " << pixel.B;
        //    m_flogger->logMessage(s.str());
        //}

        auto pix_c = imageBitmapPtr->getPixelColour(widthIt, heightIt);

        if (*algorithmParam == static_cast<float>(NoiseCrawler)) {
            *i = (pix_c.getFloatRed() + pix_c.getFloatGreen() + pix_c.getFloatBlue()) / 3.f;
        }



        if (*algorithmParam == static_cast<float>(SineChordCrawler)) { //https://sites.google.com/umich.edu/eecs351-project-sonify/how-we-sonify?authuser=0
            float currentSample = 0;
            for (short unsigned int j = 0; j < 3; j++) {
                currentSample += (float)std::sin(currentAngle[j]);
                currentAngle[j] += angleDelta[j];
            }

            float t = (float)std::sin(currentAngle[0]);
            *i = currentSample/3;

        }

        // ITERATORS
        EECS_it++;

        if (*algorithmParam == static_cast<float>(SineChordCrawler) && EECS_it >= 15000) {
            EECS_it = 0;
            short unsigned int randomInt = juce::Random::getSystemRandom().nextInt(4);
            if (widthIt < imageWidth-1 && randomInt == 0) {
                widthIt += 1;
            }
            if (heightIt < imageHeight-1 && randomInt == 1) {
                heightIt += 1;
            }
            if (widthIt > 1 && randomInt == 2) {
                widthIt -= 1;
            }
            if (heightIt > 1 && randomInt == 3) {
                heightIt -= 1;
            }

            if (*algorithmParam == static_cast<float>(SineChordCrawler)) { //https://sites.google.com/umich.edu/eecs351-project-sonify/how-we-sonify?authuser=0
                short unsigned int r = pix_c.getRed();
                short unsigned int g = pix_c.getGreen();
                short unsigned int b = pix_c.getBlue();
                if (r < EECS_limit && g < EECS_limit && b < EECS_limit) {
                    chords[0] = 64;
                    chords[1] = 67;
                    chords[1] = 71;
                }
                else if (r > 255 - EECS_limit && g > 255 - EECS_limit && b > 255 - EECS_limit) {
                    chords[0] = 62;
                    chords[1] = 65;
                    chords[1] = 69;
                }
                else if (g >= b && g >= r) {
                    chords[0] = 69;
                    chords[1] = 72;
                    chords[1] = 66;
                }
                else if (b >= g && b >= r) {
                    chords[0] = 65;
                    chords[1] = 68;
                    chords[1] = 72;
                }
                else if (r >= g && r >= b) {
                    chords[0] = 60;
                    chords[1] = 64;
                    chords[1] = 67;
                }
                else {
                    chords[0] = 60;
                    chords[1] = 64;
                    chords[1] = 67;
                }

                for (unsigned short int j = 0; j < 3; j++) {
                    double chords_hertz = 440.0 * pow(2.0, ((chords[j] - 69) * 1.0) / 12.0);
                    double chords_cyclesPerSample = chords_hertz / currentSampleRate;
                    angleDelta[j] = chords_cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
                }

            }
        }


        // Go from left to right in rows
        if (*algorithmParam == static_cast<float>(NoiseCrawler)) {
            widthIt += 1;
            if (widthIt >= imageWidth) {
                widthIt = 0;
                heightIt += 1;

                if (heightIt >= imageHeight) {
                    heightIt = 0;
                }
            }
        }
    }


    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        auto* channelData = buffer.getWritePointer(channel);
        for (int i = 0; i < sample_len; ++i) {
            channelData[i] = mono_sine[i];
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

void ImageSonificationProcessor::resetImageData(unsigned int image_width, unsigned int image_height)
{
    imageWidth = image_width;
    imageHeight = image_height;
    heightIt = 0;
    widthIt = 0;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ImageSonificationProcessor();
}
