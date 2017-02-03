#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUMTIMES 1024

void * thread_routine( void *data ) {
    char printchar;
    printchar = *( (char *)data );
    for( int i = 0; i < NUMTIMES; ++i ) {
        printf( "%c\n", printchar );
        for( int j = 0; j < 1024; ++j )
            for( int k = 0; k < 32768; ++k )
                ;
    }
    return NULL;
}

int main() {

    pthread_t t1, t2;
    char c1, c2;

    c1 = 'A'; c2 = 'B';

    pthread_create( &t1, NULL, ( void * (*)(void *) ) thread_routine, (void *)( &c1 ) );
    pthread_create( &t2, NULL, ( void * (*)(void *) ) thread_routine, (void *)( &c2 ) );

    pthread_join( t1, NULL );
    pthread_join( t2, NULL );

    return 0;
}
