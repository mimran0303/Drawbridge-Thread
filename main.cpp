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
pthread_t ThreadIDs[MAXTHREADS];

static pthread_mutex_t bridge;
pthread_cond_t signal = PTHREAD_COND_INITIALIZER;

static int timetoRaiseDrawbridge;
static int timetoLowerDrawbridge;
static int nCars, nShips;
enum DrawBridgeStatus { SHIPSCANGO, CARSCANGO };

struct VehicleData {
  string Type;
  string Name;
  int Delay;
  int TimeToCross;
};

DrawBridgeStatus bridgeStatus = CARSCANGO;

bool DEBUG = false;

bool IsAlphaNum(string s);

static void *Car(void *vehicledata) {

  VehicleData *vData = (struct VehicleData *)vehicledata;
  cout << "Car " << vData->Name << " arrives at the bridge." << endl;

  pthread_mutex_lock(&bridge);
  {
    while (bridgeStatus != CARSCANGO) {
      if (DEBUG)
        cout << "## DEBUG: Car " << vData->Name << " waiting " << endl;
      pthread_cond_wait(&signal, &bridge);
    }

    cout << "Car " << vData->Name << " goes on the bridge." << endl;
    sleep(vData->TimeToCross);
    cout << "Car " << vData->Name << " leaves the bridge." << endl;

    nCars++;
  }
  pthread_cond_signal(&signal);
  pthread_mutex_unlock(&bridge);

  pthread_exit(NULL);
}

static void *Ship(void *vehicledata) {

  VehicleData *vData = (struct VehicleData *)vehicledata;
  cout << "Ship " << vData->Name << " arrives at the bridge." << endl;

  bridgeStatus = SHIPSCANGO;
  cout << "Bridge is closed to car traffic" << endl;

  pthread_mutex_lock(&bridge);
  {
    cout << "Bridge can now be raised." << endl;
    sleep(timetoRaiseDrawbridge);

    cout << "Ship " << vData->Name << " goes under the bridge." << endl;
    sleep(vData->TimeToCross);
    cout << "Ship " << vData->Name << " is leaving." << endl;

    sleep(timetoLowerDrawbridge);
    nShips++;
  }
  pthread_cond_signal(&signal);
  pthread_mutex_unlock(&bridge);

  bridgeStatus = CARSCANGO;
  cout << "Bridge can now accommodate car traffic." << endl;

  pthread_exit(NULL);
}

int main() {
  pthread_mutex_init(&bridge, NULL);
  string line;

  while (getline(cin, line)) {
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
      timetoRaiseDrawbridge = stoi(result->at(1));
      timetoLowerDrawbridge = stoi(result->at(2));
    }
    if (strcasecmp(comp, "Car") == 0) {
      pthread_t tid;
      struct VehicleData *data = new VehicleData;
      data->Name = result->at(1);
      data->Delay = stoi(result->at(2));
      data->TimeToCross = stoi(result->at(3));
      sleep(data->Delay);
      pthread_create(&tid, NULL, Car, (void *)data);
      ThreadIDs[nThreads++] = tid;
    }
    if (strcasecmp(comp, "Ship") == 0) {
      pthread_t tid;
      struct VehicleData *data = new VehicleData;
      data->Name = result->at(1);
      data->Delay = stoi(result->at(2));
      data->TimeToCross = stoi(result->at(3));
      sleep(data->Delay);
      pthread_create(&tid, NULL, Ship, (void *)data);
      ThreadIDs[nThreads++] = tid;
    }
  }

  for (int i = 0; i < nThreads; i++) {
    pthread_join(ThreadIDs[i], NULL);
  }
  cout << nCars << " car(s) crossed the bridge." << endl;
  cout << nShips << " ship(s) went under the raised bridge." << endl;
}

bool IsAlphaNum(string s) {
  bool result = false;
  for (int i = 0; i < s.length(); i++) {
    if (isalnum(s.at(i)))
      result = true;
  }
  return result;
}
