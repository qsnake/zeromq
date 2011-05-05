//
//  Test PATCH socket type
//
#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

int main (void)
{
    void *context = zmq_init (1);

    //  Create three patch sockets
    void *sender = zmq_socket (context, ZMQ_PATCH);
    void *recip1 = zmq_socket (context, ZMQ_PATCH);
    void *recip2 = zmq_socket (context, ZMQ_PATCH);
    assert (sender);
    assert (recip1);
    assert (recip2);

    //  Connect both recipients to the sender
    zmq_bind (sender, "tcp://*:23000");
    zmq_connect (recip1, "tcp://localhost:23000");
    zmq_connect (recip2, "tcp://localhost:23000");
    sleep (1);

    //  Send a message, it must go to both recipients
    zmq_msg_t msg;

    printf ("Testing PATCH output:");
    zmq_msg_init_size (&msg, 4);
    memcpy (zmq_msg_data (&msg), "MSG1", 4);
    printf (" - sending test message...\n");
    zmq_send (sender, &msg, 0);
    zmq_msg_close (&msg);

    zmq_msg_init (&msg);
    printf (" - receiving copy 1...\n");
    zmq_recv (recip1, &msg, 0);
    zmq_msg_close (&msg);

    zmq_msg_init (&msg);
    printf (" - receiving copy 2...\n");
    zmq_recv (recip2, &msg, 0);
    zmq_msg_close (&msg);
    printf (" - OK - sending works successfully\n");

    printf ("Testing PATCH input:");

    zmq_msg_init_size (&msg, 4);
    memcpy (zmq_msg_data (&msg), "MSG2", 4);
    printf (" - sending test message 1...\n");
    zmq_send (recip1, &msg, 0);
    zmq_msg_close (&msg);

    zmq_msg_init_size (&msg, 4);
    memcpy (zmq_msg_data (&msg), "MSG3", 4);
    printf (" - sending test message 2...\n");
    zmq_send (recip2, &msg, 0);
    zmq_msg_close (&msg);

    int count;
    for (count = 0; count < 2; count++) {
        zmq_msg_init (&msg);
        printf (" - receiving message %d...\n", count);
        zmq_recv (sender, &msg, 0);
        zmq_msg_close (&msg);
    }
    printf (" - OK - receiving works successfully\n");

    zmq_close (sender);
    zmq_close (recip1);
    zmq_close (recip2);
    zmq_term (context);
    return 0;
}
