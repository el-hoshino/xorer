xorer
=====
This is a program to encrypt / decrypt file.

To use this program, you need to compile it first.
I've tested it on my Mac only, but hopefully it'll work on Linux too. And probably it won't work on Windows.

To compile it you just need a gcc compiler, that's it. You can go to the program source code directory and type:
$ gcc xorer.c -o xorer
the the program named "xorer" will appear in your current directory.
If you like you can also put it into your system "bin" directory (say /usr/local/bin/) then you can use it anytime by just typing "xorer" anywhere in your terminal

Usage:
-i: Input file name. If you leave this blank, program will convert all files in the directory (exclude subdirectories) and ignore output setting.
-o: Output file name. If you leave this blank, it will generate the output file in \"xorered\" sub directory.
-x: XOR parameter. Binary will be XORed by this number. Default is 0x19 (25). You can use 0xXX as a hexadecimal number, 0bXX as a binary number, or just XX as a decimal number.
-s: Start point parameter (Byte). Default is 0. Program will apply conversion right from this point.
-e: End point parameter (Byte). Default is 0. While it's 0 it'll be fixed to file size, and program will stop applying encryption before this point.
