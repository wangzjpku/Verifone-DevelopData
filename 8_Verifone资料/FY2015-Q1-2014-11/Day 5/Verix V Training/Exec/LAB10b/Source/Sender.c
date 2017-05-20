/*****************************************************************
*
*   Sender.C
*
*   Sample Program for  UCL
*
*****************************************************************/

#include <stdio.h>		
#include <string.h>
#include <svc.h>
#include <uclfactory.h>       // Header file for UCL Factory class
#include "tcpinterface.h"       // Header file for UCL Factory class
#include "verixtimer.h"       // Header file for Lib defined VerixCommTimer class  
#include "..\include\appobj.h"         // Header file for ApplicationObj class
#include "netsetup.h"
#include "vsocket.h"
#include "inet.h"
#include "hostdata.h"

//#include "logsys.h"  
//#include "eeslapi.h"  
//#include "vtcpip.h"
#include <errno.h>

int Socket_Connect(void);
short DoSendReceive(int );
void SocketProgram(void);
void msg_display(char *);



extern unsigned int CommunicateWithServer(void);

short szBufferLen[] = {1,10,11,127,128,245,257,500,512,940,990,999,1100,1200,1300,1400};
//short szBufferLen[] = {128,245,255,257,300,350,400,450,500,512};
//short szBufferLen[] = {1400,1455,1499,1800,2000,2500,2995};
//short szBufferLen[] = {2500,2600,2700,2800,2900,2998};

short szMedia[] ={1,0,1,0,1,1,0,1,0,1,0};
char HostResponse = 'N';
UclFactory *uclFactory;
Ucl* comnObj;
CommTimer *timer;
AppObj *llAppObj;
AppObj *gsmAppObj;
AppObj *gprsAppObj;
char media[10]={0};
char hostIP[20]={0};
char port[10]={0};
int totalComm= 0;
int successComm = 0;
short count =0;
char protocol[5]={'\0'};
unsigned long waitTime=0l;
struct sockaddr_in socket_host;

extern unsigned char TxBuffPkt[];
extern unsigned char RxBuffPkt[];

void msg_display(char *sz_msg )
{
    short len = strlen(sz_msg);
	char temp[21];
	short i;
	short l = 1;
	short y = 1;

		l += len/21;
		
		for (i=0;i<l; i++)
		{
			memset(temp, 0, 21);
			strncpy(temp,(char*)(sz_msg+(i*21)), 21);
			y = 1+i;
			if( y > 8)
				y = 1;
			len = strlen(temp);
			write_at( temp, len, 1,y);
		}
	

    SVC_WAIT(2000);
}


unsigned int CommunicateWithServer(void)
{
    short  retVal;
    char buff[50];
//	int conHandle;
	char version[10];
	short time=0;
	unsigned long complTime = 0;
	unsigned long startTime = 0;
	char model[MAX_MODEL_SIZE] = {'\0'};


    uclFactory = CreateUclFactory();
    	    
    //  Creating object for VerixCommTimer structure
 	timer = (CommTimer*)CreateVerixCommTimer();

	memset(version,0,10);
	getversion(version,10);
	sprintf(buff, "TCP/IP Ver. %s",version);
	Nprintf(buff);

	get_env("#MODEL",model,sizeof(model));


	if(get_env("#MEDIA",media,10) <0)
		write_at("No Media Provided,\nUsing Default Media",strlen("No Media Provided,\nUsing Default Media"),1,1);

	if(strcmp(media,"ETHERNET") == 0)
	{
		llAppObj = CreateAppObj(4); // for landline
		comnObj = uclFactory->Create(COM3, &retVal, (ApplicationObj *)llAppObj,ETHERNET);
	}
	
    sprintf(buff, "Using %s media %s Model Create Result %d",media, model,retVal);
	Nprintf(buff);

	retVal = AssignUcltoTCP(comnObj,timer,50000,60000,30000UL,15000UL,50000,50000);
	retVal = SetTimeout(SEND_TIMEOUT,50UL);
	retVal = SetTimeout(RECEIVE_TIMEOUT,80UL);
	while(1)
	{	
		memset(buff,0,sizeof(buff));
		startTime = complTime = time = 0;
		{
			sprintf(buff, "\fStarting Comm...%d",totalComm);
			Nprintf("Starting Trans %d",totalComm);
			SVC_WAIT(700);
			startTime = read_ticks();
			SocketProgram();
			complTime = read_ticks() - (startTime + waitTime +1700);//Beacuse of the waits in the functions
			time = complTime/1000;
			Nprintf("Transacation Time is %d sec.",time);
			sprintf(buff, "\fTrans Time is %d",time);
            if (HostResponse != 'N')
                break;

			SVC_WAIT(1000);
			totalComm++;
		}
	}
	return 0;

}

