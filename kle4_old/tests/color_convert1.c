#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
 char a[1000];
 int r,g,b;
 char name[200];
 int count=0;
 while(fgets(a,1000,stdin)){
	if(strlen(a)<5 || a[0]=='#') {
		printf("%s",a);
		continue;
	}
	sscanf(a,"%d|%d|%d %s",&r,&g,&b,name);
//	printf("%d - %d - %d [%s]\n",r,g,b,name);
	printf("{\"%s\",%*s\"#%02X%02X%02X\"},// %s",name,(int)(20-strlen(name)),"",r,g,b,a);
	count++;
 };
 return count;
}