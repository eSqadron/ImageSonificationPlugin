//
//  TerrainAlgorithm.hpp
//  ImageSonificationPlugin - VST3
//
//  Created by Karolina Płaneta on 03/01/2024.
//



#include <JuceHeader.h>
#include "../AlgorithmBase.h"
#pragma once


class TerrainAlgorithm: public AlgorithmBase {
public:
    TerrainAlgorithm();

    void generateNextSamples(float* output_buffer, unsigned int buffer_length) override;
};
