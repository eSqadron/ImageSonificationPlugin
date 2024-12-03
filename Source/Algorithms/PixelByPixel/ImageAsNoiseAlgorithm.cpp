/*
  ==============================================================================

    ImageAsNoiseAlgorithm.cpp
    Created: 11 Dec 2023 5:59:17pm
    Author:  OEM

  ==============================================================================
*/

#include "ImageAsNoiseAlgorithm.h"

ImageAsNoiseAlgorithm::ImageAsNoiseAlgorithm(PixelByPixelDirection& directionOfPlay, int& windowSize): PixelByPixelBase(directionOfPlay, windowSize)
{
}

float ImageAsNoiseAlgorithm::getSampleFromPixel(juce::Colour pixel)
{
    return (pixel.getFloatRed() + pixel.getFloatGreen() + pixel.getFloatBlue()) / 3.f;
}
