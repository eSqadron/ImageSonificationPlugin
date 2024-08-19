/*
  ==============================================================================

    EECS351WN22algorithm.cpp
    Created: 11 Dec 2023 7:31:49pm
    Author:  OEM

  ==============================================================================
*/

#include "EECS351WN22algorithm.h"


EECS351WN22algorithm::EECS351WN22algorithm(CrawlingDirection& directionOfPlay, int& windowSize) : PixelByPixelBase(directionOfPlay, windowSize)
{
    SamplesPerPixel = 1500;
}

float EECS351WN22algorithm::getSampleFromPixel(juce::Colour)
{
    float currentSample = 0;
    for (short unsigned int j = 0; j < 3; j++) {
        currentSample += (float)std::sin(currentAngle[j]);
        currentAngle[j] += angleDelta[j];
    }

    return currentSample / 3;
}

void EECS351WN22algorithm::iteratePixelAlgorithmSpecific(juce::Colour pixel)
{
    short unsigned int r = pixel.getRed();
    short unsigned int g = pixel.getGreen();
    short unsigned int b = pixel.getBlue();
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
        double chords_cyclesPerSample = chords_hertz / SampleRate;
        angleDelta[j] = chords_cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
    }
}
