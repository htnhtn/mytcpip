#include<types.h>
#include<pcap.h>
#include<device.h>
#include<packetio.h>
int f(const void*a,int b,int c)
{
	return 0;
}
int main()
{
	int i=addDevice("\\Device\\NPF_{5077EC0E-85E0-4473-8842-50C8F6C4CF4E}");
	printf("%d\n",findDevice("\\Device\\NPF_{5077EC0E-85E0-4473-8842-50C8F6C4CF4E}"));
	puts(devices[i].name);
	printf("%s\n%s\n",iptos(devices[i].addr),iptos(devices[i].mask));
	setFrameReceiveCallback(f);
}
