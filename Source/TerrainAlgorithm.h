//
//  TerrainAlgorithm.hpp
//  ImageSonificationPlugin - VST3
//
//  Created by Karolina Płaneta on 03/01/2024.
//



#include <JuceHeader.h>
#pragma once


class TerrainAlgorithm {
public:
    TerrainAlgorithm(unsigned int& WidthIt, unsigned int& HeightIt, std::shared_ptr<juce::Image::BitmapData> imageBitmapPtr);

    void generate_next_samples(float* output_buffer, unsigned int buffer_length);

    std::shared_ptr<juce::Image::BitmapData> imageBitmapPtr;
private:
    unsigned int& WidthIt;
    unsigned int& HeightIt;
};
