#define  SYS_DEBUG_METHOD printf

#define SysLogDebug(flag,message)       do{ 																\
                                                                                        if((flag) != 0)								\
                                                                                        {	\
                                                                                                SYS_DEBUG_METHOD	message ;	\
                                                                                        }	\
                                                                                    }while(0)

typedef union {
	uint32_t address;
	struct {
	  uint32_t device_port: 4; //     (bit0 ~ bit3)
      uint32_t device_num:  4; //     (bit4 ~ bit7)
      uint32_t line_num:    4; //     (bit8 ~ bit11)
	  uint32_t kilometer:  10; //     (bit12 ~ bit21)
      uint32_t rail_way_id:10; //     (bit22 ~ bit31)
	 }bit;
}u_AddrResolution;

X_Void DataLinkTransDisplay(u_AddrResolution local,u_AddrResolution src, 
                                    u_AddrResolution dest,u_AddrResolution next,uint8_t type,uint16_t length)
{
    X_Boolean isDirectionForward;
    X_Boolean isLocolInTheMiddle;
    X_Boolean isNextInTheMiddle;
    isDirectionForward = (dest.address >= src.address);

    SysLogDebug(1,("~~~~~~~type %2x,length %d ~~~~~ trans path below : \r\n",type,length));
    if(isDirectionForward == X_True)
    {
        if(local.bit.line_num == 0)
        {
            if(local.bit.device_num == src.bit.device_num)
            {
                
                SysLogDebug(1,(" {%d.%d} -->> ",local.bit.line_num,local.bit.device_num));
                isLocolInTheMiddle = X_False;
            
            }
            else if(local.bit.device_num == dest.bit.device_num)
            {
                SysLogDebug(1,("                                                    {%d.%d}",local.bit.line_num,local.bit.device_num)); // OK
                isLocolInTheMiddle = X_False;
            }
            else
            {
                SysLogDebug(1,("                  {%d.%d} -->> ",local.bit.line_num,local.bit.device_num)); // OK
                isLocolInTheMiddle = X_True;
            }
        }

        if(next.bit.line_num == 0)
        {
            if(next.bit.device_num == dest.bit.device_num)
            {        
                    if(isLocolInTheMiddle == X_True)
                    {
                                                 SysLogDebug(1,("                    (%d.%d) ",next.bit.line_num,next.bit.device_num));
                    }
                    else
                    {
                              SysLogDebug(1,("                                        [%d.%d] ",next.bit.line_num,next.bit.device_num)); // OK
                    }
                                                
            }
            else
            {
                    if(isLocolInTheMiddle == X_False)
                    {
                               SysLogDebug(1,("                                   (%d.%d) ",next.bit.line_num,next.bit.device_num)); // OK
                    }
                    else
                    {
                                                SysLogDebug(1,("  (%d.%d) ",next.bit.line_num,next.bit.device_num)); // OK
                    }
            }
        }
                SysLogDebug(1,("\r\n"));
                SysLogDebug(1,("   A                                                  | \r\n"));
                SysLogDebug(1,("   |                                                  V \r\n"));
                SysLogDebug(1,(" [%d.%d] -------------------------------------------- [%d.%d] \r\n\r\n\r\n"
                    ,src.bit.line_num,src.bit.device_num,dest.bit.line_num,dest.bit.device_num));

    }
    else
    {
        isNextInTheMiddle = X_False;
        if(next.bit.line_num == 0)
        {
            if(next.bit.device_num == dest.bit.device_num)
            {
                SysLogDebug(1,(" [%d.%d]  ",next.bit.line_num,next.bit.device_num)); // OK
                isNextInTheMiddle = X_False;
            }
            else
            {
                SysLogDebug(1,("                    [%d.%d]  ",next.bit.line_num,next.bit.device_num));
                isNextInTheMiddle = X_True;
            }
        }

        if(local.bit.line_num == 0)
        {
            if(local.bit.device_num == src.bit.device_num)
            {
                if(isNextInTheMiddle == X_True)
                {
                                                  SysLogDebug(1,("              {%d.%d}  ",local.bit.line_num,local.bit.device_num)); // OK
                }
                else
                {
                                SysLogDebug(1,("                                       <<-- {%d.%d}  ",local.bit.line_num,local.bit.device_num)); // OK
                }
               
            
            }
            else if(local.bit.device_num == dest.bit.device_num)
            {
                SysLogDebug(1,(" {%d.%d} ",local.bit.line_num,local.bit.device_num)); // OK
            }
            else
            {
                SysLogDebug(1,("       <<-- {%d.%d}  ",local.bit.line_num,local.bit.device_num)); // OK
            }
        }
                SysLogDebug(1,("\r\n"));
                SysLogDebug(1,("   |                                                  A \r\n"));
                SysLogDebug(1,("   V                                                  | \r\n"));
                SysLogDebug(1,(" [%d.%d] -------------------------------------------- [%d.%d] \r\n\r\n\r\n"
                   ,dest.bit.line_num,dest.bit.device_num,src.bit.line_num,src.bit.device_num));
    }
    /*
    SysLogDebug(1,(" ~~~~~ from %d.%d  (trans by %d.%d ,next hop addr %d.%d(%2x)) to %d.%d , type %2x length %d\r\n"
                    
					,local.bit.line_num,local.bit.device_num
                    ,next.bit.line_num,next.bit.device_num,next.address
					
					,type,length));
    */
}


