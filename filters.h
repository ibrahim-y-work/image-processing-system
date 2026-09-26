//
// Created by ibrahim on 9/22/26.
//

#ifndef IMAGE_PROCESSING_SYSTEM_FILTERS_H
#define IMAGE_PROCESSING_SYSTEM_FILTERS_H
#include "Image_Class.h"
//ibrahim Abdul-Wahab ->1
void grayscale(Image &image)
{

    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            unsigned int gray = 0;
            for (int k = 0; k < image.channels; k++)
            {
                gray += image(i, j, k);
            }
            gray = gray / 3;
            for (int k = 0; k < image.channels; k++)
            {
                image(i, j, k) = gray;
            }
        }
    }
};
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
void flipHorizontal(Image &image)
{
    for (int i = 0; i < image.width / 2; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            for (int k = 0; k < image.channels; k++)
            {
                std::swap(image(i, j, k), image(image.width - 1 - i, j, k));
            }
        }
    }
}
void flipVertical(Image &image)
{
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height / 2; j++)
        {
            for (int k = 0; k < image.channels; k++)
            {
                std::swap(image(i, j, k), image(i, image.height - 1 - j, k));
            }
        }
    }
}
void flipImage(Image& img,char direction) {
    direction = tolower(direction);
    if (direction == 'h')
    {
        flipHorizontal(img);
    }
    else if (direction == 'v')
    {
        flipVertical(img);
    }

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
Image merge(Image &image1, Image &image2)
{
    int nwidth;
    int nheight;

    nwidth = std::min(image1.width, image2.width);
    nheight = std::min(image1.height, image2.height);

    Image image3(nwidth, nheight);
    for (int i = 0; i < image3.width; i++)
    {
        for (int j = 0; j < image3.height; j++)
        {
            for (int k = 0; k < image3.channels; k++)
            {
                image3(i, j, k) = (image1(i, j, k) + image2(i, j, k)) / 2;
            }
        }
    }
    return image3;
};
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
void natural_sunlight(Image &image)
{
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            image(i, j, 0) = std::min(255, image(i, j, 0) + 30);
            image(i, j, 1) = std::min(255, image(i, j, 1) + 20);
            image(i, j, 2) = std::max(0, image(i, j, 2) - 10);
        }
    }
};

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


//Ibrahim Abdulwahab -> 17
void imageSkewing(Image &image, double angle)
{
    double radian = angle * (3.141592653589793 / 180.0);
    double shift = tan(radian) * 0.7;
    int nwidht = image.width + abs(shift * image.height);
    Image nimage(nwidht, image.height);
    for (int i = 0; i < nimage.width; i++)
    {
        for (int j = 0; j < nimage.height; j++)
        {
            for (int k = 0; k < nimage.channels; k++)
            {
                nimage(i, j, k) = 255;
            }
        }
    }
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            int x = i + (image.height - 1 - j) * shift;
            if (x >= 0 && x < nimage.width)
            {
                for (int k = 0; k < image.channels; k++)
                {
                    nimage(x, j, k) = image(i, j, k);
                }
            }
        }
    }
    nimage.saveImage("Skewing1.png");
};
void  oilPainting(Image& img) {

}


#endif //IMAGE_PROCESSING_SYSTEM_FILTERS_H
