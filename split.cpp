#include <iostream>
#include <vector>
#include <cstdio>
#include <string>
#include <cstring>
#include <sstream>

using namespace std;

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
        std::stringstream ss(s);
        std::string item;
        while(std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        return split(s, delim, elems);
}


int main(){

    string str = "hi/there how u doin?";
    vector<string> s = split(str,' '); 

    for(int i=0;i<s.size();i++)
        cout<<s[i]<<endl;
    return 0;
}
