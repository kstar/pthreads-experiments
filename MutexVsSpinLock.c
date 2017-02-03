#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

pthread_spinlock_t lock;
pthread_mutex_t mutex;

void echo_input() {
    char ch;
    ch = getchar();
    fflush( stdin );
    while( getchar() != '\n' );
    printf( "You typed: %c\n", ch );
}

void *input_with_spinlock( void *data ) {

    /* Spin till we acquire lock */
    pthread_spin_lock( &lock );

    /* Call echo input */
    echo_input();

    /* Release lock */
    pthread_spin_unlock( &lock );

}

void *input_with_mutex( void *data ) {

    /* Block till we acquire mutex */
    pthread_mutex_lock( &mutex );

    /* Call echo input */
    echo_input();

    /* Release mutex */
    pthread_mutex_unlock( &mutex );
}

int main() {

    pthread_t t1, t2;

    /* Initialize locks */
    pthread_spin_init( &lock, PTHREAD_PROCESS_PRIVATE );
    pthread_mutex_init( &mutex, NULL );

    printf( "==== Using a spinlock ====\n" );

    /* Create threads */
    pthread_create( &t1, NULL, (void * (*)( void * )) input_with_spinlock, NULL );
    pthread_create( &t2, NULL, (void * (*)( void * )) input_with_spinlock, NULL );

    /* Wait for threads to finish */
    pthread_join( t1, NULL );
    pthread_join( t2, NULL );

    printf( "==== Using a mutex ====\n" );

    /* Create threads */
    pthread_create( &t1, NULL, (void * (*)( void * )) input_with_mutex, NULL );
    pthread_create( &t2, NULL, (void * (*)( void * )) input_with_mutex, NULL );

    /* Wait for threads to finish */
    pthread_join( t1, NULL );
    pthread_join( t2, NULL );

    /* Destroy locks */
    pthread_spin_destroy( &lock );
    pthread_mutex_destroy( &mutex );

    return 0;

}
