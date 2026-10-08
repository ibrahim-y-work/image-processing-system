CS213 - Assignment 1 - Part 2 (GUI)  |  FCAI, Cairo University  |  Dr. Mohammad El-Ramly
Authors: Toqa Alaa Eldeen (20250136), Ibrahem Abdel-Wahap (20250004),
         Ibrahem Yasser (20250007), Hatem Ashraf (20250173)

FILES
  main.cpp            Header (team, IDs, sections, filters) + program entry point
  MainWindow.h/.cpp   GUI: menu bar, sidebar, preview, dialogs
  ImageBridge.h/.cpp  GUI-side wrapper; only calls the functions of filters.h
  filters.h           Our filters (unchanged)
  Image_Class.h, stb_image.h, stb_image_write.h   Course library (unchanged)
  CMakeLists.txt      Project file
  video_link.txt      Demo video link + shared doc link

MENU (numbered exactly like the filters in filters.h, then file options like the console menu)
   1 Grayscale            2 Black and White       3 Invert Colors        4 Add a Frame
   5 Flip (H/V)           6 Rotate (90/180/270)   7 Darken/Lighten       8 Resize (ratio or pixels)
   9 Merge (2nd image)   10 Detect Edges         11 Crop (x,y,w,h)      12 Blur
  13 Natural Sunlight    14 TV / Scanline        15 Purple / Night      16 Infrared
  17 Skew (angle)        18 Oil Painting (radius, levels)
  19 Load   20 Save (same file / new file)   21 Exit (asks to save in place)
  Extra GUI convenience: Undo (Ctrl+Z, last 10 steps). Shortcuts: Ctrl+O, Ctrl+S, Ctrl+Z.
  Supported files: .png .jpg .jpeg .bmp .tga (lowercase extension, as required by Image_Class.h).
  Note: imageSkewing() in filters.h saves "Skewing1.png" instead of changing the image, so the GUI
  runs it in the temp folder, reads that file back into the image and deletes it.

TOOLS USED  (fill in your exact versions)
  Qt 6.x (Widgets) - MinGW 64-bit build, NOT MSVC   (Qt is not included in this zip)
  CMake 3.18 or newer, GCC/MinGW (C++17), CLion 20xx.x, Windows 10/11

SET UP Qt  (choose ONE option; the compiler and Qt must match)
  Option A - MSYS2 (recommended if CLion uses the MSYS2 toolchain):
     In the "MSYS2 MINGW64" terminal:   pacman -S mingw-w64-x86_64-qt6-base
     CLion: Settings > Build,Execution,Deployment > Toolchains > MinGW, path = C:\msys64\mingw64
  Option B - Qt Online Installer:
     Install Qt 6.x > "MinGW 64-bit" (e.g. 6.8.x) and Developer and Designer Tools > "MinGW 13.1.0 64-bit".
     CLion: Toolchains > MinGW, path = C:\Qt\Tools\mingw1310_64
     CLion: Settings > CMake > CMake options:  -DCMAKE_PREFIX_PATH=C:/Qt/6.x.x/mingw_64
  (CMakeLists.txt also tries C:/msys64/mingw64 and C:/Qt/6.*/mingw_64 automatically.)

BUILD AND RUN
  1. CLion: File > Open > select this folder (it contains CMakeLists.txt).
  2. Settings > CMake: make sure the MinGW toolchain above is selected.
  3. Tools > CMake > Reload CMake Project.  Output must say Qt6 was found.
  4. Build > Build Project, then Run "ImageStudio". (windeployqt runs after build and copies Qt DLLs.)
  Command line: cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH=<Qt path>
                cmake --build build
