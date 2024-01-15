/*
  ==============================================================================

    EECS351WN22algorithm.h
    Created: 11 Dec 2023 7:31:49pm
    Author:  OEM

  ==============================================================================
*/
#include <JuceHeader.h>
#include "PixelByPixelBase.h"

#pragma once

class EECS351WN22algorithm : public PixelByPixelBase {
public:
    EECS351WN22algorithm(CrawlingDirection& directionOfPlay, int& windowSize);

    float getSampleFromPixel(juce::Colour pixel) override;
    void iteratePixelAlgorithmSpecific(juce::Colour pixel) override;

private:
    short unsigned int chords[3] = { 36, 40, 43 };
    double angleDelta[3] = { 0, 0, 0 };
    double currentAngle[3] = { 0, 0, 0 };

    short unsigned int EECS_limit = 35;
};