#include "DG_protocol.h"

uint8_t GetType(uint8_t *p_buf)
{
	if(p_buf[0] == 0x66 || p_buf[0] == 0xcc)
	{
		s_DG_response_common *p_response = (s_DG_response_common *)p_buf;
		return p_response ->type;
	}
	else if(p_buf[0] == 0x55 || p_buf[0] == 0xaa)
	{
		s_DG_data_common *p_command  = (s_DG_data_common *)p_buf;
		return p_command ->type;
	}
	else
	{
		return 0xff;
	}
}
 uint16_t GetLength(uint8_t *p_buf)
{
	if(p_buf[0] == 0x66 || p_buf[0] == 0xcc)
	{
		s_DG_response_common *p_response = (s_DG_response_common *)p_buf;
		return p_response ->length;
	}
	else if(p_buf[0] == 0x55 || p_buf[0] == 0xaa)
	{
		s_DG_data_common *p_command  = (s_DG_data_common *)p_buf;
		return p_command ->length;
	}
	else
	{
		return 0xffff;
	}
}
 uint8_t GetSrc(uint8_t *p_buf)
{
	if(p_buf[0] == 0x66 || p_buf[0] == 0xcc)
	{
		s_DG_response_common *p_response = (s_DG_response_common *)p_buf;
		return p_response ->src;
	}
	else if(p_buf[0] == 0x55 || p_buf[0] == 0xaa)
	{
		s_DG_data_common *p_command  = (s_DG_data_common *)p_buf;
		return p_command ->src;
	}
	else
	{
		return 0xff;
	}
}
uint8_t GetDest(uint8_t *p_buf)
{
	if(p_buf[0] == 0x66 || p_buf[0] == 0xcc)
	{
		s_DG_response_common *p_response = (s_DG_response_common *)p_buf;
		return p_response ->dest;
	}
	else if(p_buf[0] == 0x55 || p_buf[0] == 0xaa)
	{
		s_DG_data_common *p_command  = (s_DG_data_common *)p_buf;
		return p_command ->dest;
	}
	else
	{
		return 0xff;
	}
}
X_Boolean CopyFrame(uint8_t *p_src,uint8_t *p_dest)
{
	uint8_t i;
	uint8_t length = GetLength(p_src);
	if(length >MAX_DG_FRAME_LENGTH) {return X_False;}
	for(i=0;i<length;i++)
	{
		p_dest[i] = p_src[i];
	}
	return X_True;
}
X_Boolean SetSrcDest(uint8_t *p_buf,uint8_t src,uint8_t dest)
{
	if(p_buf[0] == 0x66 || p_buf[0] == 0xcc)
	{
		s_DG_response_common *p_response = (s_DG_response_common *)p_buf;
		p_response ->src =  src;
		p_response ->dest = dest;
		return X_True;
	}
	else if(p_buf[0] == 0x55 || p_buf[0] == 0xaa)
	{
		s_DG_data_common *p_command  = (s_DG_data_common *)p_buf;
		p_command ->src =  src;
		p_command ->dest = dest;
		return X_True;
	}
	else
	{
		return X_False;
	}
}
X_Boolean LoadCheckSum(uint8_t *p_buf,uint8_t length)
{
	uint8_t i;
	uint8_t sum = 0;
	if(length == 0 || length == 255) {return X_False;}
	for(i=0;i<length - 1;i++)
	{
		sum += p_buf[i];
	}
	p_buf[length - 1] = sum;
	return X_True;
}
X_Boolean DoesItAboutMeAndComeFromAdjcent(uint8_t me,uint8_t foreward,uint8_t backward,uint8_t src,uint8_t dest)
{
   if((me >= dest && me <= src) || (me >= src && me <= dest))
   {
		if(src == foreward || src == backward)
		{
			return X_True;
		}
   }
   return X_False;
}
X_Boolean DoesImediatelyAckForMe(uint8_t *p_buf,uint8_t me,uint8_t src,uint8_t expect_type)
{
	s_DG_immedicate_ack *p_response = (s_DG_immedicate_ack *)p_buf; 
	if(p_response ->header != 0x66cc) {return X_False;}
	if(p_response ->length != 9){return X_False;}
	if(p_response ->src != src){return X_False;}
	if(p_response ->dest != me){return X_False;}
	if(p_response ->type != IMMEDIATELY_ACK_TYPE){return X_False;}
	if(p_response ->data_or_command_type != expect_type){return X_False;}
	// check sum has been checked by protocol_recv module
	return X_True;
}


