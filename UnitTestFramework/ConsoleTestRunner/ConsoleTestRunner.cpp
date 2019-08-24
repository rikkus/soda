#include <UnitTest/UnitTest.h>
#include "UnitTestRunner.h"
#include <iostream>

class ConsoleTestRunner : public Unit::TestRunner
{
  public:

    void runAllTests()
    {
      std::map<std::string, Unit::TestCase *> cases = caseCollection();

      std::map<std::string, Unit::TestCase *>::const_iterator caseIterator =
        cases.begin();

      for (; caseIterator != cases.end(); ++caseIterator)
        runTestCase(caseIterator->first, caseIterator->second);
    }

    void runTestCase(std::string name, Unit::TestCase * testCase)
    {
      std::cerr << "Running test case: " << name << std::endl;

      std::list<Unit::TestBase *> testCollection = testCase->testCollection();

      std::cerr << "Collection has " << testCollection.size() << " tests" << std::endl;

      std::list<Unit::TestBase *>::const_iterator testIterator =
        testCollection.begin();

      for (; testIterator != testCollection.end(); ++testIterator)
      {
        Unit::TestBase * test = *testIterator;
        std::cerr << test->testName() << " ... ";
        test->setUp();
        test->run();
        test->tearDown();
        std::cerr << (test->passed() ? "PASS" : "FAIL") << std::endl;
      }
    }
};

int main(int argc, char ** argv)
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <test shared library>" << std::endl;
    return 1;
  }

  ConsoleTestRunner runner;

  if (!runner.openLibrary(argv[1]))
    return 1;

  runner.runAllTests();
}
