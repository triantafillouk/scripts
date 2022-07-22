#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void MESG(const char *fmt, ...)
{
 va_list args;
 static char mline[512];
    if (fmt != NULL) {
		va_start(args,fmt);
		vsnprintf(mline,511,fmt,args);
		va_end(args);

		fprintf(stderr,"%s\n",mline);
		fflush(stderr);
    }
}


//0x0763
char * read_utf16_2_utf8(char *file_name,size_t *size)
{
 FILE *fi=fopen(file_name,"r");
 static char *start_out;
 start_out=NULL;
 if(fi!=NULL) {
 	size_t res;
	size_t in_bytes=0;	// input length in bytes
	size_t out_bytes=0;	// out length in bytes
	size_t in_chars=0;
	size_t out_chars=0;
	char b2[4];	// used for 2 , 4 bytes input
	unsigned short in_i;
	*size=0;
	res=fread((char *)&in_i,2,1,fi);
	if(res==1) {
		if(in_i==0xFEFF) {
			MESG("A utf16 BOM file!");
			// find the size (out_bytes)
			while((res=fread((char *)&in_i,2,1,fi))==1) {
				unsigned int c,d;
				char wchar[5];
				// printf("%4X\n",in_i);
				in_bytes+=2;
				in_chars++;
			    c= in_i;
				
		        if ((c & 0xFC00) == 0xD800) {    /* surrogates */
					res=fread((char *)&in_i,2,1,fi);
					in_bytes+=2;
			    	if (res!=1) {  break; };         /* shouldn't happens */
					d = in_i; // second part of surrogate
			    
		            if ((d & 0xFC00) == 0xDC00) {
		                c &= 0x03FF;
		                c <<= 10;
		                c |= d & 0x03FF;
		                c += 0x10000;
		            } else {
						MESG("error in surrogate!");
						c = '?';
			        	// return(-2);
			    	}
		        }

				/* assertion: c is a single UTF-4 value */
				int i=0;
#if	0
		        if      (c <    0x80) {  wchar[i++]=  c; }
        		else if (c <   0x800) {  wchar[i++]= ((c >>  6) & 0x1F) | 0xC0; }
		        else if (c < 0x10000) {  wchar[i++]= ((c >> 12) & 0x0F) | 0xE0; }
        		else                  {  wchar[i++]= ((c >> 18) & 0x07) | 0xF0; }
				wchar[i]=0;
				printf("%s",wchar);
#else
		        if      (c <    0x80) {  i=1 ;}
        		else if (c <   0x800) {  i=2 ;}
		        else if (c < 0x10000) {  i=3 ;}
        		else                  {  i=4 ;}
#endif
				out_bytes += i;
				out_chars ++;
			};

			// MESG("-----------------------------------");
			// MESG("input bytes %ld chars %ld, output bytes %ld chars %ld",in_bytes,in_chars,out_bytes,out_chars);

			// =======================================================================
			MESG("|----------------------------------------------------------------------|");
			*size = out_bytes;
			fseek(fi,2,0);// start after bom
			char *out=NULL;
			start_out = out = malloc(out_bytes+1);
			
			while((res=fread((char *)&in_i,2,1,fi))==1) {
				unsigned int c,d;
				char wchar[5];
				// printf("%4X\n",in_i);
				// in_bytes+=2;
				// in_chars++;
			    c= in_i;
				
		        if ((c & 0xFC00) == 0xD800) {    /* surrogates */
					res=fread((char *)&in_i,2,1,fi);
					// in_bytes+=2;
			    	if (res!=1) {  break; };         /* shouldn't happens */
					d = in_i; // second part of surrogate
			    
		            if ((d & 0xFC00) == 0xDC00) {
		                c &= 0x03FF;
		                c <<= 10;
		                c |= d & 0x03FF;
		                c += 0x10000;
		            } else {
						MESG("error in surrogate!");
						c = '?';
			    	}
		        }
				int bits;
				/* assertion: c is a single UTF-4 value */
		        if      (c <    0x80) {  *out++=  c;                bits= -6; }
		        else if (c <   0x800) {  *out++= ((c >>  6) & 0x1F) | 0xC0;  bits=  0; }
		        else if (c < 0x10000) {  *out++= ((c >> 12) & 0x0F) | 0xE0;  bits=  6; }
		        else                  {  *out++= ((c >> 18) & 0x07) | 0xF0;  bits= 12; }
		 
		        for ( ; bits >= 0; bits-= 6) {
		            // if (out >= outend)  break;
		            *out++= ((c >> bits) & 0x3F) | 0x80;
		        }

			};
			
			fclose(fi);
			// at this point we have the file in the buffer 'out' as utf8.

			fwrite(start_out,out-start_out,1,stdout);
			MESG("|----------------------------------------------------------------------|");
			MESG("input bytes %ld chars %ld, output bytes %ld chars %ld",in_bytes,in_chars,out_bytes,out_chars);
		} else {
			MESG("No bom 16 le found!");
			fclose(fi);
		};
	} else { 
		MESG("file smaller than bom size");
		fclose(fi);
	}
 } else {
 	MESG("file error opening %s",file_name);
 };
 return start_out;
}



