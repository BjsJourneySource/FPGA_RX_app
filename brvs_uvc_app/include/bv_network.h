#ifndef __NETWORK_H__
#define __NETWORK_H__

#define BROADCAST_PORT     (9999)

#define BROADCAST_SERVER_MASK    (0x42525653)
#define BROADCAST_CLIENT_MASK    (0x62727673)

struct network_server_info{
	unsigned int mask;
    char name[32];
    char ip[16];
    short port;
    char mac[6];
};

struct network_client_info{
	unsigned int mask;
    char name[32];
    char ip[16];
    short port;
    char mac[6];
};

#endif //__NETWORK_H__
