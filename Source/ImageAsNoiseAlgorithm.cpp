/*
  ==============================================================================

    ImageAsNoiseAlgorithm.cpp
    Created: 11 Dec 2023 5:59:17pm
    Author:  OEM

  ==============================================================================
*/

#include "ImageAsNoiseAlgorithm.h"

ImageAsNoiseAlgorithm::ImageAsNoiseAlgorithm(unsigned int& WidthIt, unsigned int& HeightIt, std::shared_ptr<juce::Image::BitmapData> imageBitmapPtr): WidthIt(WidthIt), HeightIt(HeightIt), imageBitmapPtr(imageBitmapPtr)
{
}

void ImageAsNoiseAlgorithm::generate_next_samples(float* output_buffer, unsigned int buffer_length)
{
    for (unsigned int i = 0; i < buffer_length; i++)
    {
        auto pix_c = imageBitmapPtr->getPixelColour(WidthIt, HeightIt);
        *(output_buffer + i) = (pix_c.getFloatRed() + pix_c.getFloatGreen() + pix_c.getFloatBlue()) / 3.f;

        // Go from left to right in rows
        WidthIt += 1;
        if (WidthIt >= imageBitmapPtr->width) {
            WidthIt = 0;
            HeightIt += 1;

            if (HeightIt >= imageBitmapPtr->height) {
                HeightIt = 0;
            }
        }
    }
}
