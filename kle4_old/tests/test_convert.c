
#include	<stdio.h>
#include	<stdarg.h>
#include	<string.h>
#include	<fcntl.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<dirent.h>
#include	<time.h>
#include	<stdlib.h>
#include	<math.h>
#include	<glib.h>


char c_utf8[256];
char c_out[512];
char c_display[1024];

int utf_error=0;

/*
	Invalid characters return value below zero
*/
int utf8_countBytes[256] = {
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, // 128
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 
	-2,-2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,	// 192
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
	4,4,4,4,4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4
};

int utf_num_chars(char *utf)
{
 int len=strlen(utf);
 int i,j;
 int trail=0;
 int ch;
 int utflen=0;

 utf_error=0;
 i=0;
 while(i<len) {
 	ch=utf[i];
	trail=utf8_countBytes[ch]-1;
	if(trail<0) {
		utf_error=trail;
		return -1;
	};
	i++;
	utflen++;
	for(j=0;j<trail;j++,i++) {
		if(i>=len) {
			utf_error=i-j;
			return -1;
		};
		if(utf[i]<128 || utf[i]>191) {
			utf_error=i-j;
			return -1;
		};
	};
 };
 return utflen;
}

int main(int argc, char **argv)
{
 gsize in_bytes;
 gsize bytes_read, bytes_written;
 gchar *out_string;
 GError **o_error;
 gsize out_bytes;
 int num_letters=0;
 
 strcpy(c_utf8,"0Καλημέρα1砡1");
 in_bytes = strlen(c_utf8);
 out_string = g_convert(c_utf8,in_bytes,"UTF-32","UTF-8",&bytes_read,&bytes_written,o_error);
 num_letters = utf_num_chars(c_utf8);
 printf("Input string=[%s]\n",c_utf8);
 printf("Letters=%d in_bytes=%ld read=%ld write=%ld\n",num_letters,in_bytes,bytes_read,bytes_written);
 int i;
 int j;
 int display_offset=0;
 int out_str_offset=0;
 int out_size=4;
 for(i=0;i<num_letters+1;i++) {
 	for(j=0;j<out_size;j++){
	 sprintf(c_display+display_offset,"%02X",out_string[out_str_offset++]);
	 display_offset+=2;
	};
	c_display[display_offset++]=',';
 };
 c_display[display_offset]=0;
 printf("out [%s]\n",c_display);

}