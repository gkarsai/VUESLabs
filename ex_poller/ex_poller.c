
//  Reading from multiple sockets
//  This version uses zmq_poll()

#include "zhelpers.h"
#include "assert.h"

int main (void)
{
    //  Connect to task hwclient
    void *context = zmq_ctx_new ();
    void *receiver = zmq_socket (context, ZMQ_REP);
    int rc = zmq_bind(receiver, "tcp://*:5555");
    assert (rc == 0);

    //  Connect to weather server
    void *subscriber = zmq_socket (context, ZMQ_SUB);
    zmq_connect (subscriber, "tcp://localhost:5556");
    zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, "10001 ", 6);

    //  Process messages from both sockets
    while (1) {
        char msg [256];
        zmq_pollitem_t items [] = {
            { receiver,   0, ZMQ_POLLIN, 0 },			/* Server (REQ/REP) */
            { subscriber, 0, ZMQ_POLLIN, 0 }			/* Subscriber (PUB/SUB) */
        };
        zmq_poll (items, 2, -1);
        if (items [0].revents & ZMQ_POLLIN) {
            int size = zmq_recv (receiver, msg, 255, 0);
            if (size != -1) {
                //  Process hwserver request
            }
        }
        if (items [1].revents & ZMQ_POLLIN) {
            int size = zmq_recv (subscriber, msg, 255, 0);
            if (size != -1) {
                //  Process weather update
            }
        }
    }
    zmq_close (subscriber);
    zmq_ctx_destroy (context);
    return 0;
}
