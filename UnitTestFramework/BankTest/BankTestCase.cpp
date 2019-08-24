#include <UnitTest/UnitTest.h>
#include "Bank.h"

UNIT_TESTS

TEST_DATA(Bank)

  Bank bank;

TEST_END_DATA(Bank)

TEST_SET_UP(Bank)
{
  bank.addAccount(new BankAccount);
  bank.addAccount(new BankAccount(42, 105));
}

TEST_TEAR_DOWN(Bank)
{
  bank.removeAllAccounts();
}

TEST(Bank, InitialState)
{
  ASSERT_EQUAL(bank.accountCount(), 2);
  ASSERT_EQUAL(bank.hasAccount(0), true);
  ASSERT_EQUAL(bank.hasAccount(42), true);
}

TEST(Bank, AddAccount)
{
  bank.addAccount(new BankAccount);
  ASSERT_EQUAL(bank.accountCount(), 3);
}

TEST(Bank, RemoveAccount)
{
  bank.removeAccount(42);
  ASSERT_EQUAL(bank.accountCount(), 1);
  ASSERT_EQUAL(bank.hasAccount(0), true);
}

TEST(Bank, Bogus)
{
  ASSERT_EQUAL(bank.hasAccount(12345), true);
}

