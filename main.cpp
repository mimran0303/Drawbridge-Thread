#include <fstream>
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <vector>

using namespace std;

static pthread_mutex_t bridge;
pthread_cond_t signal = PTHREAD_COND_INITIALIZER;
// vector of threads?
static int timetoRaiseDrawbridge;
static int timetoLowerDrawbridge;
static int nCars, nShips;
enum DrawbridgeStatus {
  NOCARS,
  CARSCANGO
}; // no cars, true when there are cars, this is where we will have mutex
   // un/lock

struct cData {
  char *Type;
  char *Name;
  int Delay;
  int TimeToCross;
};

void *Car(void *arglist) {
  int i = 0;
  while (i++ < 20) {
    cout << "car waiting for " << i << " seconds" << endl;
    sleep(1);
  }
  /*cout<<"Car "<< name << " arrives at the bridge."<<endl;
  pthread_mutex_lock(&mutex);
  while()
  {
  cout<<"Car "<< name << " goes on the raised bridge."<<endl;
  //pthread wait
  }
  pthread_cond_signal(&signal);
  cout<<"Car "<< name << " is leaving"<<endl;
      pthread_mutex_unlock(&mutex);*/
}

void *Ship(void *arglist) {
  int i = 0;
  while (i++ < 20) {
    cout << "ship waiting for " << i << " seconds" << endl;
    sleep(1);
  }
  /*cout<<"Ship "<< name << " arrives at the bridge."<<endl;
  pthread_mutex_lock(&mutex);
  while()
  {
  cout<<"Ship "<< name << " goes under the raised bridge."<<endl;
  //pthread wait
  }
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
    const char *comp = result->at(0).c_str();

    if (strcasecmp(comp, "Bridge") == 0) {
      // insert some function that will recognize this as a mutex
      cout << "----MUTEX DETECTED----" << endl;
    }
    if (strcasecmp(comp, "Car") == 0) {
      // insert some function that will recognize this as a thread and start
      // working
      // pthread create
      cout << "####CAR THREAD DETECTED####" << endl;
      pthread_t tid;
      struct cData *data;
      pthread_create(&tid, NULL, Car, (void *)&data);
    }
    if (strcasecmp(comp, "Ship") == 0) {
      // insert some function that will recognize this as a thread and start
      // working
      // pthread create
      cout << "^^^^Ship THREAD DETECTED^^^^" << endl;
      pthread_t tid;
      struct cData data;
      pthread_create(&tid, NULL, Ship, (void *)&data);
    }
  }
  // from vector threads can be constructed
}