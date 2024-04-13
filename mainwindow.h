#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qt_windows.h>
#include <QtWidgets>
#include <string>

#include <QImage>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>

#include <opencv2/opencv.hpp>


QT_BEGIN_NAMESPACE
class QLabel;
class QComboBox;
class QVBoxLayout;
class QIcon;
class QLibrary;
class QMenu;
class QSystemTrayIcon;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT
 public:
  MainWindow();
  ~MainWindow();
  void setWindowSizeLocation();
  void onQuit();


private slots:
  void onLoad();

private:
  QVBoxLayout *contentLayout;
  QPushButton *quitButton;
  QGraphicsView *view;
  QGraphicsScene *scene;
  QGraphicsPixmapItem *pixmap;
  QImage image;

  QTimer *timer; 

  cv::Mat mat;
};

#endif // MAINWINDOW_H
