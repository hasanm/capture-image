#include <QtWidgets>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QIcon>
#include "mainwindow.h"
#include "Windows.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


using namespace cv;

// Crosslink between Qt class and win callback
MainWindow *mwReference;


BITMAPINFOHEADER createBitmapHeader(int width, int height)
{
    BITMAPINFOHEADER  bi;
    // create a bitmap
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = -height;  //this is the line that makes it draw upside down or not
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    return bi;
}


Mat captureScreenMat(HWND hwnd)
{
     Mat src;

     // get handles to a device context (DC)
     HDC hwindowDC = GetDC(hwnd);
     HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
     SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

     // define scale, height and width
     int screenx = GetSystemMetrics(SM_XVIRTUALSCREEN);
     int screeny = GetSystemMetrics(SM_YVIRTUALSCREEN);
     int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
     int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);

     // create mat object
     src.create(height, width, CV_8UC4);

     // create a bitmap
     HBITMAP hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
     BITMAPINFOHEADER bi = createBitmapHeader(width, height);

     // use the previously created device context with the bitmap
     SelectObject(hwindowCompatibleDC, hbwindow);

     // copy from the window device context to the bitmap device context
     StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, screenx, screeny, width, height, SRCCOPY);  //change SRCCOPY to NOTSRCCOPY for wacky colors !
     GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);            //copy from hwindowCompatibleDC to hbwindow

     // avoid memory leak
     DeleteObject(hbwindow);
     DeleteDC(hwindowCompatibleDC);
     ReleaseDC(hwnd, hwindowDC);
     return src;
 }



MainWindow::~MainWindow(){
}

MainWindow::MainWindow() :
  pixmap(nullptr)
{
  mwReference = this;

  QWidget *root = new QWidget(this);
  QWidget *top = new QWidget(this);
  QWidget *content = new QWidget(this);

  /* Top Layout */
  QHBoxLayout *topLayout = new QHBoxLayout(top);

  quitButton = new QPushButton(QString ("Quit"), this);
  connect(quitButton, &QPushButton::clicked, this, &MainWindow::onQuit);
  topLayout->addWidget(quitButton);

  captureButton = new QPushButton(QString("Capture"), this);
  connect(captureButton, &QPushButton::clicked, this, &MainWindow::onCapture);
  topLayout->addWidget(captureButton);

  captureCheckBox = new QCheckBox(QString("Auto Capture"), this);
  topLayout->addWidget(captureCheckBox);

  spinLabel = new QLabel(QString("Interval"),this);
  topLayout->addWidget(spinLabel);

  intervalBox = new QSpinBox(this);
  intervalBox->setMinimum(1);
  intervalBox->setValue(1);
  connect(intervalBox, &QSpinBox::valueChanged, this,&MainWindow::onSpinValueChanged);

  topLayout->addWidget(intervalBox);

  top->setLayout(topLayout);

  /* Content Layout */
  contentLayout = new QVBoxLayout(content);
  scene = new QGraphicsScene(this);
  view = new QGraphicsView(this);
  view->setScene(scene);
  view->scale(2,2);
  contentLayout->addWidget(view);

  /* Root Layout */
  QVBoxLayout *rootLayout = new QVBoxLayout(root);


  rootLayout->addWidget(top);
  rootLayout->addWidget(content);
  setCentralWidget(root);

  timer = new QTimer(this);
  connect(timer,&QTimer::timeout , this, &MainWindow::onTimer);
  timer->start(2000);
}

void MainWindow::onQuit() {
  qApp->quit();
}

void MainWindow::load(QString fileName) {
  Mat dest;
  Mat mat;
  if (QFile::exists(fileName)){
    mat = imread(fileName.toStdString().c_str(), IMREAD_COLOR);
    cvtColor(mat, dest, COLOR_BGR2RGB);
    const QImage newImage((uchar*) dest.data, dest.cols, dest.rows, dest.step, QImage::Format_RGB888);
    QPixmap pix = QPixmap::fromImage(newImage);
    if (pixmap != nullptr) {
      scene->removeItem(pixmap);
      delete pixmap;
    }
    pixmap = scene->addPixmap(pix);
  }
}


void MainWindow::setWindowSizeLocation() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect rec = screen->availableGeometry();
    qDebug() << "Hello " << rec.width() << " x " << rec.height();

    // int targetWidth = this->width();
    int targetWidth = 800;

    int height = 680;
    int width = rec.width();
    // int x=(width - targetWidth);
    int x = 300;
    int y= 50;
    this->setGeometry(x,y,targetWidth,height);
}

