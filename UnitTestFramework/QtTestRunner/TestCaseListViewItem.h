// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2004 Rik Hemsley (rikkus) <rik@kde.org>

#ifndef TEST_CASE_LIST_VIEW_ITEM_H
#define TEST_CASE_LIST_VIEW_ITEM_H

#include <string>
#include <qlistview.h>

namespace Unit
{
  class TestCase;
}

class TestCaseListViewItem : public QCheckListItem
{
  public:

    TestCaseListViewItem
      (QListView * parent, std::string name, Unit::TestCase * testCase);

    bool runSelectedTests();
    bool runTest(std::string name);

  protected:

    void stateChange(bool);

  private:

    Unit::TestCase * testCase_;
};

#endif // TEST_CASE_LIST_VIEW_ITEM_H

