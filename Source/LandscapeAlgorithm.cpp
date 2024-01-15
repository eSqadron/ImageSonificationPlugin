//
//  TerrainAlgorithm.cpp
//  ImageSonificationPlugin - VST3
//
//  Created by Karolina Płaneta on 03/01/2024.
//

#include "landscapeAlgorithm.h"



TerrainAlgorithm::TerrainAlgorithm(): AlgorithmBase()
{
}

void TerrainAlgorithm::generateNextSamples(float* output_buffer, unsigned int buffer_length)
{      
        unsigned int k = 0;
        int height = imageBitmapPtr->height;
        int width = imageBitmapPtr->width;
        
        
        if(HeightIt == 0){
            HeightIt = 1;
        }
        
        while (true) {
            for (; WidthIt < width; ++WidthIt) {
                float maxDifference = 0;
                int index = 0;
                
                
                for (; HeightIt < height; ++HeightIt) {
                    
                    auto pix_c = imageBitmapPtr->getPixelColour(WidthIt, HeightIt);
                    float pixelBrightness = (pix_c.getFloatRed() + pix_c.getFloatGreen() + pix_c.getFloatBlue()) / 3.f;
                    
                    auto previousPixel = imageBitmapPtr->getPixelColour(WidthIt, HeightIt-1);
                    float previousPixelBrightness = (previousPixel.getFloatRed() + previousPixel.getFloatGreen() + previousPixel.getFloatBlue()) / 3.f;
                    
                    
                    float difference = std::abs(pixelBrightness - previousPixelBrightness);
                    
                    if(difference > maxDifference){
                        maxDifference = difference;
                        index = HeightIt-1;
                    }
                    
                }
                HeightIt = 1;
                
                  imageBitmapPtr->setPixelColour(WidthIt, index, juce::Colour(255, 0, 0));
        
                
                if( k < buffer_length){
                    *(output_buffer + k) = float(index)/float(height);
                    
                    k++;
                }
                else{
                    return;
                }
                
            }
            WidthIt = 0;
        }
    }