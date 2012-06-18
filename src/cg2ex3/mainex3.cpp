#include "mainwindowex3.h"
#include <QApplication>
#include <tbd/log.h>

LOG_INIT

int main(int argc, char * argv[])
{
  QApplication a(argc, argv);
  MainWindowEx3 w;
  w.show();
  return a.exec();
}

