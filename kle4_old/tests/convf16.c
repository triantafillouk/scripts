#include <stdio.h>
#include <stdlib.h>



int main(int argc, char **argv)
{
int char1 = 0;
int char2;
char *fname;
char bom[4];
char dat[4];
size_t res;

if(argc==2) {
	fname = argv[1];
} else return 1;

FILE *fin = fopen(fname,"r");
res=fread(bom,1,2,fin);
if(res==2) {
	printf("%X %X\n",bom[0],bom[1]);
} else return 1;
return 0;
#if	1
while (fread(dat,2,1,fin) 
{
		char1 = dat[0];
		char2 = dat[1];
		char1 <<= 8;
		char1 += char2;

		if (char1 >= 0xD800 && char1 <= 0xDBFF) {
			
			char2 = getchar();
			int tempchar = getchar();
			char2<<= 8;
			char2 += tempchar;

			if (char2 >= 0xDC00 && char2 <= 0xDFFF)
			{
				char1 -= 0xD800;
				char2 -= 0xDC00;
				char1 <<= 10;
				char1 += char2;
				char1 += 0x010000;
				//write code that converts to utf 8

			}
 else if((char1 >= 0x0000 && char1 <= 0xD7FF )||(char1 >= 0xE000 && char1 <= 0xFFFF)){
			//write code that converts to utf 8
		}
}
}
#endif
}