void MainWindow::capture()
{
    int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
    HWND hDesktopWnd = GetDesktopWindow();
    HDC hDesktopDC = GetDC(hDesktopWnd);
    HDC hCaptureDC = CreateCompatibleDC(hDesktopDC);
    HBITMAP hCaptureBitmap =CreateCompatibleBitmap(hDesktopDC,
                            nScreenWidth, nScreenHeight);
    SelectObject(hCaptureDC,hCaptureBitmap);
    BitBlt(hCaptureDC,0,0,nScreenWidth,nScreenHeight,
           hDesktopDC,0,0,SRCCOPY|CAPTUREBLT);
    // SaveCapturedBitmap(hCaptureBitmap); //Place holder - Put your code
    SaveHBITMAPToFile(hCaptureBitmap, L"C:\\tmp\\out.bmp");
    //here to save the captured image to disk
    ReleaseDC(hDesktopWnd,hDesktopDC);
    DeleteDC(hCaptureDC);
    DeleteObject(hCaptureBitmap);
}

void MainWindow::onCapture(){
  capture();
  convert(QString("C:\\tmp\\out.bmp"));

  // https://superkogito.github.io/blog/2020/07/25/capture_screen_using_opencv.html
  // HWND hwnd = GetDesktopWindow();
  // Mat src = captureScreenMat(hwnd);
  //
  // std::vector<uchar> buf;
  // imencode(".png", src, buf);
  // imwrite("C:\\tmp\\another.png", src);
  // buf.clear();
}

void MainWindow::convert(QString fileName) {
  Mat img;
  if (QFile::exists(fileName)) {
      img = imread(fileName.toStdString().c_str(), IMREAD_COLOR);
      imwrite("C:\\tmp\\out.png",img);

      Mat minimap = img(Rect(Point(1250,1200), Point(1750,1425)));
      imwrite("C:\\tmp\\minimap.png", minimap);
      load(QString("C:\\tmp\\minimap.png"));
    }
}


BOOL MainWindow::SaveHBITMAPToFile(HBITMAP hBitmap, LPCTSTR lpszFileName)
{
    HDC hDC;
    int iBits;
    WORD wBitCount;
    DWORD dwPaletteSize = 0, dwBmBitsSize = 0, dwDIBSize = 0, dwWritten = 0;
    BITMAP Bitmap0;
    BITMAPFILEHEADER bmfHdr;
    BITMAPINFOHEADER bi;
    LPBITMAPINFOHEADER lpbi;
    HANDLE fh, hDib, hPal, hOldPal2 = NULL;
    hDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
    iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
    DeleteDC(hDC);
    if (iBits <= 1)
        wBitCount = 1;
    else if (iBits <= 4)
        wBitCount = 4;
    else if (iBits <= 8)
        wBitCount = 8;
    else
        wBitCount = 24;
    GetObject(hBitmap, sizeof(Bitmap0), (LPSTR)&Bitmap0);
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = Bitmap0.bmWidth;
    bi.biHeight = -Bitmap0.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = wBitCount;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrImportant = 0;
    bi.biClrUsed = 256;
    dwBmBitsSize = ((Bitmap0.bmWidth * wBitCount + 31) & ~31) / 8
        * Bitmap0.bmHeight;
    hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
    *lpbi = bi;

    hPal = GetStockObject(DEFAULT_PALETTE);
    if (hPal)
    {
        hDC = GetDC(NULL);
        hOldPal2 = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
        RealizePalette(hDC);
    }


    GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap0.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
        + dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS);

    if (hOldPal2)
    {
        SelectPalette(hDC, (HPALETTE)hOldPal2, TRUE);
        RealizePalette(hDC);
        ReleaseDC(NULL, hDC);
    }

    fh = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

    if (fh == INVALID_HANDLE_VALUE)
        return FALSE;

    bmfHdr.bfType = 0x4D42; // "BM"
    dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
    bmfHdr.bfSize = dwDIBSize;
    bmfHdr.bfReserved1 = 0;
    bmfHdr.bfReserved2 = 0;
    bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

    WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

    WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
    GlobalUnlock(hDib);
    GlobalFree(hDib);
    CloseHandle(fh);
    return TRUE;
}

void MainWindow::onTimer() {
  if (captureCheckBox->isChecked()) {
    capture();
    convert(QString("C:\\tmp\\out.bmp"));
  }
}

void MainWindow::onSpinValueChanged(int val) {
  QMessageBox msgBox;
  msgBox.setText(QString("The document has been modified. %1").arg(val));
  msgBox.exec();
}
