rm pentip
make 

if [ $? -ne "0" ]; then
echo "make failed!!! please Check error"
exit
fi

./pentip
py= python draw.py
echo $py
#sleep 6 && ps ux | grep -E 'python' | grep -v grep |awk '{print $2}' |xargs kill -s 9 & py= python draw.py  

