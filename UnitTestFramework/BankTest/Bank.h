#ifndef BANK_H
#define BANK_H

#include "BankAccount.h"

#include <map>

class Bank
{
  public:

    Bank()
    {
    }

    void removeAllAccounts()
    {
      std::map<unsigned int, BankAccount *>::const_iterator it =
        accountCollection_.begin();

      for (; it != accountCollection_.end(); ++it)
        delete it->second;

      accountCollection_.clear();
    }

    unsigned int accountCount() const
    {
      return accountCollection_.size();
    }

    bool hasAccount(unsigned int id) const
    {
      return accountCollection_.find(id) != accountCollection_.end();
    }

    void addAccount(BankAccount * account)
    {
      accountCollection_[account->id()] = account;
    }

    bool removeAccount(unsigned int id)
    {
      if (!hasAccount(id))
        return false;

      accountCollection_.erase(id);
    }

  private:

    std::map<unsigned int, BankAccount *> accountCollection_;
};

#endif // BANK_H
