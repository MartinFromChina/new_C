make clean
make
if [ $? -ne "0" ]; then
echo "make failed!!! please Check error"
exit
fi
./mStack_out