C99 = cc -std=c99
CFLAGS = -Wall -pedantic -Werror

cfind : cfind.c read.o ../cslinux/adhoc/lib/libstatexpr-osx.a
  $(C99) $(CFLAGS) -o cfind cfind.c read.o
read.o : cfind.h read.c ../cslinux/adhoc/lib/libstatexpr-osx.a
  $(C99) $(CFLAGS) -c -o read.o read.c 
