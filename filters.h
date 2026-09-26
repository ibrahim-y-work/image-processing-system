//
// Created by ibrahim on 9/22/26.
//

#ifndef IMAGE_PROCESSING_SYSTEM_FILTERS_H
#define IMAGE_PROCESSING_SYSTEM_FILTERS_H
#include <vector>

//ibrahim Abdul-Wahab ->1
void grayscaleConversion(Image& img) {


}
//ibrahim Yasser ->2
void  blackAndWhite(Image& img) {

}
//Toqa ->3
void invertImage(Image &img) {
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            int R = img.getPixel(i, j, 0);
            int G = img.getPixel(i, j, 1);
            int B = img.getPixel(i, j, 2);
            R = 255 - R;
            G = 255 - G;
            B = 255 - B;
            img.setPixel(i, j, 0, R);
            img.setPixel(i, j, 1, G);
            img.setPixel(i, j, 2, B);
        }
    }
}

//Hatem ->4
void  addingAFrameToThePicture(Image& img) {

}

//ibrahim Abdul-Wahab ->5
void flipImage(Image& img) {

}

//ibrahim Yasser ->6
void  rotateImage(Image& img) {

}


//Toqa ->7
void lighten(int &R, int &G, int &B, short levelOfLight) {
    R += R * (levelOfLight / 100.0);
    G += G * (levelOfLight / 100.0);
    B += B * (levelOfLight / 100.0);
    if (R > 255) {
        R = 255;
    }
    if (G > 255) {
        G = 255;
    }
    if (B > 255) {
        B = 255;
    }
}

void darken(int &R, int &G, int &B, short levelOfLight) {
    R -= R * (levelOfLight / 100.0);
    G -= G * (levelOfLight / 100.0);
    B -= B * (levelOfLight / 100.0);
    if (R < 0) {
        R = 0;
    }

    if (G < 0) {
        G = 0;
    }

    if (B < 0) {
        B = 0;
    }
}


void darkenAndLightenImage(Image &img, bool dark, short levelOfLight) {
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            int R = img.getPixel(i, j, 0);
            int G = img.getPixel(i, j, 1);
            int B = img.getPixel(i, j, 2);
            if (dark == 0) {
                lighten(R, G, B, levelOfLight);
            } else {
                darken(R, G, B, levelOfLight);
            }

            img.setPixel(i, j, 0, R);
            img.setPixel(i, j, 1, G);
            img.setPixel(i, j, 2, B);
        }
    }
}
//Hatem ->8
void resizingImages(Image& img) {

}
//Ibrahim Abdul-Wahab ->9
void mergeImages(Image& img) {

}
//ibrahim Yasser -> 10
void detectImageEdges(Image& img) {

}

//Toqa ->11
Image cropImages(Image &img, Image &img2, int x, int y, int w, int h) {
    for (int i = x; i < x + w; i++) {
        for (int j = y; j < y + h; j++) {
            int R = img.getPixel(i, j, 0);
            int G = img.getPixel(i, j, 1);
            int B = img.getPixel(i, j, 2);

            int newx = i - x;
            int newy = j - y;

            img2.setPixel(newx, newy, 0, R);
            img2.setPixel(newx, newy, 1, G);
            img2.setPixel(newx, newy, 2, B);
        }
    }

    return img2;
}


//Hatem ->12
void blurImages(Image& img) {

}
//ibrahim Abdul-Wahab ->13
void  naturalSunlight(Image& img) {

}

//Ibrahim Yasser ->14
void  TVImages(Image& img) {

}

//Toqa->15
void convertToPurple(Image &img) {
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            int R = img.getPixel(i, j, 0);
            int G = img.getPixel(i, j, 1);
            int B = img.getPixel(i, j, 2);

            G -= G * (0.45);
            img.setPixel(i, j, 0, R);
            img.setPixel(i, j, 1, G);
            img.setPixel(i, j, 2, B);
        }
    }
}

//Hatem -> 16
void convertToInfrared(Image& img) {

}


void imageSkewing(Image& img) {

}

//Toqa->18
void oilPainting(Image &img, int radius, int numLevels) {
    int levelSize = 256 / numLevels;
    Image result = img;
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            std::vector<int> v(numLevels, 0);
            std::vector<int> sumR(numLevels, 0);
            std::vector<int> sumG(numLevels, 0);
            std::pmr::vector<int> sumB(numLevels, 0);
            for (int ni = -radius; ni <= radius; ni++) {
                for (int nj = -radius; nj <= radius; nj++) {
                    int x = i + ni;
                    int y = j + nj;
                    if (x >= 0 && x < img.width && y >= 0 && y < img.height) {
                        int R = img.getPixel(x, y, 0);
                        int G = img.getPixel(x, y, 1);
                        int B = img.getPixel(x, y, 2);
                        int intensity = (R + G + B) / 3;
                        int level = intensity / levelSize;
                        if (level >= numLevels) {
                            level = numLevels - 1;
                        }
                        v[level]++;
                        sumR[level] += R;
                        sumG[level] += G;
                        sumB[level] += B;
                    }
                }
            }

            int maxLevel = 0;
            for (int k = 0; k < numLevels; k++) {
                if (v[k] > v[maxLevel]) {
                    maxLevel = k;
                }
            }
            int newR = sumR[maxLevel] / v[maxLevel];
            int newG = sumG[maxLevel] / v[maxLevel];
            int newB = sumB[maxLevel] / v[maxLevel];
            result.setPixel(i, j, 0, newR);
            result.setPixel(i, j, 1, newG);
            result.setPixel(i, j, 2, newB);
        }
    }
    img = result;
}


#endif //IMAGE_PROCESSING_SYSTEM_FILTERS_H
