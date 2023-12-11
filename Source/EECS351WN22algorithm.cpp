/*
  ==============================================================================

    EECS351WN22algorithm.cpp
    Created: 11 Dec 2023 7:31:49pm
    Author:  OEM

  ==============================================================================
*/

#include "EECS351WN22algorithm.h"


EECS351WN22algorithm::EECS351WN22algorithm(unsigned int& WidthIt, unsigned int& HeightIt, std::shared_ptr<juce::Image::BitmapData> imageBitmapPtr) : WidthIt(WidthIt), HeightIt(HeightIt), imageBitmapPtr(imageBitmapPtr)
{
}

void EECS351WN22algorithm::generate_next_samples(float* output_buffer, unsigned int buffer_length)
{
    for (unsigned int i = 0; i < buffer_length; i++)
    {
        float currentSample = 0;
        for (short unsigned int j = 0; j < 3; j++) {
            currentSample += (float)std::sin(currentAngle[j]);
            currentAngle[j] += angleDelta[j];
        }

        float t = (float)std::sin(currentAngle[0]);
        *(output_buffer + i) = currentSample / 3;

        EECS_it++;


        if (EECS_it >= 15000) {  //https://sites.google.com/umich.edu/eecs351-project-sonify/how-we-sonify?authuser=0
            EECS_it = 0;
            auto pix_c = imageBitmapPtr->getPixelColour(WidthIt, HeightIt);

            short unsigned int randomInt = juce::Random::getSystemRandom().nextInt(4);
            if (WidthIt < imageBitmapPtr->width - 1 && randomInt == 0) {
                WidthIt += 1;
            }
            if (HeightIt < imageBitmapPtr->height - 1 && randomInt == 1) {
                HeightIt += 1;
            }
            if (WidthIt > 1 && randomInt == 2) {
                WidthIt -= 1;
            }
            if (HeightIt > 1 && randomInt == 3) {
                HeightIt -= 1;


                short unsigned int r = pix_c.getRed();
                short unsigned int g = pix_c.getGreen();
                short unsigned int b = pix_c.getBlue();
                if (r < EECS_limit && g < EECS_limit && b < EECS_limit) {
                    chords[0] = 64;
                    chords[1] = 67;
                    chords[1] = 71;
                }
                else if (r > 255 - EECS_limit && g > 255 - EECS_limit && b > 255 - EECS_limit) {
                    chords[0] = 62;
                    chords[1] = 65;
                    chords[1] = 69;
                }
                else if (g >= b && g >= r) {
                    chords[0] = 69;
                    chords[1] = 72;
                    chords[1] = 66;
                }
                else if (b >= g && b >= r) {
                    chords[0] = 65;
                    chords[1] = 68;
                    chords[1] = 72;
                }
                else if (r >= g && r >= b) {
                    chords[0] = 60;
                    chords[1] = 64;
                    chords[1] = 67;
                }
                else {
                    chords[0] = 60;
                    chords[1] = 64;
                    chords[1] = 67;
                }

                for (unsigned short int j = 0; j < 3; j++) {
                    double chords_hertz = 440.0 * pow(2.0, ((chords[j] - 69) * 1.0) / 12.0);
                    double chords_cyclesPerSample = chords_hertz / currentSampleRate;
                    angleDelta[j] = chords_cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
                }

            }

        }
    }
}

void EECS351WN22algorithm::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    this->currentSampleRate = sampleRate;
}
