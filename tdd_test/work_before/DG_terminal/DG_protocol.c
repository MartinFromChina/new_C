#include "DG_protocol.h"

#define MULTICAST_DEBUG   0
#define MULTICAST_SPEED_SET_DEBUG   0


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
	X_Boolean isUpload = X_False,isTransToBigNumDirefction;
	s_DG_response_common *p_response 	= (s_DG_response_common *)p_send;
	s_DG_info_mul_get     *p_recv_command = (s_DG_info_mul_get *)p_recv;

	
	
	if(frame_type == eft_command && direct != efd_for_me)
	{
	  s_DG_data_common *p_command  = (s_DG_data_common *)p_send;// p_recv have been copy to p_send
	  p_command ->src = p_terminal ->terminal_num;
	  if(type == MULTICAST_SET_SPEED)
	  {
	  	s_DG_speed_mul_set * p_speed = (s_DG_speed_mul_set *)p_recv;
		isTransToBigNumDirefction = (p_speed ->common.src == p_terminal ->forward_num);
	  	if((p_speed ->start_terminal <= p_terminal ->terminal_num &&  isTransToBigNumDirefction == X_True)
						|| (p_speed ->start_terminal >= p_terminal ->terminal_num && isTransToBigNumDirefction == X_False))
	  	{
			INSERT(LogDebug)( MULTICAST_SPEED_SET_DEBUG,("***********terminal %d speed set ; start terrminal % d ; direct big %s\r\n"
								,p_terminal ->terminal_num,p_speed ->start_terminal
								,(isTransToBigNumDirefction == X_True) ? "yes": "no"));
			s_DG_speed_mul_set * p_speed_set = (s_DG_speed_mul_set *)p_recv;
			p_terminal ->p_info ->DG_wave_speed = p_speed_set ->speed;
		}		
	  }
      return X_True;
	}
	
	switch (direct)
	{
		case efd_for_me:
			if(frame_type == eft_command)
			{
				if(type == MULTICAST_GET_INFO)
				{
					p_response ->header 				= 0x66cc;
					p_response ->length 				= 12;
					p_response ->src					= p_terminal ->terminal_num;
					p_response ->dest					= p_recv_command ->common.src;
					p_response ->type                   = type;
					p_response ->local_terminal         = p_recv_command ->start_terminal;

					uint8_t * p_temp 					= (uint8_t *)(&p_send[8]);
					uint16_t *p_speed                 	= (uint16_t *)(&p_send[9]);
					*p_temp      						= p_terminal ->p_info ->temperature_threshold;
					*p_speed							= p_terminal ->p_info ->DG_wave_speed;
					
					isUpload 							= X_True;
				}
				else if(type == MULTICAST_SET_SPEED)
				{
					s_DG_speed_mul_set * p_speed_set = (s_DG_speed_mul_set *)p_recv;
					p_terminal ->p_info ->DG_wave_speed = p_speed_set ->speed;
				}
			}
			else
			{
				if(p_terminal ->p_wait_ack ->isStartPoint == X_True) 
				{
					INSERT(LogDebug)( MULTICAST_DEBUG,("****************meet touch point ; report the data to the host\r\n"));
					return X_False;
				}
				
				if(type == MULTICAST_GET_INFO)
				{	
					uint8_t start_terminal = p_response ->local_terminal;
					isTransToBigNumDirefction = (p_response ->src == p_terminal ->forward_num);
					p_response ->src = p_terminal ->terminal_num;
				    
					if(isTransToBigNumDirefction == X_True) {p_response ->dest = p_terminal ->backward_num;}
					else {p_response ->dest = p_terminal ->forward_num;}
			
					if((start_terminal > p_terminal ->terminal_num &&  isTransToBigNumDirefction == X_False)
						|| (start_terminal < p_terminal ->terminal_num && isTransToBigNumDirefction == X_True))// just trans up 
					{
		  				isUpload = X_True;
						INSERT(LogDebug)( MULTICAST_DEBUG,("******************just trans up start terminal %d ; to big num %s\r\n",start_terminal,(isTransToBigNumDirefction == X_True) ? "yes": "no"));
					}
					else  // add local info then trans up
					{
						p_response ->length 				= p_response ->length + 3;
						//p_response ->local_terminal         = unchange

						uint8_t * p_temp 					= (uint8_t *)(&p_send[p_response ->length - 4]);
						uint16_t *p_speed                 	= (uint16_t *)(&p_send[p_response ->length - 3]);
						*p_temp      						= p_terminal ->p_info ->temperature_threshold;
						*p_speed							= p_terminal ->p_info ->DG_wave_speed;
						
						 isUpload = X_True;
						 INSERT(LogDebug)( MULTICAST_DEBUG,("******************add local info then trans up\r\n"));
					}
				}
			}
			break;
		case efd_trans_down: // must be response data type , and must be multicast type
			break;
		case efd_trans_up:   // must be response data type   and must be multicast type
			break;
		default:
			 break;
	}
	return isUpload;
}

