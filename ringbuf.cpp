/**************************************
     *@file       ringbuf.c
     *@author     ZW
     *@version    V1.0
     *@date       2024.08.18
     *@brief      环形缓冲区
	 *@note       None
***************************************/

#include "stdio.h"
//#include "ringbuf.h"

typedef  unsigned int      uint16_t;             //放到Keil时不用写这两行 
typedef  unsigned char     uint8_t;

typedef struct
{
	uint8_t     *buffer;                       //数据缓冲区的首地址 
	uint16_t     size;                        //数据缓冲区的大小     两者结合等价于buffer[size] 
	uint16_t     in;                         //要写入位置的指针 
	uint16_t     out;                       //要读取位置的指针       此指针非真正的指针，只是记录这个位置的标识符 
}CricularBuffer; 

/**************************************
     *@brief     初始化环形缓冲区 
	 *@param     fifo：缓冲区实例  buffer:fifo的缓冲区 size：缓冲区大小 
	 *@note      None
	 *@retval    None 
***************************************/
void InitBuffer(CricularBuffer *fifo,uint8_t *buffer,uint16_t size)
{
	fifo->buffer = buffer;
	fifo->in = 0;
	fifo->out = 0;
	fifo->size = size;
}

/**************************************
     *@brief     判断fifo缓冲区是否为空 
	 *@param     fifo：缓冲区实例 
	 *@note      None
	 *@retval    1为空 0不为空（有数据） 
***************************************/
bool  RingBuffer_IsEmpty(CricularBuffer *fifo)
{
	return fifo->in == fifo->out;
}

/**************************************
     *@brief     判断fifo缓冲区是否为满 
	 *@param     fifo：缓冲区实例 
	 *@note      None
	 *@retval    1为满 0不为满 
***************************************/
bool  RingBuffer_IsFull(CricularBuffer *fifo)
{
	return (fifo->in+1)%fifo->size == fifo->out;
}

/**************************************
     *@brief     获取已使用的空间 
	 *@param     fifo：缓冲区实例 
	 *@note      None
	 *@retval    已使用个数 
***************************************/
uint16_t  RingBuffer_GetUsedSize(CricularBuffer *fifo)
{
	if(fifo->in >= fifo->out)
	{
		return (fifo->in - fifo->out);
	}
	else
	{
		return (fifo->size - fifo->out + fifo->in);
	}
}

/**************************************
     *@brief     获取未使用的空间 
	 *@param     fifo：缓冲区实例 
	 *@note      None
	 *@retval    未使用个数 
***************************************/
uint16_t  RingBuffer_GetAvailableSize(CricularBuffer *fifo)
{
	return (fifo->size -RingBuffer_GetUsedSize(fifo) - 1);
}

/**************************************
  * @brief  发送数据到环形缓冲区（不检测剩余空间）
  * @param  fifo: 实例
  * @param  data: &#&
  * @param  len: &#&
  * @retval none
***************************************/
void RingBuffer_In(CricularBuffer *fifo,uint8_t *data,uint16_t len)
{
	for(int i=0;i<len;i++)
	{
		fifo->buffer[fifo->in] = data[i];
		fifo->in = (fifo->in+1)%fifo->size;
	}
	
} 

/**************************************
  * @brief  发送数据到环形缓冲区(带剩余空间检测，空间不足发送失败)
  * @param  fifo: 实例
  * @param  data: &#&
  * @param  len: &#&
  * @retval uint8_t: 0 成功 1失败（空间不足）
***************************************/
uint8_t RingBuffer_In_Check(CricularBuffer *fifo,uint8_t *data,uint16_t len)
{
	if(RingBuffer_IsFull(fifo)==1)                        //这里要注意函数赋值不可加& 
	{
		return 1;
	}
	
	RingBuffer_In(fifo,data,len);
	
	return 0;
}

/**************************************
  * @brief  从环形缓冲区读取数据
  * @param  fifo: 实例
  * @param  buf: 存放数组
  * @param  len: 存放数组长度
  * @retval uint16_t: 实际读取个数
***************************************/
uint16_t RingBuffer_Out(CricularBuffer *fifo,uint8_t *buf,uint16_t len) 
{
	uint16_t AvailableData = RingBuffer_GetUsedSize(fifo);
	
	if(AvailableData == 0)                                       //此判断为判断是否环形缓冲区为空，可用函数RingBuffer_IsEmpty替代 
	{
		return AvailableData;
	}
	else if(AvailableData >len)
	{
		AvailableData = len;
	}
	
	for(int i=0;i<len;i++)
	{
		buf[i] = fifo->buffer[fifo->out];
		fifo->out = (fifo->out+1) % fifo->size;
	} 
	
	return AvailableData;
}

