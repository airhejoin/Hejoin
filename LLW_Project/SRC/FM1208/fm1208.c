/*
*********************************************************************************
*                                 dsPIC30F4011
*                     ����MPLAB���ɿ�������,MCC30������Ӧ�ÿ�
*									 Ver2.0
*                                   �ʼҿƼ�
*
*ģ  �飺FM1208
*����ߣ�Andy
*ʱ  �䣺2011-03-21
*********************************************************************************
*/
#include"..\Lib\includes.h"
/*************    ��������Rats                    ****************************
�����ܡ�����ѡ��Ӧ�������
����������
�����ء���
��˵������IS014443-4Ҫ��
**************   ��ƣ����������� 2011-03-22  23:57:11***********************/
static INT8U FirstPark=0;
INT8U Rats(INT8U *ATS)
{
	INT8U	status=0;
	#ifdef DEBUGFM1208
	INT8U i=0;

	for(i=0;i<16;i++)
	   	S50.revbuffer[i]=0;
	#endif

	SPIWrite(CRCPresetLSB,0x63);
	SPIWrite(CWConductance,SETCWConductance);
	SPIWrite(ChannelRedundancy,0x0f);

	/*ѡ��Ӧ������*/
	S50.revbuffer[0] = 0xE0;
	S50.revbuffer[1] = 0x51;
	status = Command_Send(2, S50.revbuffer, Transceive);

    if(status==FALSE)     
    {
    	*ATS = 0;
		#ifdef DEBUGFM1208
		printf("Rats ERR:\r\n");
		#endif
    	return(FM1702_NOTAGERR);	
    } 
    else 
    { 
    	/* ��FIFO�ж�ȡӦ����Ϣ */
    	status = SPIRead(ErrorFlag);
		if((status & 0x02) == 0x02) return FM1702_PARITYERR;
		if((status & 0x04) == 0x04) return FM1702_FRAMINGERR;
		if((status & 0x08) == 0x08) return FM1702_CRCERR;
		status = SPIRead(FIFO_Length);
	       	
		if(status == 0x10)	
		{
			Read_FIFO(S50.revbuffer);
			FirstPark=0;
			#ifdef DEBUGFM1208
				printf("Rats S50.revbuffer:\r\n");
			   	for(i=0;i<16;i++)
			   		printf(" %02x ",S50.revbuffer[i]);
			   	printf("\r\n");
		   	#endif					  
			MemoryCopy(S50.revbuffer,ATS,16);
			return FM1702_OK;
		}
		else if(status == 0x04)	
		{
			Read_FIFO(S50.revbuffer);
			FirstPark=0;
			#ifdef DEBUGFM1208
				printf("Rats S50.revbuffer:\r\n");
			   	for(i=0;i<4;i++)
			   		printf(" %02x ",S50.revbuffer[i]);
			   	printf("\r\n");
		   	#endif
			MemoryCopy(S50.revbuffer,ATS,4);
			return FM1702_OK;
		}
		else
		{
			return FM1702_BYTECOUNTERR;
		}
    }
    return status; 
}
/*************    ��������Coscommand              ****************************
�����ܡ�������FCOS����
����������
�����ء���
��˵������
		send:	���ͻ������׵�ַ
		s_len:	���ͻ���������
		rece:	���ջ������׵�ַ
		r_len:  ���ջ���������
		mode:   1 ��ʾ������PCB+CID,������Ҫ����PCB+CID
**************   ��ƣ����������� 2011-03-23  11:21:51***********************/
INT8U Coscommand(INT8U *send,INT8U s_len,INT8U *rece,INT8U *r_len,INT8U mode)
{
	INT8U	status=0,len=0;
	INT8U   i=0;

	#ifdef DEBUGFM1208
		for(i=0;i<16;i++)
		   	S50.revbuffer[i]=0;
	#endif
	
	SPIWrite(CRCPresetLSB,0x63);
	SPIWrite(CWConductance,SETCWConductance);
	SPIWrite(ChannelRedundancy,0x0f);

	/***********ʹ���͸�ʽ����ISO14443A-4��׼************/
	if(1==mode)
	{
		for(i=0;i<s_len;i++)
		{
			S50.revbuffer[i]=send[i];
		}	
		len=s_len;
	}
	else
	{
		if(FirstPark)
		{
			FirstPark=0;
			S50.revbuffer[0]=0x0B;		
		}
		else
		{
			FirstPark=1;
			S50.revbuffer[0]=0x0A;
		}
		S50.revbuffer[1]=0x01;  

		/********************��֯����**************************/
		for(i=0;i<s_len;i++)
		{
			S50.revbuffer[i+2]=send[i];
		}	
		len=2+s_len;
	}
	status = Command_Send(len, S50.revbuffer, Transceive);
	
	if(status==FALSE)
	{
		*rece=0;
		*r_len=0;
		#ifdef DEBUGFM1208
			printf("Coscommand ERR1:\r\n");
			printf("Coscommand S50.revbuffer[0]:%d\r\n",S50.revbuffer[0]);
			printf("Coscommand S50.revbuffer[1]:%d\r\n",S50.revbuffer[1]);
		#endif
	}
	else
	{
        /* ��FIFO�ж�ȡӦ����Ϣ */
    	status = SPIRead(ErrorFlag);
		if((status & 0x02) == 0x02) return FM1702_PARITYERR;
		if((status & 0x04) == 0x04) return FM1702_FRAMINGERR;
		if((status & 0x08) == 0x08) return FM1702_CRCERR;
		status = SPIRead(FIFO_Length);
	    #ifdef DEBUGFM1208
			printf("Coscommand FIFO_Length:%d\r\n",status);
		#endif
		if(status == 0x10)	
		{
			Read_FIFO(S50.revbuffer);

			#ifdef DEBUGFM1208
				printf("Coscommand S50.revbuffer:\r\n");
			   	for(i=0;i<status;i++)
			   		printf(" %20x ",S50.revbuffer[i]);
			   	printf("\r\n");
		   	#endif
			*r_len=status;
			MemoryCopy(S50.revbuffer,rece,status);
			return FM1702_OK;
		}
		else if(status == 0x04)	
		{
			Read_FIFO(S50.revbuffer);
			
			#ifdef DEBUGFM1208
				printf("Coscommand S50.revbuffer:\r\n");
			   	for(i=0;i<status;i++)
			   		printf(" %02x ",S50.revbuffer[i]);
			   	printf("\r\n");
		   	#endif
			*r_len=status;
			MemoryCopy(S50.revbuffer,rece,status);
			return FM1702_OK;
		}
		else if(status == 0x0c)	
		{
			Read_FIFO(S50.revbuffer);
			
			#ifdef DEBUGFM1208
				printf("Coscommand S50.revbuffer:\r\n");
			   	for(i=0;i<status;i++)
			   		printf(" %02x ",S50.revbuffer[i]);
			   	printf("\r\n");
		   	#endif
			*r_len=status;
			MemoryCopy(S50.revbuffer,rece,status);
			return FM1702_OK;
		}
		else if(status == 0x1b)	
		{
			Read_FIFO(S50.revbuffer);
			
			#ifdef DEBUGFM1208
				printf("Coscommand S50.revbuffer:\r\n");
			   	for(i=0;i<status;i++)
			   		printf(" %02x ",S50.revbuffer[i]);
			   	printf("\r\n");
		   	#endif
			*r_len=status;
			MemoryCopy(S50.revbuffer,rece,status);
			return FM1702_OK;
		}
		else if(status == 0x0a)	
		{
			Read_FIFO(S50.revbuffer);
			
			#ifdef DEBUGFM1208
				printf("Coscommand S50.revbuffer:\r\n");
			   	for(i=0;i<status;i++)
			   		printf(" %02x ",S50.revbuffer[i]);
			   	printf("\r\n");
		   	#endif
			*r_len=status;
			MemoryCopy(S50.revbuffer,rece,status);
			return FM1702_OK;
		}
		else if(status == 0x0b)	
		{
			Read_FIFO(S50.revbuffer);
			
			#ifdef DEBUGFM1208
				printf("Coscommand S50.revbuffer:\r\n");
			   	for(i=0;i<status;i++)
			   		printf(" %02x ",S50.revbuffer[i]);
			   	printf("\r\n");
		   	#endif
			*r_len=status;
			MemoryCopy(S50.revbuffer,rece,status);
			return FM1702_OK;
		}
		else if(status == 0x03)	
		{
			Read_FIFO(S50.revbuffer);
			
			#ifdef DEBUGFM1208
				printf("Coscommand S50.revbuffer:\r\n");
			   	for(i=0;i<status;i++)
			   		printf(" %02x ",S50.revbuffer[i]);
			   	printf("\r\n");
		   	#endif
			*r_len=status;
			MemoryCopy(S50.revbuffer,rece,status);

			return FM1702_OK;
		}
		else if(status == 0x16)	
		{
			Read_FIFO(S50.revbuffer);
			
			#ifdef DEBUGFM1208
				printf("Coscommand S50.revbuffer:\r\n");
			   	for(i=0;i<status;i++)
			   		printf(" %02x ",S50.revbuffer[i]);
			   	printf("\r\n");
		   	#endif
			*r_len=status;
			MemoryCopy(S50.revbuffer,rece,status);

			return FM1702_OK;
		}
		else if(status == 0x1a)	
		{
			Read_FIFO(S50.revbuffer);
			
			#ifdef DEBUGFM1208
				printf("Coscommand S50.revbuffer:\r\n");
			   	for(i=0;i<status;i++)
			   		printf(" %02x ",S50.revbuffer[i]);
			   	printf("\r\n");
		   	#endif
			*r_len=status;
			MemoryCopy(S50.revbuffer,rece,status);

			return FM1702_OK;
		}
		else
		{
			Read_FIFO(S50.revbuffer);
			
			#ifdef DEBUGFM1208
				printf("Coscommand S50.revbuffer:\r\n");
			   	for(i=0;i<status;i++)
			   		printf(" %02x ",S50.revbuffer[i]);
			   	printf("\r\n");
		   	#endif
			*r_len=status;
			MemoryCopy(S50.revbuffer,rece,status);

			return FM1702_OK;
		}	
	}
	return status; 
}
/*************    ��������WaitFM1208Ready          ***************************
�����ܡ���
����������
�����ء���
��˵������
		���յ�FM1208����FA 01 01 ����ʾFM1208������ʱ��������������ֱ�ӻش���FM1208��
		ֱ�����յ�90 00
**************   ��ƣ����������� 2011-04-06  12:00:05***********************/
void WaitFM1208Ready(void)
{
	INT8U data[3]={0xfa,0x01,0x01};
	INT8U redata[3]={0};
	INT8U len=0;
	#ifdef DEBUGFM1208
	   INT8U i=0;
	#endif
	while((S50.revbuffer[0]==0xfa)&&(S50.revbuffer[1]==0x01))
	{
		S50.revbuffer[0]=0;
		S50.revbuffer[1]=0;
		Coscommand(data,3,redata,&len,1);	
		vTaskDelay(50);
		#ifdef DEBUGFM1208
			printf("������ʱ:\r\n");
			for(i=0;i<3;i++)
			   	printf(" %02x ",S50.revbuffer[i]);
			printf("\r\n");
		#endif
	}
}

