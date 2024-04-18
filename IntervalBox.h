#ifndef INTERVALBOX_H
#define INTERVALBOX_H

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

class IntervalBox : public QLineEdit
{
  Q_OBJECT

 public:
  explicit IntervalBox(QWidget *parent);
  ~IntervalBox();

  QSize sizeHint();
  
};


#endif 