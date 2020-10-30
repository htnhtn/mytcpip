#define _CRT_SECURE_NO_WARNINGS
#include<cstring>
#include<pcap.h>
#include<device.h>
#include<types.h>
static void getip(const char*name,unsigned int*addr,unsigned int*mask)
{
	pcap_if_t*devs,*d;
	pcap_findalldevs(&devs,err);//因为已经activate了所以肯定会成功

	d=devs;
	while(strcmp(d->name,name))d=d->next;
	for(pcap_addr_t*a=d->addresses;a;a=a->next)
		if(a->addr->sa_family==AF_INET)
		{
			*addr=((struct sockaddr_in*)a->addr)->sin_addr.s_addr;
			*mask=((struct sockaddr_in*)a->netmask)->sin_addr.s_addr;
			//*board=((struct sockaddr_in*)a->boardaddr)->sin_addr.s_addr;
		}

	pcap_freealldevs(devs);
}

int addDevice(const char*name)
{
	int t=findDevice(name);
	if(~t)return t;

	DEVICES d;
	d.p=pcap_create(name,err);
	if(!d.p)return puts(err),-1;

	if(errno=pcap_activate(d.p))
	{
		pcap_close(d.p);
		printf("error! pcap_activate %d.\n",errno);
		return -1;
	}

	strcpy(d.name,name);
	getip(name,&d.addr,&d.mask);
	//printf("%u %u\n",d.addr,d.mask);//3151167498//14745599
	devices.push_back(d);
	return devices.size()-1;
}

int findDevice(const char*name)
{
	for(int i=0;i<devices.size();i++)
		if(!strcmp(devices[i].name,name))
			return i;
	return -1;
}
