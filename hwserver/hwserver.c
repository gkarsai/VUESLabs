//  Hello World server

#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

int main (void)
{
    //  Socket to talk to clients
    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_REP);	/* Create socket */
    int rc = zmq_bind (responder, "tcp://*:5555");		/* Bind socket on all interfaces to port 5555 */
    assert (rc == 0);

    while (1) {											/* Loop forever */
        char buffer [10];
        zmq_recv (responder, buffer, 10, 0);			/* Receive 10 byte message */
        printf ("Received Hello\n");
        sleep (1);          //  Do some 'work'
        zmq_send (responder, "World", 5, 0);			/* Send 5 byte response */
    }
    return 0;
}
