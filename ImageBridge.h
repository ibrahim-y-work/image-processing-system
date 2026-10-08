// ImageBridge.h  -  Qt-free GUI-side wrapper around the team's existing filters (filters.h).
// It never re-implements a filter: every method just calls the original function.
#pragma once
#include <functional>
#include <memory>
#include <string>
#include <vector>

class Image;  // defined in Image_Class.h (only included in ImageBridge.cpp)

class ImageDocument {
public:
    ImageDocument();
    ~ImageDocument();

    bool load(const std::string& path);
    bool save(const std::string& path);

    bool hasImage() const { return cur_ != nullptr; }
    int width() const;
    int height() const;
    const unsigned char* rgb() const;          // packed RGB, width*height*3
    const std::string& path() const { return path_; }
    bool modified() const { return modified_; }

    bool canUndo() const { return !history_.empty(); }
    void undo();

    // ---- one wrapper per filter in filters.h (numbered like the file: 1..18) ----
    void toGrayscale();                                    //  1 grayscale
    void toBlackAndWhite();                                //  2 blackAndWhite
    void invert();                                         //  3 invertImage
    void addFrame();                                       //  4 addingADecoratedFrameToThePicture
    void flip(char direction);                             //  5 flipImage       'h' / 'v'
    void rotate(int degree);                               //  6 rotateImage     90 / 180 / 270
    void darkenLighten(bool darken, int level);            //  7 darkenAndLightenImage (0-100)
    void resizeByRatio(double percent);                    //  8 resizingImages(img, ratio)
    void resizeTo(int newWidth, int newHeight);            //  8 resizingImages(img, newHeight, newWidth)
    bool mergeWith(const std::string& otherPath);          //  9 merge
    void detectEdges();                                    // 10 detectImageEdges
    bool crop(int x, int y, int w, int h);                 // 11 cropImages
    void blur();                                           // 12 blurImages
    void naturalSunlight();                                // 13 natural_sunlight
    void tvEffect();                                       // 14 TVImages
    void toPurple();                                       // 15 convertToPurple
    void toInfrared();                                     // 16 convertToInfrared
    void skew(double angleDegrees);                        // 17 imageSkewing
    void oilPaint(int radius, int levels);                 // 18 oilPainting

private:
    void apply(const std::function<void(Image&)>& f);

    std::unique_ptr<Image> cur_;
    std::vector<std::unique_ptr<Image>> history_;
    std::string path_;
    bool modified_ = false;
};
