#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qt_windows.h>
#include <QtWidgets>
#include <string>

#include <QImage>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include "IntervalBox.h"
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
  void capture();
  BOOL SaveHBITMAPToFile(HBITMAP hBitmap, LPCTSTR lpszFileName);
  void convert(QString fileName);
  void load(QString fileName);


private slots:

  void onCapture();
  void onTimer();
  void onSpinValueChanged(int val);
  void onEditingFinished();
  void onTextEdited(const QString &text);
  void onApply();
  void onQuitShortcut();

private:
  QVBoxLayout *contentLayout;
  QPushButton *quitButton;
  QPushButton *captureButton;
  QCheckBox *captureCheckBox;
  QSpinBox *intervalBox;
  IntervalBox *intervalTextBox;
  QPushButton *intervalButton;
  QLabel *spinLabel;
  QShortcut *quitShortcut;


  QGraphicsView *view;
  QGraphicsScene *scene;
  QGraphicsPixmapItem *pixmap;
  QImage image;

  QTimer *timer;

};

#endif // MAINWINDOW_H
