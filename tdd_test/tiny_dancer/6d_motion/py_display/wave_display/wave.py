


def WaveDispaly(p_link,jlink_read):
    sixD_data = []
    isNew = jlink_read(p_link,sixD_data)
    if(isNew == True):
         print(sixD_data[0])
         print(sixD_data[1])
         print(sixD_data[2])
         print(sixD_data[3])
         print(sixD_data[4])
         print(sixD_data[5])
    return True
