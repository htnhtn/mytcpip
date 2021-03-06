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

static void getmac(u_int ip,char*mac)
{
#ifdef WIN32
	FILE*fp=_popen("ipconfig /all","r");
	char tmp1[18];int p1=0;
	char tmp2[15];int p2=0;
	char tmp3[18];
	char*s=iptos(ip);
	while(!feof(fp))
	{
		char c=fgetc(fp);
		if((c>='0'&&c<='9' ||
			c>='A'&&c<='Z' ||
			c=='-')&& p1<17 )
		{
			tmp1[p1++]=c;
			if(p1==17)
				tmp1[17]='\0',
				strcpy(tmp3,tmp1);
		}
		else p1=0;
		if((c>='0'&&c<='9' ||
			c=='.')&& p2<14 )
			tmp2[p2++]=c;
		else {
			tmp2[p2]='\0';
			p2=0;
			if(!strcmp(tmp2,s))
			{
				int a[6];//puts(tmp3);
				sscanf(tmp3,"%x-%x-%x-%x-%x-%x",a,a+1,a+2,a+3,a+4,a+5);
				for(int i=0;i<6;i++)
					mac[i]=a[i];
				return;
			}
		}
	}
	puts("error!get mac");
	while(true);
#else
	int a[6];
	FILE*fp=popen("ifconfig|grep ether|awk '{print $2}'","r");
	fscanf(fp,"%x:%x:%x:%x:%x:%x",a,a+1,a+2,a+3,a+4,a+5);
	for(int i=0;i<6;i++)
		mac[i]=a[i];
#endif
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
	getmac(d.addr,d.mac);
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
