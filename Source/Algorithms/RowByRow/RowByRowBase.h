/*
  ==============================================================================

    RowByRowBase.h
    Created: 19 Aug 2024 2:21:37pm
    Author:  Jakub Mazur

  ==============================================================================
*/

#pragma once
#include "..\AlgorithmBase.h"

class RowByRowBase : public AlgorithmBase {
public:
    void generateNextSamples(float* output_buffer, unsigned int buffer_length) override;

protected:
    static int RowOrColIt;
private :
    virtual float getSampleFromRowOrCol() = 0;
};

