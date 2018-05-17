# Image-Processing-cpp
This project presents a non exhaustive list of image processing algorithms. All algorithms are implemented in c++ and all images used are in pgm format.

## Documentation

All instructions required for testing the different algorithms can be found in the documentation provided. See [documentation](image-processing-doc.pdf) for more information on the project.

## Prerequisites

To follow this tutorial a linux machine with gcc compiler driver. 

## How to use the program
The program presents 26 image processing algorithms. Lets suppose we want to binarize the image _lena.pgm_ in the folder _ImageProcessing/images_

### Modifying the code
Using a text editor or an IDE, open the main.cpp file. In the main function, uncomment
the line corresponding to image binarization (otsuBinarize). 

```
Image imageIn = readImage("images/lena.pgm");
Image imageOut=imageIn.otsuBinarize();

```
Leave the other algorithm implementations commented, so as not to produce the
wrong imageOut object. Save the changes to the main.cpp file.

### Compiling the code
If you are using the linux command line, open a linux terminal in the folder : ImageProcessing
and type the following command to compile and run the program :

```
sudo g++ main.cpp Image.cpp -o app && ./app

```
If the above command runs without any errors (ignore the warnings), then the image
has been binarized and written to the file :
_ImageProcessing/output.pgm_

You can open this output file to view the output and make sure its coherent.


## Authors

* **Peterson Yuhala** 

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details


