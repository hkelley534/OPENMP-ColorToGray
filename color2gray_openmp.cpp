/*
Convert an image to grayscale.

The code is written for a mini-project of ITCS 5145 Parallel Programming at UNCC.(Q.C.)

To compile the code, we use
        g++ -g -Wall -o color2grapy stb_image/stb_image.h stb_image/stb_image_write.h color2gray.cpp -lm

To run the code, type
        ./color2gray ${input color image} ${output grayscale image} ${image type}

        The format of images depends on its types.
        To specify image type, we have ${image type} as follows:
            1 is for .png file
            2 is for .jpg file
        
        For example,
        ./color2grapy lena1.png lena2.png 1
        ./color2grapy lizard1.jpg lizard2.jpg 2
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// TODO Include the necessary header file for openmp
#include <omp.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

void colorToGrayscale(unsigned char *gray_img, unsigned char * color_img, int width, int height);

const int IS_PNG = 1;
const int IS_JPG = 2;
const int DESIRED_CHANNELS = 3;
const int MAX_NAME_LENGTH = 500;

int numOfThreads = 1;

int main(int argc, char *argv[]) {
    if (argc < 4){
        printf("Usage: color2Grayscale ${input color image file} ${output grayscale image file} ${image type}\n Image Types:\n\t1: PGN\n\t2: JPG");
	exit(-1);
    }

    int width, height, channels, type;
    char  in_name[MAX_NAME_LENGTH], out_name[MAX_NAME_LENGTH];
    strcpy(in_name, argv[1]);
    strcpy(out_name, argv[2]);
    type = atoi(argv[3]);
    numOfThreads = atoi(argv[4]);

    unsigned char *color_img = stbi_load(in_name, &width, &height, &channels, 0); // load and conver the image to 3 channels (ignore the transparancy channel)
    if(color_img == NULL) {
        printf("Error in loading the image\n");
        exit(-1);
    }
    printf("Loaded image %s with a width of %dpx, a height of %dpx and %d channels\n", in_name, width, height, channels);

    // Convert the input image to gray
    int gray_channels = channels == 4 ? 2 : 1;
    size_t gray_img_size = width * height * gray_channels;
    
    unsigned char *gray_img = new unsigned char[gray_img_size];
    if(gray_img == NULL) {
        printf("Unable to allocate memory for the gray image.\n");
        exit(1);
    }
    printf("Create a image array with a width of %dpx, a height of %dpx and %d channels\n", width, height, gray_channels);

//   long start=clock();
    colorToGrayscale(gray_img, color_img, width, height);
 //  long end=clock();
  // printf("Number of threads: %d Color to grayscale conversion completes (%f milliseconds\n", numOfThreads, ((end-start)*1000 / (CLOCKS_PER_SEC + 0.0)));

    if (type == IS_PNG)
    	stbi_write_png(out_name, width, height, gray_channels, gray_img, width * gray_channels);
    else
        if (type == IS_JPG)
            stbi_write_jpg(out_name, width, height, gray_channels, gray_img, 100); //The last parameter of the stbi_write_jpg function is a quality parameter that goes from 1 to 100. Since JPG is a lossy image format, you can chose how much data is dropped at save time. Lower quality means smaller image size on disk and lower visual image quality.
    printf("Wrote image %s with a width of %dpx, a height of %dpx and %d channels\n", out_name, width, height, channels);

    delete [] gray_img;
    stbi_image_free(color_img);
}

void colorToGrayscale(unsigned char *gray_img, unsigned char * color_img, int width, int height){
    // TODO convert color pixel from color_img to gray scale pixel by pixel and 
	// save the results to gray_img.
	// In color_img, a pixel has been represented by a tuple of (r, g,b) values 
	// In gray_img, a pixel has been represented by a weighted value
	unsigned char pixel[DESIRED_CHANNELS];

	double dtime = omp_get_wtime();
	#pragma omp parallel for num_threads( numOfThreads ) schedule (dynamic)
		for (int row = 0; row < height; ++row)
		{
			for (int col = 0; col < width; ++col)
			{
			   //If the input image has a transparency channel this will be simply copied to the second channel of the gray image, while the first channel of the gray image will contain the gray pixel values. If the input image has three channels, the output image will have only one channel with the gray data.

				int greyOffset = row * width + col;
				int rgbOffset = greyOffset * DESIRED_CHANNELS;
				pixel[0] = color_img[rgbOffset];
				pixel[1] = color_img[rgbOffset + 1];
				pixel[2] = color_img[rgbOffset + 2];

				gray_img[greyOffset] = pixel[0] * 0.3 + pixel[1] * 0.58 + pixel[2] * 0.11;
			}
		}
     dtime = omp_get_wtime() - dtime;
     printf("Number of threads: %d Color to grayscale conversion completes (%f milliseconds\n", numOfThreads, dtime);
}
