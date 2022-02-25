#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;


struct Path
{
    vector<string> words;
    int length;
    Path()
    {
        length = 0;
    }
};

bool findInVector(vector<string> vec, string s)
{
    for (int i = 0; i < vec.size(); i++)
    {
        if(vec[i]==s)
            return true;
    }
    return false;
}

bool IsNeighbor(string s, string s1)
{
    if(s==s1)
        return true;
    if(s.length()>s1.length()+1 || s1.length()>s.length()+1)
        return false;
    if(s.length()==s1.length())
    {
        int count = 0;
        for(int i=0;i<s.size();i++)
        {
            if(s[i]==s1[i])
                count++;
        }
        return(count==s.length()-1);
    }
    if(s.length()<s1.length())
    {
        for(int i=0;i<s1.length();i++)
        {
            string news1 = s1.substr(0,i) + s1.substr(i+1,s1.length());
            if(s==news1)
                return true;
        }
        return false;
    }
    return IsNeighbor(s1,s);
}

vector<string> FindNeighbors(vector<string> data, string starting)
{
    vector<string> words;
    for (int i = 0; i < data.size(); i++)
    {
        if(data[i]!=starting)
            if(IsNeighbor(data[i],starting))
                words.push_back(data[i]);
    }
    return words;
}

Path findClosestPath(string starting, string finishing,vector<string> data, vector<string> tabu, Path existing)
{
    if(starting==finishing)
        return existing;
    vector<string> words = FindNeighbors(data,starting);
    vector<string> lookupList;
    for(int i=0; i < words.size(); i++)
    {
        bool istabu = false;
        for (int j = 0; j < tabu.size(); j++)
        {
            if(words[i]==tabu[j])
                istabu=true;
        }
        if(!istabu)
            lookupList.push_back(words[i]);
    }
    if(lookupList.size()==0)
    {
        existing.words.push_back(starting);
        existing.length = data.size();
        return existing;
    }
    tabu.push_back(starting);
    int minLength = data.size()*2;
    int minIndex = 0;
    vector<Path> subsolutions;
    Path returnSlt;
    returnSlt.length = data.size();
    for (int i = 0; i < lookupList.size(); i++)
    {
        Path solution = existing;
        solution.length+=1;
        solution.words.push_back(lookupList[i]);
        Path found =findClosestPath(lookupList[i],finishing,data,tabu,solution);
        subsolutions.push_back(found);
        if(subsolutions[i].length<minLength)
        {
            minLength=subsolutions[i].length;
            minIndex = i;
        }
    }
    if(subsolutions.size()==0)
        return returnSlt;
    return subsolutions[minIndex];
}

vector<string> getData(string f)
{
    string s;
    ifstream i;
    i.open(f.c_str());
    vector<string> v;
    while(i>>s)
    {
        v.push_back(s);
    }
    return v;
}

void printConversion(string s, string s1)
{
    if(s.length()==s1.length())
    {
        cout << "change ";
        int count = 0, pos=0;
        for(int i=0;i<s.size();i++)
        {
            if(s[i]==s1[i])
                count++;
            else
                pos = i;
        }
        cout << s[pos] << " at position " << pos << " to " << s1[pos];
    }
    else if(s.length()>s1.length()){
        cout << "delete " ;
        int pos = 0;
        for(int i=0;i<s.length();i++)
        {
            string news = s.substr(0,i) +  s.substr(i+1,s.length());
            if(news==s)
                pos = i;
        }
        cout << s[pos] << " at position " << pos ;
    }
    else
    {
        cout << "insert " ;
        int pos = 0;
        for(int i=0;i<s1.length();i++)
        {
            string news1 = s1.substr(0,i) +  s1.substr(i+1,s1.length());
            if(s==news1)
                pos = i;
        }
        cout << s1[pos] << " after position " << pos ;
    }
    
}

void printPath(Path path)
{
    cout << path.words[0] << endl;
    for(int i=1;i<path.words.size();i++)
    {
        cout << path.words[i] << " (";
        printConversion(path.words[i-1], path.words[i]);
        cout << ")" << endl;
    }
}

int main()
{
    string f = "words.txt";
    vector<string> data = getData(f);
    string query1=".",query2;
    while(query1[0]!='*')
    {
        cout << "Enter words to perform search: ";
        cin >> query1 >> query2;
        if(query1[0]!='*')
        {
            if(findInVector(data,query1) && findInVector(data,query2))
            {
                vector<string> tabu;
                tabu.push_back(query1);
                Path existing;
                existing.length=1;
                existing.words.push_back(query1);
                Path path = findClosestPath(query1,query2,data,tabu,existing);
                if(path.length>=data.size())
                {
                    cout << query1 << " could not be converted to " << query2 << endl;
                }
                else{
                    cout << "Here is the list of transitions that converts " << query1 << " to " << query2 << endl;
                    printPath(path);
                }
            }
            else
            {
                cout << "Input word/words do not exist in the database!!" << endl;
            }
            
            
        }
    }
    cout << "end of the program!" << endl;
}