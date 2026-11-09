/*
 * File:   Functions.h
 * Author: Peterson Yuhala
 * Subject: Image Processing

 * 
 *
 */

#ifndef IMAGE_H
#define IMAGE_H
#include <vector>

class Image
{
public:
    Image();
    Image(int numRows, int numCols, int grayLevels);
    Image(int numRows, int numCols, int grayLevels, bool zeroFill);
    ~Image();
    Image(const Image &orig);
    //virtual ~Image();
    void setImageInfo(int numRows, int numCols, int maxVal);
    void getImageInfo(int &numRows, int &numCols, int &maxVal);
    int getPixelVal(int row, int col) const { return pixelVal[row * cols + col]; };
    void setPixelVal(int row, int col, int value) { pixelVal[row * cols + col] = value; };
    bool inBounds(int row, int col); //checks to see if pixel is in the bounds
    Image threshold(int threshold);  //binarize a grayscale image
    Image otsuBinarize();            //Binarize grayscale image based on Otsu's thresholding method

    Image logicNOT(); //Logic NOT operation on an image

    int rows;       //number of rows
    int cols;       //number of columns
    int gray;       //number of gray levels
    int *pixelVal; //storing in 1D array (was int **pixelVal, 2D array containing pixel values)
};

#endif /* IMAGE_H */
