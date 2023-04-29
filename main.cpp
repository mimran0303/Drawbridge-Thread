#include <iostream>
#include <string>
#include <pthread.h>
#include <vector>
#include <fstream>
#include <string.h>
#include <stdio.h>

using namespace std;

static pthread_mutex_t bridge;

bool IsAlphaNum(string s)
{
	bool result = false;
	for(int i=0; i< s.length(); i++)
	{
		if(isalnum(s.at(i)))
			result = true;
	}
	return result;
}

char* ConvertToCharPointer(string& s)
{
	int len = s.length();
	char* str = new char[len + 1];
	strcpy(str, s.c_str());
	return str;
}


int main()
{
    pthread_mutex_init(&bridge, NULL); 
    string line;
    
    cout<<"PRINTING LINES"<<endl;
    while (getline(cin, line))
	{
        
        if(!IsAlphaNum(line))
            continue;
        
        vector<string>* result = new vector<string>();
        char* split = strtok(strdup(line.c_str()), " ");
        while (split != NULL)
        {
            result->push_back(split);
            split = strtok(NULL, " ");
        }
        
        cout<<"---SPLIT ITEMS IN LINE---"<<endl;
        
        for(int i=0; i<result->size();i++)
        {
            cout<<result->at(i)<<endl;
            const char* comp = result->at(i).c_str();
            
            if(strcasecmp(comp,"Bridge")==0)
            {
                //insert some function that will recognize this as a mutex
                cout<<"----MUTEX DETECTED----"<<endl;
            }
            if(strcasecmp(comp,"Car")==0)
            {
                //insert some function that will recognize this as a thread and start working
                cout<<"####CAR THREAD DETECTED####"<<endl;
            }
             if(strcasecmp(comp,"Ship")==0)
            {
                //insert some function that will recognize this as a thread and start working
                cout<<"^^^^Ship THREAD DETECTED^^^^"<<endl;
            }
        }
    }
    //from vector threads can be constructed
    
}