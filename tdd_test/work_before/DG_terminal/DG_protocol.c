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

X_Boolean DoesMultiCastType(uint8_t type)
{
	return ((MULTICAST_MASK & type) == MULTICAST_PREFIX);
}
X_Boolean DG_CommandHandle(const s_terminal *p_terminal,e_frame_type frame_type,e_frame_direction direct,uint8_t type,uint8_t *p_recv,uint8_t *p_send) // return true means need response;
{
	X_Boolean isUpload = X_False;
	if(frame_type == eft_command && direct != efd_for_me)
	{
	  s_DG_data_common *p_command  = (s_DG_data_common *)p_send;// p_recv have been copy to p_send
	  p_command ->src = p_terminal ->terminal_num;
      return X_True;
	}
	switch (direct)
	{
		case efd_for_me:
			if(frame_type == eft_command)
			{
				if(type == MULTICAST_GET_INFO)
				{
					s_DG_response_common *p_response 	= (s_DG_response_common *)p_send;
					s_DG_data_common     *p_recv_command = (s_DG_data_common *)p_recv;
					
					p_response ->header 				= 0x66cc;
					p_response ->length 				= 12;
					p_response ->src					= p_terminal ->terminal_num;
					p_response ->dest					= p_recv_command ->src;
					p_response ->type                   = type;
					p_response ->local_terminal         = p_terminal ->terminal_num;

					s_terminal_inf * p_info 			= (s_terminal_inf *)(&p_send[8]);
					p_info ->temperature_threshold      = p_terminal ->p_info ->temperature_threshold;
					p_info ->DG_wave_speed				= p_terminal ->p_info ->DG_wave_speed;

					isUpload = X_True;
				}
			}
			break;
		case efd_trans_down:
			break;
		case efd_trans_up:
			break;
		default:
			 break;
	}
	return isUpload;
}

