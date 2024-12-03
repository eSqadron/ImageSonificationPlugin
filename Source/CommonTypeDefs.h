/*
  ==============================================================================

    CommonTypeDefs.h
    Created: 11 Dec 2023 5:49:54pm
    Author:  OEM

  ==============================================================================
*/

#pragma once

#define IMEPLEMENTED_ALGORITHMS_COUNT 3

enum SythesiserAlgorithm {
    NoiseCrawler,
    SineChordCrawler,
    //Windowing, OBSOLETE
    Landscape
};


#define MAX_WINDOW_SIZE_FOR_PIXEL_BY_PIXEL 40 // Windowing optimization doesn't allow for more. uint8 allows for values up to 255

enum PixelByPixelDirection {
    LeftToRight,
    UpToDown,
    Random
    // TODO - add IncrementalRandom
};
