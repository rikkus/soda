// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2004 Rik Hemsley (rikkus) <rik@kde.org>

#include <qpixmap.h>
#include <qlistview.h>

#include <UnitTest/UnitTest.h>

#include "TestCaseListViewItem.h"
#include "TestListViewItem.h"

TestListViewItem::TestListViewItem
  (TestCaseListViewItem * parent, std::string name)
  : QCheckListItem(parent, name.c_str(), QCheckListItem::CheckBox)
{
  setPixmap(0, QPixmap("gear.png"));
}

void TestListViewItem::stateChange(bool on)
{
  if (0 == parent())
    return;

  QCheckListItem * theParent = static_cast<QCheckListItem *>(parent());

  if (on && theParent->isOn())
    theParent->setOn(true);
}
