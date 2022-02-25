//Mustafa Sergen Haysal
#include <iostream>
#include <fstream>
#include "Hashtable.h"
#include <exception>
using namespace std;

vector<Pair<string> >  produceLetters()
{
    vector<Pair<string> > v(256);
    string s = " ";
    for (int i = 0; i < 256; i++)
    {
        s[0]= char(i);
        Pair<string> p(s, i);
        v[i]=p;
    }
    return v;
}

vector<string> readFile()
{
    string filename = "compin.txt";
    ifstream i;
    i.open(filename.c_str());
    vector<string> v;
    char c;
    string s = " ";
    while (i.get(c))
    {
        s[0] = c;
        v.push_back(s);
    }
    return v;
}

string findLongestPrefix(vector<string> input, int pos, const HashTable<string>  &ht)
{
    string prefix = "";
    for (int i = pos; i < input.size(); i++)
    {
        prefix += input[i];
        if (ht.find(prefix) == -1)
        {
            
            return prefix.substr(0, prefix.length() - 1);
        }
    }
    return prefix;
}

int main()
{
    string filename = "compout.txt";
    ofstream o;
    o.open(filename.c_str());
    vector<Pair<string> > letters = produceLetters();
    vector<string> input = readFile();
    HashTable<string> ht;
    ht.initialize(letters);
    vector<bool> flag(input.size(), false);
    string q;
    int codetogive = 256;
    int pos = 0;
    int count = 0;
    while (!flag[flag.size() - 1]) //while there are letters that are not processed yet
    {
        string prefix = findLongestPrefix(input, pos, ht); //find longest prefix
        if (prefix.length() + pos == input.size())         //if prefix happens to be the last remaining part of input, just print its code
        {
            o << ht.find(prefix);
            for (int i = 0; i < prefix.length(); i++)
            {
                flag[pos + i] = true; //mark these letters as completed
            }
        }
        else //if prefix is not the whole remaining part
        {
            q = input[prefix.length() + pos];       //determine what q is
            Pair<string> p(prefix + q, codetogive); //give pq a code
            ht.insert(p);                           //insert pq to the table
            codetogive++;                           //increase code by 1
            o << ht.find(prefix) << " ";            //print prefix
            for (int i = 0; i < prefix.length(); i++)
            {
                flag[pos + i] = true; //mark these letters as completed
            }
        }
        pos += prefix.length(); //increase pos by prefix length
    }
    o.close();
    return 0;
}