//
//  WindowingAlgorithm.cpp
//  ImageSonificationPlugin - VST3
//
//  Created by Karolina Płaneta on 15/12/2023.
//

// TODO - To be rewritten into PixelByPixelBase

#include <stdio.h>

#include "WindowingAlgorithm.h"

WindowingAlgorithm::WindowingAlgorithm(int& windowSize): AlgorithmBase(), WindowSize(windowSize)
{
}



float WindowingAlgorithm::calculateWindow(int windowSize, int currentHeight, int currentWidth, float previousSum)
{
    float sum = 0;
    float columnSum = 0;
    float nextcolumnSum = 0;
    juce::Colour pix_c;
    float average;

    if (currentWidth == 0 || windowSize == 1) {
        for (int x = 0; x < windowSize; ++x) {
            for (int y = 0; y < windowSize; ++y) {
                
                pix_c =  imageBitmapPtr->getPixelColour(currentWidth + y, currentHeight + x);
                average = (pix_c.getFloatRed() + pix_c.getFloatGreen() + pix_c.getFloatBlue()) / 3.f;
                sum += average;
                
            }
        }
        return sum;
    }
    else {
        
        for(int y = 0; y < windowSize; ++y){
            
            pix_c =  imageBitmapPtr->getPixelColour(currentWidth -1, currentHeight + y);
            average = (pix_c.getFloatRed() + pix_c.getFloatGreen() + pix_c.getFloatBlue()) / 3.f;
            columnSum += average;
        }
        
        for(int y = 0; y < windowSize; ++y){
            
            pix_c =  imageBitmapPtr->getPixelColour(currentWidth + windowSize, currentHeight + y);
            average = (pix_c.getFloatRed() + pix_c.getFloatGreen() + pix_c.getFloatBlue()) / 3.f;
            nextcolumnSum += average;
        }
        return previousSum - columnSum + nextcolumnSum;
        
    }
}


void WindowingAlgorithm::generateNextSamples(float* output_buffer, unsigned int buffer_length)
{
    
    
    unsigned int k = 0;
    int height = imageBitmapPtr->height;
    int width = imageBitmapPtr->width;
    float sum = 0;
    
     
    
    while (true) {
        for (; HeightIt < height - WindowSize + 1; ++HeightIt) {
            
            for (; WidthIt < width - WindowSize + 1; ++WidthIt) {
                sum = calculateWindow(WindowSize, HeightIt, WidthIt, sum);
                
                if( k < buffer_length){
                    *(output_buffer + k) = sum / (WindowSize * WindowSize);
                    
                    k++;
                }
                else{
                    return;
                }
            }
            WidthIt = 0;
        }
        HeightIt = 0;
    }
}

int WindowingAlgorithm::WidthIt = 0;
int WindowingAlgorithm::HeightIt = 0;
