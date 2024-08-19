//
//  TerrainAlgorithm.hpp
//  ImageSonificationPlugin - VST3
//
//  Created by Karolina Płaneta on 03/01/2024.
//



#include <JuceHeader.h>
#include "../AlgorithmBase.h"
#include "RowByRowBase.h"
#pragma once


class TerrainAlgorithm: public RowByRowBase {
public:
    TerrainAlgorithm();

    float getSampleFromRowOrCol() override;

};
