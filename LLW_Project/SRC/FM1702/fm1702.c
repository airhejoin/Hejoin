/*
*********************************************************************************
*                                 dsPIC30F4011
*                     ����MPLAB���ɿ�������,MCC30������Ӧ�ÿ�
*									 Ver1.1
*                                   �ʼҿƼ�
*
*ģ  �飺FM1702
*����ߣ�Andy
*ʱ  �䣺2010-06-10
*********************************************************************************
*/
#include"..\Lib\includes.h"
#include "usb_type.h"


S_CARD S50;
//IO�ڻ�������
/***********************************/
#define	RF_RST_H()				  GPIO_SetBits(GPIOB, GPIO_Pin_0);
#define	RF_RST_L()				  GPIO_ResetBits(GPIOB, GPIO_Pin_0);
#define sel_device()    	      GPIO_ResetBits(GPIOA, GPIO_Pin_4);
#define unsel_device()            GPIO_SetBits(GPIOA, GPIO_Pin_4);
/*****************************************************************/
/*����: FM1702��λ����
����: ��
���: N/A*/
/****************************************************************/
void reset_device(void)
{
    RF_RST_H();						/* FM1702��λ */
    vTaskDelay(140);					/*�ȴ�Լ140ms, */
    RF_RST_L();	
    vTaskDelay(140);
}
/****************************************************************/
/*����: �ú������ݸ����ĵ�ַ��ȡFM1702SL�ļĴ���
����: �Ĵ�����ַ<SpiAddress>, ����ֵ����׵�ַ<ReadBuf>
���: �Ĵ���ֵ*/
/****************************************************************/
INT8U SPIRead(INT8U SpiAddress)
{
	INT8U ch;
	taskENTER_CRITICAL(); /*����������*/
	sel_device();
	SpiAddress<<=1;
	SPI_ReadWriteByte(SpiAddress|0x80);
	ch = SPI_ReadWriteByte(0xff);	
	unsel_device();
	taskEXIT_CRITICAL(); /*����������*/
	return ch;
}
/*****************************************************************/
/*����: �ú������ݸ����ĵ�ַд��FM1702SL�ļĴ���
����: �Ĵ�����ַ<SpiAddress>,Ҫд����ֽ���<WriteCount>
���: N/A*/
/****************************************************************/
void SPIWrite(INT8U SpiAddress,INT8U SpiData)
{
	taskENTER_CRITICAL(); /*����������*/
	sel_device();
	SPI_ReadWriteByte(SpiAddress<<1);
	SPI_ReadWriteByte(SpiData);
	unsel_device();
	taskEXIT_CRITICAL(); /*����������*/
}
/****************************************************************/
/*����: Clear_FIFO */
/*����: �ú���ʵ�����FM1702��FIFO������*/
/*����: N/A */
/*���: TRUE, FIFO�����*/
/* FALSE, FIFOδ�����*/
/****************************************************************/
INT8U Clear_FIFO(void)   //ok
{
	INT8U	temp;
	INT16U	i;
	temp = SPIRead(Control);			/* ���FIFO */
	temp = (temp | 0x01);
	SPIWrite(Control,temp);
	for(i = 0; i < RF_TimeOut; i++) /* ���FIFO�Ƿ���� */
	{
		temp = SPIRead(FIFO_Length);
		if(temp == 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}
/****************************************************************/
/*����: Write_FIFO */
/*����: �ú���ʵ����FM1702��FIFO��д��x bytes����*/
/*����: count, ��д���ֽڵĳ���*/
/* buff, ָ���д�����ݵ�ָ��*/
/*���: N/A */
/****************************************************************/
void Write_FIFO(INT8U count, INT8U  *buff)  //ok
{
	INT8U	i;
	
	for(i = 0; i < count; i++)
	{
		SPIWrite(FIFO,*(buff + i));
	}
}
/****************************************************************/
/*����: Read_FIFO */
/*����: �ú���ʵ�ִ�FM1702��FIFO�ж���x bytes����*/
/*����: buff, ָ��������ݵ�ָ��*/
/*���: N/A */
/****************************************************************/
INT8U Read_FIFO(INT8U  *buff)  //ok
{
	INT8U	temp;
	INT8U	i;
	
	temp = SPIRead(FIFO_Length);
	if(temp == 0)		return 0;
	if(temp >= 32)	    temp = 32;	

	for(i = 0; i < temp; i++)
	{
		*(buff + i) = SPIRead(FIFO);
	}

	return temp;
}
/****************************************************************/
/*����: Judge_Req */
/*����: �ú���ʵ�ֶԿ�Ƭ��λӦ���źŵ��ж�*/
/*����: *buff, ָ��Ӧ�����ݵ�ָ��*/
/*���: TRUE, ��ƬӦ���ź���ȷ*/
/* FALSE, ��ƬӦ���źŴ���*/
/****************************************************************/
//gengxuetao
//(temp1 == 0x08)
INT8U Judge_Req(INT8U  *buff) // ok
{
	INT8U	temp1, temp2;
	
	temp1 = *buff;
	temp2 = *(buff + 1);
	//printf("temp1:%d\r\n",temp1);
	//printf("temp2:%d\r\n",temp2);
	if((temp1 == 0x02) || (temp1 == 0x04) || (temp1 == 0x05) || (temp1 == 0x53) || (temp1 == 0x03)||(temp1 == 0x08)) //3  4  5  53 
	{
		if (temp2 == 0x00)
		{
			return TRUE;
		}
	}
	return FALSE;
}
/****************************************************************/
/*����: Check_UID */
/*����: �ú���ʵ�ֶ��յ��Ŀ�Ƭ�����кŵ��ж�*/
/*����: N/A */
/*���: TRUE: ���к���ȷ*/
/* FALSE: ���кŴ���*/
/****************************************************************/
INT8U Check_UID(void)  //ok
{
	INT8U	temp;
	INT8U	i;
	
	temp = 0x00;
	for(i = 0; i < 5; i++)
	{
		temp = temp ^ S50.uid[i];
	}

	if(temp == 0)	
		return TRUE;

	return FALSE;
}
/****************************************************************/
/*����: Save_UID */
/*����: �ú���ʵ�ֱ��濨Ƭ�յ������к�*/
/*����: row: ������ͻ����*/
/* col: ������ͻ����*/
/* length: �Ӆ�����UID���ݳ���*/
/*���: N/A */
/****************************************************************/
void Save_UID(INT8U row, INT8U col, INT8U length) //ok
{
	INT8U	i;
	INT8U	temp;
	INT8U	temp1;
	
	if((row == 0x00) && (col == 0x00))
	{
		for(i = 0; i < length; i++)
		{
			S50.uid[i] = S50.revbuffer[i];
		}
	}
	else
	{
		temp = S50.revbuffer[0];
		temp1 = S50.uid[row - 1];
		switch(col)
		{
			case 0:		temp1 = 0x00; row = row + 1; break;
			case 1:		temp = temp & 0xFE; temp1 = temp1 & 0x01; break;
			case 2:		temp = temp & 0xFC; temp1 = temp1 & 0x03; break;
			case 3:		temp = temp & 0xF8; temp1 = temp1 & 0x07; break;
			case 4:		temp = temp & 0xF0; temp1 = temp1 & 0x0F; break;
			case 5:		temp = temp & 0xE0; temp1 = temp1 & 0x1F; break;
			case 6:		temp = temp & 0xC0; temp1 = temp1 & 0x3F; break;
			case 7:		temp = temp & 0x80; temp1 = temp1 & 0x7F; break;
			default:	break;
		}

		S50.revbuffer[0] = temp;
		S50.uid[row - 1] = temp1 | temp;
		for(i = 1; i < length; i++)
		{
			S50.uid[row - 1 + i] = S50.revbuffer[i];
		}
	}
}
/****************************************************************/
/*����: Set_BitFraming */
/*����: �ú������ô��������ݵ��ֽ���*/
/*����: row: ������ͻ����*/
/* col: ������ͻ����*/
/*���: N/A */
/****************************************************************/
void Set_BitFraming(INT8U row, INT8U col) //ok
{
	switch(row)
	{
		case 0:		S50.revbuffer[1] = 0x20; break;
		case 1:		S50.revbuffer[1] = 0x30; break;
		case 2:		S50.revbuffer[1] = 0x40; break;
		case 3:		S50.revbuffer[1] = 0x50; break;
		case 4:		S50.revbuffer[1] = 0x60; break;
		default:	break;
	}

	switch(col)
	{
		case 0:		SPIWrite(Bit_Frame,0x00); break;
		case 1:		SPIWrite(Bit_Frame,0x11); S50.revbuffer[1] = (S50.revbuffer[1] | 0x01); break;
		case 2:		SPIWrite(Bit_Frame,0x22); S50.revbuffer[1] = (S50.revbuffer[1] | 0x02); break;
		case 3:		SPIWrite(Bit_Frame,0x33); S50.revbuffer[1] = (S50.revbuffer[1] | 0x03); break;
		case 4:		SPIWrite(Bit_Frame,0x44); S50.revbuffer[1] = (S50.revbuffer[1] | 0x04); break;
		case 5:		SPIWrite(Bit_Frame,0x55); S50.revbuffer[1] = (S50.revbuffer[1] | 0x05); break;
		case 6:		SPIWrite(Bit_Frame,0x66); S50.revbuffer[1] = (S50.revbuffer[1] | 0x06); break;
		case 7:		SPIWrite(Bit_Frame,0x77); S50.revbuffer[1] = (S50.revbuffer[1] | 0x07); break;
		default:	break;
	}
}
/****************************************************************/
/*����: Init_FM1702 */
/*����: �ú���ʵ�ֶ�FM1702��ʼ������*/
/*����: mode:����ģʽ, 0:TYPEAģʽ*/
/* 1:TYPEBģʽ*/
/* 2:�Ϻ�ģʽ*/
/*���: N/A */
/****************************************************************/
void Init_FM1702(INT8U mode)  //ok
{	
	INT16U	i;
	unsel_device();
	vTaskDelay(100);	
	reset_device();
	vTaskDelay(200);	
	
	while(SPIRead(Command) != 0)		        /* �ȴ�Command = 0,FM1702��λ�ɹ� */
	{
		__NOP();
	}
	
    SPIWrite(Page_Sel,0x80);//��ʼ��FM1702��spi�ӿ�

	for(i = 0; i < 0x1fff; i++) /* ��ʱ */
	{
		if(SPIRead(Command) == 0x00)	//temp=
		{
			SPIWrite(Page_Sel,0x00);//�л�������Ѱַ��ʽ
		}
	}

	SPIWrite(TimerClock,0x0b);		   //address 2AH    /* ��ʱ���������üĴ��� */   
	SPIWrite(TimerControl,0x02);	   //address 2BH	/* ��ʱ�����ƼĴ��� */    
	SPIWrite(TimerReload,0x42);	       //address 2CH    /* ��ʱ����ֵ�Ĵ��� */ 	  
	SPIWrite(InterruptEn,0x7f);		   //address 06H    /* �ж�ʹ��/��ֹ�Ĵ��� */     
	SPIWrite(Int_Req,0x7f);            //address 07H    /* �ж������ʶ�Ĵ��� */
	SPIWrite(MFOUTSelect,0x02);		   //address 26H    /* mf OUT ѡ�����üĴ��� */     
    //���õ�����������ԴΪ�ڲ�������, ��������TX1��TX2
	SPIWrite(TxControl,0x5b);		   //address 11H    /* ���Ϳ��ƼĴ��� */ 
	SPIWrite(RxControl2,0x01);			//0xe1
	SPIWrite(RxWait,0x05);					//0x21
	if(mode ==2)
	{
		SPIWrite(TypeSH,0x01);				//0x31 ����SH��׼����֤ģʽ
	}
	else
	{
		SPIWrite(TypeSH,0x00);// ����MIFIRE��׼����֤ģʽ
	}
}
/****************************************************************/
/*����: Command_Send */
/*����: �ú���ʵ����FM1702��������Ĺ���*/
/*����: count, ����������ĳ���*/
/* buff, ָ����������ݵ�ָ��*/
/* Comm_Set, ������*/
/*���: TRUE, �����ȷִ��*/
/* FALSE, ����ִ�д���*/
/****************************************************************/
INT8U Command_Send(INT8U count, INT8U  *buff, INT8U Comm_Set)//ok
{
	INT16U		j=0;
	INT8U 	temp=0, temp1=0;
	
	SPIWrite(Command,0x00);
	temp = Clear_FIFO();
	
	Write_FIFO(count, buff);
	
	SPIWrite(Command,Comm_Set);		/* ����ִ�� */
	for(j = 0; j <RF_TimeOut ; j++) /* �������ִ�з� RF_TimeOut*/
	{
		temp = SPIRead(Command);
		temp1 = SPIRead(Int_Req) & 0x80;
		
		if((temp == 0x00) || (temp1 == 0x80))
		{
			return TRUE;
		}
	}
	//printf("temp:%d\r\n",temp);
	//printf("temp1:%d\r\n",temp1);
	return FALSE;
}
/****************************************************************/
/*����: Read_E2 */
/*����: �ú���ʵ�ִ�FM1702��EE�ж�������*/
/*����: lsb, EE��ַ(���ֽ�) */
/* msb, EE��ַ(���ֽ�) */
/* count, ����������EE���ֽڸ���*/
/* buff, ָ����������ݵ�ָ��*/
/*���: TRUE, EE������ȷ����*/
/* FALSE, EE���ݶ�������*/
/****************************************************************/
INT8U Read_E2(INT8U lsb, INT8U msb, INT8U count, INT8U  *buff) //ok
{
	INT8U	temp;
	
	*buff = lsb;
	*(buff + 1) = msb;
	*(buff + 2) = count;
	temp = Command_Send(3, buff, ReadE2);
	Read_FIFO(buff);
	if(temp == FALSE) return(TRUE);
	return(FALSE);
}
/****************************************************************/
/*����: Write_E2 */
/*����: �ú���ʵ����FM1702��EE��д������*/
/*����: lsb, EE��ַ(���ֽ�) */
/* msb, EE��ַ(���ֽ�) */
/* count, ��д������EE���ֽڸ���*/
/* buff, ָ���д�����ݵ�ָ��*/
/*���: TRUE, EE������ȷд��*/
/* FALSE, EE����д������*/
/****************************************************************/
INT8U Write_E2(INT8U lsb, INT8U msb, INT8U count, INT8U  *buff) //ok
{
	INT8U 	temp, i;

	for(i = 0; i < count; i++)
	{
		*(buff + count - i + 2) = *(buff - i + count);
	}

	*buff = lsb;
	*(buff + 1) = msb;
	temp = Command_Send(count + 2, buff, WriteE2);
	temp = SPIRead(SecondaryStatus);
	temp = temp & 0x40;
	if(temp == 0x40)
	{
		SPIWrite(Command,0x00);			//added 2006/03/15
		return TRUE;
	}

	return FALSE;
}
/****************************************************************/
/*����: MIF_Halt */
/*����: �ú���ʵ����ͣMIFARE��*/
/*����: N/A */
/*���: FM1702_OK: Ӧ����ȷ*/
/* FM1702_PARITYERR: ��żУ���*/
/* FM1702_CRCERR: CRCУ���*/
/* FM1702_NOTAGETR: �޿�*/
/****************************************************************/
INT8U MIF_Halt(void)  //ok
{
	INT8U	temp;
	INT16U	i;
	
	SPIWrite(CRCPresetLSB,0x63);
	SPIWrite(CWConductance,0x3f);
	SPIWrite(ChannelRedundancy,0x03);
	*S50.revbuffer = RF_CMD_HALT;
	*(S50.revbuffer + 1) = 0x00;
	temp = Command_Send(2, S50.revbuffer, Transmit);
	if(temp == TRUE)
	{
		for(i = 0; i < 0x50; i++)
		{
			__NOP();
		}

		return FM1702_OK;
	}
	else
	{
		temp = SPIRead(ErrorFlag);
		if((temp & 0x02) == 0x02)
		{
			return(FM1702_PARITYERR);
		}

		if((temp & 0x04) == 0x04)
		{
			return(FM1702_FRAMINGERR);
		}

		return(FM1702_NOTAGETR);
	}
}
///////////////////////////////////////////////////////////////////////
// ת����Կ��ʽ
///////////////////////////////////////////////////////////////////////
INT8U M500HostCodeKey(  INT8U *uncoded, INT8U *coded)  //ok 
{
    INT8U  cnt = 0;
    INT8U  ln  = 0;     
    INT8U  hn  = 0;      
    for (cnt = 0; cnt < 6; cnt++)
    {
        ln = uncoded[cnt] & 0x0F;
        hn = uncoded[cnt] >> 4;
        coded[cnt * 2 + 1] = (~ln << 4) | ln;
        coded[cnt * 2 ] = (~hn << 4) | hn;
    }
    return FM1702_OK;
}
/****************************************************************/
/*����: Load_keyE2 */
/*����: �ú���ʵ�ְ�E2���������FM1702��keyRevBuffer��*/
/*����: Secnr: EE��ʼ��ַ*/
/*���: True: ��Կװ�سɹ�*/
/* False: ��Կװ��ʧ��*/
/****************************************************************/
INT8U Load_keyE2_CPY(INT8U Secnr, INT8U Mode) //ok
{
	INT8U	temp=0;	
	INT8U	msb = 0;
	INT8U	lsb = 0;
	INT8U coded_keys[12]={0};
	INT8U	temp1=0;

	//ȥ������������
	msb+=Mode;
	msb=0;
	
	lsb+=Mode;
	lsb=0;
	////////////////////////////////////
	if(Secnr >= 0x20)
	{
	  temp1 = Secnr - 0x20;
	  Secnr = 0x20 + temp1 * 4;
	}

	temp = Secnr * 12;  //12

	if(Mode == RF_CMD_AUTH_LA)
	{
		if(temp >= 0x80)	
		{
			lsb = temp - 0x80;
			msb = 0x01;
		}
		else
		{
			lsb = temp + 0x80;//
			msb = 0x00;
		}
	}
	else
	{
		msb = 0x01;
		lsb = temp + 0x40;
	}
	
    M500HostCodeKey(&S50.revbuffer[3],coded_keys);////////////////
    //�޸�˵����ע�͵���Ϊͨ��EEPROM������Կ������Ϊͨ��FIFO������Կ
	#ifdef LOADEE 
		temp = Write_E2(lsb, msb, 12, coded_keys);	     ////////////////       
		S50.revbuffer[0] = lsb;
		S50.revbuffer[1] = msb;	
		temp = Command_Send(2, S50.revbuffer, LoadKeyE2);
    #else   
    	Command_Send(12,coded_keys,LoadKey);//ͨ��FIFO������Կ
	#endif

	temp = SPIRead(ErrorFlag) & 0x40;
	if(temp == 0x40)
	{
		return FALSE;
	}

	return TRUE;
}
/****************************************************************/
/*����: Request */
/*����: �ú���ʵ�ֶԷ���FM1702������Χ֮�ڵĿ�Ƭ��Request����*/
/*����: mode: ALL(�������FM1702������Χ֮�ڵĿ�Ƭ) */
/* STD(�����FM1702������Χ֮�ڴ���HALT״̬�Ŀ�Ƭ) */
/*���: FM1702_NOTAGETR: �޿�*/
/* FM1702_OK: Ӧ����ȷ*/
/* FM1702_REQERR: Ӧ�����*/
/****************************************************************/
INT8U Request(INT8U mode) //ok
{	
	INT8U 	temp;
	
	SPIWrite(CRCPresetLSB,0x63);		//CRCPresetLSB address is 0x23
	SPIWrite(CWConductance,0x3f);		//ѡ�����TX1��TX2�������ߵ��迹
	S50.revbuffer[0] = mode;		/* Requestģʽѡ�� */
	SPIWrite(Bit_Frame,0x07);		
	SPIWrite(ChannelRedundancy,0x03);	
	temp = SPIRead(Control);
	temp = temp & (0xf7);
	SPIWrite(Control,temp);			//Control reset value is 00
	temp = Command_Send(1, S50.revbuffer, Transceive);   //Transceive=0x1E	/* ���ͽ������� */
	//printf("���ⶨλ1:%d\r\n",temp);
	if(temp == FALSE)
	{
		return FM1702_NOTAGETR;
	}

	Read_FIFO(S50.revbuffer);		/* ��FIFO�ж�ȡӦ����Ϣ��RevBuffer[]�� */
	temp = Judge_Req(S50.revbuffer);	/* �ж�Ӧ���ź��Ƿ���ȷ */
	//printf("���ⶨλ2:%d\r\n",temp);
	if(temp == TRUE)
	{
		S50.tagtype[0] = S50.revbuffer[0];
		S50.tagtype[1] = S50.revbuffer[1];
		return FM1702_OK;
	}

	return FM1702_REQERR;
}
/****************************************************************/
/*����: AntiColl */
/*����: �ú���ʵ�ֶԷ���FM1702������Χ֮�ڵĿ�Ƭ�ķ���ͻ���*/
/*����: N/A */
/*���: FM1702_NOTAGETR: �޿�*/
/* FM1702_BYTECOUNTERR: �����ֽڴ���*/
/* FM1702_SERNRERR: ��Ƭ���к�Ӧ�����*/
/* FM1702_OK: ��ƬӦ����ȷ*/
/****************************************************************/
INT8U AntiColl(void)  //ok
{	
	INT8U	temp;
	INT8U	i;
	INT8U	row, col;
	INT8U	pre_row;
	
	row = 0;
	col = 0;
	pre_row = 0;
	SPIWrite(CRCPresetLSB,0x63);
	SPIWrite(CWConductance,0x3f);
	S50.revbuffer[0] = RF_CMD_ANTICOL;
	S50.revbuffer[1] = 0x20;
	SPIWrite(ChannelRedundancy,0x03);
	temp = Command_Send(2, S50.revbuffer, Transceive);
	while(1)
	{
		if(temp == FALSE)
		{
			return(FM1702_NOTAGETR);
		}

		temp = SPIRead(FIFO_Length);//FIFO�����ݵ�BYTE��
		if(temp == 0)
		{
			return FM1702_BYTECOUNTERR;
		}

		Read_FIFO(S50.revbuffer);//����FIFO������
		Save_UID(row, col, temp);			/* ���յ���UID����UID������ */
		
		
		temp = SPIRead(ErrorFlag);				    /* �жϽ��������Ƿ���� */
		temp = temp & 0x01;
		if(temp == 0x00)
		{//�޳�ͻλ
			temp = Check_UID();			/* У���յ���UID */
			if(temp == FALSE)
			{
				return(FM1702_SERNRERR);//��Ƭ���кŴ���
			}

			return(FM1702_OK);
		}
		else
		{//�г�ͻλ
			temp = SPIRead(CollPos);				/* ��ȡ��ͻ���Ĵ��� */
			row = temp / 8;//��row��UID�ֽ��г�ͻ
			col = temp % 8;//��row��UID�ֽڵ�colλ�г�ͻ
			S50.revbuffer[0] = RF_CMD_ANTICOL;
			Set_BitFraming(row + pre_row, col);	/* ���ô��������ݵ��ֽ��� */
			pre_row = pre_row + row;
			for(i = 0; i < pre_row + 1; i++)
			{
				S50.revbuffer[i + 2] = S50.uid[i];
			}

			if(col != 0x00)
			{
				row = pre_row + 1;
			}
			else
			{
				row = pre_row;
			}

			temp = Command_Send(row + 2, S50.revbuffer, Transceive);
		}
	}
}
/****************************************************************/
/*����: Select_Card */
/*����: �ú���ʵ�ֶԷ���FM1702������Χ֮�ڵ�ĳ�ſ�Ƭ����ѡ��*/
/*����: N/A */
/*���: FM1702_NOTAGETR: �޿�*/
/* FM1702_PARITYERR: ��żУ���*/
/* FM1702_CRCERR: CRCУ���*/
/* FM1702_BYTECOUNTERR: �����ֽڴ���*/
/* FM1702_OK: Ӧ����ȷ*/
/* FM1702_SELERR: ѡ������*/
/****************************************************************/
INT8U Select_Card(void)  //ok
{	
	INT8U	temp, i;
	
	SPIWrite(CRCPresetLSB,0x63);
	SPIWrite(CWConductance,0x3f);
	S50.revbuffer[0] = RF_CMD_SELECT;
	S50.revbuffer[1] = 0x70;
	for(i = 0; i < 5; i++)
	{
		S50.revbuffer[i + 2] = S50.uid[i];
	}

	SPIWrite(ChannelRedundancy,0x0f);	
	temp = Command_Send(7, S50.revbuffer, Transceive);
	if(temp == FALSE)
	{
		return(FM1702_NOTAGETR);
	}
	else
	{
		temp = SPIRead(ErrorFlag);
		if((temp & 0x02) == 0x02) return(FM1702_PARITYERR);
		if((temp & 0x04) == 0x04) return(FM1702_FRAMINGERR);
		if((temp & 0x08) == 0x08) return(FM1702_CRCERR);
		temp = SPIRead(FIFO_Length);
		if(temp != 1) return(FM1702_BYTECOUNTERR);
		Read_FIFO(S50.revbuffer);	/* ��FIFO�ж�ȡӦ����Ϣ */
		temp = *S50.revbuffer;
		//printf("temp:%d\r\n",temp);
		if((temp == 0x18) || (temp == 0x08) || (temp == 0x88) || (temp == 0x53)||(temp == 0x20)||(temp == 0x28))	/* �ж�Ӧ���ź��Ƿ���ȷ */
			return(FM1702_OK);
		else
			return(FM1702_SELERR);
	}
}
/****************************************************************/
/*����: Authentication */
/*����: �ú���ʵ��������֤�Ĺ���*/
/*����: UID: ��Ƭ���кŵ�ַ*/
/* SecNR: ������*/
/* mode: ģʽ*/
/*���: FM1702_NOTAGETR: �޿�*/
/* FM1702_PARITYERR: ��żУ���*/
/* FM1702_CRCERR: CRCУ���*/
/* FM1702_OK: Ӧ����ȷ*/
/* FM1702_AUTHERR: Ȩ����֤�д�*/
/****************************************************************/
INT8U Authentication(INT8U  *UID, INT8U SecNR, INT8U mode) //ok
{	
	INT8U 	i;
	INT8U 	temp, temp1;

	INT8U temp0;
	if(SecNR >= 0x20)
	{
		temp0 = SecNR -0x20;
		SecNR = 0x20 + temp0 * 4;
	}
	
	SPIWrite(CRCPresetLSB,0x63);
	SPIWrite(CWConductance,0x3f); // 

	if(mode == RF_CMD_AUTH_LB)	 //		
		S50.revbuffer[0] = RF_CMD_AUTH_LB;
	else
		S50.revbuffer[0] = RF_CMD_AUTH_LA;
	S50.revbuffer[1] = SecNR * 4 + 3;
	for(i = 0; i < 4; i++)
	{
		S50.revbuffer[2 + i] = UID[i];
	}

	SPIWrite(ChannelRedundancy,0x0f);	
	temp = Command_Send(6, S50.revbuffer, Authent1); //���Ƭ
	if(temp == FALSE)
	{	
		return 0x99;
	}

	temp = SPIRead(ErrorFlag);             //ErrorFlag address is 0x0A.
	if((temp & 0x02) == 0x02) return FM1702_PARITYERR;
	if((temp & 0x04) == 0x04) return FM1702_FRAMINGERR;
	if((temp & 0x08) == 0x08) return FM1702_CRCERR;
	temp = Command_Send(0, S50.revbuffer, Authent2);	//�ڶ�������֤
	if(temp == FALSE)
	{	
		return 0x88;
	}

	temp = SPIRead(ErrorFlag);  //��������֤
	if((temp & 0x02) == 0x02) return FM1702_PARITYERR;
	if((temp & 0x04) == 0x04) return FM1702_FRAMINGERR;
	if((temp & 0x08) == 0x08) return FM1702_CRCERR;
	temp1 = SPIRead(Control);
	temp1 = temp1 & 0x08;	
	if(temp1 == 0x08)
	{
		return FM1702_OK;
	}

	return FM1702_AUTHERR;
}
/****************************************************************/
/*����: MIF_Read */
/*����: �ú���ʵ�ֶ�MIFARE�������ֵ*/
/*����: buff: �������׵�ַ*/
/* Block_Adr: ���ַ*/
/*���: FM1702_NOTAGETR: �޿�*/
/* FM1702_PARITYERR: ��żУ���*/
/* FM1702_CRCERR: CRCУ���*/
/* FM1702_BYTECOUNTERR: �����ֽڴ���*/
/* FM1702_OK: Ӧ����ȷ*/
/****************************************************************/
INT8U MIF_READ(INT8U  *buff, INT8U Block_Adr)  //ok
{	
	INT8U	temp;
	
	SPIWrite(CRCPresetLSB,0x63);
	SPIWrite(CWConductance,0x3f);
	SPIWrite(ModConductance,0x3f);
	SPIWrite(ChannelRedundancy,0x0f);

	buff[0] = RF_CMD_READ;
	buff[1] = Block_Adr;
	temp = Command_Send(2, buff, Transceive);
       
	if(temp == 0)
	{
		return FM1702_NOTAGETR;
	}

	temp = SPIRead(ErrorFlag);
	if((temp & 0x02) == 0x02) return FM1702_PARITYERR;
	if((temp & 0x04) == 0x04) return FM1702_FRAMINGERR;
	if((temp & 0x08) == 0x08) return FM1702_CRCERR;
	temp = SPIRead(FIFO_Length);
       	
	if(temp == 0x10)	
	{
		Read_FIFO(buff);
		return FM1702_OK;
	}
	else if(temp == 0x04)	
	{
		Read_FIFO(buff);
		return FM1702_OK;
	}
	else
	{
		return FM1702_BYTECOUNTERR;
	}
}
/****************************************************************/
/*����: MIF_Write */
/*����: �ú���ʵ��дMIFARE�������ֵ*/
/*����: buff: �������׵�ַ*/
/* Block_Adr: ���ַ*/
/*���: FM1702_NOTAGETR: �޿�*/
/* FM1702_BYTECOUNTERR: �����ֽڴ���*/
/* FM1702_NOTAUTHERR: δ��Ȩ����֤*/
/* FM1702_EMPTY: �����������*/
/* FM1702_CRCERR: CRCУ���*/
/* FM1702_PARITYERR: ��żУ���*/
/* FM1702_WRITEERR: д�������ݳ���*/
/* FM1702_OK: Ӧ����ȷ*/
/****************************************************************/
INT8U MIF_Write(INT8U  *buff, INT8U Block_Adr) //ok
{
	INT8U 	temp;
	INT8U 	*F_buff;

	SPIWrite(CRCPresetLSB,0x63);
	SPIWrite(CWConductance,0x3f);
	F_buff = buff + 0x10;
	SPIWrite(ChannelRedundancy,0x07);    /* Note: this line is for 1702, different from RC500*/
	*F_buff = RF_CMD_WRITE;
	*(F_buff + 1) = Block_Adr;
	temp = Command_Send(2, F_buff, Transceive);
	if(temp == FALSE)
	{
		return(FM1702_NOTAGETR);
	}

	temp = SPIRead(FIFO_Length);
	if(temp == 0)
	{
		return(FM1702_BYTECOUNTERR);
	}

	Read_FIFO(F_buff);
	temp = *F_buff;
	switch(temp)
	{
		case 0x00:	return(FM1702_NOTAUTHERR);	
		case 0x04:	return(FM1702_EMPTY);
		case 0x0a:	break;
		case 0x01:	return(FM1702_CRCERR);
		case 0x05:	return(FM1702_PARITYERR);
		default:	return(FM1702_WRITEERR);
	}

	temp = Command_Send(16, buff, Transceive);
	if(temp == TRUE)
	{
		return(FM1702_OK);
	}
	else
	{
		temp = SPIRead(ErrorFlag);
		if((temp & 0x02) == 0x02)
			return(FM1702_PARITYERR);
		else if((temp & 0x04) == 0x04)
			return(FM1702_FRAMINGERR);
		else if((temp & 0x08) == 0x08)
			return(FM1702_CRCERR);
		else
			return(FM1702_WRITEERR);
	}
}
/****************************************************************/
/*����: MIF_Increment */
/*����: �ú���ʵ��MIFARE���Զ���ֵ����*/
/*����: buff: �ĸ��ֽ���ֵ��ʼ��ַ*/
/* Block_Adr: ���ַ*/
/*���: FM1702_NOTAGETR: �޿�*/
/* FM1702_BYTECOUNTERR: �����ֽڴ���*/
/* FM1702_NOTAUTHERR: δ��Ȩ����֤*/
/* FM1702_EMPTY: �����������*/
/* FM1702_CRCERR: CRCУ���*/
/* FM1702_PARITYERR: ��żУ���*/
/* FM1702_INCRERR: ��Ƭ�������ʧ��*/
/* FM1702_OK: Ӧ����ȷ*/
/****************************************************************/
INT8U MIF_Increment(INT8U  *buff, INT8U Block_Adr)  //ok
{	
	INT8U	temp;
	INT8U 	*F_buff;
	
	SPIWrite(CRCPresetLSB,0x63);
	SPIWrite(CWConductance,0x3f);
	F_buff = buff + 4;
	*F_buff = RF_CMD_INC;
	*(F_buff + 1) = Block_Adr;
	SPIWrite(ChannelRedundancy,0x07);
	temp = Command_Send(2, F_buff, Transceive);
	if(temp == FALSE)
	{
		return FM1702_NOTAGETR;
	}

	temp = SPIRead(FIFO_Length);
	if(temp == 0)
	{
		return FM1702_BYTECOUNTERR;
	}

	Read_FIFO(F_buff);
	temp = *F_buff;
	switch(temp)
	{
		case 0x00:	return(FM1702_NOTAUTHERR);
		case 0x04:	return(FM1702_EMPTY);
		case 0x0a:	break;
		case 0x01:	return(FM1702_CRCERR);
		case 0x05:	return(FM1702_PARITYERR);
		default:	return(FM1702_INCRERR);
	}
        
	temp = Command_Send(4, buff, Transmit);
	if(temp == FALSE)
	{
		return FM1702_INCRERR;
	}

	return FM1702_OK;
}
/****************************************************************/
/*����: MIF_Decrement */
/*����: �ú���ʵ��MIFARE���Զ���ֵ����*/
/*����: buff: �ĸ��ֽ���ֵ��ʼ��ַ*/
/* Block_Adr: ���ַ*/
/*���: FM1702_NOTAGETR: �޿�*/
/* FM1702_BYTECOUNTERR: �����ֽڴ���*/
/* FM1702_NOTAUTHERR: δ��Ȩ����֤*/
/* FM1702_EMPTY: �����������*/
/* FM1702_CRCERR: CRCУ���*/
/* FM1702_PARITYERR: ��żУ���*/
/* FM1702_DECRERR: ��Ƭ�ۿ����ʧ��*/
/* FM1702_OK: Ӧ����ȷ*/
/****************************************************************/
INT8U MIF_Decrement(INT8U  *buff, INT8U Block_Adr) //ok
{	
	INT8U	temp;
	INT8U 	*F_buff;
	
	SPIWrite(CRCPresetLSB,0x63);
	SPIWrite(CWConductance,0x3f);
	F_buff = buff + 4;
	*F_buff = RF_CMD_DEC;
	*(F_buff + 1) = Block_Adr;
	SPIWrite(ChannelRedundancy,0x07);
	temp = Command_Send(2, F_buff, Transceive);
	if(temp == FALSE)
	{
		return FM1702_NOTAGETR;
	}

	temp = SPIRead(FIFO_Length);
	if(temp == 0)
	{
		return FM1702_BYTECOUNTERR;
	}

	Read_FIFO(F_buff);
	temp = *F_buff;
	switch(temp)
	{
		case 0x00:	return(FM1702_NOTAUTHERR);	
		case 0x04:	return(FM1702_EMPTY);
		case 0x0a:	break;
		case 0x01:	return(FM1702_CRCERR);
		case 0x05:	return(FM1702_PARITYERR);
		default:	return(FM1702_DECRERR);
	}

	temp = Command_Send(4, buff, Transmit);
	if(temp == FALSE)
	{
		return(FM1702_DECRERR);
	}

	return FM1702_OK;
}
/****************************************************************/
/*����: MIF_Restore */
/*����: �ú���ʵ��MIFARE���Զ��ָ�,���ݲ���*/
/*����: Block_Adr: ���ַ*/
/*���: FM1702_NOTAGETR: �޿�*/
/* FM1702_BYTECOUNTERR: �����ֽڴ���*/
/* FM1702_NOTAUTHERR: δ��Ȩ����֤*/
/* FM1702_EMPTY: �����������*/
/* FM1702_CRCERR: CRCУ���*/
/* FM1702_PARITYERR: ��żУ���*/
/* FM1702_RESTERR: ��Ƭ�ָ�,���ݲ���ʧ��*/
/* FM1702_OK: Ӧ����ȷ*/
/****************************************************************/
INT8U MIF_Restore(INT8U Block_Adr)
{	
	INT8U	temp, i;
	
	SPIWrite(CRCPresetLSB,0x63);
	SPIWrite(CWConductance,0x3f);
	SPIWrite(ChannelRedundancy,0x07);
	*S50.revbuffer = RF_CMD_RESTORE;
	*(S50.revbuffer + 1) = Block_Adr;
	temp = Command_Send(2, S50.revbuffer, Transceive);
	if(temp == FALSE)
	{
		return FM1702_NOTAGETR;
	}

	temp = SPIRead(FIFO_Length);
	if(temp == 0)
	{
		return FM1702_BYTECOUNTERR;
	}

	Read_FIFO(S50.revbuffer);
	temp = *S50.revbuffer;
	switch(temp)
	{
		case 0x00:	return(FM1702_NOTAUTHERR);	
		case 0x04:	return(FM1702_EMPTY);
		case 0x0a:	break;
		case 0x01:	return(FM1702_CRCERR);
		case 0x05:	return(FM1702_PARITYERR);
		default:	return(FM1702_RESTERR);
	}

	for(i = 0; i < 4; i++) S50.revbuffer[i] = 0x00;
	temp = Command_Send(4, S50.revbuffer, Transmit);
	if(temp == FALSE)
	{
		return FM1702_RESTERR;
	}

	return FM1702_OK;
}
/****************************************************************/
/*����: MIF_Transfer */
/*����: �ú���ʵ��MIFARE������Ǯ���������*/
/*����: Block_Adr: ���ַ*/
/*���: FM1702_NOTAGETR: �޿�*/
/* FM1702_BYTECOUNTERR: �����ֽڴ���*/
/* FM1702_NOTAUTHERR: δ��Ȩ����֤*/
/* FM1702_EMPTY: �����������*/
/* FM1702_CRCERR: CRCУ���*/
/* FM1702_PARITYERR: ��żУ���*/
/* FM1702_TRANSERR: ��Ƭ�ָ�,���ݲ���ʧ��*/
/* FM1702_OK: Ӧ����ȷ*/
/****************************************************************/
INT8U MIF_Transfer(INT8U Block_Adr)
{	
	INT8U	temp;
	SPIWrite(CRCPresetLSB,0x63);
	SPIWrite(CWConductance,0x3f);
	SPIWrite(ChannelRedundancy,0x07);	
	S50.revbuffer[0] = RF_CMD_TRANSFER;
	S50.revbuffer[1] = Block_Adr;
	temp = Command_Send(2, S50.revbuffer, Transceive);
	if(temp == FALSE)
	{
		return FM1702_NOTAGETR;
	}

	temp = SPIRead(FIFO_Length);
	if(temp == 0)
	{
		return FM1702_BYTECOUNTERR;
	}

	Read_FIFO(S50.revbuffer);
	temp = *S50.revbuffer;
	switch(temp)
	{
		case 0x00:	/* break; */return(FM1702_NOTAUTHERR);	
		case 0x04:	return(FM1702_EMPTY);
		case 0x0a:	return(FM1702_OK);
		case 0x01:	return(FM1702_CRCERR);
		case 0x05:	return(FM1702_PARITYERR);
		default:	return(FM1702_TRANSERR);
	}
}
/****************************************************************/
/*����: HL_Active */
/*����: �ú���ʵ�ָ߼�MIFARE����������*/
/*����: Secnr: ������*/
/* Block_Adr: ���ַ*/
/*���: ����״̬��*/
/* �������ݴ���RevBuffer��*/
/****************************************************************/
INT8U HL_Active(INT8U Block_Adr, INT8U Mode)
{
	
	INT8U	temp;
	INT8U   Secnr;
	Secnr = Block_Adr / 4;
	MIF_Halt();	/* Halt */
	temp = Request(RF_CMD_REQUEST_STD);		
	if(temp != FM1702_OK)
	{
		return(FM1702_REQERR);
	}

	temp = AntiColl();				
	if(temp != FM1702_OK)
	{
		return(FM1702_ANTICOLLERR);
	}

	temp = Select_Card();				
	if(temp != FM1702_OK)
	{
		return(FM1702_SELERR);
	}

	Load_keyE2_CPY(Secnr, Mode);	//%40	
	temp = Authentication(S50.uid, Secnr, Mode);	
	if(temp != FM1702_OK)
	{
		return(FM1702_AUTHERR);
	}

	return FM1702_OK;
}
/****************************************************************/
/*����: MIF_Initival */
/*����: �ú���ʵ��MIFARE����ʼ��ֵ����*/
/*����: buff: �ĸ��ֽڳ�ʼ����ֵ��ʼ��ַ*/
/* Block_Adr: ���ַ*/
/*���: FM1702_NOTAGETR: �޿�*/
/* FM1702_BYTECOUNTERR: �����ֽڴ���*/
/* FM1702_NOTAUTHERR: δ��Ȩ����֤*/
/* FM1702_EMPTY: �����������*/
/* FM1702_CRCERR: CRCУ���*/
/* FM1702_PARITYERR: ��żУ���*/
/* FM1702_WRITEERR: д�������ݳ���*/
/* FM1702_OK: Ӧ����ȷ*/
/****************************************************************/
INT8U MIF_Initival(INT8U *buff, INT8U Block_Adr)
{
	INT8U 	temp;
	INT8U		i;
	
	for(i = 0; i < 4; i++)
	{
		*(buff + 4 + i) = ~(*(buff + i));
	}

	for(i = 0; i < 4; i++)
	{
		*(buff + 8 + i) = *(buff + i);
	}

	*(buff + 12) = Block_Adr;
	*(buff + 13) = ~Block_Adr;
	*(buff + 14) = Block_Adr;
	*(buff + 15) = ~Block_Adr;
	temp = MIF_Write(buff, Block_Adr);
	return temp;
}
/****************************************************************/
/*����: HL_Read */
/*����: �ú���ʵ�ָ߼�������*/
/*����: Secnr: ������*/
/* Block_Adr: ���ַ*/
/*���: ����״̬��*/
/* �������ݴ���RevBuffer��*/
/****************************************************************/
INT8U HL_Read(INT8U *buff, INT8U Block_Adr, INT8U Mode)
{
	INT8U	temp;
	
	temp = HL_Active(Block_Adr, Mode);
	if(temp != FM1702_OK)
	{
		return temp;
	}
	
	temp = MIF_READ(buff, Block_Adr);
	if(temp != FM1702_OK)
	{
		return temp;
	}

	return FM1702_OK;
}
/****************************************************************/
/*����: HL_Write */
/*����: �ú���ʵ�ָ߼�д����*/
/*����: buff: ��д�����ݵ��׵�ַ*/
/* Secnr: ������*/
/* Block_Adr: ���ַ*/
/*���:����״̬��*/
/****************************************************************/
INT8U HL_Write(INT8U *buff, INT8U Block_Adr, INT8U Mode)
{
	INT8U	temp;
	
	temp = HL_Active(Block_Adr, Mode);
	if(temp != FM1702_OK)
	{
		return temp;
	}
	
	temp = MIF_Write(buff, Block_Adr);
	if(temp != FM1702_OK)
	{
		return FM1702_WRITEERR;
	}

	return FM1702_OK;
}



