// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2004 Rik Hemsley (rikkus) <rik@kde.org>

#ifndef UNIT_TEST_BASE_H
#define UNIT_TEST_BASE_H

#include <string>

namespace Unit
{
  class TestBase
  {
    public:

      virtual ~TestBase() { /* Empty */ }
      virtual void run() { /* Empty */ }
      virtual void setUp() { /* Empty */ }
      virtual void tearDown() { /* Empty */ }
      virtual std::string className() const = 0;
      virtual std::string testName() const = 0;
      virtual bool passed() const = 0;
  };
}

#endif // UNIT_TEST_BASE_H
