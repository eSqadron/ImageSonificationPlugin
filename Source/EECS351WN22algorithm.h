/*
  ==============================================================================

    EECS351WN22algorithm.h
    Created: 11 Dec 2023 7:31:49pm
    Author:  OEM

  ==============================================================================
*/
#include <JuceHeader.h>

#pragma once

class EECS351WN22algorithm {
public:
    EECS351WN22algorithm(unsigned int& WidthIt, unsigned int& HeightIt, std::shared_ptr<juce::Image::BitmapData> imageBitmapPtr);

    void generate_next_samples(float* output_buffer, unsigned int buffer_length);

    void prepareToPlay(double sampleRate, int samplesPerBlock);

    std::shared_ptr<juce::Image::BitmapData> imageBitmapPtr;
private:
    unsigned int& WidthIt;
    unsigned int& HeightIt;

    short unsigned int chords[3] = { 36, 40, 43 };
    double angleDelta[3] = { 0, 0, 0 };
    double currentAngle[3] = { 0, 0, 0 };

    short unsigned int EECS_it = 0;
    short unsigned int EECS_limit = 35;

    double currentSampleRate = 0.0;
};