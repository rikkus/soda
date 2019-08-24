// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2004 Rik Hemsley (rikkus) <rik@kde.org>

#ifndef UNIT_TEST_CASE_H
#define UNIT_TEST_CASE_H

#include "UnitTestBase.h"

#include <list>
#include <string>

namespace Unit
{
  class TestCase
  {
    public:

      TestCase()
        : setUp_(0),
          tearDown_(0)
      {
      }

      ~TestCase()
      {
      }

      void addTest(TestBase * test)
      {
        testCollection_.push_back(test);
      }

      std::list<TestBase *> testCollection()
      {
        return testCollection_;
      }

      TestBase * test(std::string name)
      {
        std::list<TestBase *>::const_iterator it = testCollection_.begin();

        for (; it != testCollection_.end(); ++it)
        {
          if ((*it)->testName() == name)
            return *it;
        }

        return 0;
      }

    private:

      TestBase * setUp_;
      std::list<TestBase *> testCollection_;
      TestBase * tearDown_;
  };
}

#endif // UNIT_TEST_CASE_H
