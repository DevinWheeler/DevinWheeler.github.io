/*
 * BankingAPP
 *  Date: [2/11/2024]
 *  Author: [Devin Wheeler]
 */

#include "InvestmentCalculator.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>

using namespace std;

// Display initial input screen
void InvestmentCalculator::displayInputScreen() const {
    cout << "Airgead Banking Investment Calculator\n";
    cout << "-------------------------------------\n";
    cout << "Please enter the following values:\n";
    cout << "Initial Investment Amount: ";
}

// Process user input with validation and option to rerun the calculation
void InvestmentCalculator::processUserInput() {
    double initialInvestment, monthlyDeposit, annualInterest;
    int years;
    char continueCalc = 'y';

    do {
        displayInputScreen();

        cin >> initialInvestment;
        cout << "Monthly Deposit: ";
        cin >> monthlyDeposit;
        cout << "Annual Interest: ";
        cin >> annualInterest;
        cout << "Number of Years: ";
        cin >> years;

        displayInvestmentReport(initialInvestment, monthlyDeposit, annualInterest, years);

        cout << "\nWould you like to try different values? (y/n): ";
        cin >> continueCalc;
        // Clear input buffer to handle next loop iteration correctly
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        system("cls");
    } while (continueCalc == 'y' || continueCalc == 'Y');
}

// Display investment report using both with and without monthly deposits
// This could have been 2 seperate funcions----
void InvestmentCalculator::displayInvestmentReport(double initialInvestment, double monthlyDeposit, double annualInterest, int years) const {
    cout << fixed << setprecision(2);

    // Report without monthly deposits
    displayReport(initialInvestment, 0, annualInterest, years, "without");

    // Report with monthly deposits
    displayReport(initialInvestment, monthlyDeposit, annualInterest, years, "with");
}


void InvestmentCalculator::displayReport(double initialInvestment, double monthlyDeposit, double annualInterest, int years, const string& scenario) const {
    cout << "\nYear-End Balances and Year-End Earned Interest " << scenario << " Additional Monthly Deposits:\n";
    cout << "-------------------------------------------------------\n";
    cout << "Year      Year-End Balance    Year-End Earned Interest\n";
    cout << "-------------------------------------------------------\n";

    double currentBalance = initialInvestment;

    for (int year = 1; year <= years; ++year) {
        double yearlyInterest = 0;
        double startingBalance = currentBalance; // Save starting balance for interest calculation

        // calculates monthy per requirements
        if (scenario == "without") {
            for (int month = 1; month <= 12; ++month) {
                yearlyInterest += calculateMonthlyInterest(currentBalance, annualInterest);
            }
            currentBalance += yearlyInterest;
        }
        else { // "with" scenario includes monthly deposits
            for (int month = 1; month <= 12; ++month) {
                double monthlyInterest = calculateMonthlyInterest(currentBalance + monthlyDeposit, annualInterest);
                yearlyInterest += monthlyInterest;
                currentBalance += monthlyDeposit + monthlyInterest;
            }
        }
        //Print out with some formating 
        cout << year << "        $"
            << fixed << setprecision(2) << setw(15) << left << currentBalance
            << "    $" << setw(15) << left << yearlyInterest << "\n";
    }
}



// Calculate monthly interest
double InvestmentCalculator::calculateMonthlyInterest(double balance, double annualInterestRate) const {
    return balance * (annualInterestRate / 100) / 12;
}