#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(){

/*
    char str[] = "hi";
    string s(str);
    cout<<s<<endl;

    */

 //test if file is created or not   
    ofstream fp;
    fp.open("my_text.txt");
//    fp<<"hi"<<endl;


    return 0;
}
