/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Functions.h
 * Author: Peterson Yuhala
 * Subject: Image Processing
 * School: ENSP Yaounde
 *
 * Created on November 12, 2017, 3:15 PM
 */
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include "Image.h"
using namespace std;

Image::Image() {
    rows = 0;
    cols = 0;
    gray = 0;
    pixelVal = NULL;
}

Image::Image(int numRows, int numCols, int grayLevels) {
    rows = numRows;
    cols = numCols;
    gray = grayLevels;

    pixelVal = new int*[rows];
    for (int i = 0; i < rows; i++) {
        pixelVal[i] = new int[cols];
        for (int j = 0; j < cols; j++) {
            pixelVal[i][j] = 0;
        }

    }
}

Image::Image(const Image& oldImage) {
    rows = oldImage.rows;
    cols = oldImage.cols;
    gray = oldImage.gray;

    pixelVal = new int* [rows];
    for (int i = 0; i < rows; i++) {
        pixelVal[i] = new int [cols];
        for (int j = 0; j < cols; j++)
            pixelVal[i][j] = oldImage.pixelVal[i][j];
    }
}

Image::~Image() {
    rows = 0;
    cols = 0;
    gray = 0;

    for (int i = 0; i < rows; i++)
        delete pixelVal [rows];

    delete pixelVal;
}

int Image::getPixelVal(int row, int col)
/*returns the gray value of a specific pixel*/ {
    return pixelVal[row][col];
}

void Image::setPixelVal(int row, int col, int value) {
    pixelVal[row][col] = value;
}

bool Image::inBounds(int row, int col)
/*checks to see if a pixel is within the image, returns true or false*/ {
    if (row >= rows || row < 0 || col >= cols || col < 0)
        return false;
    //else
    return true;
}

Image Image::threshold(int threshold) {
    int pixel = 0, val = 0;
    Image newImage(rows, cols, gray);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            pixel = this->getPixelVal(i, j);
            val = pixel >= threshold ? gray : 0;

            newImage.setPixelVal(i, j, val);

        }

    }

    return newImage;
}

Image Image::logicNOT() {
    int pixel = 0, val = 0;

    Image newImage = this->otsuBinarize();


    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            pixel = newImage.getPixelVal(i, j);
            val = (pixel == 0) ? 255 : 0;

            newImage.setPixelVal(i, j, val);

        }

    }

    return newImage;
}

Image Image::otsuBinarize() {
    double histogram[gray + 1] = {0};
    int sum = 0;
    //Calculate image histogram
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int pixel =getPixelVal(i, j);
            histogram[pixel]++;

        }
    }
    //Calculate pixel sum
    for (int i = 0; i < gray + 1; i++) {

        sum += histogram[i];

    }
    //Find pixel probabilities
    for (int i = 0; i < gray + 1; i++) {
        
         histogram[i]=(double)histogram[i]/(double)sum;
         

    }
    //Begin Otsu's algorithmImage newImage =this->threshold(100);

    double probability[gray + 1], mean[gray + 1];
    double max_between, between[gray + 1];
    int threshold;

    /*
    probability = class probability
    mean = class mean
    between = between class variance
    */

    for(int i = 0; i < gray + 1; i++) {
        probability[i] = 0.0;
        mean[i] = 0.0;
        between[i] = 0.0;
    }

    probability[0] = histogram[0];

    for(int i = 1; i < gray + 1; i++) {
        probability[i] = probability[i - 1] + histogram[i];
        mean[i] = mean[i - 1] + i * histogram[i];
    }

    threshold = 0;
    max_between = 0.0;

    for(int i = 0; i < 255; i++) {
        if(probability[i] != 0.0 && probability[i] != 1.0)
            between[i] = pow(mean[255] * probability[i] - mean[i], 2) / (probability[i] * (1.0 - probability[i]));
    else
        between[i] = 0.0;
        if(between[i] > max_between) {
            max_between = between[i];
            threshold = i;
        }
    }
 
    //Apply thresholding algorithm using Otsu's threshold value
    cout << "Otso Threshold Value: "<<threshold<< endl;
    Image newImage = this->threshold(threshold);
    return newImage;
}