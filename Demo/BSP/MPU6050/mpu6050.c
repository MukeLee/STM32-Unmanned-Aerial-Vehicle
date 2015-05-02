#include "include.h"
#define UART_Put_Char(x)  UART1_Put_Char(x)

struct _sensor sensor;	
u8		 mpu6050_buffer[14];					//iic读取后存放数据 	

u8		ACC_OFFSET_OK = 0;


extern vs16 Moto_duty[4];

//**************************************
//初始化MPU6050
//**************************************
u8 InitMPU6050(void)
{
	u8 date;
	date = Single_Write(MPU6050_ADDRESS, PWR_MGMT_1, 0x00);  	//解除休眠状态
	date += Single_Write(MPU6050_ADDRESS, SMPLRT_DIV, 0x07);     
	date += Single_Write(MPU6050_ADDRESS, cONFIG, 0x03);         //低通滤波
	date += Single_Write(MPU6050_ADDRESS, GYRO_CONFIG, 0x10);    //陀螺仪量程 +-1000
	date += Single_Write(MPU6050_ADDRESS, ACCEL_CONFIG, 0x09);   //加速度量程 +-4G
	return date;
}
//**************************实现函数********************************************
//将iic读取到得数据分拆,放入相应寄存器,更新MPU6050_Last
//******************************************************************************
void MPU6050_Read(void)
{
	mpu6050_buffer[0]=Single_Read(MPU6050_ADDRESS, 0x3B);
	mpu6050_buffer[1]=Single_Read(MPU6050_ADDRESS, 0x3C);
	mpu6050_buffer[2]=Single_Read(MPU6050_ADDRESS, 0x3D);
	mpu6050_buffer[3]=Single_Read(MPU6050_ADDRESS, 0x3E);
	mpu6050_buffer[4]=Single_Read(MPU6050_ADDRESS, 0x3F);
	mpu6050_buffer[5]=Single_Read(MPU6050_ADDRESS, 0x40);
	mpu6050_buffer[8]=Single_Read(MPU6050_ADDRESS, 0x43);
	mpu6050_buffer[9]=Single_Read(MPU6050_ADDRESS, 0x44);
	mpu6050_buffer[10]=Single_Read(MPU6050_ADDRESS, 0x45);
	mpu6050_buffer[11]=Single_Read(MPU6050_ADDRESS, 0x46);
	mpu6050_buffer[12]=Single_Read(MPU6050_ADDRESS, 0x47);
	mpu6050_buffer[13]=Single_Read(MPU6050_ADDRESS, 0x48);
	
}
/**************************实现函数********************************************
//将iic读取到得数据分拆,放入相应寄存器
*******************************************************************************/
void MPU6050_Dataanl(void)
{
	MPU6050_Read();
	
	sensor.acc.origin.x = ((((int16_t)mpu6050_buffer[0]) << 8) | mpu6050_buffer[1]) - sensor.acc.quiet.x;
	sensor.acc.origin.y = ((((int16_t)mpu6050_buffer[2]) << 8) | mpu6050_buffer[3]) - sensor.acc.quiet.y;
	sensor.acc.origin.z = ((((int16_t)mpu6050_buffer[4]) << 8) | mpu6050_buffer[5]);

	sensor.gyro.origin.x = ((((int16_t)mpu6050_buffer[8]) << 8) | mpu6050_buffer[9]);
	sensor.gyro.origin.y = ((((int16_t)mpu6050_buffer[10]) << 8)| mpu6050_buffer[11]);
	sensor.gyro.origin.z = ((((int16_t)mpu6050_buffer[12]) << 8)| mpu6050_buffer[13]);
  
	sensor.gyro.radian.x = sensor.gyro.origin.x * Gyro_Gr - sensor.gyro.quiet.x * Gyro_Gr;
	sensor.gyro.radian.y = sensor.gyro.origin.y * Gyro_Gr - sensor.gyro.quiet.y * Gyro_Gr;
	sensor.gyro.radian.z = sensor.gyro.origin.z * Gyro_Gr - sensor.gyro.quiet.z * Gyro_Gr;


////////////////////////////////////////////////////
//    	The calibration  of  gyro and  acc        //
////////////////////////////////////////////////////	

  if(!ACC_OFFSET_OK && Rc_Data.THROTTLE <= 1200 && Rc_Data.THROTTLE >= 900)
	{
		if(Rc_Data.ROLL <= 1200 && Rc_Data.PITCH >= 1750)
		{	
			LED2(ON);
				static int32_t	tempax=0,tempay=0,tempaz=0;
				static uint8_t cnt_a=0;

				if(cnt_a==0)
				{
					sensor.acc.quiet.x = 0;
				  sensor.acc.quiet.y = 0;
				  sensor.acc.quiet.z = 0;
					tempax = 0;
					tempay = 0;
					tempaz = 0;
					cnt_a = 1;
					return;
				}
				tempax+= sensor.acc.origin.x;
				tempay+= sensor.acc.origin.y;
				tempaz+= sensor.acc.origin.z;
				
				if(cnt_a==200)
				{
					sensor.acc.quiet.x = tempax/cnt_a;
					sensor.acc.quiet.y = tempay/cnt_a;
					sensor.acc.quiet.z = tempaz/cnt_a;
					cnt_a = 0;
					ACC_OFFSET_OK = 1;
					EE_SAVE_ACC_OFFSET();//保存数据
					return;
				}
				cnt_a++;		
			}	
  }
	else ACC_OFFSET_OK = 1;
}

