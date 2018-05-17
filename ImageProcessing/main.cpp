/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Peterson Yuhala Jr.
 * Subject: Image Processing
 * School: ENSP Yaounde
 * email: petersonyuhala@gmail.com
 *
 * Created on November 12, 2017, 3:15 PM
 * 
 *
 * 
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <cstdlib>
#include "Image.h"
using namespace std;

/*
 * The following constant specifies the size of the kernel/mask. This value should be modified 
 *each time the kernel size needs to be changed
 */
const int SIZE = 3;
#include "Functions.h"

int main(int argc, char** argv) {
    int M = 0, N = 0, Q = 0; // rows, cols, grayscale
    int val;
    bool type;
    // M=300;N=300;Q=255;
    ofstream ofile("output.pgm", std::ios::out);

    /*Here we read the input images
     * All images used are found in the images directory
     */
    Image imageIn = readImage("images/lena.pgm");
    Image imageIn2 = readImage("images/aya.pgm");


    /*The following are implementations of each algorithm
     * Uncomment the required line to test a given algorithm
     */
    
   // Image imageOut = imageIn.otsuBinarize();
    //Image imageOut=imageIn.logicNOT();
    //Image imageOut=imageIn.logicAND(imageIn, imageIn2);
    //Image imageOut=imageIn.logicNAND(imageIn, imageIn2);
    //Image imageOut=imageIn.logicOR(imageIn, imageIn2);
    //Image imageOut=imageIn.logicXOR(imageIn, imageIn2);

    
    //Image imageOut = addition(imageIn, imageIn2);
    //Image imageOut = subtraction(imageIn, imageIn2);
    //Image imageOut = multiplication(imageIn,6);

    
    //Image imageOut = linearContrast(imageIn);
    //Image imageOut=linearContrastSaturation(imageIn,70,150);
    //printHistogram(imageIn);
    //Image imageOut = histogramEquilization(imageIn);
    //cout << luminance(imageIn) << endl
    //cout << calculateContrast(imageIn) << endl;
    
    
    //Image imageOut = sobelFilter(imageIn);
    //Image imageOut = prewittFilter(imageIn);
    //Image imageOut = gaussFilter(imageIn);    
    //Image imageOut=smoothingFilter(imageIn;    
    //Image imageOut = robertFilter(imageIn);    
    //Image imageOut = laplacienConvo(imageIn);    
    //Image imageOut=sharpen(imageIn);
    //Image imageOut=edgeDetect(imageIn);
    //Image imageOut=mirror(imageIn);
    
    
    //Image imageOut=scalingNN(imageIn,0.65);

    Image imageOut=erosion(imageIn);

    //write to the output file
    ofile << "P2\n" << imageOut.cols << " " << imageOut.rows << "\n255\n";
    for (int i = 0; i < imageOut.rows; i++) {
        for (int j = 0; j < imageOut.cols; j++) {
            ofile << imageOut.getPixelVal(i, j) << endl;

        }

    }

    return 0;
}

Image readImage(char fname[]) {
    int i, j;
    int N = 0, M = 0, Q = 0;
    unsigned char *charImage;
    char header[100], *ptr;


    ifstream ifs(fname, ios::in | ios::binary);

    stringstream ss;
    string inputLine = "";


    if (!ifs) {
        cout << "Can't read image:" << fname << endl;
        exit(1);
    }

    getline(ifs, inputLine); //read the first line



    if (inputLine.compare("P5") != 0 && inputLine.compare("P2") != 0) {
        cerr << "Version error" << endl;
    }
    cout << "Version : " << inputLine << endl;

    getline(ifs, inputLine); // read the second line : comment
    cout << "Comment : " << inputLine << endl;

    ss << ifs.rdbuf(); //read the third line : width and height
    ss >> M >> N;
    cout << M << " columns and " << N << " rows" << endl;


    ss >> Q;
    ss.ignore();
    cout << Q << endl;

    Image image(N, M, Q);



    // Read and record the pixel values into the image object

    unsigned int pixel;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            ss>>pixel;

            image.setPixelVal(i, j, pixel);

        }

    }

    ifs.close();


    return image;
}
//Logic Operations on images

