rm handwriting
make 

if [ $? -ne "0" ]; then
echo "make failed!!! please Check error"
exit
fi

./handwriting

cd py
sleep 35 && ps ux | grep -E 'python' | grep -v grep |awk '{print $2}' |xargs kill -s 9 & python handwriting.py
