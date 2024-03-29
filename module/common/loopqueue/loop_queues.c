#include "loop_queues.h"

#define BUF_FREE   0
#define BUF_USED   1

#define USE_INLINE  //__inline // it seems use _-inline cost more time in google_test_frame , I don't understand it 

typedef enum
{
	QueueEmpty = 0,
	QueueNormal,
	QueueFull,
	QueueStateError,
}QueueState;

static USE_INLINE X_Void UpdataListState( const sListManager *p_manager)
{
	//if(p_manager == X_Null) {return;} // no need null check ,because the caller is me 
	if(p_manager->p_LMP->used_node_num == 0)
	{
		p_manager->p_LMP->state = QueueEmpty;
		return;
	}
	if(p_manager->p_LMP->used_node_num >= p_manager->ValidNodeNumber)
	{
		p_manager->p_LMP->state = QueueFull;
		return;
	}
	p_manager->p_LMP->state = QueueNormal;
}

static USE_INLINE uint16_t NodeMoveForward(uint16_t max_node,uint16_t current_node)
{
	uint16_t next_node;
	if((current_node+1) >= max_node)
	{
		next_node = 0;
	}
	else
	{
		next_node = current_node + 1;
	}
	return next_node;
}
static USE_INLINE uint16_t NodeNumberInMoveForward(const sListManager *p_manager)
{
		uint16_t new_in_node_number;
		//if(p_manager == X_Null) {return 0;}  // no need null check ,because the caller is me 
		new_in_node_number = NodeMoveForward(p_manager ->ValidNodeNumber,p_manager->p_LMP->first_in_node_num);
		p_manager->p_LMP->first_in_node_num = new_in_node_number;
		return new_in_node_number;
}
static USE_INLINE uint16_t NodeNumberOutMoveForward(const sListManager *p_manager)
{
	uint16_t new_out_node_number;
	//if(p_manager == X_Null) {return 0;} // no need null check ,because the caller is me 
	new_out_node_number = NodeMoveForward(p_manager ->ValidNodeNumber,p_manager->p_LMP->first_out_node_num);
	p_manager->p_LMP->first_out_node_num = new_out_node_number;
	return new_out_node_number;

}
X_Void 		LoopQueueInitialize(const sListManager *p_manager)
{
	uint16_t i;

	if(p_manager == X_Null) {return;}
	if(p_manager->ValidNodeNumber >= INVALID_LOOP_QUEUE_NODE_NUM) {return;}

	for(i = 0;i< p_manager->ValidNodeNumber;i++)
	{
		p_manager->p_buf[i] = BUF_FREE;
	}
	
	p_manager->p_LMP->isInit = X_True;
	p_manager->p_LMP->state = QueueEmpty;
	p_manager->p_LMP->first_in_node_num = 0;
	p_manager->p_LMP->first_out_node_num = 0;
	p_manager->p_LMP->used_node_num = 0;

}

uint16_t    LoopQueueFirstIn(const sListManager *p_manager,X_Boolean is_OccupyPermit)
{
	uint16_t buf_number,current_free_node_number;
	if(p_manager == X_Null) { return INVALID_LOOP_QUEUE_NODE_NUM;}
	if(p_manager->p_LMP->isInit == X_False) { return INVALID_LOOP_QUEUE_NODE_NUM;}

	buf_number = 0;
	current_free_node_number = p_manager->p_LMP->first_in_node_num;

	switch(p_manager->p_LMP->state)
	{
		case QueueEmpty:
		case QueueNormal:
			if(p_manager->p_buf[current_free_node_number] == BUF_FREE )
			{
				NodeNumberInMoveForward(p_manager);
				if(is_OccupyPermit == X_False) {p_manager->p_buf[current_free_node_number] = BUF_USED;}
				else {p_manager->p_buf[current_free_node_number] = BUF_FREE;}
				p_manager->p_LMP->used_node_num ++;
				UpdataListState(p_manager);

				buf_number = current_free_node_number;
			}
			else
			{
				buf_number = INVALID_LOOP_QUEUE_NODE_NUM;
			}
		break;
		case QueueFull:

			if(p_manager->p_buf[current_free_node_number] == BUF_FREE )
			{
				NodeNumberInMoveForward(p_manager);
				NodeNumberOutMoveForward(p_manager);
				if(is_OccupyPermit == X_False) {p_manager->p_buf[current_free_node_number] = BUF_USED;}
				else {p_manager->p_buf[current_free_node_number] = BUF_FREE;}

				buf_number = current_free_node_number;
			}
			else
			{
				buf_number = INVALID_LOOP_QUEUE_NODE_NUM;
			}
		break;
		default:
			buf_number = INVALID_LOOP_QUEUE_NODE_NUM;
		break;
	}
	return buf_number;
}
uint16_t    LoopQueueFirstOut(const sListManager *p_manager)
{
	uint16_t buf_number,current_filled_node_number;
	if(p_manager == X_Null) { return INVALID_LOOP_QUEUE_NODE_NUM;}
	if(p_manager->p_LMP->isInit == X_False) { return INVALID_LOOP_QUEUE_NODE_NUM;}

	buf_number = 0;
	current_filled_node_number = p_manager->p_LMP->first_out_node_num;
	switch(p_manager->p_LMP->state)
	{
		case QueueEmpty:
			buf_number = INVALID_LOOP_QUEUE_NODE_NUM;
		break;
		case QueueNormal:
		case QueueFull:
			NodeNumberOutMoveForward(p_manager);
			if(p_manager->p_LMP->used_node_num > 0) {p_manager->p_LMP->used_node_num --;}
			UpdataListState(p_manager);
			buf_number = current_filled_node_number;
		break;
		default:
			buf_number = INVALID_LOOP_QUEUE_NODE_NUM;
		break;
	}
	return buf_number;
}
X_Void      ClearLoopQueue(const sListManager *p_manager)
{
	LoopQueueInitialize(p_manager);
}
X_Void      RealseLoopQueueBuf(const sListManager *p_manager,uint8_t buf_num)
{
	if(p_manager == X_Null) {return;}
	if(buf_num >= p_manager->ValidNodeNumber) {return;}
	p_manager->p_buf[buf_num] = BUF_FREE;
}
uint16_t GetLoopQueueUsedNodeNumber(const sListManager *p_manager)
{
	if(p_manager == X_Null) {return 0;}
	if(p_manager->p_LMP->isInit == X_False) { return 0;}
	return p_manager->p_LMP->used_node_num;
}
X_Boolean   DoesLoopQueueEmpty(const sListManager *p_manager)
{
	if(p_manager == X_Null) {return X_True;}
	if(p_manager->p_LMP->isInit == X_False) { return X_True;}
	return (p_manager->p_LMP->state == QueueEmpty);
}

