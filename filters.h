//
// Created by ibrahim on 9/22/26.
//

#ifndef IMAGE_PROCESSING_SYSTEM_FILTERS_H
#define IMAGE_PROCESSING_SYSTEM_FILTERS_H
#include "Image_Class.h"
//ibrahim Abdul-Wahab ->1
void grayscaleConversion(Image& img) {


}
//ibrahim Yasser ->2
void  blackAndWhite(Image& img) {
    int height=img.height;
    int width=img.width;

    for (int i=0;i<width;++i) {
        for (int j=0;j<height;++j) {
            short r = img.getPixel(i, j, 0);
            short g = img.getPixel(i, j, 1);
            short b = img.getPixel(i, j, 2);
            short avg=(r+g+b)/3;

            for (short color=0;color<3;color++)
                img.setPixel(i,j,color,avg);
        }
    }
}
//Toqa ->3
void  invertImage(Image& img) {

}
//Hatem ->4
void  addingAFrameToThePicture(Image& img) {

}

//ibrahim Abdul-Wahab ->5
void flipImage(Image& img) {

}

//ibrahim Yasser -> 6
void rotate90(Image& img) {
    int height=img.height;
    int width=img.width;
    Image temp(height,width);

    for (int i=0;i<height;++i) {
        for (int j=0;j<width;++j) {
            for (short c=0;c<3;++c)
                temp.setPixel(height - 1 - i, j, c, img.getPixel(j, i, c));

        }
    }
    img=temp;
}

void rotate180(Image& img) {
    int height=img.height;
    int width=img.width;
    Image temp(width, height);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int c = 0; c < 3; c++) {
                temp.setPixel(width - 1 - j, height - 1 - i, c, img.getPixel(j, i, c));
            }
        }
    }
    img = temp;
}

void rotate270(Image& img) {
    int height=img.height;
    int width=img.width;
    Image temp(height,width);

    for (int i=0;i<height;++i) {
        for (int j=0;j<width;++j) {
            for (short c=0;c<3;++c)
                temp.setPixel(i, width-j-1, c, img.getPixel(j, i, c));
        }
    }
    img=temp;
}

void  rotateImage(Image& img,short degree) { // 90,180,270
    if (degree==90)
        rotate90(img);
    else if (degree==180)
        rotate180(img);
    else if (degree==270)
        rotate270(img);
}

//Toqa ->7
void darkenAndLightenImage(Image& img,bool dark,short levelOfLight) {

}
//Hatem ->8
void resizingImages(Image& img) {

}
//Ibrahim Abdul-Wahab ->9
void mergeImages(Image& img) {

}
//ibrahim Yasser -> 10
void detectImageEdges(Image& img) {
    Image gray(img.width, img.height);
    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            int avg = (img.getPixel(j, i, 0) + img.getPixel(j, i, 1) + img.getPixel(j, i, 2)) / 3;
            for (short c = 0; c < 3; c++) {
                gray.setPixel(j, i, c, avg);
            }
        }
    }
    Image result(img.width, img.height);
    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            for (int c = 0; c < 3; c++) {
                result.setPixel(j, i, c, 255);
            }
        }
    }
    int threshold = 20;
    for (int i = 0; i < img.height - 1; i++) {
        for (int j = 0; j < img.width - 1; j++) {
            int current = gray.getPixel(j, i, 1);
            int right = gray.getPixel(j + 1, i, 1);
            int bottom = gray.getPixel(j, i + 1, 1);

            int diffX = abs(current - right);
            int diffY = abs(current - bottom);

            if (diffX > threshold || diffY > threshold) {
                for (int c = 0; c < 3; c++) {
                    result.setPixel(j, i, c, 0);
                }
            }
        }
    }

    img = result;

}

//Toqa ->11
void cropImages(Image& img) {

}

//Hatem ->12
void blurImages(Image& img) {

}
//ibrahim Abdul-Wahab ->13
void  naturalSunlight(Image& img) {

}

//Ibrahim Yasser ->14
void  TVImages(Image& img) {
    int height=img.height;
    int width=img.width;

    for (int i=0;i<height;i++) {
        for (int j=0;j<width;++j) {
            for (short c=0;c<3;c++) {
                short thePixel=img.getPixel(j,i,c);
                bool isScanline=i%2==0;
                if (isScanline) {
                    thePixel -= 40;
                } else {
                    thePixel += 15;
                }

                if (thePixel > 255) thePixel = 255;
                if (thePixel < 0) thePixel = 0;

                img.setPixel(j, i, c, thePixel);
            }
        }
    }
}

//Toqa->15
void convertToPurple(Image& img) {

}

//Hatem -> 16
void convertToInfrared(Image& img) {

}


void imageSkewing(Image& img) {

}

void  oilPainting(Image& img) {

}


#endif //IMAGE_PROCESSING_SYSTEM_FILTERS_H
