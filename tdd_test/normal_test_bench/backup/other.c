uint8_t test_buf[] = {
0xd ,0xa ,0x2b ,0x49 ,0x50 ,0x44 ,0x2c ,0x31 ,0x32 ,0x39 ,0x38 ,0x3a ,0x48 ,0x54 ,0x54 ,0x50 ,0x2f ,0x31 ,0x2e ,0x31 ,0x20 ,0x32 ,0x30 ,0x36 ,0x20 ,0x50 ,0x61 ,0x72 ,0x74 ,0x69 ,0x61 ,0x6c ,0x20 ,0x43 ,0x6f ,0x6e ,0x74 ,0x65 ,0x6e ,0x74 ,
0xd ,0xa ,0x53 ,0x65 ,0x72 ,0x76 ,0x65 ,0x72 ,0x3a ,0x20 ,0x6e ,0x67 ,0x69 ,0x6e ,0x78 ,
0xd ,0xa ,0x44 ,0x61 ,0x74 ,0x65 ,0x3a ,0x20 ,0x53 ,0x75 ,0x6e ,0x2c ,0x20 ,0x31 ,0x37 ,0x20 ,0x4a ,0x75 ,0x6c ,0x20 ,0x32 ,0x30 ,0x32 ,0x32 ,0x20 ,0x30 ,0x32 ,0x3a ,0x31 ,0x32 ,0x3a ,0x34 ,0x31 ,0x20 ,0x47 ,0x4d ,0x54 ,
0xd ,0xa ,0x43 ,0x6f ,0x6e ,0x74 ,0x65 ,0x6e ,0x74 ,0x2d ,0x54 ,0x79 ,0x70 ,0x65 ,0x3a ,0x20 ,0x61 ,0x70 ,0x70 ,0x6c ,0x69 ,0x63 ,0x61 ,0x74 ,0x69 ,0x6f ,0x6e ,0x2f ,0x6f ,0x63 ,0x74 ,0x65 ,0x74 ,0x2d ,0x73 ,0x74 ,0x72 ,0x65 ,0x61 ,0x6d ,
0xd ,0xa ,0x43 ,0x6f ,0x6e ,0x74 ,0x65 ,0x6e ,0x74 ,0x2d ,0x4c ,0x65 ,0x6e ,0x67 ,0x74 ,0x68 ,0x3a ,0x20 ,0x31 ,0x30 ,0x32 ,0x34 ,
0xd ,0xa ,0x4c ,0x61 ,0x73 ,0x74 ,0x2d ,0x4d ,0x6f ,0x64 ,0x69 ,0x66 ,0x69 ,0x65 ,0x64 ,0x3a ,0x20 ,0x57 ,0x65 ,0x64 ,0x2c ,0x20 ,0x32 ,0x39 ,0x20 ,0x4a ,0x75 ,0x6e ,0x20 ,0x32 ,0x30 ,0x32 ,0x32 ,0x20 ,0x30 ,0x35 ,0x3a ,0x34 ,0x32 ,0x3a ,0x31 ,0x39 ,0x20 ,0x47 ,0x4d ,0x54 ,
0xd ,0xa ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x1 ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xfe ,0x80 ,0x0 ,0x80 ,0x1 ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x1 ,0x7f ,0xff ,0x80 ,0x1 ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x1 ,
0xd ,0xa ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x1 ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xfd ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x1 ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xfe ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x1 ,0x80 ,0x1 ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xfe ,0x80 ,0x0 ,0x80 ,0x2 ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xfe ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x1 ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xfe ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x1 ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xfe ,0x80 ,0x1 ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xfe ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x1 ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xfe ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xfe ,0x80 ,0x0 ,0x7f ,0xfe ,0x7f ,0xff ,0x7f ,0xfe ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x1 ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x1 ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x1 ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x1 ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x2 ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x1 ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xfe ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x1 ,0x7f ,0xff ,0x7f ,0xfe ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xfe ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x1 ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xfe ,0x80 ,0x1 ,0x80 ,0x0 ,0x80 ,0x1 ,0x7f ,0xff ,0x7f ,0xfe ,0x7f ,0xff ,0x80 ,0x1 ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xfe ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xfe ,0x80 ,0x0 ,0x80 ,0x1 ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xfe ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xfe ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x2 ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x1 ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xfd ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x1 ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xfe ,0x7f ,0xfe ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x1 ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xfe ,0x80 ,0x1 ,0x80 ,0x1 ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xfe ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x1 ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x1 ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x2 ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x1 ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xfe ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xfe ,0x80 ,0x1 ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xfe ,0x80 ,0x1 ,0x7f ,0xfe ,0x80 ,0x1 ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x1 ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xfe ,0x80 ,0x0 ,0x80 ,0x1 ,0x7f ,0xff ,0x80 ,0x1 ,0x80 ,0x0 ,0x80 ,0x1 ,0x80 ,0x2 ,0x80 ,0x0 ,0x7f ,0xfe ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x1 ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xfe ,0x80 ,0x1 ,0x7f ,0xfe ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xfe ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x1 ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x1 ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x1 ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x80 ,0x1 ,0x80 ,0x0 ,0x80 ,0x1 ,0x80 ,0x2 ,0x80 ,0x1 ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x1 ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x0 ,0x7f ,0xfe ,0x80 ,0x0 ,0x7f ,0xff ,0x80 ,0x0 ,0x7f ,0xff ,0x7f ,0xff ,0x7f ,0xff ,0x80 ,0x0 ,0x80 ,0x1 ,0x80 ,0x0 ,
0xd ,0xa ,0x2b ,0x49 ,0x50 ,0x44 ,0x2c ,0x32 ,0x39 ,0x35 ,0x3a ,0x48 ,0x54 ,0x54 ,0x50 ,0x2f ,0x31 ,0x2e ,0x31 ,0x20 ,0x34 ,0x30 ,0x30 ,0x20 ,0x42 ,0x61 ,0x64 ,0x20 ,0x52 ,0x65 ,0x71 ,0x75 ,0x65 ,0x73 ,0x74 ,
0xd ,0xa ,0x53 ,0x65 ,0x72 ,0x76 ,0x65 ,0x72 ,0x3a ,0x20 ,0x6e ,0x67 ,0x69 ,0x6e ,0x78 ,
0xd ,0xa ,0x44 ,0x61 ,0x74 ,0x65 ,0x3a ,0x20 ,0x53 ,0x75 ,0x6e ,0x2c ,0x20 ,0x31 ,0x37 ,0x20 ,0x4a ,0x75 ,0x6c ,0x20 ,0x32 ,0x30 ,0x32 ,0x32 ,0x20 ,0x30 ,0x32 ,0x3a ,0x31 ,0x32 ,0x3a ,0x34 ,0x39 ,0x20 ,0x47 ,0x4d ,0x54 ,
0xd ,0xa ,0x43 ,0x6f ,0x6e ,0x74 ,0x65 ,0x6e ,0x74 ,0x2d ,0x54 ,0x79 ,0x70 ,0x65 ,0x3a ,0x20 ,0x74 ,0x65 ,0x78 ,0x74 ,0x2f ,0x68 ,0x74 ,0x6d ,0x6c ,
0xd ,0xa ,0x43 ,0x6f ,0x6e ,0x74 ,0x65 ,0x6e ,0x74 ,0x2d ,0x4c ,0x65 ,0x6e ,0x67 ,0x74 ,0x68 ,0x3a ,0x20 ,0x31 ,0x35 ,0x30 ,
0xd ,0xa ,0x43 ,0x6f ,0x6e ,0x6e ,0x65 ,0x63 ,0x74 ,0x69 ,0x6f ,0x6e ,0x3a ,0x20 ,0x63 ,0x6c ,0x6f ,0x73 ,0x65 ,
0xd ,0xa ,0xd ,0xa ,0x3c ,0x68 ,0x74 ,0x6d ,0x6c ,0x3e ,
0xd ,0xa ,0x3c ,0x68 ,0x65 ,0x61 ,0x64 ,0x3e ,0x3c ,0x74 ,0x69 ,0x74 ,0x6c ,0x65 ,0x3e ,0x34 ,0x30 ,0x30 ,0x20 ,0x42 ,0x61 ,0x64 ,0x20 ,0x52 ,0x65 ,0x71 ,0x75 ,0x65 ,0x73 ,0x74 ,0x3c ,0x2f ,0x74 ,0x69 ,0x74 ,0x6c ,0x65 ,0x3e ,0x3c ,0x2f ,0x68 ,0x65 ,0x61 ,0x64 ,0x3e ,
0xd ,0xa ,0x3c ,0x62 ,0x6f ,0x64 ,0x79 ,0x3e ,
0xd ,0xa ,0x3c ,0x63 ,0x65 ,0x6e ,0x74 ,0x65 ,0x72 ,0x3e ,0x3c ,0x68 ,0x31 ,0x3e ,0x34 ,0x30 ,0x30 ,0x20 ,0x42 ,0x61 ,0x64 ,0x20 ,0x52 ,0x65 ,0x71 ,0x75 ,0x65 ,0x73 ,0x74 ,0x3c ,0x2f ,0x68 ,0x31 ,0x3e ,0x3c ,0x2f ,0x63 ,0x65 ,0x6e ,0x74 ,0x65 ,0x72 ,0x3e ,
0xd ,0xa ,0x3c ,0x68 ,0x72 ,0x3e ,0x3c ,0x63 ,0x65 ,0x6e ,0x74 ,0x65 ,0x72 ,0x3e ,0x6e ,0x67 ,0x69 ,0x6e ,0x78 ,0x3c ,0x2f ,0x63 ,0x65 ,0x6e ,0x74 ,0x65 ,0x72 ,0x3e ,
0xd ,0xa ,0x3c ,0x2f ,0x62 ,0x6f ,0x64 ,0x79 ,0x3e ,
0xd ,0xa ,0x3c ,0x2f ,0x68 ,0x74 ,0x6d ,0x6c ,0x3e ,
0xd ,0xa ,
0xd ,0xa ,0x43 ,0x4c ,0x4f ,0x53 ,0x45 ,0x44 ,
0xd ,0xa

};

#define DOWNLOAD_BLOCK_SIZE_IN_BYTES            

TEST(convert,http_head)
{
  uint16_t i,cur_index;
  uint8_t dl_buf[DOWNLOAD_BLOCK_SIZE_IN_BYTES + 1];

  cur_index = 0;
  byteBufInit(dl_buf,DOWNLOAD_BLOCK_SIZE_IN_BYTES+1,'\0');
  for(i=0;i<(sizeof(test_buf)/sizeof(uint8_t));i++)
  {
    dl_buf[cur_index ++] = test_buf[i];

  }
  printf(" %s ",(char*)dl_buf);
}