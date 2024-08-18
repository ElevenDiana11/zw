/**************************************
     *@file       ringbuf.c
     *@author     ZW
     *@version    V1.0
     *@date       2024.08.18
     *@brief      ���λ�����
	 *@note       None
***************************************/

#include "stdio.h"
//#include "ringbuf.h"

typedef  unsigned int      uint16_t;             //�ŵ�Keilʱ����д������ 
typedef  unsigned char     uint8_t;

typedef struct
{
	uint8_t     *buffer;                       //���ݻ��������׵�ַ 
	uint16_t     size;                        //���ݻ������Ĵ�С     ���߽�ϵȼ���buffer[size] 
	uint16_t     in;                         //Ҫд��λ�õ�ָ�� 
	uint16_t     out;                       //Ҫ��ȡλ�õ�ָ��       ��ָ���������ָ�룬ֻ�Ǽ�¼���λ�õı�ʶ�� 
}CricularBuffer; 

/**************************************
     *@brief     ��ʼ�����λ����� 
	 *@param     fifo��������ʵ��  buffer:fifo�Ļ����� size����������С 
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
     *@brief     �ж�fifo�������Ƿ�Ϊ�� 
	 *@param     fifo��������ʵ�� 
	 *@note      None
	 *@retval    1Ϊ�� 0��Ϊ�գ������ݣ� 
***************************************/
bool  RingBuffer_IsEmpty(CricularBuffer *fifo)
{
	return fifo->in == fifo->out;
}

/**************************************
     *@brief     �ж�fifo�������Ƿ�Ϊ�� 
	 *@param     fifo��������ʵ�� 
	 *@note      None
	 *@retval    1Ϊ�� 0��Ϊ�� 
***************************************/
bool  RingBuffer_IsFull(CricularBuffer *fifo)
{
	return (fifo->in+1)%fifo->size == fifo->out;
}

/**************************************
     *@brief     ��ȡ��ʹ�õĿռ� 
	 *@param     fifo��������ʵ�� 
	 *@note      None
	 *@retval    ��ʹ�ø��� 
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
     *@brief     ��ȡδʹ�õĿռ� 
	 *@param     fifo��������ʵ�� 
	 *@note      None
	 *@retval    δʹ�ø��� 
***************************************/
uint16_t  RingBuffer_GetAvailableSize(CricularBuffer *fifo)
{
	return (fifo->size -RingBuffer_GetUsedSize(fifo) - 1);
}

/**************************************
  * @brief  �������ݵ����λ������������ʣ��ռ䣩
  * @param  fifo: ʵ��
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
  * @brief  �������ݵ����λ�����(��ʣ��ռ��⣬�ռ䲻�㷢��ʧ��)
  * @param  fifo: ʵ��
  * @param  data: &#&
  * @param  len: &#&
  * @retval uint8_t: 0 �ɹ� 1ʧ�ܣ��ռ䲻�㣩
***************************************/
uint8_t RingBuffer_In_Check(CricularBuffer *fifo,uint8_t *data,uint16_t len)
{
	if(RingBuffer_IsFull(fifo)==1)                        //����Ҫע�⺯����ֵ���ɼ�& 
	{
		return 1;
	}
	
	RingBuffer_In(fifo,data,len);
	
	return 0;
}

/**************************************
  * @brief  �ӻ��λ�������ȡ����
  * @param  fifo: ʵ��
  * @param  buf: �������
  * @param  len: ������鳤��
  * @retval uint16_t: ʵ�ʶ�ȡ����
***************************************/
uint16_t RingBuffer_Out(CricularBuffer *fifo,uint8_t *buf,uint16_t len) 
{
	uint16_t AvailableData = RingBuffer_GetUsedSize(fifo);
	
	if(AvailableData == 0)                                       //���ж�Ϊ�ж��Ƿ��λ�����Ϊ�գ����ú���RingBuffer_IsEmpty��� 
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

