#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>
#include <vector>
#include <functional>

using namespace std;

/* Takes a tree of size N, node x, and a vector dist of size N.
   Fills dist with numbers such that dist[i] contains distance between
   nodes x and i. 

   Time complexity: O(N).
*/
void calc_dists(int x, vector<vector<int>>& E, vector<int>& dist, int parent = -1, int cur_dist = 0) {
  dist[x] = cur_dist;
  for (int y: E[x]) {
    if (y != parent) {
      calc_dists(y, E, dist, x, cur_dist + 1);
    }
  }
}

/* Takes a tree of size N and returns two most distant nodes.

   Time complexity: O(N).
 */
pair<int, int> most_distant_pair(vector<vector<int>>& E) {
  vector<int> dist(E.size());
  calc_dists(0, E, dist);
  int first = max_element(dist.begin(), dist.end()) - dist.begin();
  calc_dists(first, E, dist);
  int second = max_element(dist.begin(), dist.end()) - dist.begin();
  return {first, second};
}

/* Takes a tree of size N, and an integer K. 
   Using suboptimal greedy algorithm finds a subset of K nodes, 
   such that sum of their pair-wise distances is maximized.
   Returns a pair (vector of chosen K nodes, sum of pair-wise distances).
   Assumes 2 <= K <= N.

   Time complexity: O(NK).
*/
pair<vector<int>, int64_t> greedy_solve(vector<vector<int>>& E, int K) {
  int N = E.size();
  assert(2 <= K && K <= N);

  // chosen contains currently chosen nodes
  vector<int> chosen;
  // is_chosen[i] is true iff node i is currently chosen  
  vector<bool> is_chosen(N, false);
  // dists[i] is sum of distances from node i to currently chosen nodes
  vector<int64_t> dists(N, 0);
  // total_dists is sum of pair-wise distances of chosen nodes
  int64_t total_dists = 0;

  auto choose = [&] (int x) { // make node x chosen
    assert(!is_chosen[x]);
    total_dists += dists[x];
    is_chosen[x] = true;
    chosen.push_back(x);
    static vector<int> dist_from_x;
    dist_from_x.resize(N);
    calc_dists(x, E, dist_from_x);
    for (int i = 0; i < N; ++i)
      dists[i] += dist_from_x[i];
  };

  auto first_two = most_distant_pair(E);
  choose(first_two.first);
  choose(first_two.second);

  while ((int)chosen.size() < K) {
    int best = -1;
    for (int i = 0; i < N; ++i)
      if (!is_chosen[i] && (best == -1 || dists[i] > dists[best])) best = i;
    choose(best);
  }

  return {chosen, total_dists};
}

/* Takes a tree of size N, and an integer K. 
   Using optimal DP algorithm finds a subset of K nodes, 
   such that sum of their pair-wise distances is maximized.
   Returns a pair (vector of chosen K nodes, sum of pair-wise distances).
   Assumes 2 <= K <= N.

   Time complexity: O(NK^2).
*/
pair<vector<int>, int64_t> dp_solve(vector<vector<int>>& E, int K) {
  int N = E.size();

  const int64_t inf = 1e18;
  
  // dp and reconstruction matrix
  vector<vector<vector<int64_t>>> f(N), r(N);
  
  // helper vectors
  vector<int64_t> nf(K);
  
  function<void (int, int)> calc_dp = [&] (int x, int parent) {
    int M = 0; // number of children
    for (int y: E[x])
      if (y != parent) {
        calc_dp(y, x);
        M++;
      }

    f[x] = vector<vector<int64_t>>(M + 1, vector<int64_t>(K + 1, -inf));
    r[x] = vector<vector<int64_t>>(M + 1, vector<int64_t>(K + 1, -1));

    f[x][0][0] = f[x][0][1] = 0;
    int idx = 0;
    for (int y: E[x]) {
      if (y != parent) {
        
        for (int i = 0; i <= K; ++i)
          for (int j = 0; i+j <= K; ++j) {
            int64_t nf = f[x][idx][i] + f[y].back()[j] + j*1LL*(K-j);
            if (nf > f[x][idx+1][i+j]) {
              f[x][idx+1][i+j] = nf;
              r[x][idx+1][i+j] = j;
            }
          }
        idx++;
      }
    }
  };

  calc_dp(0, -1);
  int64_t total_dists = f[0].back()[K];

  vector<int> chosen;
  function<void (int, int, int)> reconstruct = [&] (int x, int parent, int k) {
    int idx = (int)f[x].size() - 1;
    for (int i = (int)E[x].size() - 1; i >= 0; --i) {
      int y = E[x][i];
      if (y == parent) continue;

      assert(idx >= 0);
      int y_k = r[x][idx][k];
      reconstruct(y, x, y_k);
      k -= y_k;
      --idx;
    }

    assert(idx == 0);
    assert(0 <= k && k <= 1);
    if (k == 1) chosen.push_back(x);
  };

  reconstruct(0, -1, K);
  return {chosen, total_dists};
}

typedef decltype(greedy_solve) tree_solver;

pair<vector<int>, int64_t> forest_solver(vector<vector<int>>& E, int K, tree_solver solve_tree) {
  int N = E.size();
  vector<bool> bio(N, false);
  vector<int> toIdx(N);
  
  vector<int> chosen;
  int64_t total = 0;
  for (int i = 0; i < N; ++i)
    if (!bio[i]) {
      // new component
      vector<int> component;
       
      function<void (int)> dfs = [&] (int x) {
        if (bio[x]) return;
        bio[x] = true;
        component.push_back(x);
        for (int y: E[x])
          dfs(y);
      };

      dfs(i);
      
      int K_component = K * component.size() / N;
      for (int j = 0; j < (int)component.size(); ++j)
        toIdx[component[j]] = j;

      vector<vector<int>> E_component(component.size());
      int m = 0;
      for (int j = 0; j < (int)component.size(); ++j) {
        int x = component[j];
        for (int y: E[x]) {
          E_component[toIdx[x]].push_back(toIdx[y]);
          m++;
        }
      }

      assert(m == (2*(int)component.size() - 2));

      if (K_component > 1) {
        auto component_sol = solve_tree(E_component, K_component);
        total += component_sol.second;
        for (int x: component_sol.first)
          chosen.push_back(component[x]);
      }
    }
  
  return {chosen, total};
}

int main(void) {
  int N, M;
  int K = 500;
  scanf("%d %d", &N, &M);

  vector<vector<int>> E(N);
  for (int i = 0; i < M; ++i) {
    int a, b;
    scanf("%d %d", &a, &b);
    E[a].push_back(b);
    E[b].push_back(a);
  }


  
  double start = clock();
  auto greedy = forest_solver(E, K, greedy_solve);
  cout << "GREEDY: " << endl;
  cout << "\tscore: " << greedy.second << endl;
  cout << "\ttime: " << (clock()-start)/CLOCKS_PER_SEC << endl;

  cout << "Greedy chosen:" << endl;
  for (int x: greedy.first)
    cout << x << " ";
  cout << endl;
  
  start = clock();
  auto dp = forest_solver(E, K, dp_solve);
  cout << "DP: " << endl;
  cout << "\tscore: " << dp.second << endl;
  cout << "\ttime: " << (clock()-start)/CLOCKS_PER_SEC << endl;

  cout << "DP chosen:" << endl;
  for (int x: greedy.first)
    cout << x << " ";
  cout << endl;
  
  return 0;
}
