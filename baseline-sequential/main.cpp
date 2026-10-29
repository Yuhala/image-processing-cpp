
/* 
 * File:   main.cpp
 * Author: Peterson Yuhala Jr.
 * Subject: Image Processing
 * School: ENSP Yaounde
 * email: petersonyuhala@gmail.com
 *
 * Created on November 12, 2017, 3:15 PM
 * This projects represents my image processing class project and contains code written by me, 
 * code copied from the internet, and code copied from friends.
 * Feel free to use the code for any reason.
 *
 * 
 */

#include "Functions.hpp"
#include <string.h>

Image runOperation(int argc, char **argv);
Image runOp3(char **argv);
Image runOp4(char **argv);
Image runOp5(char **argv);
void printHelp();

int main(int argc, char **argv)
{
    int M = 0, N = 0, Q = 0; // rows, cols, grayscale
    int val;
    bool type;
    ofstream ofile("output.pgm", std::ios::out);

    Image imageOut = runOperation(argc, argv);

    //write to the output file
    ofile << "P2\n"
          << imageOut.cols << " " << imageOut.rows << "\n255\n";
    for (int i = 0; i < imageOut.rows; i++)
    {
        for (int j = 0; j < imageOut.cols; j++)
        {
            ofile << imageOut.getPixelVal(i, j) << endl;
        }
    }

    return 0;
}

Image runOperation(int argc, char **argv)
{
    switch (argc)
    {
    case 3:
        return runOp3(argv);
        break;

    case 4:
        return runOp4(argv);
        break;

    case 5:
        return runOp5(argv);
        break;

    default:
        printHelp();
    }
}

Image runOp3(char **argv)
{
    if (strcmp(argv[1], "not") == 0)
    {
        printf("Applying NOT operation on image\n");
        Image imageIn = readImage(argv[2]);
        return imageIn.logicNOT();
    }
    else if (strcmp(argv[1], "otsuBinarize") == 0)
    {
        printf("Applying otsu binarize operation on image\n");
        Image imageIn = readImage(argv[2]);
        return imageIn.otsuBinarize();
    }
    else if (strcmp(argv[1], "linearContrast") == 0)
    {
        printf("Applying linear contrast operation on image\n");
        Image imageIn = readImage(argv[2]);
        return linearContrast(imageIn);
    }
    else if (strcmp(argv[1], "histogramEquilization") == 0)
    {
        printf("Doing histogram equilization on image\n");
        Image imageIn = readImage(argv[2]);
        return histogramEquilization(imageIn);
    }
    else if (strcmp(argv[1], "sobelFilter") == 0)
    {
        printf("Applying sobel filter on image\n");
        Image imageIn = readImage(argv[2]);
        return sobelFilter(imageIn);
    }
    else if (strcmp(argv[1], "prewittFilter") == 0)
    {
        printf("Applying prewitt filter on image\n");
        Image imageIn = readImage(argv[2]);
        return prewittFilter(imageIn);
    }
    else if (strcmp(argv[1], "gaussFilter") == 0)
    {
        printf("Applying gauss filter on image\n");
        Image imageIn = readImage(argv[2]);
        return gaussFilter(imageIn);
    }
    else if (strcmp(argv[1], "smoothingFilter") == 0)
    {
        printf("Applying smoothing filter on image\n");
        Image imageIn = readImage(argv[2]);
        return smoothingFilter(imageIn);
    }
    else if (strcmp(argv[1], "robertFilter") == 0)
    {
        printf("Applying robert filter on image\n");
        Image imageIn = readImage(argv[2]);
        return robertFilter(imageIn);
    }
    else if (strcmp(argv[1], "laplacienConvo") == 0)
    {
        printf("Applying laplacien convolution on image\n");
        Image imageIn = readImage(argv[2]);
        return laplacienConvo(imageIn);
    }
    else if (strcmp(argv[1], "sharpen") == 0)
    {
        printf("Sharpening image\n");
        Image imageIn = readImage(argv[2]);
        return sharpen(imageIn);
    }
    else if (strcmp(argv[1], "edgeDetect") == 0)
    {
        printf("Doing edge detection on image\n");
        Image imageIn = readImage(argv[2]);
        return edgeDetect(imageIn);
    }
    else if (strcmp(argv[1], "mirror") == 0)
    {
        printf("Mirror image\n");
        Image imageIn = readImage(argv[2]);
        return mirror(imageIn);
    }
    else if (strcmp(argv[1], "printHistogram") == 0)
    {
        printf("Printing image histogram \n");
        Image imageIn = readImage(argv[2]);
        printHistogram(imageIn);
        return Image();
    }
    else if (strcmp(argv[1], "luminance") == 0)
    {
        printf("Image luminance: \n");
        Image imageIn = readImage(argv[2]);
        cout << luminance(imageIn) << endl;
        return Image();
    }
    else if (strcmp(argv[1], "calculateContrast") == 0)
    {
        printf("Image contrast: \n");
        Image imageIn = readImage(argv[2]);
        cout << calculateContrast(imageIn) << endl;
        return Image();
    }
    else if (strcmp(argv[1], "erosion") == 0)
    {
        printf("Doing image erosion\n");
        Image imageIn = readImage(argv[2]);
        return erosion(imageIn);
    }

    else
    {
        printHelp();
        return Image();
    }
}

