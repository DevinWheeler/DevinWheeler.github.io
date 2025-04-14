/*
 * BankingAPP
 *  Date: [2/11/2024]
 *  Author: [Devin Wheeler]
 */
#ifndef INVESTMENTCALCULATOR_H_
#define INVESTMENTCALCULATOR_H_

#include <string> 

class InvestmentCalculator {
public:
    void displayInputScreen() const;
    void processUserInput();
    void displayInvestmentReport(double initialInvestment, double monthlyDeposit, double annualInterest, int years) const;
private:
    double calculateMonthlyInterest(double balance, double annualInterestRate) const;
    void displayReport(double initialInvestment, double monthlyDeposit, double annualInterest, int years, const std::string& scenario) const;
};

#endif // INVESTMENTCALCULATOR_H_
