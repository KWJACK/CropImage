# Introduction
 This application is made for **collecting sample image data** during my intership period in Hyper-connected Communication Research Laboratory, ETRI.

 You can easily crop part of image which you had dragged.
 This serve expected **binarization result** on **left-corner splitted window**

EPRE means ETRI PreProcessing (sorry)
# Screenshoot
![result1](/assets/result1.png)

# Development Enviroment
- Windows 7 64bit,(and Windos 10 no problem)
- **MFC**, Visual Studio 2012 (if you have different IDE, easily migrate upper ver)
- Gdi+ (this library is already included in project directory)
- And this made from https://www.codeproject.com/Articles/5065/Thumbnails-viewer-and-image-processing-using-GDI-a

# Specification
- Image resizing along with application window size
- Directory and file list view
- The cropped area can be moved and resized
- bmp, png, jpg image file read and saved cropped-image as bmp
- SDI(Single Document Interface) and Splitted Windows
- Histogram binarization manner used and 1bpp level's binarization
- **Labeling is also supported**(This is in test branch but not integrated)
- Labeling color is decided as the pixel that you clicked at first
![label](/assets/label.png)
(labeling for privacy information in mail)

# How to use
- Search to the directory containing the files you want to work with
- Select the file (jpg, png, bmp) and drag the area you want to cut.
- Write the file name that you want to save once
  - but I set the naming format so you should name according to rule
  - For example, you cut the character '성', write 'ad0000_성북구_0'
  - Until the last character(in this case '구'), you don't need to re-write the file name
  - **If you want not to yield, remove the OnSaveCropImageFile() in BMPZoomView.cpp**
- Finally, check it the result of binarization.

# Caution
- When you write saved file name, the index can only be **0 to 9.**
- Before adjust this binary result, **you need to check it first that palette setting in my version is same as your enviroment.**
- 1bpp format in bmp is annoying. Although the image what you see is same with different setting but along the palette setting, result bit inside is opposite.
- Previously this program was created with scroll bars also use in mind, **but scrolling was not implemented.**
- Expected results are shown only, not actually saved. So when the operation is done, you should apply the binarization in the menu at once.
