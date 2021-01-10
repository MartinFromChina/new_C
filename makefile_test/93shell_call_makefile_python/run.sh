make clean
make

if [ $? -ne "0" ]; then
echo "make failed!!! please Check error"
exit
fi

./target

sleep 10 && ps ux | grep -E 'python' | grep -v grep |awk '{print $2}' |xargs kill -s 9 & python hello.py



cat -n test.txt
