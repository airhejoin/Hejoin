/*
*********************************************************************************
*                                 dsPIC30F4011
*                     基于MPLAB集成开发环境,MCC30编译器应用库
*									 Ver1.1
*                                   皇家科技
*
*模  块：FM1702
*设计者：Andy
*时  间：2010-06-10
*********************************************************************************
*/
#include"..\Lib\includes.h"
#include "usb_type.h"


S_CARD S50;
//IO口基本配置
/***********************************/
#define	RF_RST_H()				  GPIO_SetBits(GPIOB, GPIO_Pin_0);
#define	RF_RST_L()				  GPIO_ResetBits(GPIOB, GPIO_Pin_0);
#define sel_device()    	      GPIO_ResetBits(GPIOA, GPIO_Pin_4);
#define unsel_device()            GPIO_SetBits(GPIOA, GPIO_Pin_4);
/*****************************************************************/
/*功能: FM1702复位函数
输入: 无
输出: N/A*/
/****************************************************************/
void reset_device(void)
{
    RF_RST_H();						/* FM1702复位 */
    vTaskDelay(140);					/*等待约140ms, */
    RF_RST_L();	
    vTaskDelay(140);
}
/****************************************************************/
/*功能: 该函数根据给定的地址读取FM1702SL的寄存器
输入: 寄存器地址<SpiAddress>, 读出值存放首地址<ReadBuf>
输出: 寄存器值*/
/****************************************************************/
INT8U SPIRead(INT8U SpiAddress)
{
	INT8U ch;
	taskENTER_CRITICAL(); /*调度器上锁*/
	sel_device();
	SpiAddress<<=1;
	SPI_ReadWriteByte(SpiAddress|0x80);
	ch = SPI_ReadWriteByte(0xff);	
	unsel_device();
	taskEXIT_CRITICAL(); /*调度器开锁*/
	return ch;
}
/*****************************************************************/
/*功能: 该函数根据给定的地址写入FM1702SL的寄存器
输入: 寄存器地址<SpiAddress>,要写入的字节数<WriteCount>
输出: N/A*/
/****************************************************************/
void SPIWrite(INT8U SpiAddress,INT8U SpiData)
{
	taskENTER_CRITICAL(); /*调度器上锁*/
	sel_device();
	SPI_ReadWriteByte(SpiAddress<<1);
	SPI_ReadWriteByte(SpiData);
	unsel_device();
	taskEXIT_CRITICAL(); /*调度器开锁*/
}
/****************************************************************/
/*名称: Clear_FIFO */
/*功能: 该函数实现清空FM1702中FIFO的数据*/
/*输入: N/A */
/*输出: TRUE, FIFO被清空*/
/* FALSE, FIFO未被清空*/
/****************************************************************/
INT8U Clear_FIFO(void)   //ok
{
	INT8U	temp;
	INT16U	i;
	temp = SPIRead(Control);			/* 清空FIFO */
	temp = (temp | 0x01);
	SPIWrite(Control,temp);
	for(i = 0; i < RF_TimeOut; i++) /* 检查FIFO是否被清空 */
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
/*名称: Write_FIFO */
/*功能: 该函数实现向FM1702的FIFO中写入x bytes数据*/
/*输入: count, 待写入字节的长度*/
/* buff, 指向待写入数据的指针*/
/*输出: N/A */
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
/*名称: Read_FIFO */
/*功能: 该函数实现从FM1702的FIFO中读出x bytes数据*/
/*输入: buff, 指向读出数据的指针*/
/*输出: N/A */
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
/*名称: Judge_Req */
/*功能: 该函数实现对卡片复位应答信号的判断*/
/*输入: *buff, 指向应答数据的指针*/
/*输出: TRUE, 卡片应答信号正确*/
/* FALSE, 卡片应答信号错误*/
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
/*名称: Check_UID */
/*功能: 该函数实现对收到的卡片的序列号的判断*/
/*输入: N/A */
/*输出: TRUE: 序列号正确*/
/* FALSE: 序列号错误*/
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
/*名称: Save_UID */
/*功能: 该函数实现保存卡片收到的序列号*/
/*输入: row: 产生冲突的行*/
/* col: 产生冲突的列*/
/* length: 接収到的UID数据长度*/
/*输出: N/A */
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
/*名称: Set_BitFraming */
/*功能: 该函数设置待发送数据的字节数*/
/*输入: row: 产生冲突的行*/
/* col: 产生冲突的列*/
/*输出: N/A */
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
/*名称: Init_FM1702 */
/*功能: 该函数实现对FM1702初始化操作*/
/*输入: mode:工作模式, 0:TYPEA模式*/
/* 1:TYPEB模式*/
/* 2:上海模式*/
/*输出: N/A */
/****************************************************************/
void Init_FM1702(INT8U mode)  //ok
{	
	INT16U	i;
	unsel_device();
	vTaskDelay(100);	
	reset_device();
	vTaskDelay(200);	
	
	while(SPIRead(Command) != 0)		        /* 等待Command = 0,FM1702复位成功 */
	{
		__NOP();
	}
	
    SPIWrite(Page_Sel,0x80);//初始化FM1702的spi接口

	for(i = 0; i < 0x1fff; i++) /* 延时 */
	{
		if(SPIRead(Command) == 0x00)	//temp=
		{
			SPIWrite(Page_Sel,0x00);//切换到线性寻址方式
		}
	}

	SPIWrite(TimerClock,0x0b);		   //address 2AH    /* 定时器周期设置寄存器 */   
	SPIWrite(TimerControl,0x02);	   //address 2BH	/* 定时器控制寄存器 */    
	SPIWrite(TimerReload,0x42);	       //address 2CH    /* 定时器初值寄存器 */ 	  
	SPIWrite(InterruptEn,0x7f);		   //address 06H    /* 中断使能/禁止寄存器 */     
	SPIWrite(Int_Req,0x7f);            //address 07H    /* 中断请求标识寄存器 */
	SPIWrite(MFOUTSelect,0x02);		   //address 26H    /* mf OUT 选择配置寄存器 */     
    //设置调制器的输入源为内部编码器, 并且设置TX1和TX2
	SPIWrite(TxControl,0x5b);		   //address 11H    /* 发送控制寄存器 */ 
	SPIWrite(RxControl2,0x01);			//0xe1
	SPIWrite(RxWait,0x05);					//0x21
	if(mode ==2)
	{
		SPIWrite(TypeSH,0x01);				//0x31 兼容SH标准的认证模式
	}
	else
	{
		SPIWrite(TypeSH,0x00);// 兼容MIFIRE标准的认证模式
	}
}
/****************************************************************/
/*名称: Command_Send */
/*功能: 该函数实现向FM1702发送命令集的功能*/
/*输入: count, 待发送命令集的长度*/
/* buff, 指向待发送数据的指针*/
/* Comm_Set, 命令码*/
/*输出: TRUE, 命令被正确执行*/
/* FALSE, 命令执行错误*/
/****************************************************************/
INT8U Command_Send(INT8U count, INT8U  *buff, INT8U Comm_Set)//ok
{
	INT16U		j=0;
	INT8U 	temp=0, temp1=0;
	
	SPIWrite(Command,0x00);
	temp = Clear_FIFO();
	
	Write_FIFO(count, buff);
	
	SPIWrite(Command,Comm_Set);		/* 命令执行 */
	for(j = 0; j <RF_TimeOut ; j++) /* 检查命令执行否 RF_TimeOut*/
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
/*名称: Read_E2 */
/*功能: 该函数实现从FM1702的EE中读出数据*/
/*输入: lsb, EE地址(低字节) */
/* msb, EE地址(高字节) */
/* count, 待读出数据EE的字节个数*/
/* buff, 指向待读出数据的指针*/
/*输出: TRUE, EE数据正确读出*/
/* FALSE, EE数据读出有误*/
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
/*名称: Write_E2 */
/*功能: 该函数实现向FM1702的EE中写入数据*/
/*输入: lsb, EE地址(低字节) */
/* msb, EE地址(高字节) */
/* count, 待写入数据EE的字节个数*/
/* buff, 指向待写入数据的指针*/
/*输出: TRUE, EE数据正确写入*/
/* FALSE, EE数据写入有误*/
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
/*名称: MIF_Halt */
/*功能: 该函数实现暂停MIFARE卡*/
/*输入: N/A */
/*输出: FM1702_OK: 应答正确*/
/* FM1702_PARITYERR: 奇偶校验错*/
/* FM1702_CRCERR: CRC校验错*/
/* FM1702_NOTAGETR: 无卡*/
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
// 转换密钥格式
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
/*名称: Load_keyE2 */
/*功能: 该函数实现把E2中密码存入FM1702的keyRevBuffer中*/
/*输入: Secnr: EE起始地址*/
/*输出: True: 密钥装载成功*/
/* False: 密钥装载失败*/
/****************************************************************/
INT8U Load_keyE2_CPY(INT8U Secnr, INT8U Mode) //ok
{
	INT8U	temp=0;	
	INT8U	msb = 0;
	INT8U	lsb = 0;
	INT8U coded_keys[12]={0};
	INT8U	temp1=0;

	//去掉编译器警报
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
    //修改说明，注释掉的为通过EEPROM加载密钥，现在为通过FIFO加载密钥
	#ifdef LOADEE 
		temp = Write_E2(lsb, msb, 12, coded_keys);	     ////////////////       
		S50.revbuffer[0] = lsb;
		S50.revbuffer[1] = msb;	
		temp = Command_Send(2, S50.revbuffer, LoadKeyE2);
    #else   
    	Command_Send(12,coded_keys,LoadKey);//通过FIFO加载密钥
	#endif

	temp = SPIRead(ErrorFlag) & 0x40;
	if(temp == 0x40)
	{
		return FALSE;
	}

	return TRUE;
}
/****************************************************************/
/*名称: Request */
/*功能: 该函数实现对放入FM1702操作范围之内的卡片的Request操作*/
/*输入: mode: ALL(监测所以FM1702操作范围之内的卡片) */
/* STD(监测在FM1702操作范围之内处于HALT状态的卡片) */
/*输出: FM1702_NOTAGETR: 无卡*/
/* FM1702_OK: 应答正确*/
/* FM1702_REQERR: 应答错误*/
/****************************************************************/
INT8U Request(INT8U mode) //ok
{	
	INT8U 	temp;
	
	SPIWrite(CRCPresetLSB,0x63);		//CRCPresetLSB address is 0x23
	SPIWrite(CWConductance,0x3f);		//选择发射脚TX1和TX2发射天线的阻抗
	S50.revbuffer[0] = mode;		/* Request模式选择 */
	SPIWrite(Bit_Frame,0x07);		
	SPIWrite(ChannelRedundancy,0x03);	
	temp = SPIRead(Control);
	temp = temp & (0xf7);
	SPIWrite(Control,temp);			//Control reset value is 00
	temp = Command_Send(1, S50.revbuffer, Transceive);   //Transceive=0x1E	/* 发送接收命令 */
	//printf("问题定位1:%d\r\n",temp);
	if(temp == FALSE)
	{
		return FM1702_NOTAGETR;
	}

	Read_FIFO(S50.revbuffer);		/* 从FIFO中读取应答信息到RevBuffer[]中 */
	temp = Judge_Req(S50.revbuffer);	/* 判断应答信号是否正确 */
	//printf("问题定位2:%d\r\n",temp);
	if(temp == TRUE)
	{
		S50.tagtype[0] = S50.revbuffer[0];
		S50.tagtype[1] = S50.revbuffer[1];
		return FM1702_OK;
	}

	return FM1702_REQERR;
}
/****************************************************************/
/*名称: AntiColl */
/*功能: 该函数实现对放入FM1702操作范围之内的卡片的防冲突检测*/
/*输入: N/A */
/*输出: FM1702_NOTAGETR: 无卡*/
/* FM1702_BYTECOUNTERR: 接收字节错误*/
/* FM1702_SERNRERR: 卡片序列号应答错误*/
/* FM1702_OK: 卡片应答正确*/
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

		temp = SPIRead(FIFO_Length);//FIFO中数据的BYTE数
		if(temp == 0)
		{
			return FM1702_BYTECOUNTERR;
		}

		Read_FIFO(S50.revbuffer);//读出FIFO中数据
		Save_UID(row, col, temp);			/* 将收到的UID放入UID数组中 */
		
		
		temp = SPIRead(ErrorFlag);				    /* 判断接收数据是否出错 */
		temp = temp & 0x01;
		if(temp == 0x00)
		{//无冲突位
			temp = Check_UID();			/* 校验收到的UID */
			if(temp == FALSE)
			{
				return(FM1702_SERNRERR);//卡片序列号错误
			}

			return(FM1702_OK);
		}
		else
		{//有冲突位
			temp = SPIRead(CollPos);				/* 读取冲突检测寄存器 */
			row = temp / 8;//第row个UID字节有冲突
			col = temp % 8;//第row个UID字节的col位有冲突
			S50.revbuffer[0] = RF_CMD_ANTICOL;
			Set_BitFraming(row + pre_row, col);	/* 设置待发送数据的字节数 */
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
/*名称: Select_Card */
/*功能: 该函数实现对放入FM1702操作范围之内的某张卡片进行选择*/
/*输入: N/A */
/*输出: FM1702_NOTAGETR: 无卡*/
/* FM1702_PARITYERR: 奇偶校验错*/
/* FM1702_CRCERR: CRC校验错*/
/* FM1702_BYTECOUNTERR: 接收字节错误*/
/* FM1702_OK: 应答正确*/
/* FM1702_SELERR: 选卡出错*/
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
		Read_FIFO(S50.revbuffer);	/* 从FIFO中读取应答信息 */
		temp = *S50.revbuffer;
		//printf("temp:%d\r\n",temp);
		if((temp == 0x18) || (temp == 0x08) || (temp == 0x88) || (temp == 0x53)||(temp == 0x20)||(temp == 0x28))	/* 判断应答信号是否正确 */
			return(FM1702_OK);
		else
			return(FM1702_SELERR);
	}
}
/****************************************************************/
/*名称: Authentication */
/*功能: 该函数实现密码认证的过程*/
/*输入: UID: 卡片序列号地址*/
/* SecNR: 扇区号*/
/* mode: 模式*/
/*输出: FM1702_NOTAGETR: 无卡*/
/* FM1702_PARITYERR: 奇偶校验错*/
/* FM1702_CRCERR: CRC校验错*/
/* FM1702_OK: 应答正确*/
/* FM1702_AUTHERR: 权威认证有错*/
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
	temp = Command_Send(6, S50.revbuffer, Authent1); //激活卡片
	if(temp == FALSE)
	{	
		return 0x99;
	}

	temp = SPIRead(ErrorFlag);             //ErrorFlag address is 0x0A.
	if((temp & 0x02) == 0x02) return FM1702_PARITYERR;
	if((temp & 0x04) == 0x04) return FM1702_FRAMINGERR;
	if((temp & 0x08) == 0x08) return FM1702_CRCERR;
	temp = Command_Send(0, S50.revbuffer, Authent2);	//第二论轮认证
	if(temp == FALSE)
	{	
		return 0x88;
	}

	temp = SPIRead(ErrorFlag);  //第三轮认证
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
/*名称: MIF_Read */
/*功能: 该函数实现读MIFARE卡块的数值*/
/*输入: buff: 缓冲区首地址*/
/* Block_Adr: 块地址*/
/*输出: FM1702_NOTAGETR: 无卡*/
/* FM1702_PARITYERR: 奇偶校验错*/
/* FM1702_CRCERR: CRC校验错*/
/* FM1702_BYTECOUNTERR: 接收字节错误*/
/* FM1702_OK: 应答正确*/
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
/*名称: MIF_Write */
/*功能: 该函数实现写MIFARE卡块的数值*/
/*输入: buff: 缓冲区首地址*/
/* Block_Adr: 块地址*/
/*输出: FM1702_NOTAGETR: 无卡*/
/* FM1702_BYTECOUNTERR: 接收字节错误*/
/* FM1702_NOTAUTHERR: 未经权威认证*/
/* FM1702_EMPTY: 数据溢出错误*/
/* FM1702_CRCERR: CRC校验错*/
/* FM1702_PARITYERR: 奇偶校验错*/
/* FM1702_WRITEERR: 写卡块数据出错*/
/* FM1702_OK: 应答正确*/
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
/*名称: MIF_Increment */
/*功能: 该函数实现MIFARE卡自动增值操作*/
/*输入: buff: 四个字节数值起始地址*/
/* Block_Adr: 块地址*/
/*输出: FM1702_NOTAGETR: 无卡*/
/* FM1702_BYTECOUNTERR: 接收字节错误*/
/* FM1702_NOTAUTHERR: 未经权威认证*/
/* FM1702_EMPTY: 数据溢出错误*/
/* FM1702_CRCERR: CRC校验错*/
/* FM1702_PARITYERR: 奇偶校验错*/
/* FM1702_INCRERR: 卡片增款操作失败*/
/* FM1702_OK: 应答正确*/
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
/*名称: MIF_Decrement */
/*功能: 该函数实现MIFARE卡自动减值操作*/
/*输入: buff: 四个字节数值起始地址*/
/* Block_Adr: 块地址*/
/*输出: FM1702_NOTAGETR: 无卡*/
/* FM1702_BYTECOUNTERR: 接收字节错误*/
/* FM1702_NOTAUTHERR: 未经权威认证*/
/* FM1702_EMPTY: 数据溢出错误*/
/* FM1702_CRCERR: CRC校验错*/
/* FM1702_PARITYERR: 奇偶校验错*/
/* FM1702_DECRERR: 卡片扣款操作失败*/
/* FM1702_OK: 应答正确*/
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
/*名称: MIF_Restore */
/*功能: 该函数实现MIFARE卡自动恢复,备份操作*/
/*输入: Block_Adr: 块地址*/
/*输出: FM1702_NOTAGETR: 无卡*/
/* FM1702_BYTECOUNTERR: 接收字节错误*/
/* FM1702_NOTAUTHERR: 未经权威认证*/
/* FM1702_EMPTY: 数据溢出错误*/
/* FM1702_CRCERR: CRC校验错*/
/* FM1702_PARITYERR: 奇偶校验错*/
/* FM1702_RESTERR: 卡片恢复,备份操作失败*/
/* FM1702_OK: 应答正确*/
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
/*名称: MIF_Transfer */
/*功能: 该函数实现MIFARE卡电子钱包保存操作*/
/*输入: Block_Adr: 块地址*/
/*输出: FM1702_NOTAGETR: 无卡*/
/* FM1702_BYTECOUNTERR: 接收字节错误*/
/* FM1702_NOTAUTHERR: 未经权威认证*/
/* FM1702_EMPTY: 数据溢出错误*/
/* FM1702_CRCERR: CRC校验错*/
/* FM1702_PARITYERR: 奇偶校验错*/
/* FM1702_TRANSERR: 卡片恢复,备份操作失败*/
/* FM1702_OK: 应答正确*/
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
/*名称: HL_Active */
/*功能: 该函数实现高级MIFARE卡激活命令*/
/*输入: Secnr: 扇区号*/
/* Block_Adr: 块地址*/
/*输出: 操作状态码*/
/* 读出数据存于RevBuffer中*/
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
/*名称: MIF_Initival */
/*功能: 该函数实现MIFARE卡初始化值操作*/
/*输入: buff: 四个字节初始化数值起始地址*/
/* Block_Adr: 块地址*/
/*输出: FM1702_NOTAGETR: 无卡*/
/* FM1702_BYTECOUNTERR: 接收字节错误*/
/* FM1702_NOTAUTHERR: 未经权威认证*/
/* FM1702_EMPTY: 数据溢出错误*/
/* FM1702_CRCERR: CRC校验错*/
/* FM1702_PARITYERR: 奇偶校验错*/
/* FM1702_WRITEERR: 写卡块数据出错*/
/* FM1702_OK: 应答正确*/
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
/*名称: HL_Read */
/*功能: 该函数实现高级读命令*/
/*输入: Secnr: 扇区号*/
/* Block_Adr: 块地址*/
/*输出: 操作状态码*/
/* 读出数据存于RevBuffer中*/
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
/*名称: HL_Write */
/*功能: 该函数实现高级写命令*/
/*输入: buff: 待写入数据的首地址*/
/* Secnr: 扇区号*/
/* Block_Adr: 块地址*/
/*输出:操作状态码*/
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



