// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2004 Rik Hemsley (rikkus) <rik@kde.org>

#ifndef TEST_LIST_VIEW_ITEM_H
#define TEST_LIST_VIEW_ITEM_H

#include <string>
#include <qlistview.h>

class TestCaseListViewItem;

class TestListViewItem : public QCheckListItem
{
  public:

    TestListViewItem(TestCaseListViewItem * parent, std::string name);

  protected:

    void stateChange(bool);
};

#endif // TEST_LIST_VIEW_ITEM_H
