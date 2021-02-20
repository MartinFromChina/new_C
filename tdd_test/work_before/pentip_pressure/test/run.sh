echo "the first param is ": $1
if [ $# == 0 ]; then
echo "please enter param such as 1 ; 2 ; 3 ..."
exit
fi

rm pentip
make 

if [ $? -ne "0" ]; then
echo "make failed!!! please Check error"
exit
fi

./pentip

delay=$1

if [ $delay == 0 ]; then
echo " wrong param ; num must bigger than 0"
exit
fi

py= python draw.py $delay
echo $py




#sleep 6 && ps ux | grep -E 'python' | grep -v grep |awk '{print $2}' |xargs kill -s 9 & py= python draw.py  

