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
    ImageAsNoiseAlgorithm(unsigned int& WidthIt, unsigned int& HeightIt, std::shared_ptr<juce::Image::BitmapData> imageBitmapPtr, CrawlingDirection& directionOfPlay);

    double getSampleFromPixel() override;
};
