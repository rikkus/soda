// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2004 Rik Hemsley (rikkus) <rik@kde.org>

#ifndef UNIT_TEST_REGISTRY_H
#define UNIT_TEST_REGISTRY_H

#include "UnitTestBase.h"
#include "UnitTestCase.h"

#include <map>
#include <string>

namespace Unit
{
  class TestRegistry
  {
    public:

      static TestRegistry * instance()
      {
        if (0 == instance_)
          instance_ = new TestRegistry;

        return instance_;
      }

      void addTest(TestBase * test)
      {
        if (0 == caseCollection_[test->className()])
          caseCollection_[test->className()] = new TestCase;

        caseCollection_[test->className()]->addTest(test);
      }

      std::map<std::string, TestCase *> TestRegistry::caseCollection()
      {
        return caseCollection_;
      }

    private:

      static TestRegistry * instance_;

      std::map<std::string, TestCase *> caseCollection_;
  };
}

#endif // UNIT_TEST_REGISTRY_H
