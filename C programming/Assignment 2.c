#include<stdio.h>
#include<string.h>
int ln(int);
int name(char name1[]);
int main()
{
    char yourname[50];
    int a,i,j,s,count,small,location=0,sum=0,b=0;

    printf("Please enter your name:\t");
    scanf("%[^\n]s",yourname);
    s=name(yourname);
    printf("%s, your lucky number is %d.\n",yourname,s);


    int freq[5]={-1,-1,-1,-1,-1};
    char w[5];//w stores diff of city name from name1 s
    int ls[5];//ls=least sum of city name
    char cityname[5][50];
    printf("\nPlease enter the 5 cities with which you wish to match:\t");

    for (i=0;i<4;i++)
    {
        scanf(" %[^,],",cityname[i]);
    }
    scanf(" %[^.].",cityname[4]);
    printf("\nThe lucky numbers are:\n");

    for (i=0;i<5;i++)
    {
        ls[i]=name(cityname[i]);
        printf("%s = %d\n",cityname[i],ls[i]);
    }

    for (i=0;i<5;i++)
    {
        w[i]=s-ls[i];
        if (w[i]<0)
        w[i]=w[i]*(-1);
    }

    for(i=0; i<5; i++)
    {
        count = 1;
        for(j=i+1; j<5; j++)
        {
            if(w[i]==w[j])
            {
                count++;
                freq[j] = 0;
            }
        }

        if(freq[i] != 0)
        {
            freq[i] = count;
        }
    }

    small=w[0];
    for (i=1;i<5;i++)
    {
        if (w[i]<small)
        {
            small=w[i];
            location=i;
        }
    }

    a=freq[location]; //xx stores the no of times min diff occurs
    int lo[a];
    for (i=0;i<5;i++)
    {
        if (w[i]==small)
            {
                lo[b]=i; // lo stores the locations of min diff // b is initialized from 0
                b++;
            }
    }

    char ex[a][50]; // ex = extracting the cityname corresponding to the min diff
    for (i=0;i<a;i++)
    {
        strcpy(ex[i],cityname[lo[i]]);
    }

    char t[50];
    for (i=0;i< (a-1) ;i++)
    {
        for (j=i+1;j<a;j++)
        {
            if (strcmp(ex[i], ex[j]) > 0)
            {
                strcpy(t,ex[i]);
                strcpy(ex[i],ex[j]);
                strcpy(ex[j],t);
            }
        }
    }
    printf("\nYou will do well if you join %s.\n",ex[0]);

}//main closed

int ln(int s)
{
    int rem,sum=0,a;
    if (s<10)
    return s;
    else
    {
        for (;s>0;)
        {
            rem=s%10;
            sum=sum+rem;
            s=s/10;
        }
    a=ln(sum);
    return a;
    }
}

int name(char name1[])
{
    int sum = 0,i,l,s;
    l = strlen(name1);
    for (i = 0; i<l; i++)
    {
        sum = sum + name1[i];
    }
    s=ln(sum);
    return s;
}
