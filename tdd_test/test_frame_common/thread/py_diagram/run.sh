cd ..
make
if [ $? -ne "0" ]; then
echo "make failed!!! please Check error"
exit
fi
./thread
cd py_diagram
sleep 6 && ps ux | grep -E 'python' | grep -v grep |awk '{print $2}' |xargs kill -s 9 & python draw.py
