
#include<types.h>

char err[PCAP_ERRBUF_SIZE];
int errno;
std::vector<DEVICES>devices;

/* 来自官方文档 */
/* 将一个unsigned long 型的IP转换为字符串类型的IP */
#define IPTOSBUFFERS     12 
char* iptos(u_long in)
{
    static char output[IPTOSBUFFERS][3 * 4 + 3 + 1];
    static short which;
    u_char* p;

    p = (u_char*)&in;
    which = (which + 1 == IPTOSBUFFERS ? 0 : which + 1);
    sprintf(output[which], "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
    return output[which];
}
