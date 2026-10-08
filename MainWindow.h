#pragma once
#include <QMainWindow>
#include <QPixmap>
#include <functional>
#include <vector>
#include "ImageBridge.h"

class QAction;
class QLabel;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

protected:
    void closeEvent(QCloseEvent* e) override;
    void resizeEvent(QResizeEvent* e) override;

private slots:
    void loadImage();
    void saveImage();
    void undo();

private:
    void buildUi();
    void refresh();
    void updatePreview();
    void runFilter(const QString& name, const std::function<void()>& f);
    QAction* addFilter(const QString& name, const std::function<void()>& handler);

    // filters that need input from the user
    void onFlip();
    void onRotate();
    void onDarkenLighten();
    void onResize();
    void onMerge();
    void onCrop();
    void onSkew();
    void onOilPainting();

    ImageDocument doc_;
    QPixmap pix_;
    QLabel* preview_ = nullptr;
    QAction* undoAct_ = nullptr;
    QAction* saveAct_ = nullptr;
    std::vector<QAction*> filterActions_;
    std::vector<QAction*> editActions_;
};
