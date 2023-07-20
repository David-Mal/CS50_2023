#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>


int main(int argc, string argv[])
{
    if (argc == 2)
    {
        if (strlen(argv[1]) == 26)
        {
            for (int i = 0; i < 26; ++i)
            {
            if (!isalpha(argv[1][i]))
            {
                printf("Key must contain 26 characters.");
                return 1;
            }

                for (int j = i + 1; j < 26; ++j)
                {
                    if (toupper(argv[1][i]) == toupper(argv[1][j]))
                    {
                        printf("Key must contain 26 characters.");
                        return 1;
                    }
                }
            }

                string plain = get_string("plaintext: ");
                int n = strlen(plain);
                for (int i = 0; i < n; ++i)
                {
                    if (islower(plain[i]))
                    {
                        int index = plain[i] - 'a';
                        plain[i] = tolower(argv[1][index]);
                    }
                    if (isupper(plain[i]))
                    {
                        int index = plain[i] - 'A';
                        plain[i] = toupper(argv[1][index]);
                    }
                }
                printf("ciphertext: %s\n", plain);
                return 0;
        }
        else
        {
            printf("Key must contain 26 characters.");
            return 1;
        }
    }

    else
    {
        printf("Usage: ./substitution key");
        return 1;
    }
}