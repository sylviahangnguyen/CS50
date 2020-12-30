#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2) // make sure that the user give only 1 command argument
    {
        printf("Usage: ./recover forensic_image\n");
        return 1; // exit the program
    }

    // Open input file
    FILE *infile = fopen(argv[1], "r");
    if (infile == NULL) // if input file cannot be read, exit the program
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 1;
    }
    // Keep track of how many JPEGs read
    int count_jpeg = 0;

    // Allocate 8 bytes for outfile name
    char *filename = malloc(sizeof(char) * 8);
    if (filename == NULL) // if there's not enough available memory, exit the program
    {
        printf("Not enough memory");
        return 1;
    }

    // Declare the outfile (to keep opening it to write several blocks)
    FILE *outfile;

    // Allocate 512 bytes for each block to read
    unsigned char block[512];

    while (fread(block, sizeof(BYTE), 512, infile) == 512) // while the total bytes read is still 512 (no null bytes)
    {
        // Check first four bytes in a 512-bytes block
        if (block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff && (block[3] & 0xf0) == 0xe0) // JPEG first 4 bytes pattern
        {
            sprintf(filename, "%03i.jpg", count_jpeg); // set the new outfile name

            if (count_jpeg == 0) // the first image
            {
                outfile = fopen(filename, "w"); // open outfile
                fwrite(block, sizeof(BYTE), 512, outfile); // write to outfile
            }
            else
            {
                fclose(outfile); // close the last outfile
                outfile = fopen(filename, "w"); // open new outfile
                fwrite(block, syizeof(BYTE), 512, outfile); // write to outfile
            }
            count_jpeg ++; // increment the number of images read by 1
        }
        else
        {
            if (count_jpeg != 0) // if the 1st image has been found, and the current block is not the 1st block of an image
            {
                fwrite(block, sizeof(BYTE), 512, outfile); // continue to write into the opening outfile
            }

            // if the 1st image has not been found, and the current block is not the 1st block of an image, do nothing
        }
    }

    // Close file
    fclose(infile);
    fclose(outfile);
}