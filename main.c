#include<stdio.h>
#include"encode.h"
#include"decode.h"
#include"types.h"
#include<string.h>

int main(int argc,char*argv[])
{
    if(argc < 2)
{
    printf("ERROR: Insufficient arguments\n");
    printf("Usage:\n");
    printf("Encode: ./a.out -e beautiful.bmp secret.txt stego.bmp\n");
    printf("Decode: ./a.out -d stego.bmp\n");
    return e_failure;
}

// Check operation type (encode/decode)
    if(check_operation_type(argv) == e_encode)
    {
        printf("Choosed encoding\n");

        EncodeInfo encInfo;

        if(argc < 4)
        {
            printf("Usage: ./a.out -e beautiful.bmp secret.txt stego.bmp\n");
            return e_failure;
        }

         // Validate encoding arguments
        if(read_and_validate_encode_args(argv, &encInfo) == e_success)
        {
            printf("Read and validate successful\n");
            if(do_encoding(&encInfo) == e_success)
            {
                printf("Encoding successfull\n");
                return e_success;
            }
            else
            {
                printf("Encoding unsuccessfull\n");
                return e_failure;

            }
        }
        else
        {
            printf("Read and validate failure\n");  
            return e_failure;

        }

        
        return e_success;
    }
    // Decode operation
    else if(check_operation_type(argv) == e_decode)
{
    if(argc < 3)
    {
        printf("Usage: ./a.out -d stego.bmp\n");
        return e_failure;
    }

    DecodeInfo decInfo;

    decInfo.stego_image_fname = argv[2];
    decInfo.output_fname = "decoded.txt";

    if(do_decoding(&decInfo) == e_success)
        printf("Decoding successful\n");
    else
        printf("Decoding failed\n");

    return e_success;
}
    else
    {
        printf("Invalid arguments\nfor encode -> ./a.out -e beautiful.bmp secret.txt\n");
        printf("for decoding -> ./a.out -d stego.bmp\n");
        return e_failure;

    }

    if(argc < 4)
{
    printf("Error: ./a.out -e beautiful.bmp secret.txt\n");
    return e_failure;
}
}

OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1], "-e") == 0)
    {
        return e_encode;
    }
    else if(strcmp(argv[1], "-d") == 0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}