typedef union {
	uint32_t address;
	struct {
	  uint32_t device_port: 4; //     (bit0 ~ bit3)
      uint32_t device_num:  4; //     (bit4 ~ bit7)
      uint32_t line_num:    4; //     (bit8 ~ bit11)
	  uint32_t kilometer:  10; //     (bit12 ~ bit21)
      uint32_t rail_way_id:10; //     (bit22 ~ bit31)
	 }bit;
}u_AddrResolution;

extern X_Void DataLinkTransDisplay(u_AddrResolution local,u_AddrResolution src, 
                                    u_AddrResolution dest,u_AddrResolution next,uint8_t type,uint16_t length);
                                    
TEST(trans,display)
{
  u_AddrResolution  local,src,dest,next;     
	local.address = 0x00438000;
	src.address   = 0x00438100;
	dest.address  = 0x00438110;
	next.address  = 0x00438010;
    
	DataLinkTransDisplay(local,src,dest,next,0x21,15);

  local.address = 0x00438010;
	src.address   = 0x00438100;
	dest.address  = 0x00438110;
	next.address  = 0x00438110;
    
	DataLinkTransDisplay(local,src,dest,next,0x21,15);

/*
0          1          2          3          4          5          6          7          8          9          10         11         12         13         14
0x00438000 0x00438010 0x00438020 0x00438030 0x00438040 0x00438050 0x00438060 0x00438070 0x00438080 0x00438090 0x004380a0 0x004380b0 0x004380c0 0x004380d0 0x004380e0

0x00438100 0x00438110 0x00438120 0x00438130 0x00438140 0x00438150 0x00438160 0x00438170 0x00438180 0x00438190 0x004381a0 0x004381b0 0x004381c0 0x004381d0 0x004381e0
0x00438200 0x00438210 0x00438220 0x00438230 0x00438240 0x00438250 0x00438260 0x00438270 0x00438280 0x00438290 0x004382a0 0x004382b0 0x004382c0 0x004382d0 0x004382e0
0x00438300 0x00438310 0x00438320 0x00438330 0x00438340 0x00438350 0x00438360 0x00438370 0x00438380 0x00438390 0x004383a0 0x004383b0 0x004383c0 0x004383d0 0x004383e0
0x00438400 0x00438410 0x00438420 0x00438430 0x00438440 0x00438450 0x00438460 0x00438470 0x00438480 0x00438490 0x004384a0 0x004384b0 0x004384c0 0x004384d0 0x004384e0


0x00439000 0x00439010 0x00439020 0x00439030 0x00439040 0x00439050 0x00439060 0x00439070 0x00439080 0x00439090 0x004390a0 0x004390b0 0x004390c0 0x004390d0 0x004390e0

0x00439100 0x00439110 0x00439120 0x00439130 0x00439140 0x00439150 0x00439160 0x00439170 0x00439180 0x00439190 0x004391a0 0x004391b0 0x004391c0 0x004391d0 0x004391e0
0x00439200 0x00439210 0x00439220 0x00439230 0x00439240 0x00439250 0x00439260 0x00439270 0x00439280 0x00439290 0x004392a0 0x004392b0 0x004392c0 0x004392d0 0x004392e0
0x00439300 0x00439310 0x00439320 0x00439330 0x00439340 0x00439350 0x00439360 0x00439370 0x00439380 0x00439390 0x004393a0 0x004393b0 0x004393c0 0x004393d0 0x004393e0
0x00439400 0x00439410 0x00439420 0x00439430 0x00439440 0x00439450 0x00439460 0x00439470 0x00439480 0x00439490 0x004394a0 0x004394b0 0x004394c0 0x004394d0 0x004394e0

*/
  local.address = 0x00438010;
	src.address   = 0x00438110;
	dest.address  = 0x00438100;
	next.address  = 0x00438000;
    
	DataLinkTransDisplay(local,src,dest,next,0x21,15);

  local.address = 0x00438000;
	src.address   = 0x00438110;
	dest.address  = 0x00438100;
	next.address  = 0x00438100;
    
	DataLinkTransDisplay(local,src,dest,next,0x21,15);
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  	local.address = 0x00438020;
	src.address   = 0x00438110;
	dest.address  = 0x00438140;
	next.address  = 0x00438030;
    
	DataLinkTransDisplay(local,src,dest,next,0x21,15);

  	local.address = 0x00438020;
	src.address   = 0x00438240;
	dest.address  = 0x00438210;
	next.address  = 0x00438010;
    
	DataLinkTransDisplay(local,src,dest,next,0x21,15);

	local.address = 0x00438030;
	src.address   = 0x00438240;
	dest.address  = 0x00438210;
	next.address  = 0x00438020;
    
	DataLinkTransDisplay(local,src,dest,next,0x21,15);
}

