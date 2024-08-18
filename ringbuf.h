/**************************************
     *@file       ringbuf.c
     *@author     ZW
     *@version    V1.0
     *@date       2024.08.18
     *@brief      »·ÐÎ»º³åÇø
	 *@note       None
***************************************/
#ifndef  __RINGBUF_H__
#define  __RINGBUF_H__


typedef struct
{
	uint8_t     *buffer;
	uint16_t     size;
	uint16_t     in;
	uint16_t     out;
}CricularBuffer;

void InitBuffer(CricularBuffer *fifo,uint8_t *buffer,uint16_t size);
bool  RingBuffer_IsEmpty(CricularBuffer *fifo);
bool  RingBuffer_IsFull(CricularBuffer *fifo);
uint16_t  RingBuffer_GetUsedSize(CricularBuffer *fifo);
uint16_t  RingBuffer_GetAvailableSize(CricularBuffer *fifo);
void RingBuffer_In(CricularBuffer *fifo,uint8_t *data,uint16_t len);
uint8_t RingBuffer_In_Check(CricularBuffer *fifo,uint8_t *data,uint16_t len);
uint16_t RingBuffer_Out(CricularBuffer *fifo,uint8_t *data,uint16_t len) ;

#endif  //__RINGBUF_H__ 
