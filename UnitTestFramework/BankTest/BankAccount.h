#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H

class BankAccount
{
  public:

    BankAccount(long initialBalance = 0, unsigned int id = 0)
      : balance_(initialBalance),
        id_(id)
    {
    }

    void credit(long amount)
    {
      balance_ += amount;
    }

    void debit(long amount)
    {
      balance_ -= amount;
    }

    long balance() const
    {
      return balance_;
    }

    void setBalance(long amount)
    {
      balance_ = amount;
    }

    unsigned int id() const
    {
      return id_;
    }

  private:

    long balance_;
    unsigned int id_;
};

#endif // BANK_ACCOUNT_H
