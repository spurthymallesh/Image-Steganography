#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "decode.h"
#include "types.h"

/* Function to decode one byte from LSB */
Status decode_byte_from_lsb(char *image_buffer, char *data)
{
    *data = 0;

    for(int i = 0; i < 8; i++)
    {
        *data = (*data << 1) | (image_buffer[i] & 1);
    }

    return e_success;
}

/* Function to decode multiple bytes */
Status decode_data_from_image(char *data, int size, FILE *fptr_image)
{
    char buffer[8];

    for(int i = 0; i < size; i++)
    {
        fread(buffer, 8, 1, fptr_image);
        decode_byte_from_lsb(buffer, &data[i]);
    }

    return e_success;
}

/* Decode magic string */
Status decode_magic_string(DecodeInfo *decInfo)
{
    char user_magic[20];

    printf("Enter magic string: ");
    scanf("%s", user_magic);

    decode_data_from_image(decInfo->magic, strlen(user_magic), decInfo->fptr_stego_image);
    decInfo->magic[strlen(user_magic)] = '\0';

    if(strcmp(decInfo->magic, user_magic) == 0)
    {
        printf("Magic string matched successfully\n");
        return e_success;
    }
    else
    {
        printf("ERROR: Magic string mismatch\n");
        return e_failure;
    }
}

/* Decode secret file extension */
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    decode_data_from_image(decInfo->extn, 4, decInfo->fptr_stego_image);
    decInfo->extn[4] = '\0';

    printf("File extension decoded: %s\n", decInfo->extn);
    return e_success;
}

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char buffer[32];
    decInfo->size = 0;

    fread(buffer, 32, 1, decInfo->fptr_stego_image);

    for(int i = 0; i < 32; i++)
    {
        decInfo->size = (decInfo->size << 1) | (buffer[i] & 1);
    }

    printf("Secret file size: %ld bytes\n", decInfo->size);
    return e_success;
}

/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char ch;
    char buffer[8];

    for(long i = 0; i < decInfo->size; i++)
    {
        fread(buffer, 8, 1, decInfo->fptr_stego_image);
        decode_byte_from_lsb(buffer, &ch);
        fwrite(&ch, 1, 1, decInfo->fptr_output);
    }

    printf("Secret file data decoded successfully\n");
    return e_success;
}

/* Open files */
Status open_decode_files(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    if(decInfo->fptr_stego_image == NULL)
    {
        printf("Error opening stego image file\n");
        return e_failure;
    }

    decInfo->fptr_output = fopen(decInfo->output_fname, "wb");
    if(decInfo->fptr_output == NULL)
    {
        printf("Error opening output file\n");
        return e_failure;
    }

    return e_success;
}

/* Main decoding function */
Status do_decoding(DecodeInfo *decInfo)
{
    printf("<--------Started Decoding----------->\n");

    if(open_decode_files(decInfo) != e_success)
        return e_failure;

    /* Skip BMP header (54 bytes) */
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);
    

    /* Decode steps */
    if(decode_magic_string(decInfo) != e_success)
    {
        printf("No hidden data found\n");
        return e_failure;
    }

    if(decode_secret_file_extn(decInfo) != e_success)
        return e_failure;

    if(decode_secret_file_size(decInfo) != e_success)
        return e_failure;

    if(decode_secret_file_data(decInfo) != e_success)
        return e_failure;

    printf("Decoding completed successfully\n");

    return e_success;
}