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

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

Image writeImage(char fname[]);
Image readImage(char fname[]);

//Logic operations on images
Image logicAND(Image& im1,Image& im2);//Logic AND operation on two images
Image logicNAND(Image& im1,Image& im2);//Logic NAND operation on two images
Image logicOR(Image& im1, Image& im2);//Logic OR operation on two images
Image logicXOR(Image& im1, Image& im2);//Logic OR operation on two images

//Mathematical operations on images
Image addition(Image& im1,Image& im2);//Addition of two images
Image multiplication(Image& im,double ratio);//Multiplication an images
Image subtraction(Image& im1,Image& im2);//Subtraction of two images

//Image Histogram and Contrast Enhancement 
Image linearContrast(Image& im);//Amelioration de contraste lineaire
Image linearContrastSaturation(Image& im, double sMin,double sMax);//Amelioration de contrast lineaire avec saturation
Image histogramEquilization(Image& im);//Egalisation de l'histogram
void printHistogram(Image& im);
int calculateContrast(Image& im);
double luminance(Image& im);//Calcule la luminance moyenne de l'image

Image convolution(Image& im, double kernel[SIZE][SIZE],int kSize,int norm);//Finds the convolution of an image using a kernel
Image convo2D(Image& im, double kernel[2][2],int kSize,int norm);
//Filters
Image gaussFilter(Image& im);
Image smoothingFilter(Image& im);
Image sharpen(Image& im);

//Edge Detection Image convolution(Image& im, double kernel[SIZE][SIZE],int kSize,int norm);//Finds the convolution of an image using a kernel
Image edgeDetect(Image& im);
Image robertFilter(Image& im);
Image prewittFilter(Image& im);
Image sobelFilter(Image& im);

//Derivation
Image laplacienConvo(Image& im);


//Image interpolation  ie Image scaling
Image scalingNN(Image& im,double scale);

//Image mirror
Image mirror(Image& im);
//General functions
double min(double a,double b);
double max(double a, double b);
//Morphological Operations
Image erosion(Image& im);
//Functions to calculate maximum and minimum pixel values in an image
int maxPixel(Image &im);
int minPixel(Image &im);

#endif /* FUNCTIONS_H */

