#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int value;
    do
    {
        value = get_int("Height: ");
    }
    while (value < 1 || value > 8);

    for (int i = 1; i <= value; ++i, printf("\n"))
    {
        for (int space = 1; space <= value - i; ++space)
        {
            printf(" ");
        }

        for (int hashtag = 1; hashtag <= i; ++hashtag)
        {
            printf("#");
        }

        for (int space = 1; space <= 2; ++space)
        {
            printf(" ");
        }

        for (int hashtag = 1; hashtag <= i; ++hashtag)
        {
            printf("#");
        }
    }
}