Image logicAND(Image& im1, Image& im2) {
    if (!im1.gray == im2.gray) {
        cout << "Images must have same maximum gray values" << endl;
        exit(1);
    }
    int Max = im1.gray;
    int pixel = 0, val = 0;
    Image newIm1 = im1.otsuBinarize();
    Image newIm2 = im2.otsuBinarize();
    Image newImage = Image(im1.rows, im1.cols, im1.gray);
    for (int i = 0; i < im1.rows; i++) {
        for (int j = 0; j < im1.cols; j++) {
            pixel = newIm1.getPixelVal(i, j) && newIm2.getPixelVal(i, j);
            val = (pixel == 1) ? 255 : 0; //NB: 255 && 255=1, 255 && 0=0         
            newImage.setPixelVal(i, j, val);

        }

    }

    return newImage;



}

Image logicNAND(Image& im1, Image& im2) {//NAND=NOT(AND)
    if (!im1.gray == im2.gray) {
        cout << "Images must have same maximum gray values" << endl;
        exit(1);
    }
    int Max = im1.gray;
    int pixel = 0, val = 0;
    Image newIm1 = im1.otsuBinarize();
    Image newIm2 = im2.otsuBinarize();
    Image newImage = Image(im1.rows, im1.cols, im1.gray);
    for (int i = 0; i < im1.rows; i++) {
        for (int j = 0; j < im1.cols; j++) {
            pixel = !(newIm1.getPixelVal(i, j) && newIm2.getPixelVal(i, j)); // im1 NAND im2
            val = (pixel == 1) ? 255 : 0; //NB: 255 && 255=1, 255 && 0=0         
            newImage.setPixelVal(i, j, val);

        }

    }

    return newImage;



}

Image logicOR(Image& im1, Image& im2) {
    if (!im1.gray == im2.gray) {
        cout << "Images must have same maximum gray values" << endl;
        exit(1);
    }
    int Max = im1.gray;
    int pixel = 0, val = 0;
    Image newIm1 = im1.otsuBinarize();
    Image newIm2 = im2.otsuBinarize();
    Image newImage = Image(im1.rows, im1.cols, im1.gray);
    for (int i = 0; i < im1.rows; i++) {
        for (int j = 0; j < im1.cols; j++) {
            pixel = newIm1.getPixelVal(i, j) || newIm2.getPixelVal(i, j);
            val = (pixel == 1) ? 255 : 0; //NB: 255 && 255=1, 255 && 0=0         
            newImage.setPixelVal(i, j, val);

        }

    }

    return newImage;



}

Image logicXOR(Image& im1, Image& im2) {// A XOR B=(A AND NOT B)||(NOT A AND B)
    if (!im1.gray == im2.gray) {
        cout << "Images must have same maximum gray values" << endl;
        exit(1);
    }
    int Max = im1.gray;
    int pixel = 0, val = 0;
    Image newIm1 = im1.otsuBinarize();
    Image newIm2 = im2.otsuBinarize();
    Image newImage = Image(im1.rows, im1.cols, im1.gray);
    for (int i = 0; i < im1.rows; i++) {
        for (int j = 0; j < im1.cols; j++) {
            pixel = (newIm1.getPixelVal(i, j) && !(newIm2.getPixelVal(i, j))) || (newIm2.getPixelVal(i, j) && !(newIm1.getPixelVal(i, j)));
            val = (pixel == 1) ? 255 : 0; //NB: 255 && 255=1, 255 && 0=0         
            newImage.setPixelVal(i, j, val);

        }

    }

    return newImage;



}

//Mathematical Operations on images

Image addition(Image& im1, Image& im2) {
    if (!im1.gray == im2.gray) {
        cout << "Images must have same maximum gray values" << endl;
        exit(1);
    }
    int Max = im1.gray;
    int pixel = 0, val = 0;
    Image newImage = Image(im1.rows, im1.cols, im1.gray);
    for (int i = 0; i < im1.rows; i++) {
        for (int j = 0; j < im1.cols; j++) {
            pixel = im1.getPixelVal(i, j) + im2.getPixelVal(i, j);
            val = (int) min(pixel, Max);
            newImage.setPixelVal(i, j, val);

        }

    }

    return newImage;



}

