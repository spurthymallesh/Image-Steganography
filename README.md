# Image Steganography in C

## Overview

This project implements Image Steganography using the Least Significant Bit (LSB) technique in C. It allows users to securely hide secret text messages inside BMP image files and later extract them without visibly altering the image.

---

## Features

- Encode secret messages into BMP images
- Decode hidden messages from encoded images
- Password-independent message hiding
- Supports bitmap (.bmp) images
- Efficient bit-level manipulation

---

## Technologies Used

- C Programming
- File Handling
- Bitwise Operations
- Structures
- Modular Programming

---

## Project Structure

```
encode.c
decode.c
main.c
common.h
encode.h
decode.h
types.h
Makefile
```

---

## How to Compile

```bash
make
```

or

```bash
gcc *.c -o steganography
```

---

## Run

### Encode

```bash
./steganography -e input.bmp secret.txt output.bmp
```

### Decode

```bash
./steganography -d output.bmp
```

---

## Concepts Used

- Image Steganography
- Least Significant Bit (LSB) Encoding
- File Handling
- Bit Manipulation
- Memory Management

---

## Future Improvements

- Password-protected encoding
- PNG image support
- Audio and video steganography
- GUI application
