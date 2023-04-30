#include <fstream>
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <vector>

using namespace std;

const int MAXTHREADS = 2048;
int nThreads = 0;
pthread_t ThreadIDs[MAXTHREADS]; // array of thread IDs

static pthread_mutex_t bridge;
pthread_cond_t signal = PTHREAD_COND_INITIALIZER;
static int timetoRaiseDrawbridge;
static int timetoLowerDrawbridge;
static int nCars, nShips;
enum DrawBridgeStatus { SHIPSCANGO, CARSCANGO };

struct cData {
  string Type;
  string Name;
  int Delay;
  int TimeToCross;
};

DrawBridgeStatus bridgeStatus = CARSCANGO;

void *Car(void *arglist) {
  cData *vData = (struct cData *)arglist;
  cout << "Car " << vData->Name << " arrives at the bridge and waiting."
       << endl;
  while (bridgeStatus != CARSCANGO) {
    cout << "Car " << vData->Name << " waiting to cross the bridge." << endl;
    pthread_cond_wait(&signal, &bridge);
  }
   pthread_mutex_lock(&bridge);
  cout << "Car " << vData->Name << " is crossing the bridge" << endl;
  sleep(5);
  cout << "Car " << vData->Name << " has finished crossing the bridge" << endl;
  pthread_cond_signal(&signal);
  pthread_mutex_unlock(&bridge);
  pthread_exit(NULL);
}

void *Ship(void *arglist) {
  cData *vData = (struct cData *)arglist;
  cout << "Ship " << vData->Name << " arrives at the bridge and waiting." << endl;
  pthread_mutex_lock(&bridge);
  cout << "Ship " << vData->Name << " is raising the bridge" << endl;
  sleep(1);
  bridgeStatus = SHIPSCANGO;
  cout << "Ship " << vData->Name << " goes under the bridge" << endl;
  sleep(10);
  cout << "Ship " << vData->Name << " has finished crossing the bridge" << endl;
  sleep(1);
  cout << "Bridge " << vData->Name << " is lowered and cars can go" << endl;
  bridgeStatus = CARSCANGO;
  pthread_cond_signal(&signal);
  pthread_mutex_unlock(&bridge);
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
    const char *comp = result->at(0).c_str();

    if (strcasecmp(comp, "Bridge") == 0) {
      // TODO: populate variables for bridge, name, time to lower, time to raise
    }
    if (strcasecmp(comp, "Car") == 0) {
      pthread_t tid;
      struct cData *data = new cData;
      ;
      data->Name = result->at(1);
      pthread_create(&tid, NULL, Car, (void *)data);
      ThreadIDs[nThreads] = tid;
      nThreads++;
    }
    if (strcasecmp(comp, "Ship") == 0) {
      pthread_t tid;
      struct cData *data = new cData;
      data->Name = result->at(1);
      pthread_create(&tid, NULL, Ship, (void *)data);
      ThreadIDs[nThreads] = tid;
      nThreads++;
    }
  }

  for (int i = 0; i < nThreads; i++) {
    pthread_join(ThreadIDs[i], NULL);
  }
}