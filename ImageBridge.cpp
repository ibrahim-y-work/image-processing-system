// ImageBridge.cpp
// Qt-free wrapper. Every method only CALLS the original function from filters.h (included unchanged).
// Keep filters.h included in exactly one .cpp file - it defines functions (no inline).
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <functional>
#include <iostream>
#include <memory_resource>   // filters.h uses std::pmr::vector in oilPainting()
#include <string>
#include <vector>

// The course stb headers only contain their code when these macros are defined, and exactly
// ONE .cpp file must define them. This is that file (Image_Class.h / stb headers are untouched).
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "filters.h"          // also pulls in Image_Class.h
#include "ImageBridge.h"

namespace { const size_t kMaxHistory = 10; }

ImageDocument::ImageDocument() = default;
ImageDocument::~ImageDocument() = default;

bool ImageDocument::load(const std::string& p) {
    if (!std::filesystem::exists(p)) return false;
    try {                                  // Image(path) throws for bad/unsupported files
        auto im = std::make_unique<Image>(p);
        cur_ = std::move(im);
        history_.clear();
        path_ = p;
        modified_ = false;
        return true;
    } catch (...) { return false; }
}

bool ImageDocument::save(const std::string& p) {
    if (!cur_) return false;
    try { cur_->saveImage(p); modified_ = false; return true; }
    catch (...) { return false; }
}

int ImageDocument::width() const { return cur_ ? cur_->width : 0; }
int ImageDocument::height() const { return cur_ ? cur_->height : 0; }
const unsigned char* ImageDocument::rgb() const { return cur_ ? cur_->imageData : nullptr; }

void ImageDocument::apply(const std::function<void(Image&)>& f) {
    if (!cur_) return;
    history_.push_back(std::make_unique<Image>(*cur_));      // snapshot for Undo
    try { f(*cur_); }
    catch (...) { cur_ = std::move(history_.back()); history_.pop_back(); throw; }
    if (history_.size() > kMaxHistory) history_.erase(history_.begin());
    modified_ = true;
}

void ImageDocument::undo() {
    if (history_.empty()) return;
    cur_ = std::move(history_.back());
    history_.pop_back();
    modified_ = true;
}

// ---- one line each, straight calls into filters.h ----
void ImageDocument::toGrayscale()     { apply([](Image& i) { ::grayscale(i); }); }
void ImageDocument::toBlackAndWhite() { apply([](Image& i) { ::blackAndWhite(i); }); }
void ImageDocument::invert()          { apply([](Image& i) { ::invertImage(i); }); }
void ImageDocument::addFrame()        { apply([](Image& i) { ::addingADecoratedFrameToThePicture(i); }); }
void ImageDocument::flip(char d)      { apply([=](Image& i) { ::flipImage(i, d); }); }
void ImageDocument::rotate(int deg)   { apply([=](Image& i) { ::rotateImage(i, (short)deg); }); }
void ImageDocument::darkenLighten(bool darken, int level) {
    apply([=](Image& i) { ::darkenAndLightenImage(i, darken, (short)level); });
}
void ImageDocument::resizeByRatio(double percent) {
    apply([=](Image& i) { ::resizingImages(i, percent); });
}
void ImageDocument::resizeTo(int w, int h) {
    apply([=](Image& i) { ::resizingImages(i, h, w); });   // original order: (img, newHeight, newWidth)
}
bool ImageDocument::mergeWith(const std::string& p) {
    if (!cur_ || !std::filesystem::exists(p)) return false;
    try {
        Image other(p);
        apply([&](Image& i) { Image m = ::merge(i, other); i = m; });
        return true;
    } catch (...) { return false; }
}
void ImageDocument::detectEdges()     { apply([](Image& i) { ::detectImageEdges(i); }); }
bool ImageDocument::crop(int x, int y, int w, int h) {
    if (!cur_ || x < 0 || y < 0 || w < 1 || h < 1 || x + w > cur_->width || y + h > cur_->height)
        return false;                                       // keeps the filter inside the image
    apply([=](Image& i) {
        Image result(w, h);                                 // cropImages writes into this image
        ::cropImages(i, result, x, y, w, h);
        i = result;
    });
    return true;
}
void ImageDocument::blur()            { apply([](Image& i) { ::blurImages(i); }); }
void ImageDocument::naturalSunlight() { apply([](Image& i) { ::natural_sunlight(i); }); }
void ImageDocument::tvEffect()        { apply([](Image& i) { ::TVImages(i); }); }
void ImageDocument::toPurple()        { apply([](Image& i) { ::convertToPurple(i); }); }
void ImageDocument::toInfrared()      { apply([](Image& i) { ::convertToInfrared(i); }); }

void ImageDocument::skew(double angle) {
    // imageSkewing() does not modify the image: it saves the result to "Skewing1.png" in the current
    // folder. So: run it inside the temp folder, read that file back into our image, delete it.
    apply([=](Image& i) {
        namespace fs = std::filesystem;
        const fs::path old = fs::current_path();
        fs::current_path(fs::temp_directory_path());
        try {
            ::imageSkewing(i, angle);
            Image r(std::string("Skewing1.png"));
            i = r;
            fs::remove("Skewing1.png");
        } catch (...) { fs::current_path(old); throw; }
        fs::current_path(old);
    });
}

void ImageDocument::oilPaint(int radius, int levels) {
    apply([=](Image& i) { ::oilPainting(i, radius, levels); });
}
