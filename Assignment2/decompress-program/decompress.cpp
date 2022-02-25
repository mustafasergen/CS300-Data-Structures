//Mustafa Sergen Haysal
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

int readFromFile(vector<int> & input)
{
    string file = "compout.txt";
    ifstream i;
    i.open(file.c_str());
    int integer;
    int maxval = 0;
    while(i>>integer)
    {
        input.push_back(integer);
        if(integer>maxval)
            maxval = integer;
    }
    return maxval;
}

void initialize(vector<string> & v)
{
    string s = " ";
    for (int i = 0; i < 256; i++)
    {
        s[0] = char(i);
        v[i] = s;
    }
}

int main()
{
    ofstream o;
    string file = "decompout.txt";
    o.open(file.c_str());
    vector<int> input;
    int maxval = readFromFile(input);
    if(maxval<4096)
        maxval = 4096;
    vector<string> v(maxval);
    initialize(v);
    vector<bool> flag(input.size(),false);
    int q=-1,x=0,codetogive=256;
    string out = "";
    while(!flag[flag.size()-1])
    {
        if(input[x]<codetogive)
        {
            if(q!=-1)
            {
                v[codetogive] = v[input[q]] + v[input[x]].substr(0,1);
                codetogive++;
            }
        }
        else
        {
            v[codetogive] = v[input[q]] + v[input[q]].substr(0,1);
            codetogive++;
        }
        o << v[input[x]];
        out += v[input[x]];
        flag[x]=true;
        q=x;
        x++;  
    }
    o.close();
    return 0;
}