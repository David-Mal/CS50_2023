#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    if (file == NULL)
    {
        printf("Could not open %s\n", argv[1]);
        return 1;
    }

// creating an array of bytes
    unsigned char buffer[512];
    bool first_img = 0;
    int cnt = 0;
    FILE *img;
    //while there are blocks of 512 bytes in the source file named "file" (meaning that I won't reach the end of file) I will keep adding them to a buffer
    while (fread(buffer, 512, 1, file))
    {
        //checking for each block of 512 bytes if it is the start of a jpeg file
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (first_img == 0)
            {
                first_img = 1;
            }
            else
            {
                fclose(img);
            }
            //I create and open a new file for each image that is found
            char filename[8];
            sprintf(filename, "%03i.jpg", cnt++);
            img = fopen(filename, "w");
        }
        //if a jpeg file is found I start writing each block of 512 bytes in the new file created
        if (first_img == 1)
        {
            fwrite(&buffer, 512, 1, img);
        }
    }

//closing the files
    fclose(file);
    fclose(img);
    return 0;
}