Image subtraction(Image& im1, Image& im2) {//im1-im2
    if (!im1.gray == im2.gray) {
        cout << "Images must have same maximum gray values" << endl;
        exit(1);
    }
    int Max = im1.gray;
    int pixel = 0, val = 0;
    Image newImage = Image(im1.rows, im1.cols, im1.gray);
    for (int i = 0; i < im1.rows; i++) {
        for (int j = 0; j < im1.cols; j++) {
            pixel = im1.getPixelVal(i, j) - im2.getPixelVal(i, j);
            val = (int) max(pixel, 0);
            newImage.setPixelVal(i, j, val);

        }

    }

    return newImage;



}

Image multiplication(Image& im, double ratio) {

    double pixel = 0;
    double val = 0;
    int Max = im.gray;
    Image newImage = Image(im.rows, im.cols, im.gray);
    for (int i = 0; i < im.rows; i++) {
        for (int j = 0; j < im.cols; j++) {
            pixel = im.getPixelVal(i, j) * ratio;
            val = min(pixel, Max);
            newImage.setPixelVal(i, j, val);

        }

    }

    return newImage;



}

//Image convolution algorithm

/**
 * All kernels used here are matrices
 *
 *
 * +-+-+-+-
 * |1|2|3|
 * +-+-+-+
 * |0|4|5|
 * +-+-+-+
 * |2|3|7|
 * +-+-+-+
 * 
 * @param im The image object passed on which mean filtering is performed.
 * @param kSize - The size of the mask is an odd integer like 3, 5, 7 … etc.
 * @param kernel- The mask itself
 */
Image convolution(Image& im, double kernel[SIZE][SIZE], int kSize, int norm) {
    Image newImage = Image(im.rows, im.cols, im.gray);

    // find center position of kernel (half of kernel size)
    int kCenter = kSize / 2;
    //int mm = 0, nn = 0, ii = 0, jj = 0;
    double sum = 0;




    for (int i = 0; i < im.rows; ++i) // rows
    {
        for (int j = 0; j < im.cols; ++j) // columns
        {
            for (int m = 0; m < kSize; ++m) // kernel rows
            {
                int mm = kSize - 1 - m;

                for (int n = 0; n < kSize; ++n) // kernel columns
                {
                    int nn = kSize - 1 - n;
                    // ignore input samples which are out of bound
                    int ii = i + (m - kCenter);
                    int jj = j + (n - kCenter);
                    if (ii < 0)
                        ii = ii + 1;
                    if (jj < 0)
                        jj = jj + 1;
                    if (ii >= im.rows)
                        ii = ii - 1;
                    if (jj >= im.cols)
                        jj = jj - 1;
                    if (ii >= 0 && ii < im.rows && jj >= 0 && jj < im.cols)
                        sum += im.getPixelVal(ii, jj) * kernel[mm][nn];


                }
            }

            double val = sum / norm;

            val = val < 0 ? 0 : min(val, im.gray);
            newImage.setPixelVal(i, j, (int) val);
            sum = 0;
        }
    }

    return newImage;
}

Image convo2D(Image& im, double kernel[2][2], int kSize, int norm) {
    Image newImage = Image(im.rows, im.cols, im.gray);

    // find center position of kernel (half of kernel size)
    int kCenter = kSize / 2;
    //int mm = 0, nn = 0, ii = 0, jj = 0;
    double sum = 0;




    for (int i = 0; i < im.rows; ++i) // rows
    {
        for (int j = 0; j < im.cols; ++j) // columns
        {
            for (int m = 0; m < kSize; ++m) // kernel rows
            {
                int mm = kSize - 1 - m;

                for (int n = 0; n < kSize; ++n) // kernel columns
                {
                    int nn = kSize - 1 - n;
                    // ignore input samples which are out of bound
                    int ii = i + (m - kCenter);
                    int jj = j + (n - kCenter);
                    if (ii < 0)
                        ii = ii + 1;
                    if (jj < 0)
                        jj = jj + 1;
                    if (ii >= im.rows)
                        ii = ii - 1;
                    if (jj >= im.cols)
                        jj = jj - 1;
                    if (ii >= 0 && ii < im.rows && jj >= 0 && jj < im.cols)
                        sum += im.getPixelVal(ii, jj) * kernel[mm][nn];


                }
            }

            double val = sum / norm;

            val = val < 0 ? 0 : min(val, im.gray);
            newImage.setPixelVal(i, j, (int) val);
            sum = 0;
        }
    }

    return newImage;
}