/*
0          1          2          3          4          5          6          7          8          9          10         11         12         13         14
0x00438000 0x00438010 0x00438020 0x00438030 0x00438040 0x00438050 0x00438060 0x00438070 0x00438080 0x00438090 0x004380a0 0x004380b0 0x004380c0 0x004380d0 0x004380e0

0x00438100 0x00438110 0x00438120 0x00438130 0x00438140 0x00438150 0x00438160 0x00438170 0x00438180 0x00438190 0x004381a0 0x004381b0 0x004381c0 0x004381d0 0x004381e0
0x00438200 0x00438210 0x00438220 0x00438230 0x00438240 0x00438250 0x00438260 0x00438270 0x00438280 0x00438290 0x004382a0 0x004382b0 0x004382c0 0x004382d0 0x004382e0
0x00438300 0x00438310 0x00438320 0x00438330 0x00438340 0x00438350 0x00438360 0x00438370 0x00438380 0x00438390 0x004383a0 0x004383b0 0x004383c0 0x004383d0 0x004383e0
0x00438400 0x00438410 0x00438420 0x00438430 0x00438440 0x00438450 0x00438460 0x00438470 0x00438480 0x00438490 0x004384a0 0x004384b0 0x004384c0 0x004384d0 0x004384e0


0x00439000 0x00439010 0x00439020 0x00439030 0x00439040 0x00439050 0x00439060 0x00439070 0x00439080 0x00439090 0x004390a0 0x004390b0 0x004390c0 0x004390d0 0x004390e0

0x00439100 0x00439110 0x00439120 0x00439130 0x00439140 0x00439150 0x00439160 0x00439170 0x00439180 0x00439190 0x004391a0 0x004391b0 0x004391c0 0x004391d0 0x004391e0
0x00439200 0x00439210 0x00439220 0x00439230 0x00439240 0x00439250 0x00439260 0x00439270 0x00439280 0x00439290 0x004392a0 0x004392b0 0x004392c0 0x004392d0 0x004392e0
0x00439300 0x00439310 0x00439320 0x00439330 0x00439340 0x00439350 0x00439360 0x00439370 0x00439380 0x00439390 0x004393a0 0x004393b0 0x004393c0 0x004393d0 0x004393e0
0x00439400 0x00439410 0x00439420 0x00439430 0x00439440 0x00439450 0x00439460 0x00439470 0x00439480 0x00439490 0x004394a0 0x004394b0 0x004394c0 0x004394d0 0x004394e0

*/
