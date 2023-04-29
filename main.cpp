#include <fstream>
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

using namespace std;

static pthread_mutex_t bridge;
pthread_t tid;
pthread_cond_t signal = PTHREAD_COND_INITIALIZER;

static int timetoRaiseDrawbridge;
static int timetoLowerDrawbridge;
static int nCars;
static int nShips;
enum DrawbridgeStatus {NOCARS, CARSCANGO}; //no cars, true when there are cars, this is where we will have mutex un/lock

struct cData
{
    char* Type;
    char* Name;
    int Delay;
    int TimeToCross;
};

void Car(cData* arglist)
{
    /*cout<<"Car "<< name << " arrives at the bridge."<<endl;
    pthread_mutex_lock(&mutex);
    cout<<"Car "<< name << " goes on the raised bridge."<<endl;
    pthread_cond_signal(&signal);
    cout<<"Car "<< name << " is leaving"<<endl;
	pthread_mutex_unlock(&mutex);*/
}

void Ship(cData* arglist)
{
    /*cout<<"Ship "<< name << " arrives at the bridge."<<endl;
    pthread_mutex_lock(&mutex);
    cout<<"Ship "<< name << " goes under the raised bridge."<<endl;
    pthread_cond_signal(&signal);
    cout<<"Ship "<< name << " is leaving"<<endl;
	pthread_mutex_unlock(&mutex);*/
}

bool IsAlphaNum(string s) {
  bool result = false;
  for (int i = 0; i < s.length(); i++) {
    if (isalnum(s.at(i)))
      result = true;
  }
  return result;
}

char *ConvertToCharPointer(string &s) {
  int len = s.length();
  char *str = new char[len + 1];
  strcpy(str, s.c_str());
  return str;
}

int main() {
  pthread_mutex_init(&bridge, NULL);
  string line;

  cout << "PRINTING LINES" << endl;
#ifndef CIN
  fstream fin;
  fin.open("input30.txt");
  while (getline(fin, line))
#else
  while (getline(cin, line))
#endif
  {

    if (!IsAlphaNum(line))
      continue;

    vector<string> *result = new vector<string>();
    char *split = strtok(strdup(line.c_str()), " ");
    while (split != NULL) {
      result->push_back(split);
      split = strtok(NULL, " ");
    }

    cout << "---SPLIT ITEMS IN LINE---" << endl;

    for (int i = 0; i < result->size(); i++) {
      cout << result->at(i) << endl;
      const char *comp = result->at(i).c_str();

      if (strcasecmp(comp, "Bridge") == 0) {
        // insert some function that will recognize this as a mutex
        cout << "----MUTEX DETECTED----" << endl;
      }
      if (strcasecmp(comp, "Car") == 0) {
        // insert some function that will recognize this as a thread and start
        // working
        cout << "####CAR THREAD DETECTED####" << endl;
      }
      if (strcasecmp(comp, "Ship") == 0) {
        // insert some function that will recognize this as a thread and start
        // working
        cout << "^^^^Ship THREAD DETECTED^^^^" << endl;
      }
    }
  }
  // from vector threads can be constructed
}