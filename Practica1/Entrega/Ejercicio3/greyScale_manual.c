#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <sys/time.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <x86intrin.h>


static inline void getRGB(uint8_t *im, int width, int height, int nchannels, int x, int y, int *r, int *g, int *b)
{

    unsigned char *offset = im + (x + width * y) * nchannels;
    *r = offset[0];
    *g = offset[1];
    *b = offset[2];
}

int main(int nargs, char **argv)
{
    int width, height, nchannels;
    struct timeval fin,ini;

    if (nargs < 2)
    {
        printf("Usage: %s <image1> [<image2> ...]\n", argv[0]);
    }
    // For each image
    // Bucle 0
    for (int file_i = 1; file_i < nargs; file_i++)
    {
        printf("[info] Processing %s\n", argv[file_i]);
        /****** Reading file ******/
        uint8_t *rgb_image = stbi_load(argv[file_i], &width, &height, &nchannels, 4);
        if (!rgb_image)
        {
            perror("Image could not be opened");
        }

        /****** Allocating memory ******/
        // - RGB2Grey
        uint8_t *grey_image = malloc(width * height);
        if (!grey_image)
        {
            perror("Could not allocate memory");
        }

        // - Filenames 
        for (int i = strlen(argv[file_i]) - 1; i >= 0; i--)
        {
            if (argv[file_i][i] == '.')
            {
                argv[file_i][i] = 0;
                break;
            }
        }

        char *grey_image_filename = 0;
        asprintf(&grey_image_filename, "%s_grey.jpg", argv[file_i]);
        if (!grey_image_filename)
        {
            perror("Could not allocate memory");
            exit(-1);
        }

        /****** Computations ******/
        printf("[info] %s: width=%d, height=%d, nchannels=%d\n", argv[file_i], width, height, nchannels);

        if (nchannels != 3 && nchannels != 4)
        {
            printf("[error] Num of channels=%d not supported. Only three (RGB), four (RGBA) are supported.\n", nchannels);
            continue;
        }

        gettimeofday(&ini,NULL);
        // RGB to grey scale
        __m256 coeff = _mm256_setr_ps(0.2989f, 0.5870f, 0.1140f, 0.0f, 0.2989f, 0.5870f, 0.1140f, 0.0f);
        __m256 tot, tot_temp;
        __m128i grey;
        __m256i index = _mm256_setr_epi32(0, 4, 1, 5, 0, 0, 0, 0); // the last 4 indexes dont matter as we discard them
  
        for (int i = 0; i<width * height; i+=4)
        {
            // Load in 2 vectors high and low
            __m128i datal = _mm_loadl_epi64((__m128i *)(rgb_image + i * 4));
            __m128i datah = _mm_loadl_epi64((__m128i *)(rgb_image + i * 4 + 8));
            
            // Zero sign extend into 32 bit
            __m256i datal32 = _mm256_cvtepu8_epi32(datal);
            __m256i datah32 = _mm256_cvtepu8_epi32(datah);
            
            // Convert single precision float
            __m256 datalf = _mm256_cvtepi32_ps(datal32);
            __m256 datahf = _mm256_cvtepi32_ps(datah32);
            
            // Multiply by our vector to turn to greyscale
            __m256 datalgrey = _mm256_mul_ps(datalf, coeff);
            __m256 datahgrey = _mm256_mul_ps(datahf, coeff);
            
            // Add the results horizontally
            tot_temp = _mm256_hadd_ps(datalgrey, datahgrey);
            tot = _mm256_hadd_ps(tot_temp, tot_temp); //We add the 4 bytes we were missing
            
            //Reorder the values to be were they started
            tot = _mm256_permutevar8x32_ps(tot, index);
            // Discard the last 4 to be 32 bit
            grey = _mm_cvtps_epi32(_mm256_extractf128_ps(tot, 0));
            // Store the values on our output vector
            uint32_t *storer = (__m128i *)&grey;

            grey_image[i] = storer[0];
            grey_image[i + 1] = storer[1];
            grey_image[i + 2] = storer[2];
            grey_image[i + 3] = storer[3];
        }


        stbi_write_jpg(grey_image_filename, width, height, 1, grey_image, 10);
        free(rgb_image);

        gettimeofday(&fin,NULL);

	printf("Tiempo: %f\n", ((fin.tv_sec*1000000+fin.tv_usec)-(ini.tv_sec*1000000+ini.tv_usec))*1.0/1000000.0);
        free(grey_image_filename);
    }
}
