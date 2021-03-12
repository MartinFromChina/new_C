make clean
make
if [ $? -ne "0" ]; then
echo "make failed!!! please Check error"
exit
fi

cd ..
cd py_display
cd wave_display

python finally.py
