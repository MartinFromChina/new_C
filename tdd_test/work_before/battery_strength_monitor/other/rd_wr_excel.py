#coding=UTF-8
import xlrd
import numpy 
import math
df_list = []
print("going to read excel data and write it to a txt document\r\n")
def read_excel():
    workbook = xlrd.open_workbook(r'../basic_data/电量监测.xlsx')
    print workbook.sheet_names() # [u'sheet1', u'sheet2'  .....]
    sheet2_name= workbook.sheet_names()[1]
    print sheet2_name
    sheet2 = workbook.sheet_by_index(1)
    cols = sheet2.col_values(3) 
    print cols
    #data = sheet2.cell(2,3).value
    #print data
    #cols = str(cols).replace('u\'','')  
    #for i in cols:
    #    df_list.append(i)
    row_num = 2
    while row_num <= 215:
        df_list.append(sheet2.cell(row_num,3).value)
        row_num =row_num + 1
    #print df_list
    #for i in range(sheet2.nrows):
     #   line = sheet2.cell_value(i,3)
      #  df_list.append(line)
     # i = str(i) 
       
    
if __name__ == '__main__':#? 
    read_excel()
    filename = 'basic_data.txt'
    #txt = open(filename, 'w')
    Y = numpy.array(df_list)
    Y1 = numpy.reshape(Y,(-1,1))
    print Y1
    numpy.savetxt(filename,Y1,fmt='%.01f')
    #for a in Y1:
     #   txt.write(str(Y1)+"\r\n");