//
//  WindowingAlgorithm.cpp
//  ImageSonificationPlugin - VST3
//
//  Created by Karolina Płaneta on 15/12/2023.
//

#include <stdio.h>

#include "WindowingAlgorithm.h"

WindowingAlgorithm::WindowingAlgorithm(unsigned int& WidthIt, unsigned int& HeightIt, std::shared_ptr<juce::Image::BitmapData> imageBitmapPtr): WidthIt(WidthIt), HeightIt(HeightIt), imageBitmapPtr(imageBitmapPtr)
{
}



float calculateWindow(int windowSize, int currentHeight, int currentWidth, std::shared_ptr<juce::Image::BitmapData>imageBitmapPtr){
    float sum = 0;

    for (int x = 0; x < windowSize; ++x) {
        for (int y = 0; y < windowSize; ++y) {
            
            auto pix_c =  imageBitmapPtr->getPixelColour(currentWidth + y, currentHeight + x);
            float average = (pix_c.getFloatRed() + pix_c.getFloatGreen() + pix_c.getFloatBlue()) / 3.f;
            sum += average;
            
        }
    }
    return sum;
}


void WindowingAlgorithm::generate_next_samples(float* output_buffer, unsigned int buffer_length, int windowSize)
{
    
    
    int k = 0;
    int height = imageBitmapPtr->height;
    int width = imageBitmapPtr->width;
    
    
     
    
    while (true) {
        for (; HeightIt < height - windowSize + 1; ++HeightIt) {
            
            for (; WidthIt < width - windowSize + 1; ++WidthIt) {
                float sum = calculateWindow(windowSize, HeightIt, WidthIt,imageBitmapPtr);
                
                if( k < buffer_length){
                    *(output_buffer + k) = sum / (windowSize * windowSize);
                    
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

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    

