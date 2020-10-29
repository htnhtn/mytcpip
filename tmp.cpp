#include<pcap.h>
char errbuf[PCAP_ERRBUF_SIZE];

int main()
{
	pcap_if_t*devs;
	if(pcap_findalldevs(&devs,errbuf))
		return puts(errbuf);

	for(pcap_if_t*i=devs;i;i=i->next)
		printf("%s\t%s\n",i->name,i->description);
}
