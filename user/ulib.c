#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"
#include "param.h"
#include "syscall.h"
#include "traps.h"
#include "fs.h"
#define PGSIZE 4096


int lock_init(lock_t *lk)
{
	lk->flag = 0;
	return 0;
}

void lock_acquire(lock_t *lk){
	while(xchg(&lk->flag, 1) != 0)
	    ;
}

void lock_release(lock_t *lk){
	xchg(&lk->flag, 0);
}


int
thread_create(void (*start_routine)(void*), void *arg)
{
	lock_t lk;
	lock_init(&lk);
	lock_acquire(&lk);
	void *stack= malloc(PGSIZE*2);
	lock_release(&lk);

	if((uint)stack % PGSIZE)
		stack = stack + (PGSIZE - (uint)stack % PGSIZE);

	int result = clone(start_routine,arg,stack);
	return result;
}

int thread_join(){
	void *stack = malloc(sizeof(void*));
	int result= join(&stack);

	lock_t lk;
	lock_init(&lk);
	lock_acquire(&lk);
	free(stack);
	lock_release(&lk);

	return result;
}

char*
strcpy(char *s, char *t)
{
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
    ;
  return os;
}

int
strcmp(const char *p, const char *q)
{
  while(*p && *p == *q)
    p++, q++;
  return (uchar)*p - (uchar)*q;
}

uint
strlen(char *s)
{
  int n;

  for(n = 0; s[n]; n++)
    ;
  return n;
}

void*
memset(void *dst, int c, uint n)
{
  stosb(dst, c, n);
  return dst;
}

char*
strchr(const char *s, char c)
{
  for(; *s; s++)
    if(*s == c)
      return (char*)s;
  return 0;
}

char*
gets(char *buf, int max)
{
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
    cc = read(0, &c, 1);
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
      break;
  }
  buf[i] = '\0';
  return buf;
}




int
stat(char *n, struct stat *st)
{
  int fd;
  int r;

  fd = open(n, O_RDONLY);
  if(fd < 0)
    return -1;
  r = fstat(fd, st);
  close(fd);
  return r;
}

int
atoi(const char *s)
{
  int n;

  n = 0;
  while('0' <= *s && *s <= '9')
    n = n*10 + *s++ - '0';
  return n;
}

void*
memmove(void *vdst, void *vsrc, int n)
{
  char *dst, *src;
  
  dst = vdst;
  src = vsrc;
  while(n-- > 0)
    *dst++ = *src++;
  return vdst;
}
int getSize (char * s) {
    char * t;    
    for (t = s; *t != '\0'; t++)
        ;
    return t - s;
}





/*
itoa example
int i = 10;
char buffer [33];
itoa (i,buffer,ITOA_DECIMAL_VALUE);
printf ("decimal: %s\n",buffer);
itoa (i,buffer,ITOA_HEX_VALUE);
printf ("hexadecimal: %s\n",buffer);
itoa (i,buffer,ITOA_BINARY_VALUE);
printf ("binary: %s\n",buffer);
*/

//convert number to char *
char * itoa( int value, char * str, int base )
{
    char * rc;
    char * ptr;
    char * low;
    // Check for supported base.
    if ( base < 2 || base > 36 )
    {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    // Set '-' for negative decimals.
    if ( value < 0 && base == 10 )
    {
        *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do
    {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while ( low < ptr )
    {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}