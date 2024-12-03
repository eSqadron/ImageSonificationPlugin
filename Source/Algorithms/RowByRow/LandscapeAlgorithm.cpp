//
//  TerrainAlgorithm.cpp
//  ImageSonificationPlugin - VST3
//
//  Created by Karolina Płaneta on 03/01/2024.
//

#include "landscapeAlgorithm.h"

TerrainAlgorithm::TerrainAlgorithm(): RowByRowBase()
{
}


float TerrainAlgorithm::getSampleFromRowOrCol()
{
    int height = imageBitmapPtr->height;

    float maxDifference = 0;
    int index = 0;

    juce::Colour pixelRGB = imageBitmapPtr->getPixelColour(RowOrColIt, 0);
    float previousPixelBrightness = (pixelRGB.getFloatRed() + pixelRGB.getFloatGreen() + pixelRGB.getFloatBlue()) / 3.f;

    float pixelBrightness = 0.f;
    float difference_diff = 0.f;

    for (int height_it = 1; height_it < height; ++height_it) {

        pixelRGB = imageBitmapPtr->getPixelColour(RowOrColIt, height_it);
        pixelBrightness = (pixelRGB.getFloatRed() + pixelRGB.getFloatGreen() + pixelRGB.getFloatBlue()) / 3.f;

        difference_diff = std::abs(pixelBrightness - previousPixelBrightness);
        previousPixelBrightness = pixelBrightness;

        if (difference_diff > maxDifference) {
            maxDifference = difference_diff;
            index = height_it - 1;
        }
    }
    //imageBitmapPtr->setPixelColour(RowOrColIt, index, juce::Colour(255, 0, 0)); DEBUG ONLY
    return (float)index / (float)height;
}
