/*
  ==============================================================================

    PixelByPixelBase.cpp
    Created: 15 Jan 2024 3:37:37pm
    Author:  j.mazur, k.P³aneta

  ==============================================================================
*/

#include "PixelByPixelBase.h"
//#include <juce_MathsFunctions.h>

#define FLOAT2RGB(f_val, div) ((unsigned char)((f_val / div)*255))

PixelByPixelBase::PixelByPixelBase(CrawlingDirection& directionOfPlay, int& windowSize): AlgorithmBase(), directionOfPlay(directionOfPlay), WindowSize(windowSize), SampleRate(0u)
{
}

void PixelByPixelBase::generateNextSamples(float* output_buffer, unsigned int buffer_length)
{
    for (float* i = output_buffer; i < output_buffer + buffer_length; ++i) {
        auto pixel = Window();
        *i = this->getSampleFromPixel(pixel);
        SamplesPerPixelIterator++;
        if (SamplesPerPixel == SamplesPerPixelIterator)
        {
            this->iteratePixelCommon();
            this->iteratePixelAlgorithmSpecific(pixel);
            SamplesPerPixelIterator = 0;
        }
    }
}

void PixelByPixelBase::prepareToPlay(double sampleRate, int)
{
    SampleRate = (unsigned int)sampleRate;
}

void PixelByPixelBase::iteratePixelCommon()
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
        short unsigned int randomInt = (short unsigned int) juce::Random::getSystemRandom().nextInt(4);
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

juce::Colour PixelByPixelBase::Window()
{
    if (WindowSize == 1)
    {
        return imageBitmapPtr->getPixelColour(WidthIt, HeightIt);
    }

    unsigned int lower_bound_width = (WidthIt - WindowSize / 2 < 0) ? 0 : (WidthIt - WindowSize / 2);
    unsigned int higher_bound_width = (WidthIt + WindowSize / 2 > imageBitmapPtr->width) ? imageBitmapPtr->width : (WidthIt + WindowSize / 2);

    unsigned int lower_bound_height = (HeightIt - WindowSize / 2 < 0) ? 0 : (HeightIt - WindowSize / 2);
    unsigned int higher_bound_height = (HeightIt + WindowSize / 2 > imageBitmapPtr->height) ? imageBitmapPtr->height : (HeightIt + WindowSize / 2);

    double R = 0.0;
    double G = 0.0;
    double B = 0.0;

    unsigned int counter = 0;

    // TODO - cache correct part of the sample for the calculation of the next sample!
    for (unsigned int i = lower_bound_width; i < higher_bound_width; ++i)
    {
        for (unsigned int j = lower_bound_height; j < higher_bound_height; ++j)
        {
            R += imageBitmapPtr->getPixelColour(i, j).getFloatRed();
            G += imageBitmapPtr->getPixelColour(i, j).getFloatGreen();
            B += imageBitmapPtr->getPixelColour(i, j).getFloatBlue();

            counter++;
        }
    }
    return juce::Colour(FLOAT2RGB(R, counter), FLOAT2RGB(G, counter), FLOAT2RGB(B, counter));
}
