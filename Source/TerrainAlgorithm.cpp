//
//  TerrainAlgorithm.cpp
//  ImageSonificationPlugin - VST3
//
//  Created by Karolina Płaneta on 03/01/2024.
//

#include "TerrainAlgorithm.h"



TerrainAlgorithm::TerrainAlgorithm(unsigned int& WidthIt, unsigned int& HeightIt, std::shared_ptr<juce::Image::BitmapData> imageBitmapPtr): WidthIt(WidthIt), HeightIt(HeightIt), imageBitmapPtr(imageBitmapPtr)
{
}

void TerrainAlgorithm::generate_next_samples(float* output_buffer, unsigned int buffer_length)
{
        int k = 0;
        int height = imageBitmapPtr->height;
        int width = imageBitmapPtr->width;
        
        
        if(HeightIt == 0){
            HeightIt = 1;
        }
        
       while (true) {
            for (; WidthIt < width; ++WidthIt) {
                float maxroznica = 0;
                int indeks;
                
                
                for (; HeightIt < height; ++HeightIt) {
                    
                    auto pix_c = imageBitmapPtr->getPixelColour(WidthIt, HeightIt); 
                    float jasnoscpixela = (pix_c.getFloatRed() + pix_c.getFloatGreen() + pix_c.getFloatBlue()) / 3.f;
                    
                    auto poprzednipixel = imageBitmapPtr->getPixelColour(WidthIt, HeightIt-1);
                    float jasnoscpoprzedniegopixela = (poprzednipixel.getFloatRed() + poprzednipixel.getFloatGreen() + poprzednipixel.getFloatBlue()) / 3.f;
                    
                    
                    float roznica = std::abs(jasnoscpixela - jasnoscpoprzedniegopixela);
                    
                    if(roznica > maxroznica){
                        maxroznica = roznica;
                        indeks = HeightIt-1;
                    }
                    
                }
                HeightIt = 1;
                
                imageBitmapPtr->setPixelColour(WidthIt, indeks, juce::Colour(255, 0, 0));
        
                
                if( k < buffer_length){
                    *(output_buffer + k) = float(indeks)/float(height);
                    
                    k++;
                }
                else{
                    return;
                }
                
            }
            WidthIt = 0;
       }
    }