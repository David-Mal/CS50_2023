#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long value = get_long("Number: ");
    int sum = 0, sum2 = 0, digit, count = 0, check;
    bool ok = 0;

    while (value)
    {
        digit = value % 10;
        count ++;

        if (!ok)
        sum += digit;

        else
        {
            digit *= 2;
            sum2 += digit % 10;
            digit /= 10;
            if (digit)
            sum2 += digit;
        }

        ok = 1 - ok;
        value /= 10;

        if (value >= 10)
        check = value;
    }
    sum += sum2;

    if (sum % 10 == 0)
    {
        if (count == 15 && (check == 34 || check == 37))
        printf("AMEX\n");

        else if (count == 16 && (check >= 51 && check <= 55))
        printf("MASTERCARD\n");

        else if ((count == 13 || count == 16) && (check / 10) % 10 == 4)
        printf("VISA\n");

        else
        {
         printf("INVALID\n");
        }
    }

    else printf("INVALID\n");
}