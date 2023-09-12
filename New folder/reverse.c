// Online C compiler to run C program online
#include <stdio.h>

int main() {
     char a[] = "SURYA";
     int i,j;
     char temp;
     char*d = a;
     for(i=0,j=strlen(a)-1;i<j;i++,j--)
     {
         temp = d[i];
         d[i] = d[j];
         d[j] =  temp;
     }
     printf("%s\n",d);
    return 0;
}