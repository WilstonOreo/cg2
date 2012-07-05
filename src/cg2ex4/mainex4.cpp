#include "mainwindowex4.h"
#include <QApplication>
#include <tbd/log.h>

LOG_INIT

int main(int argc, char * argv[])
{
  QApplication a(argc, argv);
  MainWindowEx4 w;
  w.show();
  return a.exec();
}

