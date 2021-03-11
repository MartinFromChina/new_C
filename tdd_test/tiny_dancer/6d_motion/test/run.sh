rm ../c/finally.so
#make 

cd ..
cd c
gcc -shared -Wl,-soname,finally -o finally.so -fPIC finally.c

if [ $? -ne "0" ]; then
echo "make failed!!! please Check error"
exit
fi

cd ..
cd py_display
cd wave_display

python finally.py
