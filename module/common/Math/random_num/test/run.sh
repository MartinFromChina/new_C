echo "the first param is ": $1
if [ $# == 0 ]; then
echo "please enter param such as uniform ; normal ; all "
exit
fi

command=$1
rm random

case $command in
(uniform)
make -f uniform_makefile
;;
(normal)
make -f normal_makefile
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

case $command in
(uniform)
make -f uniform_makefile
./random
sleep 6 && ps ux | grep -E 'python' | grep -v grep |awk '{print $2}' |xargs kill -s 9 & python draw.py

;;
(normal)
make -f normal_makefile
./random
sleep 6 && ps ux | grep -E 'python' | grep -v grep |awk '{print $2}' |xargs kill -s 9 & python draw_normal.py

;;
(*)
echo "wrong param ; do nothing ; the end"
;;
esac
