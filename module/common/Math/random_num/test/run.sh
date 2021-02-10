echo "the first param is ": $1
if [ $# == 0 ]; then
echo "please enter param such as uniform ; normal"
exit
fi

command=$1
rm random

case $command in
(uniform)
make -f uniform_makefile
;;
(normal)
make -f nothing
;;
(*)
echo "wrong param "
make -f nothing
;;
esac

if [ $? -ne "0" ]; then
echo "make failed!!! please Check error"
exit
fi
./random
sleep 6 && ps ux | grep -E 'python' | grep -v grep |awk '{print $2}' |xargs kill -s 9 & python draw.py
