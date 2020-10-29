#define HAVE_REMOTE
#include<pcap.h>
char err[PCAP_ERRBUF_SIZE];

int main()
{
	pcap_if_t*devs;
	//if(pcap_findalldevs_ex(PCAP_SRC_IF_STRING,NULL,&devs,err))
	if(pcap_findalldevs(&devs,err))
		return puts(err);

	for(pcap_if_t*i=devs;i;i=i->next)
		printf("%s\t%s\n",i->name,i->description);

	if(!pcap_init(PCAP_CHAR_ENC_UTF_8,err))
		return puts(err);

	char*src=devs->next->name;
	pcap_t*p=pcap_create(src,err);
	if(!p)return puts(err);

	pcap_freealldevs(devs);

	int errno;
	if(errno=pcap_activate(p))
		return pcap_close(p),printf("error! pcap_activate %d.\n",errno);

	pcap_t*fp=pcap_open_offline("savefile",err);
	if(!fp)return puts(err);

	int*t;
	printf("%d link-layer header types.\n",pcap_list_datalinks(p,&t));
	pcap_free_datalinks(t);

	pcap_pkthdr h;
	pcap_next(p,&h);
	printf("%ld %ld %d %d\n",h.ts.tv_sec,h.ts.tv_usec,h.caplen,h.len);
}
