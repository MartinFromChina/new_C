make clean
make
if [ $? -ne "0" ]; then
echo "make failed!!! please Check error"
exit
fi

./RGB_Display 

#cd ..
#cd py_display
#cd wave_display

#python 3d_display.py
#python finally.py