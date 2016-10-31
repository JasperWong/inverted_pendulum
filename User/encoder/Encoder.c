/*********************************************************************************************
��������			����ֵ�����������ļ�
��д�ˣ�			vino&&jasper
��дʱ�䣺			2016-10-15
Ӳ��֧�֣�			MCU:STM32
�ӿ�˵������		CSΪ�͵�ƽ��Ч��CLK�����ض�ȡ(���ߵ�ƽ)
�޸���־��		��
����NO.1-		��ʱ�롢�������ٶ�����						
	NO,2-		����ڶ�������������select������ѡ��0����ڸ˽Ƕȱ�������1���������ٱ�����
*********************************************************************************************
˵��������Encoder_Init()����ʼ�����IO������Encoder_Read()����ȡunsigned int 10λ����
			ͨ�Ų���SSIЭ��
			����ļ���ת�٣�15000rpm
*********************************************************************************************/
#include "Encoder.h" 
#include "sys.h"
/*************************************************
									�ӿ�����
**************************************************/
#define CS		PBout(2)  // ���ֱ�����Ƭѡ	        ��
#define CLK		PBout(1)  // ��ȡʱ��				��
#define DAT		PBin(0)  // ��������				��

#define CS_2	PBout(5)  // ���ֱ�����Ƭѡ	        ��
#define CLK_2	PBout(4)  // ��ȡʱ��				��
#define DAT_2	PBin(3)  // ��������				��
/**************************************************/

#define delayTime			5
#define ReadTime			2			// 2ms��ȡһ������
#define ReadInterval	1/ReadTime	// ��λms

#define CAL_ACCURACY	7			// ��߾��ȣ���������������������Ҫ������������2Ϊ����
double Division_Value = 0.3515625;

// ��ֵ�˲���������С
#define S		4

EncoderState State_Quantity;
EncoderState State_Quantity_2;

unsigned int FIR_Data_BUFF[S];
unsigned int FIR_Data_BUFF_2[S];

/*********************************************************************************************
��������Encoder_Init��������ʼ������
��  ����
����ֵ��
��  ������ʼ�����IO��
��  ע��
**********************************************************************************************/
void Encoder_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_5;	
    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��
	
	CS = 1;
	CLK = 1;
	
	CS_2 = 1;
	CLK_2 = 1;                                                                                                                                                                                                                                                                                                                                          ;
}
///**********************************************************************************************/

///*********************************************************************************************
//��������Encoder_Read��������ȡ����
//��  ����
//����ֵ��
//��  ������SSIЭ���ȡ10λ����
//��  ע��
//**********************************************************************************************/
unsigned int Encoder_Read(int select)
{
	unsigned char k;
	unsigned int i = 0;
	unsigned int dat1=0;

	if( select == 0 )
	{
		CS = 0;
		for(i = 40; i > 0; i--);
		for(k=0;k<10;k++)
		{
			CLK = 0;
			for(i = 40; i > 0; i--);
			CLK = 1;
			for(i = 27; i > 0; i--);
			dat1=(dat1<<1)+DAT;//C5
		}
		for(k=0;k<6;k++)
		{ 
			CLK = 0;
			for(i = 40; i > 0; i--);
			CLK = 1;
			for(i = 27; i > 0; i--);
		}
		CS = 1;
	}
	else if(select==1)
	{
		CS_2 = 0;
		for(i = 40; i > 0; i--);
		for(k=0;k<10;k++)
		{ 
			CLK_2 = 0;
			for(i = 40; i > 0; i--);
			CLK_2 = 1;
			for(i = 27; i > 0; i--);
			dat1=(dat1<<1)+DAT_2;//C5
		}  
		for(k=0;k<6;k++)
		{ 
			CLK_2 = 0;
			for(i = 40; i > 0; i--);
			CLK_2 = 1;
			for(i = 27; i > 0; i--);
		}
		CS_2 = 1;
	}
	return dat1;
}
///**********************************************************************************************/

///*********************************************************************************************
//��������Encoder_SetZero��ʼ����ֹλ�ú���
//��  ����
//����ֵ��
//��  ������ʼ��������0�ľ�ֵֹ
//��  ע��
//**********************************************************************************************/
static u16 origin;
static u16 origin_2;
void Encoder_SetZero(void)
{
  origin = Encoder_Read(0);
  origin_2 = Encoder_Read(1);
}
///**********************************************************************************************/

///*********************************************************************************************
//��������Encoder_GetDataԭʼ���ݻ�ȡ����
//��  ����0: �ڸ˽Ƕ�   1: ���۽Ƕ�
//����ֵ��
//��  �������ؾ�����ֵֹ������ԭʼ����
//��  ע��
//**********************************************************************************************/
unsigned int Encoder_GetData(int select)
{
	u16 data;
	if( select ==  0 )
	{
		data = Encoder_Read(0);
		if(data < origin)
		{
			data = 1023-origin+data;
		}
		else
		{
			data = data-origin;
		}
		State_Quantity.angle[1] = State_Quantity.angle[0];
		State_Quantity.angle[0] = data;
	}
	else
	{
		data = Encoder_Read(1);
		if(data < origin_2)
		{
			data = 1023-origin_2+data;
		}
		else
		{
			data = data-origin_2;
		}
		State_Quantity_2.angle[1] = State_Quantity_2.angle[0];
		State_Quantity_2.angle[0] = data;
	}
	return data;
}
///**********************************************************************************************/

