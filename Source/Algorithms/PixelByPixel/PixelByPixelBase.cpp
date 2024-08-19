/*
  ==============================================================================

    PixelByPixelBase.cpp
    Created: 15 Jan 2024 3:37:37pm
    Author:  j.mazur, k.P³aneta

  ==============================================================================
*/

#include "PixelByPixelBase.h"
//#include <juce_MathsFunctions.h>

#define ACC2RGB(acc_val, div) ((juce::uint8)(acc_val / div))

PixelByPixelBase::PixelByPixelBase(CrawlingDirection& directionOfPlay, int& windowSize): AlgorithmBase(), directionOfPlay(directionOfPlay), WindowSize(windowSize), SampleRate(0u)
{
    // TODO - make windowSize type juce::uint8 and cut away slider at value 255!
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

    unsigned int half_win_size = WindowSize / 2;

    unsigned int lower_bound_width = std::max((WidthIt - half_win_size), 0u);
    unsigned int higher_bound_width = std::min((unsigned int)(imageBitmapPtr->width), (WidthIt + half_win_size));

    unsigned int lower_bound_height = std::max((HeightIt - half_win_size), 0u);
    unsigned int higher_bound_height = std::min((unsigned int)(imageBitmapPtr->height), (HeightIt + half_win_size));

    // max window size is 255 (uint8), so accumulators can contain values up to 255*255*255 - uint32 is needed!
    juce::uint32 R = 0u;
    juce::uint32 G = 0u;
    juce::uint32 B = 0u;

    juce::Colour temp_colour;

    // TODO - cache correct part of the sample for the calculation of the next sample!
    for (unsigned int i = lower_bound_width; i < higher_bound_width; ++i)
    {
        for (unsigned int j = lower_bound_height; j < higher_bound_height; ++j)
        {
            temp_colour = imageBitmapPtr->getPixelColour(i, j);
            R += temp_colour.getRed();
            G += temp_colour.getGreen();
            B += temp_colour.getBlue();
        }
    }

    unsigned int win_size = (higher_bound_width - lower_bound_width) * (higher_bound_height - lower_bound_height);

    return juce::Colour(ACC2RGB(R, win_size), ACC2RGB(G, win_size), ACC2RGB(B, win_size));
}

int PixelByPixelBase::WidthIt = 0;
int PixelByPixelBase::HeightIt = 0;
