/** To play the output .raw file using aplay run:
  * $aplay -t raw -c 2 -f cd -r 44100 output.raw */

#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctime>

#include "kfsys_interface.h"
#include "aave_interface.h"
#include "alsa_interface.h"
#include "test.h"


#define PORT 34492


void shutdown(int signum) 
{
    // exit in response to a SIGTERM signal, flushing stdout/stderr
    exit(0);
}


int socket_init()
{
    struct sockaddr_in addr;
    int s;
    socklen_t slen;

    /* Create a UDP socket. */
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        printf("socket");
        exit(1);
    }
    
    /* non-blocking recv */ 
    fcntl(s, F_SETFL, fcntl(s, F_GETFL) | O_NONBLOCK);

    memset(&addr, 0, sizeof(addr));     
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
     
    /* Bind socket to port. */
    if (bind(s, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
        printf("bind");
        exit(1);
    }
    return s;
}


int main()
{
    signal(SIGTERM, shutdown);

    int socket = socket_init();
    struct sockaddr_in addr;
    socklen_t slen;
    
    short buff[BUFFLEN * 2];
    memset(buff, 0, BUFFLEN * 2);

    KFSystem sys;    
    Alsa alsa;
    
    struct timeval t1, t2;

    alsa.setup(44100, 2, 8192);
    int alsa_bufflen = alsa.avail();
    printf("bufflen: %i\n", alsa_bufflen);

//	alsa.setup_default();   
    
    int recv_len, avail;
    char *recv_buff;
    recv_buff = (char *) malloc(8192);
    
	/* Aave latency compensation. */
	//delay += 333;  //mit
    //sys.delay = delay;
	//delay += -598;  //cipic
	//delay += -1124; //listen
	//delay += -2169; //tub
	
	// aave params not included in the comunication protocol
	// set_gain(8);
	// get_aave_engine()->reverb->level = 0.3;

    //int first_packet = 0;

    printf("waiting for udp packet at port %i\n", PORT);
    //int cnt = 44100 * 10 / 1024;
    while (1) {

        recv_len = recvfrom(socket, recv_buff, 8192, 0, (struct sockaddr*) &addr, &slen);
        
        //gettimeofday(&t1, NULL);
        
        if (recv_len > 0) {
        	sys.handle_datagram(recv_buff, recv_len);
        }
        
        avail = alsa.avail();
        
        if (avail > BUFFLEN) {        
            sys.render(buff, BUFFLEN);
            alsa.write(buff, BUFFLEN);
        }
        
        //gettimeofday(&t2, NULL);
        
        //int elapsed = (t2.tv_sec - t1.tv_sec) * 1000 + (t2.tv_usec - t1.tv_usec)/1000;
        //if (elapsed > 1)
        //    printf("\nprocess datagram in %d msecs\n", elapsed);
        
        
    }
    alsa.shutdown();
}
