/*
  ==============================================================================

    RowByRowBase.cpp
    Created: 19 Aug 2024 2:21:37pm
    Author:  Jakub Mazur

  ==============================================================================
*/

#include "RowByRowBase.h"

int RowByRowBase::RowOrColIt = 0;

void RowByRowBase::generateNextSamples(float* output_buffer, unsigned int buffer_length)
{
    int width = imageBitmapPtr->width;

    for (float* i = output_buffer; i < output_buffer + buffer_length; ++i) {
        *i = getSampleFromRowOrCol();
        ++RowOrColIt;
        RowOrColIt %= width;
    }
}
