// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2004 Rik Hemsley (rikkus) <rik@kde.org>

#include <qpixmap.h>
#include <qlistview.h>

#include <UnitTest/UnitTest.h>

#include "TestListViewItem.h"
#include "TestCaseListViewItem.h"

TestCaseListViewItem::TestCaseListViewItem
  (QListView * parent, std::string name, Unit::TestCase * testCase)
  : QCheckListItem(parent, name.c_str(), QCheckListItem::CheckBox),
    testCase_(testCase)
{
  setPixmap(0, QPixmap("gear.png"));
}

bool TestCaseListViewItem::runSelectedTests()
{
  bool testsSucceded = true;

  TestListViewItem * testChild = static_cast<TestListViewItem *>(firstChild());

  while (0 != testChild)
  {
    if (testChild->isOn())
    {
      if (runTest(testChild->text(0)))
      {
        testChild->setPixmap(0, QPixmap("ok.png"));
      }
      else
      {
        testChild->setPixmap(0, QPixmap("no.png"));
        testsSucceded = false;
      }
    }

    testChild = static_cast<TestListViewItem *>(testChild->nextSibling());
  }

  if (testsSucceded)
    setPixmap(0, QPixmap("ok.png"));
  else
    setPixmap(0, QPixmap("no.png"));

  return testsSucceded;
}

bool TestCaseListViewItem::runTest(std::string name)
{
  Unit::TestBase * test = testCase_->test(name);

  if (0 == test)
  {
    std::cerr << "Um..." << std::endl;
  }

  std::cerr << "Running test: " << text(0) << "::" << test->testName() << std::endl;
  test->setUp();
  test->run();
  test->tearDown();

  return test->passed();
}

void TestCaseListViewItem::stateChange(bool on)
{
  TestListViewItem * testChild = static_cast<TestListViewItem *>(firstChild());

  while (0 != testChild)
  {
    testChild->setOn(on);

    testChild = static_cast<TestListViewItem *>(testChild->nextSibling());
  }
}

