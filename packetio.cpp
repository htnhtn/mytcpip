#include<types.h>
#include<packetio.h>
#include<windows.h>
HANDLE hSemp1 = CreateSemaphore(NULL,1,1,NULL); 

int sendFrame(const void*data,int len,int type,const void*dst,int i)
{
	static char buf[1518];
	static char mac[]={0x8C,0xC8,0x4B,0xA8,0x36,0x88};
	strncpy(buf+14,(char*)data,len);
	strncpy(buf,(char*)dst,6);
	strncpy(buf+6,mac,6);
	buf[12]=type>>8&255;
	buf[13]=type&255;
	//CRC
	pcap_sendpacket(devices[i].p,(u_char*)buf,len+18);
	return 0;
}

void packet_handler(u_char*callback,const pcap_pkthdr*header,const u_char*pkt_data)
{
	((frameReceiveCallback)callback)(pkt_data+14,header->len,0);
	WaitForSingleObject(hSemp1, INFINITE);
	for(int i=0;i<14;i++)
		printf("%02x ",pkt_data[i]);
	printf("len: %d\n",header->len);
	ReleaseSemaphore(hSemp1, 1, NULL);
}

// Will NOT return!!!!!!!!!!!!!!
int setFrameReceiveCallback(frameReceiveCallback callback)
{
	pcap_loop(devices[0].p, 0, packet_handler, (u_char*)callback);

	return 0;// Will NOT return!!!!!!!!!!!!!!
}
