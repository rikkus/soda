#include <UnitTest/UnitTest.h>
#include "BankAccount.h"

UNIT_TESTS

TEST_DATA(BankAccount)

  BankAccount account;

TEST_END_DATA(BankAccount)

TEST_SET_UP(BankAccount)
{
  account.setBalance(3);
}

TEST_TEAR_DOWN(BankAccount)
{
  account.setBalance(0);
}

TEST(BankAccount, InitialBalance)
{
  ASSERT_EQUAL(account.balance(), 3);
}

TEST(BankAccount, Credit)
{
  account.credit(100);
  ASSERT_EQUAL(account.balance(), 103);
}

TEST(BankAccount, Debit)
{
  account.debit(100);
  ASSERT_EQUAL(account.balance(), -97);
}

TEST(BankAccount, Bogus)
{
  account.debit(100);
  ASSERT_EQUAL(account.balance(), -10000);
}

