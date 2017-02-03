#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define LARGEN 1024

pthread_spinlock_t lock;

void * dump_large_text( void *data ) {
    int capital = *( (int *) data );

    for( int i = 0; i < LARGEN; ++i )
        putchar( rand()%26 + ( capital ? 'A' : 'a' ) );

    return NULL;
}

void *locked_dump_large_text( void *data ) {
    /* Wait till we acquire the lock */
    pthread_spin_lock( &lock );

    /* Write the text */
    dump_large_text( data );

    /* Release the lock */
    pthread_spin_unlock( &lock );

    return NULL;
}

int main() {
    pthread_t t1, t2;
    int capital = 1, lowercase = 0;

    printf("Without locks: \n");
    pthread_create( &t1, NULL, ( void * (*)( void * ) ) dump_large_text, (void *)( &capital ) );
    pthread_create( &t2, NULL, ( void * (*)( void * ) ) dump_large_text, (void *)( &lowercase ) );

    pthread_join( t1, NULL );
    pthread_join( t2, NULL );

    putchar( '\n' );

    putchar( '\n' );
    printf( "With locks: \n" );
    pthread_spin_init( &lock, PTHREAD_PROCESS_PRIVATE );
    pthread_create( &t1, NULL, ( void * (*)( void * ) ) locked_dump_large_text, (void *)( &capital ) );
    pthread_create( &t2, NULL, ( void * (*)( void * ) ) locked_dump_large_text, (void *)( &lowercase ) );

    pthread_join( t1, NULL );
    pthread_join( t2, NULL );

    putchar( '\n' );

    return 0;
}
