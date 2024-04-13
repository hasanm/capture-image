#include <QApplication>
#include<QtDebug>


#include "mainwindow.h"


int main(int argc, char *argv[]) {
  
  QApplication app(argc, argv);
  MainWindow window;
  window.setWindowSizeLocation();

  window.show();
  
  return app.exec();
} 
