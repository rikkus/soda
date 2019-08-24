// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2004 Rik Hemsley (rikkus) <rik@kde.org>

#include <iostream>

#include <qapplication.h>
#include <qmainwindow.h>

#include "TestRunnerWidgetImpl.h"

int main(int argc, char ** argv)
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <test shared library>" << std::endl;
    return 1;
  }

  QApplication app(argc, argv);

  QMainWindow mainWindow;
  
  TestRunnerWidgetImpl * testRunnerWidget =
    new TestRunnerWidgetImpl(&mainWindow);

  testRunnerWidget->openLibrary(argv[1]);

  mainWindow.setCentralWidget(testRunnerWidget);
  
  app.setMainWidget(&mainWindow);

  mainWindow.show();

  return app.exec();
}
