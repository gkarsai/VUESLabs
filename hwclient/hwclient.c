//  Hello World client
#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main (void)
{
    printf ("Connecting to hello world server…\n");
    void *context = zmq_ctx_new ();
    void *requester = zmq_socket (context, ZMQ_REQ);	/* Create socket */
    zmq_connect (requester, "tcp://localhost:5555");	/* Connect to server on same host */

    int request_nbr;
    for (request_nbr = 0; request_nbr != 10; request_nbr++) {
        char buffer [10];
        printf ("Sending Hello %d…\n", request_nbr);
        zmq_send (requester, "Hello", 5, 0);			/* Send message of 5 bytes */
        zmq_recv (requester, buffer, 10, 0);			/* Receive message of 10 bytes */
        printf ("Received World %d\n", request_nbr);
    }
    zmq_close (requester);								/* Close socket */
    zmq_ctx_destroy (context);
    return 0;
}
