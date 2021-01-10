#include <fstream>
#include <iostream>

using namespace std;

int main(){
	ofstream ofile;

	ofile.open("test.txt");
	ofile<<'\t'<<1<<'\t'<<2<<endl;
	ofile<<'\t'<<2<<'\t'<<4<<endl;
	ofile<<'\t'<<3<<'\t'<<6<<endl;
	ofile<<'\t'<<4<<'\t'<<8<<endl;
	ofile<<'\t'<<5<<'\t'<<10<<endl;
	ofile<<'\t'<<6<<'\t'<<12<<endl;
	ofile.close();
	cout<<"----test.txt operation finished from mian.cc \r\n";
}
