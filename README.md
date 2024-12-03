# ImageSonificationPlugin
VST3 plugin that turns your image into music. (Or noise, depending on the algorithm!)

## Implemented algorithms:
* <b>Pixel By Pixel</b> - group of algorithms that analyze image pixel-by-pixel, turning single pixel into one or more samples of sound.
  * Common options:
    * Direction of play -  Direction, in which image is analyzed (left to right, top to bottom or random)
    * Window size - Size of group of pixels (window) that are averaged to one pixel. (Making image de facto more blurry.)
    * Pixel play length [Not yet Implemented] - Length, how many samples of sound are generated from single pixel. 
  * <b>Simple Image as Noise</b> algorithm - RGBs values are averaged and played as a sound samples.
  * Color Predominance to Music Note - Depending which color is the most predominant in the pixel (or window), given note is played. Inspired by [project sonify by EECS351 researchers](https://sites.google.com/umich.edu/eecs351-project-sonify/how-we-sonify).
    * Note Complexity [To be implemented] - how many sines are creating single note?
    * Envelope function [To be implemented] - What is the function that decide about share of each sine in the final sound?
    * Color Threshold [To Be implemented] - Limit, at which RGB pixel is treated as black or white, instead of pixel with actual color.
* <b>Row By Row</b> - Group of algorithms, where one note is created from the whole collumn (or row) of pixels.
  * Common Options:
    * Row or Collumn [To Be implemented] - Whether image is scanned row-by-row or collumn-by-collumn
  * <b>Landscape</b> algorithm - Algorithm finds "break point" between darker pixels (ground) and lighter pixels (sky) and treats that breakpoint as new sound intensity
    * TODO - add subalgorithm, how breakpoint is to be treated
  * <b>Spectrogram</b> algorithm [To Be Implemented] - whole collumn is treated as spectrogram - height of the pixel in the collumn is treated as note height, pixel intensity is treated as an envelope (share if this particular note in final sound).
* Musical effects - Image is treated as an musical effect
  * TODO - merge first one - Reverb in the next PR.

## Works to be analyzed and implemented:
* [The Sonification Handbook](https://sonification.de/handbook/download/TheSonificationHandbook-HermannHuntNeuhoff-2011.pdf)
  * More or less a literature overview
  * Funny alg on page 443
* [Along The Line](http://web.archive.org/web/20161212144920if_/http://grond.at:80/index.htm?html/projects/along_the_line/along_the_line.htm&html/submenues/submenu_projects.htm)
* [The sound of photographic image](https://research.gold.ac.uk/id/eprint/14651/1/sound_photographic.pdf)
* [A FRAMEWORK FOR DESIGNING IMAGE SONIFICATION METHODS](https://ccrma.stanford.edu/~woony/publications/Yeo_Berger-ICAD05.pdf) <- First one to be checked!
* [An Experimental System for Auditory Image Representations](https://www.seeingwithsound.com/voicebme.html)