///*********************************************************************************************
//��������Encoder_GetAngle�ǶȻ�ȡ����
//��  ����
//����ֵ����ǰ�Ƕ�ֵ(0~360)
//��  ����
//��  ע������2Ϊ����(����)����10������ٶ�
//**********************************************************************************************/
float Encoder_GetAngle(int select) 
{
	unsigned int data;
	// unsigned char i;
	unsigned long int accuracy = 1;
	unsigned long int division_value;
	float angle;
	
	data = Encoder_GetData(select);
	accuracy <<= CAL_ACCURACY;
	division_value = Division_Value*accuracy;
	angle = (float)((data*division_value)/accuracy);
	
	if(select == 0)
	{
		State_Quantity.real_angle[1] = State_Quantity.real_angle[0];
		State_Quantity.real_angle[0] = angle;
	}
	
	if(select == 1)
	{
		State_Quantity_2.real_angle[1] = State_Quantity_2.real_angle[0];
		State_Quantity_2.real_angle[0] = angle;
	}
	
	return angle;
}

///**********************************************************************************************/

///*********************************************************************************************
//��������Encoder_GetSpeed�ٶȻ�ȡ����
//��  ����
//����ֵ����ǰ�ٶ�ֵ(��λΪ 0.3515625��/(ReadTime*5)ms )  
//��  ������ȡ��(ReadTime*5)ms�ڵ�ƽ���ٶ�
//��  ע��δת����λ
//**********************************************************************************************/
signed int Encoder_GetSpeed(int select)
{
	signed int speed = 0;
	
	if( select == 0 )
	{
		Encoder_GetData(0);
		if(State_Quantity.angle[0] >= State_Quantity.angle[1])
		{
			if((State_Quantity.angle[0] - State_Quantity.angle[1]) >= 512)
			{
				speed = -1*(1024 - State_Quantity.angle[0] + State_Quantity.angle[1]);
			}
			else
			{
				speed = (State_Quantity.angle[0] - State_Quantity.angle[1]);
			}
		}
		else
		{
			if((State_Quantity.angle[1] - State_Quantity.angle[0]) >= 512)
			{
				speed = (1024 - State_Quantity.angle[1] + State_Quantity.angle[0]);
			}
			else
			{
				speed = (State_Quantity.angle[0] - State_Quantity.angle[1]);
			}
		}
	}
	else
	{
		Encoder_GetData(1);
		State_Quantity_2.speed[1] = State_Quantity_2.speed[0];
		if(State_Quantity_2.angle[0] >= State_Quantity_2.angle[1])
		{
			if((State_Quantity_2.angle[0] - State_Quantity_2.angle[1]) >= 512)
			{
				speed = -1*(1024 - State_Quantity_2.angle[0] + State_Quantity_2.angle[1]);
				State_Quantity_2.speed[0] = -1*(1024 - State_Quantity_2.angle[0] + State_Quantity_2.angle[1]);
			}
			else
			{
				speed = (State_Quantity_2.angle[0] - State_Quantity_2.angle[1]);
				State_Quantity_2.speed[0] = (State_Quantity_2.angle[0] - State_Quantity_2.angle[1]);
			}
		}
		else
		{
			if((State_Quantity_2.angle[1] - State_Quantity_2.angle[0]) >= 512)
			{
				speed = (1024 - State_Quantity_2.angle[1] + State_Quantity_2.angle[0]);
				State_Quantity_2.speed[0] = (1024 - State_Quantity_2.angle[1] + State_Quantity_2.angle[0]);
			}
			else
			{
				speed = (State_Quantity_2.angle[0] - State_Quantity_2.angle[1]);
				State_Quantity_2.speed[0] = (State_Quantity_2.angle[0] - State_Quantity_2.angle[1]);
			}
		}
	}
	return speed;
}
///**********************************************************************************************/


///*********************************************************************************************
//��������Encoder_GetFIRData�̶�ֵ��ȡ�����������˲���
//��  ����
//����ֵ���������̶�ֵ
//��  ����
//��  ע��
//**********************************************************************************************/
unsigned int Encoder_GetFIRData(int select)
{
	u8 i;
	unsigned int Sum = 0;
	static unsigned int cnt = 0;
	static unsigned int cnt_2 = 0;

	if( select == 0 )
	{
		FIR_Data_BUFF[cnt++] = Encoder_GetData(0);
		if( cnt >= S )
		{
			cnt = 0;
		}
		
		for(i = 0; i < S; i++)
		{
			Sum += FIR_Data_BUFF[i];
		}
		if( FIR_Data_BUFF[cnt-1] > FIR_Data_BUFF[cnt] )
		{
			if( FIR_Data_BUFF[cnt-1] - FIR_Data_BUFF[cnt] > 50 )
			{
				Sum = FIR_Data_BUFF[cnt]*S;
			}
		}
		else
		{
			if( FIR_Data_BUFF[cnt] - FIR_Data_BUFF[cnt-1] > 50 )
			{
				Sum = FIR_Data_BUFF[cnt]*S;
			}
		}
		Sum /= S;
	}
	else
	{
		FIR_Data_BUFF_2[cnt_2++] = Encoder_GetData(1);
		if( cnt_2 >= S )
		{
			cnt_2 = 0;
		}
		
		for(i = 0; i < S; i++)
		{
			Sum += FIR_Data_BUFF_2[i];
		}
		if( FIR_Data_BUFF_2[cnt_2-1] > FIR_Data_BUFF_2[cnt_2] )
		{
			if( FIR_Data_BUFF_2[cnt_2-1] - FIR_Data_BUFF_2[cnt_2] > 50 )
			{
				Sum = FIR_Data_BUFF_2[cnt_2]*S;
			}
		}
		else
		{
			if( FIR_Data_BUFF_2[cnt_2] - FIR_Data_BUFF_2[cnt_2-1] > 50 )
			{
				Sum = FIR_Data_BUFF_2[cnt_2]*S;
			}
		}
		Sum /= S;
	}
	return Sum;
}
/**********************************************************************************************/