size_t write_utf8_as_utf16(char *fname,unsigned char *in,size_t inlen)
{
 FILE *fout;

    // unsigned short* out = (unsigned short*) outb;
    const unsigned char* processed = in;
    // unsigned short* outstart= out;
    unsigned short* outend;
    unsigned char* inend= in+inlen;
    unsigned int c, d;
    int trailing;
    unsigned char *tmp;
    unsigned short tmp1, tmp2;

	unsigned short out_i=0xFEFF;

	if (in==NULL) return 0;
	fout=fopen(fname,"w");
	fwrite((char *)&out_i,2,1,fout);	// write BOM
	if (inlen<1) { fclose(fout);return 1;};	// zero length file written!

    while (in < inend) {
      d= *in++;
      if      (d < 0x80)  { c= d; trailing= 0; }
      else if (d < 0xC0) {
        /* trailing byte in leading position */
		// *outlen = (out - outstart) * 2;
		// *inlen = processed - in;
		break;
      } else if (d < 0xE0)  { c= d & 0x1F; trailing= 1; }
        else if (d < 0xF0)  { c= d & 0x0F; trailing= 2; }
        else if (d < 0xF8)  { c= d & 0x07; trailing= 3; }
      else {
		/* no chance for this in UTF-16 */
		// *outlen = (out - outstart) * 2;
		// *inlen = processed - in;
		break;
      }

      if (inend - in < trailing) {
          break;
      } 

      for ( ; trailing; trailing--) {
          if ((in >= inend) || (((d= *in++) & 0xC0) != 0x80))
	      	break;
          c <<= 6;
          c |= d & 0x3F;
      }

      /* assertion: c is a single UTF-4 value */
        if (c < 0x10000) {
			out_i = c;fwrite((void *)&out_i,2,1,fout);
        }
        else if (c < 0x110000) {
            c -= 0x10000;
	
			out_i = 0xD800 | (c >> 10);
			fwrite((void *)&out_i,2,1,fout);
			out_i = 0xDC00 | (c & 0x03FF);
			fwrite((void *)&out_i,2,1,fout);
        } else break;
		processed = in;
    }
    // *outlen = (out - outstart) * 2;
    // *inlen = processed - in;
	fclose(fout);
 return 1;
}

int main(int argc, char **argv)
{
 char *fname;
 char *buffer;
 char out_fname[256];

 size_t res;
	if(argc==2) {
		fname = argv[1];
	} else return 1;
	sprintf(out_fname,"%s.bck",fname);
	buffer=read_utf16_2_utf8(fname,&res);
	write_utf8_as_utf16(out_fname,buffer,res);	// write UTF-16 with BOM.

}
