/*********************************************************************************************
程序名：			绝对值编码器驱动文件
编写人：			vino&&jasper
编写时间：			2016-10-15
硬件支持：			MCU:STM32
接口说明：　		CS为低电平生效，CLK上升沿读取(常高电平)
修改日志：		　
　　NO.1-		逆时针、过零点的速度问题						
	NO,2-		加入第二个编码器，用select变量来选择，0代表摆杆角度编码器，1代表电机测速编码器
*********************************************************************************************
说明：调用Encoder_Init()来初始化相关IO，调用Encoder_Read()来读取unsigned int 10位数据
			通信采用SSI协议
			允许的极限转速：15000rpm
*********************************************************************************************/
#include "Encoder.h" 
#include "sys.h"
/*************************************************
									接口设置
**************************************************/
#define CS		PBout(2)  // 左轮编码器片选	        黄
#define CLK		PBout(1)  // 读取时钟				绿
#define DAT		PBin(0)  // 数据输入				白

#define CS_2	PBout(5)  // 左轮编码器片选	        黄
#define CLK_2	PBout(4)  // 读取时钟				绿
#define DAT_2	PBin(3)  // 数据输入				白
/**************************************************/

#define delayTime			5
#define ReadTime			2			// 2ms读取一次数据
#define ReadInterval	1/ReadTime	// 单位ms

#define CAL_ACCURACY	7			// 最高精度，将浮点数化成整数，若要加速运算则以2为乘数
double Division_Value = 0.3515625;

// 均值滤波缓冲区大小
#define S		4

EncoderState State_Quantity;
EncoderState State_Quantity_2;

unsigned int FIR_Data_BUFF[S];
unsigned int FIR_Data_BUFF_2[S];

/*********************************************************************************************
函数名：Encoder_Init编码器初始化函数
参  数：
返回值：
结  果：初始化相关IO口
备  注：
**********************************************************************************************/
void Encoder_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_5;	
    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化
	
	CS = 1;
	CLK = 1;
	
	CS_2 = 1;
	CLK_2 = 1;                                                                                                                                                                                                                                                                                                                                          ;
}
///**********************************************************************************************/

///*********************************************************************************************
//函数名：Encoder_Read编码器读取函数
//参  数：
//返回值：
//结  果：用SSI协议读取10位数据
//备  注：
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
//函数名：Encoder_SetZero初始化静止位置函数
//参  数：
//返回值：
//结  果：初始化编码器0的静止值
//备  注：
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
//函数名：Encoder_GetData原始数据获取函数
//参  数：0: 摆杆角度   1: 旋臂角度
//返回值：
//结  果：返回经过静止值处理后的原始数据
//备  注：
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
//函数名：Encoder_GetAngle角度获取函数
//参  数：
//返回值：当前角度值(0~360)
//结  果：
//备  注：可用2为乘数(左移)代替10以提高速度
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
//函数名：Encoder_GetSpeed速度获取函数
//参  数：
//返回值：当前速度值(单位为 0.3515625度/(ReadTime*5)ms )  
//结  果：获取近(ReadTime*5)ms内的平均速度
//备  注：未转换单位
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
//函数名：Encoder_GetFIRData刻度值获取函数（经过滤波）
//参  数：
//返回值：编码器刻度值
//结  果：
//备  注：
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





