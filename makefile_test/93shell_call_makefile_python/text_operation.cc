#include <fstream>
#include <iostream>

using namespace std;

int main(){
	ofstream ofile;

	ofile.open("test.txt");
	ofile<<1<<2<<3<<4<<5<<endl;
	ofile.close();
	cout<<"_________:\r\n----test.txt operation finished from mian.cc \r\n";
}
