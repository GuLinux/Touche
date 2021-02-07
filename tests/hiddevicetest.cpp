#include <iostream>
#include <QCoreApplication>

using namespace std;

#include "backend/hiddev/hiddev.h"

int main(int argc, char **argv)
{
  QCoreApplication app(argc, argv);
  HidDev hiddev(app.arguments().first(), nullptr);
  return 0;
}