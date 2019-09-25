#include<stdio.h>
#include<string.h>
#define last_element1 1 // ensuring the last element of chain as 1.
#define last_element2 89 // ensuring the last element of chain as 89.

int lucky_number(char yourname[]);
int single_digit(int s); //single digit lucky number
int chain(int num); // getting to the last point of the chain.

int lucky_number(char yourname[])
{
    int sum = 0,len;
    len = strlen(yourname);
    for (int i = 0; i<len; i++)
    {
        sum = sum + yourname[i];
    }
    return (single_digit(sum)); //passing the sum to single_digit
}

int single_digit(int s) //single digit lucky number
{
    int rem,sum=0;
    if (s<10) // unless s is a single digit integer, donot return it.
    return s;
    else
    {
        for (;s>0;)
        {
            rem=s%10;
            sum=sum+rem;
            s=s/10;
        }
    return(single_digit(sum));
    }
}

int chain(int num) // getting to the last point of the chain.
{
    int sum=0,rem;
    while(num>0)
    {
        rem=num%10;
        sum=sum+rem*rem;
        num/=10;
    }
    if (sum==last_element1)
        return last_element1;
    if (sum==last_element2)
        return last_element2;
    return (chain(sum));
}

int main()
{
    char yourname[50];
    int i,s,count=0,check,n;
    printf("Please enter your name:\t");
    scanf("%[^\n]s",yourname);

    s=lucky_number(yourname); // s stores the single digit integer as lucky number
    printf("%s, your lucky number is %d.\n",yourname,s);

    if (chain(s)==last_element1)
        printf("%s, you belong to the 1-kind.\n",yourname);
    else
        printf("%s, you belong to the 89-kind.\n",yourname);

    printf("\nPlease enter a number:  ");
    scanf("%d",&n);
    for (i=1;i<=n;i++)
    {
        check=chain(i);
        if (check==last_element1)
            count++; //count checks the ending of the chain should be one.

    }
    printf("Followng %d numbers between 1 and %d end in 1:\n",count,n);
    for (i=1;i<=n;i++)
    {
        check=chain(i);
        if (check==last_element1)
            printf("%d, ",i); //printing all the numbers from 1 to n whose chain ends in 1.
    }
}






