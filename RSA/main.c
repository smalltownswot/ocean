#include <stdio.h>
#include <stdlib.h>
#include"D:/小学期C语言/RSA/.H"
int main()
{
    int p=0;
    int q=0;
    while(1)
    {
        while(1)
        {
            printf("请输入素数p:\n");
            scanf("%d",&p);
            if(isPrime(p))
                break;
            printf("您输入的不是素数，请重新输入！\n\n");
        }
        while(1)
        {
            printf("请输入素数q:\n");
            scanf("%d",&q);
            fflush(stdin);
            if(isPrime(q))
                break;
            printf("您输入的不是素数，请重新输入！\n\n");
        }
        if(p*q<128)
            printf("您输入的p*q小于128,请重新输入!\n\n");
        else
        break;
    }
    int n=p*q;
    int e=0,a=0;
    int d=0,i=0;
    while(1)
    {
        printf("请输入公钥e比(p-1)*(q-1)小且与之互素的正整数:\n");
        scanf("%d",&e);
        if(extendedEuclid(e,(p-1)*(q-1),&d))
            break;
        printf("您输入的e不满足条件，请重新输入!\n\n");
    }
    printf("您的公钥为:(%d,%d)\n\n",e,n);
    for (d = 0; d < p*q; d++)
    {
		if (e * d % ((p-1)*(q-1)) == 1)
			break;
    }
    printf("您的私钥为:(%d,%d)\n\n",d,n);

    FILE *fp;
    char buffer[256];
    long length;
    long ciphertext[256];

    fp=fopen("D:\小学期C语言1.text","r");
    fseek(fp,0,SEEK_END);
    length=ftell(fp);
    fseek(fp,0,SEEK_SET);
    fread(buffer,1,length,fp);
    fclose(fp);
    printf("%s\n",buffer);

    RsaPubKey pubkey;
    pubkey.e=e;
    pubkey.n=p*q;
    RsaPriKey prikey;
    prikey.d=d;
    prikey.n=p*q;

    RsaEncipher(buffer,length,ciphertext,pubkey);

    char test[256];
    RsaDecipher(ciphertext,sizeof(ciphertext)/sizeof(ciphertext[0]),test,prikey);
    for(i=0;i<length;i++)
        printf("%ld\n",test[i]);
    for(i=0;i<length;i++)
        printf("%c\n",test[i]);

}

void RsaEncipher(char *plaintext, long length, long *ciphertext, RsaPubKey pubkey)
{
    long i=0;
    for(;i<length;i++)
    {
        ciphertext[i]=modexp(plaintext[i],pubkey.e,pubkey.n);
    }
    return;
}

void RsaDecipher(long *ciphertext, long length, char *plaintext, RsaPriKey prikey)
{
    long i=0;
    for(;i<length;i++)
    {
        plaintext[i]=modexp(ciphertext[i],prikey.d,prikey.n);
    }
    return;
}

long modexp(long a, long b, long n)
{
    a=a%n;
    if(b==0)return 1;
    if(b==1)return 0;
    if(b%2==0)
    {
        return(modexp(a*a%n,b/2,n)%n);
    }
    if(b%2==1)
    {
        return(a*modexp(a,(b-1),n)%n);
    }
    return 0;
}

int isPrime(long number)
{
    int i=2;
    int j=sqrt(number);
    for(;i<=j;i++)
    {
        if(number%i==0)
            return 0;
    }
    return 1;
}

int extendedEuclid(int e, int n, int *d)
{
    int x1, x2, x3, y1, y2, y3, t1, t2, t3, q;
    x1 = y2 = 1;
    x2 = y1 = 0;
    x3 = n;
    y3 = e;

    while(1)
    {
        if(y3 == 0)
        {
            *d = x3;
            return 0;
        }
        if(y3 == 1)
        {
            if(y2 < 0)
                y2 += n;
            *d = y2;
            return 1;
        }
        q = x3 / y3;
        t1 = x1 - q * y1;
        t2 = x2 - q * y2;
        t3 = x3 - q * y3;
        x1 = y1;
        x2 = y2;
        x3 = y3;
        y1 = t1;
        y2 = t2;
        y3 = t3;
    }
}
