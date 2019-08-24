#ifndef UNIT_TEST_RUNNER_H
#define UNIT_TEST_RUNNER_H

#include <string>
#include <list>
#include <map>

namespace Unit
{
  class TestCase;

  class TestRunner
  {
    public:

      TestRunner();

      virtual ~TestRunner();

      bool openLibrary(std::string path);
      void closeLibrary();

      std::map<std::string, TestCase *> caseCollection();

    private:

      class Private;
      Private * d;
  };
}

#endif // UNIT_TEST_RUNNER_H
