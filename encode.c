#include<stdio.h>
#include<string.h>
#include "encode.h"
#include"types.h"

// Validate encoding arguments
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    // Check source image
    if(strstr(argv[2], ".bmp") != NULL)
    {
        printf(".bmp file present");
        encInfo->src_image_fname =argv[2];
    }
    else
    {
        printf(".bmp file not present");
        return e_failure;
    }

    // Check secret file
    if(strstr(argv[3], ".txt") != NULL)
    {
        printf(".txt is present\n");
        encInfo->secret_fname = argv[3];
    }
    else
    {
        printf(".txt file not present");
        return e_failure;
    }

     // Check output file or assign default
    if(argv[4] != NULL && strstr(argv[4], ".bmp") != NULL)
{
    printf("stego.bmp is present\n");
    encInfo->stego_image_fname = argv[4];
}
else
{
    printf("Using default output file: stego.bmp\n");
    encInfo->stego_image_fname = "stego.bmp";
}

    return e_success;

}

Status do_encoding(EncodeInfo *encInfo)
{
    printf("<--------Started doing encoding ----------->\n");

    // Open required files
    if(open_files(encInfo) == e_success)
    {
        printf("All files opened successfully\n");
    }
    else
    {
        printf("All files did not opened successfully\n");
        return e_failure;
    }

    printf("Enter magic string:");
    scanf(" %[^\n]",encInfo -> magic);

    if(check_capacity(encInfo) == e_success)
    {
        printf("Capacity checked successfully\n");
    }
    else
    {
        printf("Capacity of source file is less\n");
        return e_failure;
    }

    if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {
        printf("Header copied successfully\n");
    }
    else
    {
        printf("Header copied unsuccessfully\n");
        return e_failure;

    }

    if(encode_magic_string(encInfo->magic,encInfo) == e_success)
    {
        printf("Magic string encoded successfully\n");
    }
    else{
        printf("Magic string encoded unsuccessfully\n");
        return e_failure;
    }

    if(encode_secret_file_extn(strchr(encInfo->secret_fname,'.'),encInfo)==e_success){
        printf("secrete file extention encoded successfully\n");
    }
    else
    {
        printf("encoding secrete file is unsuccessfull\n");
        return e_failure;
    }
    if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_success)
    {
        printf("encoding secret_file_size is done\n");
    }
    else
    {
        printf("secret file size encoding not done\n");
        return e_failure;
    }
    if(encode_secret_file_data(encInfo)==e_success)
    {
          printf("encoding of secrete file data is successful\n");
    }
    else
    {
        printf("encoding od secret file is unsuccessfull\n");
        return e_failure;
    }
    if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image)==e_success)
    {
          printf("coping of remaining image data is successfull\n");
    }
    else
    {
        printf("coping remaining data is unsucessfull\n");
        return e_failure;
    }
    return e_success;
 }


Status open_files(EncodeInfo *encInfo)
{
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");

    if(encInfo->fptr_src_image== NULL)
    {
        printf("Source file is not present\n");
        return e_failure;
    }
    else
    {
        printf("Source file is present\n");

    }

    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");

    if(encInfo->fptr_secret == NULL)
    {
        printf("Secret file is not present\n");
        return e_failure; 
    }
    else
    {
        printf("Secret file is opened\n");
    }

    encInfo->fptr_stego_image= fopen(encInfo->stego_image_fname,"w");

    if(encInfo->fptr_stego_image == NULL)
    {
        printf("Stego file is not present\n");
        return e_failure;
    }
    else
    {
        printf("Stego file is opened\n");
    }

    return e_success;

}

Status check_capacity(EncodeInfo *encInfo)
{

    encInfo->image_capacity =get_image_size_for_bmp(encInfo -> fptr_src_image);
    printf("Image capcity = %u\n",encInfo->image_capacity);

    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);

    printf("Secret file size = %lu\n",encInfo->size_secret_file);

    if(encInfo->image_capacity > (strlen(encInfo->magic)*8+32+32+32+encInfo->size_secret_file*8))
    return e_success;
    else
    return e_failure;
}

uint get_image_size_for_bmp(FILE *fptr_image)
{
    int wid,len;

        fseek(fptr_image,18,SEEK_SET);

        fread(&wid,4,1,fptr_image);

        fread(&len,4,1,fptr_image);

        return wid*len*3; 
}

uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    return ftell(fptr); 
    rewind(fptr);
    //return size;
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    rewind(fptr_src_image);

    char buffer[54];

    fread(buffer,54,1,fptr_src_image);
    fwrite(buffer,54,1,fptr_dest_image);

    return e_success;
}

Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
    if(encode_data_to_image(magic_string,strlen(magic_string),encInfo ) == e_success)
    {
        printf("Magic Encoded\n");
        return e_success;
    }
    else{
        printf("Magic not encoded\n");
    }
}

Status encode_data_to_image(char *data, int size, EncodeInfo *encInfo )
{
    //char buffer[8];

    for(int i=0;i<size;i++)
    {
        fread(encInfo->image_data,8,1,encInfo->fptr_src_image);
        encode_byte_to_lsb(data[i],encInfo->image_data);
        fwrite(encInfo->image_data,8,1,encInfo->fptr_stego_image);
    }
    

    return e_success;

}

Status encode_byte_to_lsb(char data, char *image_buffer){
    // for(int i=0;i<8;i++){
    //     image_buffer[i]=(image_buffer[i]& 0xFE)|((data>>i)&1);
    // }
    for(int i=7;i>=0;i--){
        image_buffer[7-i]=(image_buffer[7-i]& 0xFE)|((data &(1<<i))>>i);
    }
    return e_success;
}
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo){

     if(encode_data_to_image(file_extn, strlen(file_extn),encInfo)==e_success){
        return e_success;
     }

     return e_failure;
}
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    encode_size_to_lsb(file_size, encInfo);
    return e_success;
}
Status encode_size_to_lsb(long file_size, EncodeInfo *encInfo)
{
       char buffer[32];
       fread(buffer,32,1,encInfo->fptr_src_image);
       for(int i=31;i>=0;i--)
       {
         buffer[31-i]=(buffer[31-i]&0xFE)|((file_size & (1<<i))>>i);
       }
       fwrite(buffer,32,1,encInfo->fptr_stego_image);
       return e_success;
}

// Encode secret file data into image
Status encode_secret_file_data(EncodeInfo *encInfo){
    char buffer[encInfo->size_secret_file];
     rewind(encInfo->fptr_secret);

      // Read entire secret file
     fread(buffer,encInfo->size_secret_file,1,encInfo->fptr_secret);

      // Encode data into image
     encode_data_to_image(buffer, encInfo->size_secret_file, encInfo);
     
    return e_success;
}
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest){
    char ch;
    while(fread(&ch,1,1,fptr_src)==1)
    {
        fwrite(&ch,1,1,fptr_dest);
    }
    return e_success;
}
