#ifndef DECODE_H
#define DECODE_H

#include "types.h"

typedef struct _DecodeInfo
{
    char *stego_image_fname;
    FILE *fptr_stego_image;

    char *output_fname;
    FILE *fptr_output;

    char magic[20];
    char extn[10];
    long size;
} DecodeInfo;

Status do_decoding(DecodeInfo *decInfo);

#endif