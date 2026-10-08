#include "MainWindow.h"

#include <QApplication>
#include <QCloseEvent>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QSlider>
#include <QSpinBox>
#include <QStatusBar>
#include <QToolButton>
#include <QVBoxLayout>
#include <cmath>

static const char* kImageFilter = "Images (*.png *.jpg *.jpeg *.bmp *.tga)";

static const char* kStyle = R"(
QMainWindow, QDialog { background:#1e2127; }
QWidget { color:#e6e8eb; font-size:13px; }
QWidget#side { background:#252a31; }
QLabel#title { font-size:18px; font-weight:600; padding:6px 4px 10px 4px; }
QLabel#preview { background:#15181c; border-radius:10px; color:#8a919a; font-size:15px; }
QScrollArea { background:transparent; border:none; }
QScrollArea > QWidget > QWidget { background:transparent; }
QScrollBar:vertical { width:8px; background:transparent; }
QScrollBar::handle:vertical { background:#3b4350; border-radius:4px; min-height:30px; }
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height:0; }
QToolButton { background:#2f353d; border:none; border-radius:8px; padding:7px 12px; text-align:left; }
QToolButton:hover { background:#3b4350; }
QToolButton:disabled { color:#6b727c; background:#2a2f36; }
QToolButton#primary { background:#3d7eff; font-weight:600; }
QToolButton#primary:hover { background:#5a92ff; }
QToolButton#primary:disabled { background:#2a3a5c; color:#7d8aa5; }
QPushButton { background:#3d7eff; border:none; border-radius:6px; padding:6px 14px; }
QPushButton:hover { background:#5a92ff; }
QComboBox, QSpinBox, QDoubleSpinBox { background:#2f353d; border:1px solid #3b4350; border-radius:6px; padding:4px 8px; }
QMenuBar, QMenu { background:#252a31; }
QMenu::item:selected, QMenuBar::item:selected { background:#3d7eff; }
QStatusBar { background:#252a31; color:#9aa1ab; }
)";

namespace {
void addOkCancel(QDialog& dlg, QFormLayout& form) {
    auto* bb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    QObject::connect(bb, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(bb, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
    form.addRow(bb);
}
const int kMaxSide = 20000;   // safety limit for resize / skew results
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setStyleSheet(kStyle);
    resize(1100, 720);
    buildUi();
    refresh();
}

QAction* MainWindow::addFilter(const QString& name, const std::function<void()>& handler) {
    const int n = int(filterActions_.size()) + 1;
    auto* a = new QAction(QString("%1.  %2").arg(n).arg(name), this);
    connect(a, &QAction::triggered, this, [this, handler] { if (doc_.hasImage()) handler(); });
    filterActions_.push_back(a);
    return a;
}

void MainWindow::buildUi() {
    // ---------- filters 1..18 (same numbering/order as filters.h) ----------
    auto simple = [this](const QString& name, void (ImageDocument::*op)()) {
        addFilter(name, [this, name, op] { runFilter(name, [this, op] { (doc_.*op)(); }); });
    };
    simple("Grayscale",             &ImageDocument::toGrayscale);        // 1
    simple("Black and White",       &ImageDocument::toBlackAndWhite);    // 2
    simple("Invert Colors",         &ImageDocument::invert);             // 3
    simple("Add a Frame",           &ImageDocument::addFrame);           // 4
    addFilter("Flip Image",                 [this] { onFlip(); });       // 5
    addFilter("Rotate Image",               [this] { onRotate(); });     // 6
    addFilter("Darken and Lighten Image",   [this] { onDarkenLighten(); }); // 7
    addFilter("Resize Image",               [this] { onResize(); });     // 8
    addFilter("Merge Images",               [this] { onMerge(); });      // 9
    simple("Detect Image Edges",    &ImageDocument::detectEdges);        // 10
    addFilter("Crop Image",                 [this] { onCrop(); });       // 11
    simple("Blur Image",            &ImageDocument::blur);               // 12
    simple("Natural Sunlight",      &ImageDocument::naturalSunlight);    // 13
    simple("TV / Scanline Effect",  &ImageDocument::tvEffect);           // 14
    simple("Purple / Night Effect", &ImageDocument::toPurple);           // 15
    simple("Infrared Effect",       &ImageDocument::toInfrared);         // 16
    addFilter("Skew Image",                 [this] { onSkew(); });       // 17
    addFilter("Oil Painting",               [this] { onOilPainting(); });// 18

    // ---------- file actions (19-21 like the console menu) ----------
    auto* load = new QAction("19.  Load a new image", this);
    load->setShortcut(QKeySequence::Open);
    connect(load, &QAction::triggered, this, &MainWindow::loadImage);
    saveAct_ = new QAction("20.  Save the current image", this);
    saveAct_->setShortcut(QKeySequence::Save);
    connect(saveAct_, &QAction::triggered, this, &MainWindow::saveImage);
    auto* exitAct = new QAction("21.  Exit", this);
    exitAct->setShortcut(QKeySequence::Quit);
    connect(exitAct, &QAction::triggered, this, &QWidget::close);
    undoAct_ = new QAction("Undo", this);
    undoAct_->setShortcut(QKeySequence::Undo);
    connect(undoAct_, &QAction::triggered, this, &MainWindow::undo);

    editActions_ = filterActions_;
    editActions_.push_back(saveAct_);

    // ---------- menu bar ----------
    QMenu* file = menuBar()->addMenu("&File");
    file->addAction(load);
    file->addAction(saveAct_);
    file->addAction(undoAct_);
    file->addSeparator();
    file->addAction(exitAct);
    QMenu* fm = menuBar()->addMenu("F&ilters");
    for (QAction* a : filterActions_) fm->addAction(a);

    // ---------- sidebar ----------
    auto* side = new QWidget;
    side->setObjectName("side");
    side->setFixedWidth(270);
    auto* sl = new QVBoxLayout(side);
    sl->setSpacing(6);

    auto makeBtn = [](QAction* a, bool primary) {
        auto* b = new QToolButton;
        b->setDefaultAction(a);
        b->setToolButtonStyle(Qt::ToolButtonTextOnly);
        b->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        if (primary) b->setObjectName("primary");
        return b;
    };

    auto* title = new QLabel("Image Studio");
    title->setObjectName("title");
    sl->addWidget(title);
    sl->addWidget(makeBtn(load, true));

    auto* inner = new QWidget;
    auto* il = new QVBoxLayout(inner);
    il->setContentsMargins(0, 0, 6, 0);
    il->setSpacing(6);
    for (QAction* a : filterActions_) il->addWidget(makeBtn(a, false));
    il->addStretch();
    auto* scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setWidget(inner);
    sl->addWidget(scroll, 1);

    sl->addWidget(makeBtn(undoAct_, false));
    sl->addWidget(makeBtn(saveAct_, true));
    sl->addWidget(makeBtn(exitAct, false));

    // ---------- preview ----------
    preview_ = new QLabel;
    preview_->setObjectName("preview");
    preview_->setAlignment(Qt::AlignCenter);
    preview_->setMinimumSize(1, 1);
    preview_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    auto* central = new QWidget;
    auto* cl = new QHBoxLayout(central);
    cl->setContentsMargins(0, 0, 0, 0);
    cl->setSpacing(0);
    cl->addWidget(side);
    auto* right = new QVBoxLayout;
    right->setContentsMargins(14, 14, 14, 14);
    right->addWidget(preview_);
    cl->addLayout(right, 1);
    setCentralWidget(central);
}

void MainWindow::refresh() {
    const bool has = doc_.hasImage();
    for (QAction* a : editActions_) a->setEnabled(has);
    undoAct_->setEnabled(doc_.canUndo());

    if (has) {
        QImage img(doc_.rgb(), doc_.width(), doc_.height(), doc_.width() * 3, QImage::Format_RGB888);
        pix_ = QPixmap::fromImage(img);   // copies the pixels
        setWindowTitle(QString("Image Studio - %1%2")
                           .arg(QFileInfo(QString::fromStdString(doc_.path())).fileName(),
                                doc_.modified() ? " *" : ""));
        statusBar()->showMessage(QString("%1 x %2 px").arg(doc_.width()).arg(doc_.height()));
    } else {
        pix_ = QPixmap();
        setWindowTitle("Image Studio");
    }
    updatePreview();
}

void MainWindow::updatePreview() {
    if (pix_.isNull()) {
        preview_->setPixmap(QPixmap());
        preview_->setText("Load an image to start  (Ctrl+O)");
        return;
    }
    preview_->setText(QString());
    preview_->setPixmap(pix_.scaled(preview_->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::resizeEvent(QResizeEvent* e) {
    QMainWindow::resizeEvent(e);
    updatePreview();
}

void MainWindow::runFilter(const QString& name, const std::function<void()>& f) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    bool ok = true;
    try { f(); } catch (...) { ok = false; }
    QApplication::restoreOverrideCursor();
    if (!ok) { QMessageBox::warning(this, name, "The operation failed. The image was left unchanged."); return; }
    refresh();
    statusBar()->showMessage(name + " applied", 4000);
}

// ------------------------------------------------------------ file operations
void MainWindow::loadImage() {
    QString p = QFileDialog::getOpenFileName(this, "Load an image", QString(), kImageFilter);
    if (p.isEmpty()) return;
    if (!doc_.load(p.toStdString())) {
        QMessageBox::warning(this, "Load",
            "Could not open this image.\nSupported: .png .jpg .jpeg .bmp .tga (lowercase extension).");
        return;
    }
    refresh();
}

void MainWindow::saveImage() {
    if (!doc_.hasImage()) return;
    QMessageBox box(this);
    box.setWindowTitle("Save");
    box.setText("Do you want to save changes on the same file?");
    auto* same  = box.addButton("Yes, same file", QMessageBox::YesRole);
    auto* other = box.addButton("No, save as new file", QMessageBox::NoRole);
    box.addButton(QMessageBox::Cancel);
    box.exec();

    QString path;
    if (box.clickedButton() == same) {
        path = QString::fromStdString(doc_.path());
    } else if (box.clickedButton() == other) {
        path = QFileDialog::getSaveFileName(this, "Save image as", QString(), kImageFilter);
        if (path.isEmpty()) return;
        if (QFileInfo(path).suffix().isEmpty()) path += ".png";
    } else {
        return;
    }
    if (!doc_.save(path.toStdString())) {
        QMessageBox::warning(this, "Save", "Invalid image path or the file could not be saved.");
        return;
    }
    refresh();
    statusBar()->showMessage("Saved to " + path, 4000);
}

void MainWindow::undo() {
    doc_.undo();
    refresh();
}

void MainWindow::closeEvent(QCloseEvent* e) {
    if (doc_.hasImage() && doc_.modified()) {
        auto r = QMessageBox::question(this, "Exit", "Do you want to save in place before exit?",
                                       QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (r == QMessageBox::Cancel) { e->ignore(); return; }
        if (r == QMessageBox::Yes && !doc_.save(doc_.path())) {
            QMessageBox::warning(this, "Save", "Could not save the file.");
            e->ignore();
            return;
        }
    }
    e->accept();
}

// ------------------------------------------------------------ filters with options
void MainWindow::onFlip() {
    bool ok = false;
    QString s = QInputDialog::getItem(this, "Flip Image", "Direction:", {"Horizontal", "Vertical"}, 0, false, &ok);
    if (!ok) return;
    const char d = (s == "Horizontal") ? 'h' : 'v';
    runFilter("Flip Image", [&] { doc_.flip(d); });
}

void MainWindow::onRotate() {
    bool ok = false;
    QString s = QInputDialog::getItem(this, "Rotate Image", "Degrees:", {"90", "180", "270"}, 0, false, &ok);
    if (!ok) return;
    const int deg = s.toInt();
    runFilter("Rotate Image", [&] { doc_.rotate(deg); });
}

void MainWindow::onDarkenLighten() {
    QDialog dlg(this);
    dlg.setWindowTitle("Darken and Lighten Image");
    QFormLayout form(&dlg);
    auto* mode = new QComboBox(&dlg);
    mode->addItems({"Lighten", "Darken"});
    auto* slider = new QSlider(Qt::Horizontal, &dlg);
    slider->setRange(0, 100);
    slider->setValue(30);
    auto* value = new QLabel("30 %", &dlg);
    QObject::connect(slider, &QSlider::valueChanged, value, [value](int v) { value->setText(QString("%1 %").arg(v)); });
    auto* row = new QHBoxLayout;
    row->addWidget(slider, 1);
    row->addWidget(value);
    form.addRow("Mode:", mode);
    form.addRow("Level:", row);
    addOkCancel(dlg, form);
    if (dlg.exec() != QDialog::Accepted) return;
    const bool darken = mode->currentIndex() == 1;
    const int level = slider->value();
    runFilter("Darken and Lighten", [&] { doc_.darkenLighten(darken, level); });
}

void MainWindow::onResize() {
    const int W = doc_.width(), H = doc_.height();
    QDialog dlg(this);
    dlg.setWindowTitle("Resize Image");
    QFormLayout form(&dlg);
    form.addRow(new QLabel(QString("Current size: %1 x %2 px").arg(W).arg(H), &dlg));
    auto* mode = new QComboBox(&dlg);
    mode->addItems({"By ratio (%)", "By size (pixels)"});
    auto* ratio = new QSpinBox(&dlg);
    ratio->setRange(1, 300);
    ratio->setValue(50);
    ratio->setSuffix(" %");
    auto* wBox = new QSpinBox(&dlg);
    wBox->setRange(1, kMaxSide);
    wBox->setValue(W);
    auto* hBox = new QSpinBox(&dlg);
    hBox->setRange(1, kMaxSide);
    hBox->setValue(H);
    auto sync = [=] {
        const bool r = mode->currentIndex() == 0;
        ratio->setEnabled(r);
        wBox->setEnabled(!r);
        hBox->setEnabled(!r);
    };
    QObject::connect(mode, &QComboBox::currentIndexChanged, &dlg, sync);
    sync();
    form.addRow("Mode:", mode);
    form.addRow("Ratio:", ratio);
    form.addRow("New width:", wBox);
    form.addRow("New height:", hBox);
    addOkCancel(dlg, form);
    if (dlg.exec() != QDialog::Accepted) return;

    if (mode->currentIndex() == 0) {
        const double r = ratio->value();
        const long long nw = (long long)(r / 100.0 * W), nh = (long long)(r / 100.0 * H);
        if (nw < 1 || nh < 1 || nw > kMaxSide || nh > kMaxSide) {
            QMessageBox::warning(this, "Resize Image", "That ratio gives an invalid image size.");
            return;
        }
        runFilter("Resize Image", [&] { doc_.resizeByRatio(r); });
    } else {
        const int nw = wBox->value(), nh = hBox->value();
        runFilter("Resize Image", [&] { doc_.resizeTo(nw, nh); });
    }
}

void MainWindow::onMerge() {
    QString p = QFileDialog::getOpenFileName(this, "Choose the second image to merge", QString(), kImageFilter);
    if (p.isEmpty()) return;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    const bool ok = doc_.mergeWith(p.toStdString());
    QApplication::restoreOverrideCursor();
    if (!ok) { QMessageBox::warning(this, "Merge Images", "Could not load or merge that image."); return; }
    refresh();
    statusBar()->showMessage("Merge Images applied", 4000);
}

void MainWindow::onCrop() {
    const int W = doc_.width(), H = doc_.height();
    QDialog dlg(this);
    dlg.setWindowTitle("Crop Image");
    QFormLayout form(&dlg);
    form.addRow(new QLabel(QString("Image size: %1 x %2 px").arg(W).arg(H), &dlg));
    auto* x = new QSpinBox(&dlg);  x->setRange(0, W - 1);
    auto* y = new QSpinBox(&dlg);  y->setRange(0, H - 1);
    auto* w = new QSpinBox(&dlg);  w->setRange(1, W);  w->setValue(W);
    auto* h = new QSpinBox(&dlg);  h->setRange(1, H);  h->setValue(H);
    QObject::connect(x, &QSpinBox::valueChanged, w, [=](int v) { w->setMaximum(W - v); });
    QObject::connect(y, &QSpinBox::valueChanged, h, [=](int v) { h->setMaximum(H - v); });
    form.addRow("X (left):", x);
    form.addRow("Y (top):", y);
    form.addRow("Width:", w);
    form.addRow("Height:", h);
    addOkCancel(dlg, form);
    if (dlg.exec() != QDialog::Accepted) return;

    const int cx = x->value(), cy = y->value(), cw = w->value(), ch = h->value();
    QApplication::setOverrideCursor(Qt::WaitCursor);
    bool ok = false;
    try { ok = doc_.crop(cx, cy, cw, ch); } catch (...) {}
    QApplication::restoreOverrideCursor();
    if (!ok) { QMessageBox::warning(this, "Crop Image", "Invalid crop area."); return; }
    refresh();
    statusBar()->showMessage("Crop Image applied", 4000);
}

void MainWindow::onSkew() {
    bool ok = false;
    const double angle = QInputDialog::getDouble(this, "Skew Image", "Angle (degrees, 1 - 75):",
                                                 30.0, 1.0, 75.0, 1, &ok);
    if (!ok) return;
    const double shift = std::tan(angle * 3.141592653589793 / 180.0) * 0.7;
    if (doc_.width() + shift * doc_.height() > kMaxSide) {
        QMessageBox::warning(this, "Skew Image", "The result would be too large. Use a smaller angle.");
        return;
    }
    runFilter("Skew Image", [&] { doc_.skew(angle); });
}

void MainWindow::onOilPainting() {
    QDialog dlg(this);
    dlg.setWindowTitle("Oil Painting");
    QFormLayout form(&dlg);
    auto* radius = new QSpinBox(&dlg);
    radius->setRange(1, 10);
    radius->setValue(3);
    auto* levels = new QSpinBox(&dlg);
    levels->setRange(2, 64);
    levels->setValue(20);
    form.addRow("Radius:", radius);
    form.addRow("Intensity levels:", levels);
    form.addRow(new QLabel("Large images and big radius can take a while.", &dlg));
    addOkCancel(dlg, form);
    if (dlg.exec() != QDialog::Accepted) return;
    const int r = radius->value(), l = levels->value();
    runFilter("Oil Painting", [&] { doc_.oilPaint(r, l); });
}
