#include <stdio.h>
#define WriteLine() printf("\n");
#define WriteLong(x) printf(" %lld", (long)x);
#define ReadLong(a) if (fscanf(stdin, "%lld", &a) != 1) a = 0;
#define long long long

void main()
{
    long b;
    long n;
    long s;
    long a;
    long i;
    long k;
    long dead1;
    long dead2;
    n=100;
    dead1=90;
    s=0;
    a=4;
    i=0;
    k=1;
    if(k==0)
    {
	b=1;
    }
    else
    {
	b=2;
    }
    while(i<n)
    {
	dead2=100;
        s=s+a*b;
	i=i+1;
    }
    WriteLong(s);
}
