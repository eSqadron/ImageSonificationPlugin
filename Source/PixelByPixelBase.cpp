/*
  ==============================================================================

    PixelByPixelBase.cpp
    Created: 15 Jan 2024 3:37:37pm
    Author:  j.mazur

  ==============================================================================
*/

#include "PixelByPixelBase.h"

PixelByPixelBase::PixelByPixelBase(unsigned int& WidthIt, unsigned int& HeightIt, std::shared_ptr<juce::Image::BitmapData> imageBitmapPtr, CrawlingDirection& directionOfPlay): WidthIt(WidthIt), HeightIt(HeightIt), imageBitmapPtr(imageBitmapPtr), directionOfPlay(directionOfPlay)
{
}

void PixelByPixelBase::generateNextSamples(float* output_buffer, unsigned int buffer_length)
{
    for (float* i = output_buffer; i < output_buffer + buffer_length; ++i) {
        *i = this->getSampleFromPixel();
        this->iteratePixel();
    }
}

void PixelByPixelBase::iteratePixel()
{
    if (directionOfPlay == LeftToRight) {
        WidthIt += 1;
        if (WidthIt >= imageBitmapPtr->width) {
            WidthIt = 0;
            HeightIt += 1;

            if (HeightIt >= imageBitmapPtr->height) {
                HeightIt = 0;
            }
        }
    }
    else if (directionOfPlay == Random) {
        short unsigned int randomInt = juce::Random::getSystemRandom().nextInt(4);
        if (WidthIt < imageBitmapPtr->width - 1 && randomInt == 0) {
            WidthIt += 1;
        }
        if (HeightIt < imageBitmapPtr->height - 1 && randomInt == 1) {
            HeightIt += 1;
        }
        if (WidthIt > 1 && randomInt == 2) {
            WidthIt -= 1;
        }
        if (HeightIt > 1 && randomInt == 3) {
            HeightIt -= 1;
        }
    }
    else if (directionOfPlay == UpToDown) {
        HeightIt += 1;
        if (HeightIt >= imageBitmapPtr->height) {
            HeightIt = 0;
            WidthIt += 1;

            if (WidthIt >= imageBitmapPtr->width) {
                WidthIt = 0;
            }
        }
    }
}
