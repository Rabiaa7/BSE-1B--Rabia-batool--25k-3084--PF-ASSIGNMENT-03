//25k-3084 q1(bank repayment in loans)
#include <stdio.h>

double calculateRepayment(double loan, double interestRate, int years, int year)
{
    if (loan <= 0 || years == 0)
        return 0;
    loan = loan + (loan * interestRate);
    double installment = 35000;
    loan -= installment;
    if (loan < 0) loan = 0;
  
    printf("Year %d: Remaining loan = %.2f\n", year, loan);

    return installment + calculateRepayment(loan, interestRate, years - 1, year + 1);
}

int main()
{
    double loan = 100000;
    double interestRate = 0.05;
    int years = 3;

    printf("Repayment Schedule:\n");

    double totalPaid = calculateRepayment(loan, interestRate, years, 1);

    printf("\nTotal Paid in %d years = %.2f\n", years, totalPaid);

    return 0;
}