void SocketProgram(void)
{
	short  retVal = -1; 
	short errNo = -1;
	netparams net_param;
	int handle;
	int nCount = 0 ;
	char username[30]={0};
	char password[30]={0};
	char subnet[20]={0};
	char buff[100]={0};
	char phoneNo[MAX_PHNO_LENGTH]={0};
	char apn[MAX_APN_LENGTH]={0};
	short temp = 0;
	unsigned long t1;
	char response[15]={0};


	if (get_env("#USER",username,sizeof(username)) <0)
		printf("Configure user Name");

	if (get_env("#PASSWORD", password,sizeof(password)) <0)
		printf("Configure password");

	if (get_env("#HOSTIP", hostIP, sizeof(hostIP)) <0)
		printf("Configure IP of the Host to Conenct");

	if (get_env("#PORT", port,sizeof(port)) <0)
		printf("Configure port No. of the Service");

	if (get_env("#SUBNETMASK", subnet,sizeof(subnet)) <0)
		printf("Configure subnet mask of the Host");

	if (get_env("#APN", apn,sizeof(apn)) <0)
		printf("Configure APN");

	if (get_env("#PROTOCOL", protocol,sizeof(protocol)) <0)
		printf("Configure Protocol");


	 get_env("#PHONE",phoneNo,MAX_PHNO_LENGTH);
	
 	 strcpy(net_param.ipAddress ,"0.0.0.0");
	 strcpy(net_param.ipSubnetMask ,subnet);
	 net_param.datalinkProtocol = DL_PPP_CO_ETH ;

	 setparam(GATEWAY,"10.64.88.1",strlen("10.64.88.1"));
	 setparam(DNSPRI,"10.64.96.32",strlen("10.64.96.32"));
	 setparam(DNSSEC,"10.64.8.8",strlen("10.64.8.8"));

	 retVal = 0;
	 if(successComm <1)
	 {
		retVal = netconfig(&net_param);
		Nprintf("netconfig Returned %d",retVal);
	 }

	 if(retVal >=0)
	 {
		retVal = netconnect(1);
		Nprintf("netconnect Returned %d",retVal);
		Nprintf("Continuing...");
    //    SVC_WAIT(1500);
		if(retVal >=0)
		{
			handle=Socket_Connect();	
			if(handle >=0)
			{
				int i=0;
				errNo = DoSendReceive(handle);
				Nprintf("DoSendReceive Returned %d ",errNo);
			}
			else
				msg_display("\fSocket Error");
		}
		else
			msg_display("\fError in connecting to Host");			 
	 }
	 else
		 msg_display("\fError in Configuring Parameters");			  		 
	 
     if(retVal >= 0 ){
	     retVal = closesocket(handle);
         Nprintf("closesocket Returned %d",retVal);
	     yield();
     }
	 
	 retVal = netdisconnect(1);	
	 Nprintf("netdisconnect Returned %d",retVal);

	 if((retVal >=0) && (errNo >=0))
	 {
		SVC_WAIT(1000);
		Nprintf("Successful Trans. %d",successComm);
		successComm++;
	 }
	
     t1= read_ticks();
	 for(temp = 0; temp<=2; temp++)
	 {
		 yield();
  		 SVC_WAIT(1000);
	 }
	 waitTime = read_ticks() - t1;
	 return ;
 
}


int Socket_Connect(void)
{
   int handle,retVal = 0;
   short Port = 0;

   memset( &socket_host, 0, sizeof(socket_host));
   Port = atoi(port);

   socket_host.sin_family = AF_INET;
   memset(socket_host.sin_zero,0,8);
   socket_host.sin_addr.s_addr = htonl(inet_addr(hostIP));
   socket_host.sin_port = htons(Port);

   if (strcmp(protocol,"TCP") == 0)
   {
		handle = socket(AF_INET,SOCK_STREAM,0);
	 	Nprintf("Socket Handle for %s socket is  %d",protocol, handle );
		retVal = connect(handle,(struct sockaddr *)&socket_host,sizeof(struct sockaddr_in));
		if(retVal < 0)
			Nprintf("socket connect errno is %d",errno);
		else
	 		Nprintf("socket connect Returned %d",retVal);
   }
   else			//For UDP
   {
		handle = socket(AF_INET,SOCK_DGRAM,0);
		Nprintf("Socket Handle for %s scoket is  %d",protocol, handle );
   }


   if(retVal >=0)
	   return handle;
	else
		return retVal;
}

short DoSendReceive(int handle)
{
	short retVal = 0;
	int iServerLen;
	short bufferLen = 3000;
	unsigned long stTime =0;
	short time =0;
	char buffer[50] = {'\0'};



	iServerLen = sizeof(socket_host);

	if(strcmp(protocol,"TCP") == 0)
		retVal = send(handle,TxBuffPkt,strlen((char*)TxBuffPkt),0);
	else
		retVal = sendto( handle, TxBuffPkt, strlen((char*)TxBuffPkt), 0, (struct sockaddr *)&socket_host, sizeof(socket_host)) ;

	if(retVal <0)
		Nprintf("Sent errno is %d ",errno);
	else
	{
		Nprintf("Sent %d Bytes ",retVal);
		if(strcmp(protocol,"TCP") == 0)
		{
			retVal = SetTimeout(RECEIVE_TIMEOUT,7000);
			retVal = SetTimeout(SEND_TIMEOUT,2800);
		//	SVC_WAIT(1000);
			stTime = read_ticks();
			retVal = recv(handle,RxBuffPkt,szBufferLen[count]+1,0);

			time = (read_ticks() - stTime)/1000;
		}
		else
		{
			retVal = recvfrom( handle, RxBuffPkt, szBufferLen[count]+1, 0, (struct sockaddr *)&socket_host, &iServerLen ) ;
		}
	}

	if(retVal <0)
		Nprintf("Receive errno is %d ",errno);
	else
	{
		sprintf(buffer,"Recv retval is %d Data len is %d",retVal, strlen((char *)RxBuffPkt));
		Nprintf(buffer);
        HostResponse = 'Y';
	}

    count++;

	return retVal;
}




