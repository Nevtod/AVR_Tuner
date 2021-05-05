#include <stdio.h>

void int_to_str(unsigned int x, char* str)
{
    unsigned char i = 0;
	
	while (x > 0)
	{
	    unsigned char dig = x % 10;
	    x /= 10;
	    str[i] = dig + 48;
	    i++;
	}
	str[i] = 0;
	unsigned char j = 0;
	i--;
	while (j < i)
	{
	    char tmp = str[i];
	    str[i] = str[j];
	    str[j] = tmp;
	    j++;
	    i--;
	}
}

int main()
{
    int x = 12345;
    char str[100];
    int_to_str(x, str);
    
    printf("%s", str);
    
    return 0;
}
