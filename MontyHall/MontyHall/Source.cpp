#include <random>
#include "windows.h"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <string>

using namespace std;

////////////////////////////////////////////////////////////////////////////////

const double runs = 10000000;

////////////////////////////////////////////////////////////////////////////////

enum
{
  GOAT = 0,
  CAR = 1
};

typedef mt19937 rng_type;
const uniform_real_distribution<> udist(0, 1);
rng_type rng;

vector<int> doors = {CAR, GOAT, GOAT};

int choice_contestant = -1;
int choice_host = -1;

double wins = 0;

////////////////////////////////////////////////////////////////////////////////

string GetFormattedRuns()
{
  string input = to_string(static_cast<int>(runs));
  string buff = "";
  int counter = 0;
  for (int i = input.length() - 1; i >= 0; --i)
  {
    buff = input[i] + buff;
    ++counter;
    if (counter == 3)
    {
      buff = "," + buff;
      counter = 0;
    }
  }
  return buff;
}

////////////////////////////////////////////////////////////////////////////////

inline void ShuffleDoors()
{
  const unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  shuffle(doors.begin(), doors.end(), default_random_engine(seed));
}

////////////////////////////////////////////////////////////////////////////////

inline void SelectDoor()
{
  choice_contestant = static_cast<int>(udist(rng) * 3);
}

////////////////////////////////////////////////////////////////////////////////

inline void OpenDoor()
{
  for(int i = 0; i < 3; ++i)
  {
    if ( (i != choice_contestant) && (doors[i] == GOAT)) 
    {
      choice_host = i;
      return;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void SwapChosenDoor()
{
  for (int i = 0; i < 3; ++i)
  {
    if ((i != choice_contestant) && (i != choice_host))
    {
      choice_contestant = i;
      return;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void RunSims(const bool doSwap)
{
  for (int i = 0; i < runs; ++i)
  {
    ShuffleDoors();
    SelectDoor();
    OpenDoor();
    if (doSwap)
    {
      SwapChosenDoor();
    }
    wins += doors[choice_contestant];
  }

  cout << "Runs : " << GetFormattedRuns() << endl;
  cout << "Wins : " << to_string(static_cast<int>(wins)) << endl;
  cout << "Percentage Wins : " << to_string((wins / runs) * 100) << endl;
  cout << endl;
  wins = 0;
}

////////////////////////////////////////////////////////////////////////////////

void main()
{  
  rng.seed(abs(static_cast<int>(GetTickCount())));
  udist(rng);

  cout << "NON-SWAP" << endl;
  RunSims(false);

  cout << "SWAP" << endl;
  RunSims(true);

  system("pause");
}

////////////////////////////////////////////////////////////////////////////////