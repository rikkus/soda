#include <iostream>
#include <dlfcn.h>
#include <cstdio>

#include <UnitTest/UnitTest.h>
#include "UnitTestRunner.h"

namespace Unit
{
  class TestRunner::Private
  {
    public:

      TestRegistry * registry;
      void * library;
  };

  TestRunner::TestRunner()
  {
    d = new Private;
    d->registry = 0;
    d->library = 0;
  }

  TestRunner::~TestRunner()
  {
    closeLibrary();
    delete d;
  }

  std::map<std::string, TestCase *> TestRunner::caseCollection()
  {
    if (0 == d->registry)
    {
      std::cerr << "Registry not available" << std::endl;
      return std::map<std::string, TestCase *>();
    }

    return d->registry->caseCollection();
  }

  bool TestRunner::openLibrary(std::string path)
  {
    char * error = 0;

    d->library = dlopen(path.c_str(), RTLD_LAZY);

    if (0 != (error = dlerror()))
    {
      std::cerr << error << std::endl;
      return false;
    }

    void * (*instance)(void);

    instance = (void * (*)(void))dlsym(d->library, "testRegistryInstance");

    if (0 != (error = dlerror()))
    {
      std::cerr << error << std::endl;
      return false;
    }

    d->registry = (Unit::TestRegistry *)(*instance)();

    return true;
  }

  void TestRunner::closeLibrary()
  {
    if (0 == d->library)
      return;

    dlclose(d->library);
  }
}
