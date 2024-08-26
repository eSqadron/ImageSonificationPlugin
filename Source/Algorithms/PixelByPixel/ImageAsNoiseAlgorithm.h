/*
  ==============================================================================

    ImageAsNoiseAlgorithm.h
    Created: 11 Dec 2023 5:59:17pm
    Author:  OEM

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PixelByPixelBase.h"
#pragma once


class ImageAsNoiseAlgorithm: public PixelByPixelBase {
public:
    ImageAsNoiseAlgorithm(PixelByPixelDirection& directionOfPlay, int& windowSize);

    float getSampleFromPixel(juce::Colour pixel) override;
};