Image robertFilter(Image& im) {
    double kern1[2][2] = {
        {-1, 0},
        {0, -1}
       
    };
     double kern2[2][2] = {
        {0, 1},
        {-1,0}
       
    };
   

    Image temp = Image(convo2D(im, kern1, 2, 1));
    return (convo2D(temp, kern2, 2, 1));


}
Image edgeDetect(Image& im) {
    double kern[3][3] = {
        {0, 1, 0},
        {1, -4, 1},
        {0, 1, 0}
    };
    return (convolution(im, kern, 3, 1));
}//Detects the edges of an image

Image gaussFilter(Image& im) {
    double gaussBlur[3][3] = {
        {1, 2, 1},
        {2, 4, 2},
        {1, 2, 1}
    };
    return (convolution(im, gaussBlur, 3, 9));

}

Image prewittFilter(Image& im) {
    double kern1[3][3] = {
        {-1, -1, -1},
        {0, 0, 0},
        {1, 1, 1}
    };
    double kern2[3][3] = {
        {-1, 0, 1},
        {-1, 0, 1},
        {-1, 0, 1}
    };

    Image temp = Image(convolution(im, kern1, 3, 1));
    return (convolution(temp, kern2, 3, 1));


}

Image sobelFilter(Image& im) {
    double kern1[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };
    double kern2[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    Image temp = Image(convolution(im, kern1, 3, 1));
    return (convolution(temp, kern2, 3, 1));


}

Image smoothingFilter(Image& im) {
    double kern[3][3] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    return (convolution(im, kern, 3, 9));
}

//Histogram and Contrast algorithms

void printHistogram(Image& im) {
    int histogram[im.gray + 1] = {0};
    int sum = 0;
    //Calculate image histogram
    for (int i = 0; i < im.rows; i++) {
        for (int j = 0; j < im.cols; j++) {
            int pixel = im.getPixelVal(i, j);
            histogram[pixel]++;

        }
    }
    //Print histogram
    for (int i = 0; i < im.gray + 1; i++) {
        cout << i << ": " << histogram[i] << endl;
        sum += histogram[i];

    }
    cout << "Sum of pixels= " << sum << endl;

}

int calculateContrast(Image& im) {
    //Calculates contrast using min-max variation

    double contrast = 0;
    int mini = minPixel(im);
    int maxi = maxPixel(im);
    int val = 0;
    contrast = (maxi - mini) / (maxi + mini);
    return contrast;
}

double luminance(Image& im) {
    double lum = 0, sum = 0;
    int total = im.cols * im.rows, pixel = 0;
    ;
    for (int i = 0; i < im.rows; i++) {
        for (int j = 0; j < im.cols; j++) {
            sum += im.getPixelVal(i, j);

        }
    }
    lum = sum / total;
    return lum;
}

Image linearContrast(Image& im) {//Amelioration de contrast lineaire
    Image newImage = Image(im.rows, im.cols, im.gray);
    int mini = minPixel(im);
    int maxi = maxPixel(im), pixel = 0;
    int val = 0;

    for (int i = 0; i < im.rows; i++) {
        for (int j = 0; j < im.cols; j++) {
            pixel = im.getPixelVal(i, j);
            val = im.gray * (pixel - mini) / (maxi - mini);
            newImage.setPixelVal(i, j, val);
        }

    }
    return newImage;
}

Image linearContrastSaturation(Image& im, double sMin, double sMax) {
    //Amelioration de contrast par transformation lineaire avec saturation

    if ((sMin > sMax) || (sMin < minPixel(im)) || (sMax > maxPixel(im))) {
        //Test simples pour niveaux de saturation
        cout << "Valeurs de saturation invalides" << endl;
        exit(1);
    }
    Image newImage = Image(im.rows, im.cols, im.gray);
    int val = 0, pixel = 0;

    for (int i = 0; i < im.rows; i++) {
        for (int j = 0; j < im.cols; j++) {
            pixel = im.getPixelVal(i, j);
            val = im.gray * (pixel - sMin) / (sMax - sMin);
            val = val < 0 ? 0 : val;
            val = val > im.gray ? im.gray : val;
            newImage.setPixelVal(i, j, val);
        }

    }


    return newImage;
}

Image histogramEquilization(Image& im) {
    Image newImage = Image(im.rows, im.cols, im.gray);
    int pixel = 0, val = 0;
    //On utilise les doubles ici pour eviter la perte de l'information pendant la division!
    double histogram[im.gray + 1] = {0};
    double hn[im.gray + 1] = {0};
    double ci[im.gray + 1] = {0};
    double nbp = im.cols * im.rows;
    int sum = 0;
    //Calculate image histogram
    for (int i = 0; i < im.rows; i++) {
        for (int j = 0; j < im.cols; j++) {
            int pixel = im.getPixelVal(i, j);
            histogram[pixel]++;

        }
    }
    //Normalise the histogram
    for (int i = 0; i < im.gray + 1; i++) {
        hn[i] = histogram[i] / nbp;

    }
    //Densite de probabilité normalisé
    for (int i = 0; i < im.gray + 1; i++) {
        for (int j = 0; j < i; j++) {
            ci[i] += hn[j];
        }
    }
    //Transformation des niveau de gris de l'image
    for (int i = 0; i < im.rows; i++) {
        for (int j = 0; j < im.cols; j++) {
            pixel = im.getPixelVal(i, j);
            val = ci[pixel] * im.gray;
            newImage.setPixelVal(i, j, val);

        }

    }


    return newImage;

}

//Derivation of images

Image laplacienConvo(Image& im) {
    //Calculate the laplacian approximation using 2D convolution
    double kern[3][3] = {
        {1, 1, 1},
        {1, -8, 1},
        {1, 1, 1}
    };
    return (convolution(im, kern, 3, 1));
}

//Interpolation of images

Image sharpen(Image& im) {
    double kern[3][3] = {
        {1.0 / 4, 1.0 / 2, 1.0 / 4},
        {1.0 / 2, 1, 1.0 / 2},
        {1.0 / 4, 1.0 / 2, 1.0 / 4}
    };
    return (convolution(im, kern, 3, 4));
}

Image scalingNN(Image& im, double scale) {
    int pixels[im.cols * im.rows] = {0};
    //Put image pixels in 1D array
    for (int i = 0; i < im.rows; i++) {
        for (int j = 0; j < im.cols; j++) {
            pixels[j * im.rows + i] = im.getPixelVal(i, j);
        }

    }

    int destW = (int) im.cols*scale;
    int destH = (int) im.rows*scale;

    int temp[destH * destW] = {0};
    int px = 0, py = 0, pixel = 0;
    Image newImage = Image(destH, destW, im.gray);
    for (int i = 0; i < destH; i++) {
        for (int j = 0; j < destW; j++) {
            py = (int) i*scale;
            px = (int) j*scale;

            temp[(j * destH) + i] = pixels[(int) (px * im.rows + py)];



        }
    }
    //Put temp pixels in final image
    for (int i = 0; i < destH; i++) {
        for (int j = 0; j < destW; j++) {
            newImage.setPixelVal(i, j, temp[j * destH + i]);
        }

    }

    return newImage;
}
//NB: 0<=scale<=1

Image mirror(Image& im) {
    //Calculates the mirror of the input image
    Image newImage = Image(im.rows, im.cols, im.gray);
    int pixel = 0;

    for (int i = 0; i < im.rows; i++) {
        for (int lx = 0, rx = im.cols - 1; lx < im.cols; lx++, rx--) {
            pixel = im.getPixelVal(i, lx);
            newImage.setPixelVal(i, rx, pixel);
        }

    }

    return newImage;
}
//Morphological Operations

Image erosion(Image& im) {
    Image binImage = Image(im.otsuBinarize());
    double kern[3][3] = {
        {1, -1, 1},
        {1, -1, 1},
        {1, -1, 1}
    };
    return (convolution(binImage, kern, 3, 3));

}

double max(double a, double b) {
    return ((a >= b) ? a : b);
}

double min(double a, double b) {
    return ((a <= b) ? a : b);
}

int maxPixel(Image &im) {
    int maxi = 0, pixel = 0;
    int val = 0;

    for (int i = 0; i < im.rows; i++) {
        for (int j = 0; j < im.cols; j++) {
            pixel = im.getPixelVal(i, j);
            maxi = (int) max(pixel, maxi);
        }

    }
    return maxi;
}

int minPixel(Image &im) {
    int mini = im.gray, pixel = 0;
    int val = 0;

    for (int i = 0; i < im.rows; i++) {
        for (int j = 0; j < im.cols; j++) {
            pixel = im.getPixelVal(i, j);
            mini = (int) min(pixel, mini);

        }

    }
    return mini;
}



