/*
===============================================================================
                         CS213 - Assignment 1
                              Part 2  (GUI)
===============================================================================
Project:  Image Processing System - GUI version (Qt Widgets)
Done as part of CS213 OOP Programming, FCAI, Cairo University,
under the supervision of Dr. Mohammad El-Ramly.

File description:
    Entry point. Starts the GUI. The menu (the 18 filters numbered 1-18 like filters.h,
    then 19 Load, 20 Save, 21 Exit, plus Undo) is built in MainWindow.cpp.
    GUI calls the original filters through ImageBridge.cpp, which includes
    filters.h unchanged.

---------------------------------- Team Details --------------------------------
1. Toqa Alaa Eldeen    | ID: 20250136 | Section: S31, S32 | Monday 09:30   | Table B
   Filters: invertImage (3), darkenAndLightenImage (7), cropImages (11),
            convertToPurple (15), oilPainting (18)

2. Ibrahim Abdel-Wahab | ID: 20250004 | Section: S31, S32 | Monday 09:30   | Table B
   Filters: grayscale (1), flipImage (5), merge (9), natural_sunlight (13), imageSkewing (17)

3. Ibrahim Yasser      | ID: 20250007 | Section: S43, S44   | Thursday 08:00 | Table A
   Filters: blackAndWhite (2), rotateImage (6), detectImageEdges (10), TVImages (14)

4. Hatem Ashraf        | ID: 20250173 | Section: S7, S8   | Thursday 08:00 | Table A
   Filters: addingADecoratedFrameToThePicture (4), resizingImages (8), blurImages (12),
            convertToInfrared (16)
===============================================================================
*/
#include <QApplication>
#include "MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setStyle("Fusion");
    MainWindow w;
    w.show();
    return app.exec();
}
