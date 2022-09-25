#include <stdio.h>
#include <stdint.h>

int main()
{
    int64_t base = 0;
    int64_t cur = 0;

    char temp_char;
    int a = scanf("%c", &temp_char);
    while (a > 0 && (temp_char != '\0' && temp_char != '\n'))
    {
        if (temp_char == '&')
        {
            base &= cur;
            cur = 0;
        }
        else if (temp_char == '|')
        {
            base |= cur;
            cur = 0;
        }
        else if (temp_char == '^')
        {
            base ^= cur;
            cur = 0;
        }
        else if (temp_char == '~')
        {
            base ^= 4611686018427387903;
            cur = 0;
        }
        else
        {
            int64_t a = 1;
            if (temp_char >= '0' && temp_char <= '9')
            {
                a <<= (int)temp_char - '0';
            }
            if (temp_char >= 'A' && temp_char <= 'Z')
            {
                a <<= (int)temp_char - 'A' + 10;
            }
            if (temp_char >= 'a' && temp_char <= 'z')
            {
                a <<= (int)temp_char - 'a' + 36;
            }

            cur |= a;
        }

        a = scanf("%c", &temp_char);
    }

    int i = 1;
    while (base != 0 && i <= 62)
    {
        if (base % 2 == 1)
        {
            if (i >= 1 && i <= 10)
            {
                printf("%c", (char)(i - 1 + '0'));
            }
            else if (i >= 11 && i <= 36)
            {
                printf("%c", (char)(i - 1 - 10 + 'A'));
            }
            else
            {
                printf("%c", (char)(i - 1 - 36 + 'a'));
            }
        }
        base >>= 1;
        ++i;
    }
}