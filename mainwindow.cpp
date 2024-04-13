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

  top->setLayout(topLayout);

  /* Content Layout */
  contentLayout = new QVBoxLayout(content);
  scene = new QGraphicsScene(this);
  view = new QGraphicsView(this);
  view->setScene(scene);
  view->scale(.5,.5);
  contentLayout->addWidget(view);

  /* Root Layout */
  QVBoxLayout *rootLayout = new QVBoxLayout(root);


  rootLayout->addWidget(top);
  rootLayout->addWidget(content);
  setCentralWidget(root);

  timer = new QTimer(this);
  connect(timer,&QTimer::timeout , this, &MainWindow::onLoad);
  timer->start(3000);
}

void MainWindow::onQuit() {
  qApp->quit();
}

void MainWindow::onLoad() {
  QString fileName = QString("z://aoe_images//out.png");
  Mat dest; 
  if (QFile::exists(fileName)){
    mat = imread(fileName.toStdString().c_str(), IMREAD_COLOR);
    cvtColor(mat, dest, COLOR_BGR2RGB);
    const QImage newImage((uchar*) dest.data, dest.cols, dest.rows, dest.step, QImage::Format_RGB888);
    QPixmap pix = QPixmap::fromImage(newImage);
    if (pixmap != nullptr) {
      scene->removeItem(pixmap);
    }
    pixmap = scene->addPixmap(pix);
  } 
} 


void MainWindow::setWindowSizeLocation() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect rec = screen->availableGeometry();
    qDebug() << "Hello " << rec.width() << " x " << rec.height();

    // int targetWidth = this->width();
    int targetWidth = 300;

    int height = 180;
    int width = rec.width();
    // int x=(width - targetWidth);
    int x = 300;
    int y= 50;
    this->setGeometry(x,y,targetWidth,height);
}

