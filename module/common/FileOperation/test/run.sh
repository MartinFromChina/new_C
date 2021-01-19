cd write_data

cd empty/
cp -f ../Backup/empty.txt .

cd ..
cd FirstLineEmpty
cp -f ../Backup/first_empty.txt .

cd ..
cd MiddleLineEmpty
cp -f ../Backup/middle_empty.txt .

cd ..
cd full/
cp -f ../Backup/full.txt .

cd ..
cd ..

make
if [ $? -ne "0" ]; then
echo "make failed!!! please Check error"
exit
fi
./fileOP
