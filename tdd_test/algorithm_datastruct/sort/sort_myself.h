#ifndef __SORT_MYSELF_H
#define __SORT_MYSELF_H

#ifdef __cplusplus
	extern "C"{
#endif

typedef unsigned short int      uint16_t;  

bool sort_from_small_to_big(uint16_t *p_in,uint16_t *p_out,uint16_t length);

#ifdef __cplusplus
	}
#endif

#endif