Image runOp4(char **argv)
{
    if (strcmp(argv[2], "and") == 0)
    {
        Image imageIn1 = readImage(argv[1]);
        Image imageIn2 = readImage(argv[3]);
        return logicAND(imageIn1, imageIn2);
    }
    else if (strcmp(argv[2], "nand") == 0)
    {
        Image imageIn1 = readImage(argv[1]);
        Image imageIn2 = readImage(argv[3]);
        return logicNAND(imageIn1, imageIn2);
    }
    else if (strcmp(argv[2], "or") == 0)
    {
        Image imageIn1 = readImage(argv[1]);
        Image imageIn2 = readImage(argv[3]);
        return logicOR(imageIn1, imageIn2);
    }
    else if (strcmp(argv[2], "xor") == 0)
    {
        Image imageIn1 = readImage(argv[1]);
        Image imageIn2 = readImage(argv[3]);
        return logicXOR(imageIn1, imageIn2);
    }
    else if (strcmp(argv[2], "add") == 0)
    {
        Image imageIn1 = readImage(argv[1]);
        Image imageIn2 = readImage(argv[3]);
        return addition(imageIn1, imageIn2);
    }
    else if (strcmp(argv[2], "sub") == 0)
    {
        Image imageIn1 = readImage(argv[1]);
        Image imageIn2 = readImage(argv[3]);
        return subtraction(imageIn1, imageIn2);
    }
    else if (strcmp(argv[1], "mult") == 0)
    {
        Image imageIn1 = readImage(argv[3]);
        double factor = atof(argv[2]);
        return multiplication(imageIn1, factor);
    }
    else if (strcmp(argv[2], "scalingNN") == 0)
    {
        Image imageIn1 = readImage(argv[3]);
        double scale = atof(argv[2]);
        return multiplication(imageIn1, scale);
    }
    else
    {
        printHelp();
        return Image();
    }
}
Image runOp5(char **argv)
{
    if (strcmp(argv[1], "linearContrastSaturation") == 0)
    {
        Image imageIn = readImage(argv[2]);
        double smin = atof(argv[3]);
        double smax = atof(argv[4]);
        return linearContrastSaturation(imageIn, smin, smax);
    }
    else
    {
        printHelp();
        return Image();
    }
}
void printHelp()
{
    printf("--------------------Help---------------------\n"
           "- You must enter 3, 4, or 5 arguments (including the program name)\n"
           "- E.g. To apply NOT operation on an image: ./app not images/lena.pgm\n"
           "- To ADD two images: ./app images/lena.pgm add images/aya.pgm\n"
           "- To MULT an image by 2: ./app mult 2 images/lena.pgm \n"
           "- See README for more info on other operations\n"
           "---------------------------------------------\n"
           "- Contact me: petersonyuhala@gmail.com\n");
}