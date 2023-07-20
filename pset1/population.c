#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int pstart, pend, years = 0, increase, decrease;

    do
    {
        pstart = get_int("Start size: \n");
    }
    while (pstart < 9);

    do
    {
        pend = get_int("End size: \n");
    }
    while (pend < pstart);

    while (pstart < pend)
    {
        increase = pstart / 3;
        decrease = pstart / 4;
        pstart += increase - decrease;
        years ++;
    }

    printf("Years: %i/n", years);
}