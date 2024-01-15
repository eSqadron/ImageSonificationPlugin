/*
  ==============================================================================

    ImageAsNoiseAlgorithm.cpp
    Created: 11 Dec 2023 5:59:17pm
    Author:  OEM

  ==============================================================================
*/

#include "ImageAsNoiseAlgorithm.h"

ImageAsNoiseAlgorithm::ImageAsNoiseAlgorithm(unsigned int& WidthIt, unsigned int& HeightIt, std::shared_ptr<juce::Image::BitmapData> imageBitmapPtr, CrawlingDirection& directionOfPlay): PixelByPixelBase(WidthIt, HeightIt, imageBitmapPtr, directionOfPlay)
{
}

double ImageAsNoiseAlgorithm::getSampleFromPixel()
{
    auto pix_c = imageBitmapPtr->getPixelColour(WidthIt, HeightIt);
    return (pix_c.getFloatRed() + pix_c.getFloatGreen() + pix_c.getFloatBlue()) / 3.f;
}
