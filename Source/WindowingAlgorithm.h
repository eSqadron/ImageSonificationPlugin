//
//  WindowingAlgorithm.h
//  ImageSonificationPlugin
//
//  Created by Karolina Płaneta on 15/12/2023.
//
#include <JuceHeader.h>
#pragma once


class WindowingAlgorithm {
public:
    WindowingAlgorithm(unsigned int& WidthIt, unsigned int& HeightIt, std::shared_ptr<juce::Image::BitmapData> imageBitmapPtr);

    void generate_next_samples(float* output_buffer, unsigned int buffer_length, int windowSize);

    std::shared_ptr<juce::Image::BitmapData> imageBitmapPtr;
private:
    unsigned int& WidthIt;
    unsigned int& HeightIt;
};
