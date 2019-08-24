#include <qlistview.h>

#include <UnitTest/UnitTest.h>
#include "UnitTestRunner.h"

#include "TestRunnerWidgetImpl.h"
#include "TestListViewItem.h"
#include "TestCaseListViewItem.h"

TestRunnerWidgetImpl::TestRunnerWidgetImpl(QWidget * parent)
  : TestRunnerWidget(parent)
{
  runner_ = new Unit::TestRunner;
}

bool TestRunnerWidgetImpl::openLibrary(std::string path)
{
  if (!runner_->openLibrary(path))
    return false;

  testListView->clear();

  std::map<std::string, Unit::TestCase *> cases =
    runner_->caseCollection();

  std::map<std::string, Unit::TestCase *>::const_iterator caseIterator =
    cases.begin();

  for (; caseIterator != cases.end(); ++caseIterator)
  {
    addTestCase(caseIterator->first, caseIterator->second);
  }

  return true;
}

void TestRunnerWidgetImpl::addTestCase
(std::string name, Unit::TestCase * testCase)
{
  TestCaseListViewItem * testCaseItem =
    new TestCaseListViewItem(testListView, name, testCase);

  testCaseItem->setOpen(true);
  testCaseItem->setOn(true);

  std::list<Unit::TestBase *> testCollection = testCase->testCollection();

  std::list<Unit::TestBase *>::const_iterator testIterator =
    testCollection.begin();

  for (; testIterator != testCollection.end(); ++testIterator)
  {
    Unit::TestBase * test = *testIterator;

    TestListViewItem * testItem =
      new TestListViewItem(testCaseItem, test->testName());

    testItem->setOn(true);
  }
}

void TestRunnerWidgetImpl::slotRunSelectedTests()
{
  bool testsSucceded = true;

  TestCaseListViewItem * testChild =
    static_cast<TestCaseListViewItem *>(testListView->firstChild());

  while (0 != testChild)
  {
    if (testChild->isOn())
      if (!testChild->runSelectedTests())
        testsSucceded = false;

    testChild = static_cast<TestCaseListViewItem *>(testChild->nextSibling());
  }
}

