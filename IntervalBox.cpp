#include  "IntervalBox.h"


IntervalBox::IntervalBox(QWidget *parent)
  : QLineEdit(parent)
{
  setMinimumSize(25,25);
  setMaximumSize(50,25);
}

IntervalBox::~IntervalBox() {
}


QSize IntervalBox::sizeHint(){
    return QSize(minimumWidth(), minimumHeight());
}