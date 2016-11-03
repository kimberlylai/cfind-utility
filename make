cfind : cfind.o parsedate.o statexpr.o
    cc -std=c99 -Wall -pedantic -Werror -o cfind cfind.o parsedate.o statexpr.o
cfind.o : cfind.c statexpr.h
    cc -std=c99 -Wall -pedantic -Werror -c cfind.c
parsedate.o : parsedate.c statexpr.h
    cc -std=c99 -Wall -pedantic -Werror -c parsedate.c
statexpr.o : statexpr.c statexpr.h
    cc -std=c99 -Wall -pedantic -Werror -c statexpr.c
