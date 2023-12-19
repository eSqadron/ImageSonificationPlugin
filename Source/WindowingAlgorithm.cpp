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

void WindowingAlgorithm::generate_next_samples(float* output_buffer, unsigned int buffer_length, int windowSize)
{
    int k = 0;
    int height = imageBitmapPtr->height;
    int width = imageBitmapPtr->width;
    
    if (windowSize > height || windowSize > width) {
        int newWindowSize = (height > width) ? width : height;
        
        
        while (true) {
            if (height > width) {
                
                for (; HeightIt < height - newWindowSize + 1; ++HeightIt) {
                    float sum = 0;
                    
                    for (int x = 0; x < newWindowSize; ++x) {
                        for (int y = 0; y < newWindowSize; ++y) {
                            
                            auto pix_c =  imageBitmapPtr->getPixelColour(WidthIt + y, HeightIt + x);
                            float average = (pix_c.getFloatRed() + pix_c.getFloatGreen() + pix_c.getFloatBlue()) / 3.f;
                            sum += average;
                            
                        }
                    }
                    if (k < buffer_length) {
                        *(output_buffer + k) = sum / (newWindowSize * newWindowSize);
                        
                        k++;
                    }
                    else{
                        return;
                    }
                }
                HeightIt = 0;
                
            } else {
                
                for (; WidthIt < width - newWindowSize + 1; ++WidthIt) {
                    float sum = 0;
                    
                    for (int x = 0; x < newWindowSize; ++x) {
                        for (int y = 0; y < newWindowSize; ++y) {
                            
                            auto pix_c =  imageBitmapPtr->getPixelColour(WidthIt + y, HeightIt + x);
                            float average = (pix_c.getFloatRed() + pix_c.getFloatGreen() + pix_c.getFloatBlue()) / 3.f;
                            sum += average;
                            
                        }
                    }
                    if( k < buffer_length){
                        *(output_buffer + k) = sum / (newWindowSize * newWindowSize);
                        
                        k++;
                    }
                    else{
                        return;
                    }
                }
                WidthIt = 0;
            }
        }
    }
        
    
    while (true) {
        for (; HeightIt < height - windowSize + 1; ++HeightIt) {
            
            for (; WidthIt < width - windowSize + 1; ++WidthIt) {
                float sum = 0;
                
               
                
                
                for (int x = 0; x < windowSize; ++x) {
                    for (int y = 0; y < windowSize; ++y) {
                        
                        auto pix_c =  imageBitmapPtr->getPixelColour(WidthIt + y, HeightIt + x);
                        float average = (pix_c.getFloatRed() + pix_c.getFloatGreen() + pix_c.getFloatBlue()) / 3.f;
                        sum += average;
                        
                    }
                }
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

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    

