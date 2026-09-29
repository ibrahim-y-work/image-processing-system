//
// Created by ibrahim on 9/22/26.
//

#ifndef IMAGE_PROCESSING_SYSTEM_MENU_H
#define IMAGE_PROCESSING_SYSTEM_MENU_H
#include <iostream>
#include <string>
#include <filesystem>
#include <unordered_set>
#include <algorithm>
#include <cctype>
#include "filters.h"

void clearScreen() {
#ifdef _WIN32
    system("cls"); // Windows
#else
    system("clear"); // Linux / macOS
#endif
}

void welcomeStatement() {
    std::cout << std::setw(80) << "========================================\n";
    std::cout << std::setw(77) << "  Welcome to Image Processor     \n";
    std::cout << std::setw(80) << "========================================\n";
}

int getChioce() {
    std::cout << "Enter a choice from 1 to 21:";
    int choice;
    std::cin >> choice;
    while (choice > 21 || choice < 1) {
        std::cout << "Invalid choice, Please enter valid choice (1,21) :";
        std::cin >> choice;
    }
    return choice;
}

bool validExtension(const std::string &path) {
    std::unordered_set<std::string> st = {".jpeg", ".jpg", ".png", ".bmp", ".tga"};

    std::string ext = std::filesystem::path(path).extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    return st.count(ext);
}

Image getImage() {
    std::cout << "Load an image to work on. ";
    std::cout << "\nEnter the image path: ";
    std::string path;

    std::cin >> std::ws;
    std::getline(std::cin, path);

    while (!std::filesystem::exists(path) || !validExtension(path)) {
        std::cout << "\nInvalid image path !, Please enter valid image path:";
        std::cin >> std::ws;
        std::getline(std::cin, path);
    }
    return Image(path);
}

void displayMenu() {
    std::cout << "\n=================== Image Processing Menu ===================\n";
    std::cout << "1.  Grayscale Conversion\n";
    std::cout << "2.  Black and White\n";
    std::cout << "3.  Invert Image\n";
    std::cout << "4.  Add a Frame\n";
    std::cout << "5.  Flip Image (Horizontal / Vertical)\n";
    std::cout << "6.  Rotate Image (90, 180, 270 degrees)\n";
    std::cout << "7.  Darken and Lighten Image\n";
    std::cout << "8.  Resize Image\n";
    std::cout << "9.  Merge Images\n";
    std::cout << "10. Detect Image Edges\n";
    std::cout << "11. Crop Image\n";
    std::cout << "12. Blur Image\n";
    std::cout << "13. Wano Sunlight Adjustment\n";
    std::cout << "14. TV / Den Den Mushi Effect\n";
    std::cout << "15. Purple / Night Effect\n";
    std::cout << "16. Infrared Photography Effect\n";
    std::cout << "17. Skew Image\n";
    std::cout << "18. Oil Painting Effect\n";
    std::cout << "-------------------------------------------------------------\n";
    std::cout << "19. Load a new image\n";
    std::cout << "20. Save the current image\n";
    std::cout << "21. Exit\n";
    std::cout << "=============================================================\n";
}
enum enOptions {
    GrayscaleConversion=1,BlackAndWhite,InvertImage,AddFrame,Flip,Rotate,DarkenAndLighten,
    Resize,Merge,DetectEdges,Crop,Blur,Sunlight,TV,Purple,Infrared,SkewImage,OilPainting,
    LoadNewImg,Save,Exit
};
void  grayscaleConversionExe(Image& img) {
    std::cout << "Executing: Grayscale Conversion\n";
    grayscale(img);
}

void blackAndWhiteExe(Image & img) {
    std::cout << "Executing: Black and White\n";
    blackAndWhite(img);
}

void InvertImageExe(Image & img) {
    std::cout << "Executing: Invert Image\n";
    invertImage(img);
}

void addFrameExe( Image & img) {
    std::cout << "Executing: Add a Frame\n";
    short frame_type;
    short r, g, b;
    std::cout << "Enter frame type (1: Simple, 2: Fancy): ";
    std::cin >> frame_type;
    std::cout << "Enter RGB colors like (255 0 0): ";
    std::cin >> r >> g >> b;
    if (frame_type==1)
        addingASimpleFrameToThePicture(img,r,g,b);
    else if (frame_type==2)
        addingADecoratedFrameToThePicture(img,r,g,b);
}

void flipImageExe( Image & img) {
    std::cout << "Executing: Flip Image (Horizontal / Vertical)\n";
    char direction;
    std::cout << "Enter flip direction ('H' for Horizontal, 'V' for Vertical): ";
    std::cin >> direction;

    while (direction!='V'&&direction!='H') {
        std::cout<<"Invalid Option.\n";
        std::cout<<"Try again ('H' for Horizontal, 'V' for Vertical) :";
        std::cin >> direction;
    }

    if (direction=='H')
        flipHorizontal(img);
    else if (direction=='V')
        flipVertical(img);

}


void rotateExe( Image & img) {
    std::cout << "Executing: Rotate Image (90, 180, 270 degrees)\n";
    short degree;
    std::cout << "Enter rotation degree (90, 180, 270): ";
    std::cin >> degree;
    while (degree!=90&&degree!=180&&degree!=270){
        std::cout << "invalid option.\n";
        std::cout << "Try again (90, 180, 270): ";
        std::cin>>degree;
    }
    rotateImage(img,degree);
}

