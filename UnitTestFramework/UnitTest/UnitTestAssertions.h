// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2004 Rik Hemsley (rikkus) <rik@kde.org>

#ifndef UNIT_TEST_ASSERTIONS_H
#define UNIT_TEST_ASSERTIONS_H

#include <iostream>

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#define ASSERT_EQUAL(a, b) \
{ \
  if ((a) != (b)) \
  { \
    std::cerr << "Test failed" \
      << std::endl << "  File   : " << __FILE__ \
      << std::endl << "  Line   : " << __LINE__ \
      << std::endl << "  Method : " << __PRETTY_FUNCTION__ \
      << std::endl << "  Test   : " << #a << " == " << b \
      << std::endl << "  Result : " << a \
      << std::endl \
      ; \
      passed_ = false; \
  } \
}

#endif // UNIT_TEST_ASSERTIONS_H
