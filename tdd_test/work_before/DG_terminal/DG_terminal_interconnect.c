#include "DG_terminal_interconnect.h"
#include "DG_protocol.h"

typedef enum
{
	efm_for_me,
	efm_pass_me_up,
	efm_pass_me_down,
	efm_not_my_business,
}e_frame_and_me;

static e_frame_and_me FrameAndMe(uint16_t header,uint8_t type,uint8_t me,uint8_t src,uint8_t dest)
{

}

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
		return 0xffff;
	}
}

X_Boolean TerminalInterconnectHandle(const s_terminal * p_terminal,uint8_t *p_recv,uint8_t *p_send)
{
	s_DG_data_common *p_common = (s_DG_data_common*)p_recv;
	
	//if(p_terminal->terminal_num == )
	return X_False;
}

