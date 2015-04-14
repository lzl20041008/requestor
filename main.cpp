#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const int BUFLEN = 10000;

struct in6_addr addr_remote;

static void usage()
{
    fprintf(stderr, "Usage: requestor <REMOTE_IPv6_ADDR>\n");
    exit(1);
}

int fd;

void tcp_init()
{
	if ((fd = socket(AF_INET6, SOCK_STREAM, 0)) == -1) {  
		fprintf(stderr, "init_fd: failed to create socket: %m\n");
		exit(0);
	}
    struct sockaddr_in6 serv_addr;
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_port = htons(547);
    serv_addr.sin6_addr = addr_remote;
    if (connect(fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        exit(0);
    }
}

int main(int argc, char **argv)
{
    if (argc < 2)
        usage();
	printf("REMOTE_IP_ADDR: %s\n", argv[argc - 1]);
	inet_pton(AF_INET6, argv[argc - 1], &addr_remote);

    tcp_init();
    char buf[BUFLEN];
    do {
        int count = read(fd, buf, BUFLEN);
        if (count <= 0) {
            printf("Connection lost, QUIT!\n");
            break;
        }
        printf("read %d bytes: ", count);
        for (int i = 0; i < count; ++i) {
            printf("%02x ", 0xff&buf[i]);
            if (i && i % 20 == 0) printf("\n          ");
        }
        putchar('\n');
        
    } while (1);
}
