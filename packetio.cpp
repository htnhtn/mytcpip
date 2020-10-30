#include<types.h>
#include<packetio.h>
#ifdef WIN32
#include<windows.h>
HANDLE hSemp1 = CreateSemaphore(NULL,1,1,NULL);
#else
#include<cstring>
#include<malloc.h>
#include<pthread.h>
#include<semaphore.h>
sem_t mutex;
#endif

static frameReceiveCallback handle;

static void packet_handler(u_char*id,const pcap_pkthdr*header,const u_char*pkt_data)
{
	if(((frameReceiveCallback)handle)(pkt_data+14,header->len,(int)(intptr_t)id))
		puts("callback error");
#ifdef WIN32
	WaitForSingleObject(hSemp1, INFINITE);
#else
	sem_wait(&mutex);
#endif
	for(int i=0;i<14;i++)
		printf("%02x ",pkt_data[i]);
	printf("len: %d\n",header->len);
	if( pkt_data[12]==0x08 && pkt_data[13]==0x06 )
	{
		for(int i=14;i<42;i++)
			printf("%02x ",pkt_data[i]);
		puts("");
		for(int i=42;i<60;i++)
			printf("%02x ",pkt_data[i]);
		putchar('\t');
		for(int i=60;i<64;i++)
			printf("%02x ",pkt_data[i]);
		puts("");
	}
#ifdef WIN32
	ReleaseSemaphore(hSemp1, 1, NULL);
}

DWORD WINAPI ThreadProc1(__in  LPVOID lpParameter)
{
	pcap_loop(
		devices[(int)lpParameter].p,
		0,
		packet_handler,
		(u_char*)lpParameter);
	return 0;
}

#else
	sem_post(&mutex);
}

void*thread(void*id)
{
	pcap_loop(
		devices[(int)(intptr_t)id].p,
		0,
		packet_handler,
		(u_char*)id);
}

#endif

int setFrameReceiveCallback(frameReceiveCallback callback)
{
	handle=callback;
	return 0;
}

void start()
{
#ifdef WIN32
	HANDLE h[100];
	for(int i=0;i<devices.size();i++)
		h[i]=CreateThread(NULL,0,ThreadProc1,(LPVOID)i,0,NULL);

	/* 资源回收 */
	WaitForMultipleObjects(devices.size(),h,TRUE,INFINITE);
	//GetLastError();
	for(int i=0;i<devices.size();i++)
		CloseHandle( h[i] );
	CloseHandle( hSemp1 );
#else
	sem_init(&mutex,0,1);
	pthread_t tid[100];
	for(int i=0;i<devices.size();i++)
		pthread_create(tid+i,NULL,thread,(void*)i);

	for(int i=0;i<devices.size();i++)
		pthread_join(tid[i],NULL);
#endif
}

/////////  receive ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
/////////  send	   ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓

int sendFrame(const void*data,int len,int type,const void*dst,int i)
{
	static u_char buf[1518];
	//static char mac[]={0x8C,0xC8,0x4B,0xA8,0x36,0x88};
	memcpy(buf+14,data,len);
	memcpy(buf,dst,6);
	memcpy(buf+6,devices[i].mac,6);
	buf[12]=type>>8&255;//0x0806 ARP 0x0800 IP
	buf[13]=type&255;
	//*(int*)(buf+len+14)=CRC
	pcap_sendpacket(devices[i].p,buf,len+18);
	return 0;
}