/**************************实现函数********************************************
//陀螺仪零点校准
*******************************************************************************/

void Gyro_OFFEST(void)
{
   int cnt_g=2000;
	 int32_t  tempgx=0,tempgy=0,tempgz=0;
	 sensor.gyro.averag.x=0;    //零点偏移清零
	 sensor.gyro.averag.y=0;  
	 sensor.gyro.averag.z=0;
	 while(cnt_g--)       //循环采集2000次   求平均
	 {
		  MPU6050_Read();
		 
		  sensor.gyro.origin.x = ((((int16_t)mpu6050_buffer[8]) << 8) | mpu6050_buffer[9]);
	    sensor.gyro.origin.y = ((((int16_t)mpu6050_buffer[10]) << 8)| mpu6050_buffer[11]);
	    sensor.gyro.origin.z = ((((int16_t)mpu6050_buffer[12]) << 8)| mpu6050_buffer[13]);
      tempgx+= sensor.gyro.origin.x;
			tempgy+= sensor.gyro.origin.y;
			tempgz+= sensor.gyro.origin.z;
   }
	 sensor.gyro.quiet.x=tempgx/2000;
	 sensor.gyro.quiet.y=tempgy/2000;
	 sensor.gyro.quiet.z=tempgz/2000;
	  
}








void UART1_ReportIMU(void)
{
  int16_t ax,ay,az,gx,gy,gz;
  int16_t hx,hy,hz;
  int16_t yaw,pitch,roll;
  uint8_t temp;
	char ctemp;
	UART_Put_Char(0x88); 
	UART_Put_Char(0xAF);  
	UART_Put_Char(0x1C);  

	ax=sensor.acc.origin.x;

	ctemp=ax>>8;
	UART_Put_Char(ctemp); 
	temp+=ctemp;
	ctemp=ax;
	UART_Put_Char(ctemp); 
	temp+=sensor.acc.origin.y;

	ay=angle.roll*110;

	ctemp=ay>>8;
	UART_Put_Char(ctemp); 
	temp+=ctemp;
	ctemp=ay;
	UART_Put_Char(ctemp); 
	temp+=ctemp;

	az=0;

	ctemp=az>>8;
	UART_Put_Char(ctemp);
	temp+=ctemp;
	ctemp=az;
	UART_Put_Char(ctemp);
	temp+=ctemp;

	gx=0;

	ctemp=gx>>8;
	UART_Put_Char(ctemp);
	temp+=ctemp;
	ctemp=gx;
	UART_Put_Char(ctemp);
	temp+=ctemp;

	gy=0;

	ctemp=gy>>8;
	UART_Put_Char(ctemp);
	temp+=ctemp;
	ctemp=gy;
	UART_Put_Char(ctemp);
	temp+=ctemp;

  gz=0;

	ctemp=gz>>8;
	UART_Put_Char(ctemp); 
	temp+=ctemp;
	ctemp=gz;
	UART_Put_Char(ctemp);	
	temp+=ctemp;

	hx=x;

	ctemp=hx>>8;
	UART_Put_Char(ctemp);
	temp+=ctemp;
	ctemp=hx;
	UART_Put_Char(ctemp);
	temp+=ctemp;

	hy=y;

	ctemp=hy>>8;
	UART_Put_Char(ctemp);	
	temp+=ctemp;
	ctemp=hy;
	UART_Put_Char(ctemp);	
	temp+=ctemp;

	hz=z;

	ctemp=hz>>8;
	UART_Put_Char(ctemp);	 
	temp+=ctemp;
	ctemp=hz;
	UART_Put_Char(ctemp);	 
	temp+=ctemp;

	pitch = (int)(angle.pitch*100);
				   

	ctemp=pitch>>8;
	UART_Put_Char(ctemp);	 
	temp+=ctemp;
	ctemp=pitch;
	UART_Put_Char(ctemp);	 
	temp+=ctemp;

	roll = (int)(angle.roll*100);

	ctemp=roll>>8;
	UART_Put_Char(ctemp);	
	temp+=ctemp;
	ctemp=roll;
	UART_Put_Char(ctemp);	 
	temp+=ctemp;

 	yaw = 0;

	ctemp=yaw>>8;
	UART_Put_Char(ctemp);	
	temp+=ctemp;
	ctemp=yaw;
	UART_Put_Char(ctemp);	
	temp+=ctemp;

	UART_Put_Char(0x00);
	UART_Put_Char(0x00);
	UART_Put_Char(0x00);
	UART_Put_Char(0x00);

	UART1_Put_Char(temp); 
}

