//#define HAVE_REMOTE
#define _CRT_SECURE_NO_WARNINGS
#include<vector>
#include<pcap.h>
extern char err[PCAP_ERRBUF_SIZE];
extern int errno;

struct DEVICES{
	pcap_t*p;
	char name[100];
	unsigned int addr,mask;
	char mac[6];
};

extern std::vector<DEVICES>devices;

char*iptos(unsigned long);
