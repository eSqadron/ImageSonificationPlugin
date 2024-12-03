//
//  WindowingAlgorithm.h
//  ImageSonificationPlugin
//
//  Created by Karolina Płaneta on 15/12/2023.
//

// TODO - To be rewritten into PixelByPixelBase

#include <JuceHeader.h>
#include "Algorithms/AlgorithmBase.h"
#pragma once


class WindowingAlgorithm: public AlgorithmBase {
public:
    WindowingAlgorithm(int& windowSize);

    void generateNextSamples(float* output_buffer, unsigned int buffer_length) override;
private:
    int& WindowSize;

    float calculateWindow(int windowSize, int currentHeight, int currentWidth, float previousSum);

    static int WidthIt;
    static int HeightIt;
};
