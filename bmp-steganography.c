// Name: Estherlee Redman

#include <stdio.h>

int hidden_byte(FILE *file)
{
    int colour;
    int value = 0;

    for (int i = 0; i < 8; i++)
    {
        colour = fgetc(file);
        if (colour == EOF)
            return EOF;
        value = (value << 1) | (colour & 1);
    }

    return value;
}

int main(int argc, char *argv[])
{
    FILE *bmp_img;
    FILE *output;
    unsigned char header[54];
    char message[81];

    if (argc != 3)
        fprintf(stderr, "Missing BMP image and output file - ");
    else
    {
        bmp_img = fopen(argv[1], "rb");
        if (bmp_img == NULL)
            fprintf(stderr, "Could not open file - ");
    }

    fread(header, 1, 54, bmp_img);
    if (header[0] != 'B' || header[1] != 'M')
        fprintf(stderr, "Invalid file type");

    int i = 0;
    while (1)
    {
        int byte = hidden_byte(bmp_img);
        if (byte == EOF)
            fprintf(stderr, "Interrupted");
        else if (byte == 0)
            break;
        else
        {
            message[i] = byte;
            i++;
        }
    }
    message[i] = '\0';
    fclose(bmp_img);

    output = fopen(argv[2], "wb");
    fwrite(message, 1, i, output);
    fclose(output);
}