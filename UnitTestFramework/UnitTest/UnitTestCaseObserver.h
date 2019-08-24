// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2004 Rik Hemsley (rikkus) <rik@kde.org>

#ifndef UNIT_TEST_CASE_OBSERVER_H
#define UNIT_TEST_CASE_OBSERVER_H

#include "UnitTestBase.h"

namespace Unit
{
  class TestCaseObserver
  {
    public:

      virtual void aboutToRunTest(TestBase *)
      {
        // Empty.
      }

      virtual void testFinished(TestBase *)
      {
        // Empty.
      }
  };
}

#endif // UNIT_TEST_CASE_OBSERVER_H
