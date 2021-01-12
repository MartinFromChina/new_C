rm basic_data.txt
touch basic_data.txt
python rd_wr_excel.py

make
#if [ $? -ne "0" ]; then
#echo "make failed!!! please Check error"
#exit
#fi

sleep 10 && ps ux | grep -E 'python' | grep -v grep |awk '{print $2}' |xargs kill -s 9 & python draw.py

