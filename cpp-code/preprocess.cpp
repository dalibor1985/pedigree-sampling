#include <algorithm>
#include <vector>
#include <cassert>
#include <cstring>
#include <string>
#include <iostream>
#include <map>
#include <set>

using namespace std;

#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define REP(i, n) FOR(i, 0, n)
#define TRACE(x) cout << #x << " = " << x << endl
#define _ << " _ " <<

typedef long long llint;

vector<string> split(char* line, char delim = ' ') {
  int len = strlen(line);
  string cur = "";
  vector<string> v;
  for (int i = 0; i <= len; ++i) {
    if (line[i] == delim || i == len) {
      v.push_back(cur);
      cur = "";
    } else {
      cur.push_back(line[i]);
    }
  }
  return v;
}

vector<pair<string, string>> E;

map<string, string> dad;
map<string, int> id;

string findset(string x) {
  return dad[x] == x ? dad[x] : dad[x] = findset(dad[x]);
}

int main(void) {
  char buff[11111];
  scanf("%s", buff);

  while (scanf("%s", buff) == 1) {
    //scanf("%s", buff);
    auto v = split(buff, ',');

    string x = v[1];
    if (!dad.count(x)) {
      dad[x] = x;
      int sz = id.size();
      id[x] = sz;
    }
    
    for (string y: {v[3]}) {
      if (y.size()) {
        if (!dad.count(y)) {
          dad[y] = y;
          int sz = id.size();
          id[y] = sz;
        }

        if (findset(x) != findset(y)) {
          E.push_back({x, y});
          dad[findset(y)] = findset(x);
        } 
      }
    }
  }

  printf("%d %d\n", (int)id.size(), (int)E.size());
  for (auto& e: E)
    printf("%d %d\n", id[e.first], id[e.second]);
  return 0;
}
