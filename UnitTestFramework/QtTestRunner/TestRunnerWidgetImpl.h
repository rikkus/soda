// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2004 Rik Hemsley (rikkus) <rik@kde.org>

#ifndef TEST_RUNNER_WIDGET_IMPL_H
#define TEST_RUNNER_WIDGET_IMPL_H

#include <string>
#include "QtTestRunnerWidget.h"

namespace Unit
{
  class TestRunner;
  class TestCase;
}

class TestRunnerWidgetImpl : public TestRunnerWidget
{
  public:

    TestRunnerWidgetImpl(QWidget * parent);

    bool openLibrary(std::string path);
    void addTestCase(std::string name, Unit::TestCase * testCase);
    void slotRunSelectedTests();

  private:

    Unit::TestRunner * runner_;
};

#endif // TEST_RUNNER_WIDGET_IMPL_H
