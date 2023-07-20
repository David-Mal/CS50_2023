#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);


int main(void)
{
    string text = get_string("Text: ");
    float letters = 0, words = 1, sentences = 0;
    for (int i = 0; i < strlen(text); ++i)
    {
        if ( (text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z') )
        {
            letters ++;
        }
        else if (text[i] == '.' || text[i] == '?' || text[i] == '!')
            sentences ++;
        else if (text[i] == ' ')
        words ++;
    }

    float L, S;
    float index;
    L = letters / words * 100;
    S = sentences / words * 100;
    printf("%f %f\n", L, S);
    index = 0.0588 * L - 0.296 * S - 15.8;
    int ans = round(index);

    if (index >= 16)
    printf("Grade 16+\n");

    else if (index < 1)
    printf("Before Grade 1\n");

    else printf("Grade %i\n", ans);
}