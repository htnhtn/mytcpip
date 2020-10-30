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
	char device[]=
#ifdef WIN32
		"\\Device\\NPF_{5077EC0E-85E0-4473-8842-50C8F6C4CF4E}";
#else
	"eth0";
#endif
	int i=addDevice(device);
	printf("%d\n",findDevice(device));
	puts(devices[i].name);
	for(int j=0;j<6;j++)printf("%02x ",(u_char)devices[i].mac[j]);
	printf("\n%s\n%s\n",iptos(devices[i].addr),iptos(devices[i].mask));
	setFrameReceiveCallback(f);

	start();
}