void darkenAndLightenExe( Image & img) {
    std::cout << "Executing: Darken and Lighten Image\n";
    char mode;
    std::cout << "Enter mode ('D' for Darken, 'L' for Lighten): ";
    std::cin >> mode;
    while (mode!='D'&&mode!='L') {
        std::cout << "Invalid option.";
        std::cout << "Try again ('D' for Darken, 'L' for Lighten): ";
        std::cin>>mode;
    }
    int level;
    std::cout<<"Enter the level of Light or Dark (0-100)% :";
    while (level<0||level>100) {
        std::cout << "Invalid Range";
        std::cout << "Try again (0-100)%: ";
        std::cin>>level;
    }
    darkenAndLightenImage(img,mode=='D',level);

}

void resizeExe(Image & img) {
    std::cout << "Executing: Resize Image\n";
    std::cout << "Would you like to enter a new width and height, or a new resize ratio? choose (1 or 2): ";
    short  parameters_choice;
    std::cin >> parameters_choice;

    while (parameters_choice!=1&&parameters_choice!=2) {
        std::cout << "Invalid Option.";
        std::cout << "Try again (1 or 2): ";
        std::cin>>parameters_choice;
    }
    if (parameters_choice == 1) {
        int new_width, new_height;
        std::cin >> new_width >> new_height;
        resizingImages(img, new_width,new_height);
    } else {
        short ratio;
        std::cin >> ratio;
        resizingImages(img, ratio);
    }

}

void mergeExe(Image & img) {
    std::cout << "Executing: Merge Images\n";

    Image img2=getImage();

    img=merge(img,img2);
}

void detectImageEdgesExe(Image & img) {
    std::cout << "Executing: Detect Image Edges\n";
    detectImageEdges(img);
}


void cropExe(Image & img) {
    std::cout << "Executing: Crop Image\n";
    int x, y, width, height;
    while (true) {
        std::cout << "Current image dimensions: " << img.width << "x" << img.height << "\n";
        std::cout << "Enter X, Y, Width, and Height: ";

        std::cin >> x >> y >> width >> height;

        if (x < 0 || y < 0 || x >= img.width || y >= img.height) {
            std::cout << "Error: The starting point (X, Y) is out of bounds.\n";
            continue;
        }

        if (width <= 0 || height <= 0) {
            std::cout << "Error: Width and Height must be greater than 0.\n";
            continue;
        }

        if (x + width > img.width || y + height > img.height) {
            std::cout << "Error: The crop area exceeds the image dimensions.\n";
            continue;
        }

        break;
    }
    img=cropImage(img,x,y,width,height);

}

void blurExe(Image & img) {
    std::cout << "Executing: Blur Image\n";
    blurImage(img);
}

void sunlightExe( Image & img) {
    std::cout << "Executing: Wano Sunlight Adjustment\n";
    naturalSunlight(img);
}

void TVExe(Image & img) {
    std::cout << "Executing: TV Effect\n";
    TVImages(img);
}

void purpleExe( Image & img) {
    std::cout << "Executing: Purple / Night Effect\n";
    convertToPurple(img);
}


void infraredExe(Image & img) {
    std::cout << "Executing: Infrared Effect\n";
    convertToInfrared(img);
}

void skewImageExe(Image & img) {
    std::cout << "Executing: Skew Image\n";
    double angle;
    std::cout << "Enter skew angle: ";
    std::cin >> angle;
    imageSkewing(img,angle);

}

void oilPaintingExe(Image & img) {
    std::cout << "Executing: Oil Painting Effect\n";
    int radius, levels;
    std::cout << "Enter radius and intensity levels: ";
    std::cin >> radius >> levels;
}

void loadNewImg(Image& img) {
    img=getImage();
}

void saveImg(Image & img) {
    img.saveImage("output.png");
}

void executeChoice(int choice,Image& img) {

    switch (choice) {
        case enOptions::GrayscaleConversion:
            grayscaleConversionExe(img);
            break;
        case enOptions::BlackAndWhite:
            blackAndWhiteExe(img);
            break;
        case enOptions::InvertImage:
            InvertImageExe(img);
            break;
        case enOptions::AddFrame:
            addFrameExe(img);
            break;
        case enOptions::Flip:
            flipImageExe(img);
            break;
        case enOptions::Rotate:
            rotateExe(img);
            break;
        case enOptions::DarkenAndLighten:
            darkenAndLightenExe(img);
            break;

        case enOptions::Resize:
            resizeExe(img);
            break;

        case enOptions::Merge:
            mergeExe(img);
            break;

        case enOptions::DetectEdges:
            detectImageEdgesExe(img);
            break;

        case enOptions::Crop:
            cropExe(img);
            break;

        case enOptions::Blur:
            blurExe(img);
            break;

        case enOptions::Sunlight:
            sunlightExe(img);
            break;

        case enOptions::TV:
            TVExe(img);
            break;
        case enOptions::Purple:
            purpleExe(img);
            break;
        case enOptions::Infrared:
            infraredExe(img);
            break;
        case enOptions::SkewImage:
            skewImageExe(img);
            break;
        case enOptions::OilPainting:
            oilPaintingExe(img);
            break;
        case enOptions::LoadNewImg:
            loadNewImg(img);
            break;
        case enOptions::Save:
            saveImg(img);
            break;
        case enOptions::Exit:
            std::exit(0);
            break;
    }

}


void program() {
    welcomeStatement();
    Image img = getImage();
    while (true) {
        clearScreen();
        displayMenu();
        int current_choice = getChioce();
        executeChoice(current_choice,img);

        std::cout<<"\nPress Enter to continue:";
        std::cin.ignore(10000, '\n');
        std::cin.get();
    }
}
#endif //IMAGE_PROCESSING_SYSTEM_MENU_H
