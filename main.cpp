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
static int timetoRaiseDrawbridge;
static int timetoLowerDrawbridge;
static int nCars, nShips;
enum DrawBridgeStatus { SHIPSCANGO, CARSCANGO };

struct cData {
  char *Type;
  char *Name;
  int Delay;
  int TimeToCross;
};

pthread_t tid_car;
pthread_t tid_ship;
DrawBridgeStatus bridgeStatus = CARSCANGO;

void *Car(void *arglist) {
  cout << "Car " << arglist << " arrives at the bridge." << endl;
  pthread_mutex_lock(&bridge);
  while (bridgeStatus != CARSCANGO) {
    cout << "Car " << arglist << " goes across the bridge." << endl;
    pthread_cond_wait(&signal, &bridge);
  }
  cout << "Car " << arglist << " is leaving" << endl;
  pthread_mutex_unlock(&bridge);
  pthread_cond_signal(&signal);
  pthread_exit(NULL);
}

void *Ship(void *arglist) {
  cout << "Ship " << arglist << " arrives at the bridge." << endl;
  pthread_mutex_lock(&bridge);
  while (bridgeStatus != SHIPSCANGO) {
    cout << "Ship " << arglist << " goes under the raised bridge." << endl;
    pthread_cond_wait(&signal, &bridge);
  }
  cout << "Ship " << arglist << " is leaving" << endl;
  pthread_mutex_unlock(&bridge);
  pthread_cond_signal(&signal);
  pthread_exit(NULL);
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

  // cout << "PRINTING LINES" << endl;
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

    // cout << "---SPLIT ITEMS IN LINE---" << endl;
    const char *comp = result->at(0).c_str();

    if (strcasecmp(comp, "Bridge") == 0) {
      // insert some function that will recognize this as a mutex
      // cout << "----MUTEX DETECTED----" << endl;
    }
    if (strcasecmp(comp, "Car") == 0) {
      // insert some function that will recognize this as a thread and start
      // working
      // pthread create
      // cout << "####CAR THREAD DETECTED####_ca" << endl;
      pthread_t tid;
      struct cData *data;
      pthread_create(&tid_car, NULL, Car, (void *)&data);
    }
    if (strcasecmp(comp, "Ship") == 0) {
      // insert some function that will recognize this as a thread and start
      // working
      // pthread create
      // cout << "^^^^Ship THREAD DETECTED^^^^" << endl;
      pthread_t tid;
      struct cData data;
      pthread_create(&tid_ship, NULL, Ship, (void *)&data);
    }
  }

  pthread_join(tid_car, NULL);
  pthread_join(tid_ship, NULL);

  // from vector threads can be constructed
}