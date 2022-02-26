rm wav_2_c
make 

if [ $? -ne "0" ]; then
echo "make failed!!! please Check error"
exit
fi

./wav_2_c
