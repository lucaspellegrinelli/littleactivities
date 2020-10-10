#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <set>
#include <stdio.h>
#include <chrono>

using namespace std;
using namespace chrono; 

class SearchResult{
  public:
  long long encoded_bet;
  long long count;

  SearchResult(long long encoded_bet, long long count){
    this->encoded_bet = encoded_bet;
    this->count = count;
  }
};

// Calculates the amount of possible combinations
// possible witn n and k (nCr).
long long n_choose_k(long long n, long long k){
  if (k > n) return 0;
  if (k * 2 > n) k = n - k;
  if (k == 0) return 1;

  int result = n;
  for(int i = 2; i <= k; i++) {
    result *= (n - i + 1);
    result /= i;
  }
  
  return result;
}

// Pre computes the matrix with nCr where n is the matrix row and r
// is the matrix column
vector<vector<long long>> precompute_nCk(int n_options, int bet_size){
  vector<vector<long long>> precomp_mat;
  for(int i = 0; i <= n_options; i++){
    vector<long long> precomp_option;
    for(int j = 0; j <= bet_size; j++){
      precomp_option.push_back(n_choose_k(i, j));
    }

    precomp_mat.push_back(precomp_option);
  }

  return precomp_mat;
}

// Takes an integer and decodes it into a bet
set<short> decode_bet(long long bet_id, int bet_size, vector<vector<long long>> nCk_precomp){
  int choice = bet_size - 1;
  while (nCk_precomp[choice][bet_size] < bet_id) {
    choice++;
  }

  set<short> result;
  for (; choice >= 0; choice--) {
    long long c_choose_bs = nCk_precomp[choice][bet_size];
    if (c_choose_bs <= bet_id) {
      bet_id -= c_choose_bs;
      bet_size--;
      result.insert(choice);
    }
  }

  return result;
}

// Takes all the bets and returns a bet subcombination that
// has n_betters_min <= #bets <= n_betters_max
vector<SearchResult> solve(vector<set<short>> all_bets, int n_options, int bet_size, int target_bet_size, int n_betters_min, int n_betters_max){
  vector<vector<long long>> nCk_precomp = precompute_nCk(n_options, target_bet_size);

  int bet_options_count = nCk_precomp[n_options][target_bet_size];
  vector<int> bet_options(bet_options_count, 0);

  for(set<short> bet : all_bets){
    vector<bool> sub_bet_marker(bet_size);
    fill(sub_bet_marker.begin(), sub_bet_marker.begin() + target_bet_size, true);

    do{
      int k = target_bet_size;
      int encoded_sub_bet = 0;

      set<short>::reverse_iterator itr = bet.rbegin();
      for(int i = 0; itr != bet.rend(); itr++, i++){
        if(sub_bet_marker[i]){
          encoded_sub_bet += nCk_precomp[*itr][k--];
        }
      }

      bet_options[encoded_sub_bet]++;
    }while(prev_permutation(sub_bet_marker.begin(), sub_bet_marker.end()));
  }

  vector<SearchResult> all_solutions;
  for(int i = 0; i < bet_options_count; i++){
    if(bet_options[i] >= n_betters_min && bet_options[i] <= n_betters_max){
      all_solutions.push_back(SearchResult(i, bet_options[i]));
    }
  }

  return all_solutions;
}

/*
  ---- INPUT -----
  ./lottery [n_games=1000] [n_options=60] [bet_size=6]
            [target_bet_size=6] [n_betters_min=0] [n_betters_max=0] < [bet_list].txt

  Where
    "n_games" = Number of games in the text file
    "n_options" = Maximum number each player has available to them when betting
    "bet size" = How many numbers they can choose in each bet
    "target_bet_size" = The combination size to search for
    "n_betters_min" = Minimum number of betters that betted the target combination to be found
    "n_betters_max" = Maximum number of betters that betted the target combination to be found
    "bet_list" = Text file with the bets.

  > bet_list_example.txt
  0 1 2 3 4 5
  0 1 2 3 4 6
  0 1 2 3 4 7
  4 5 6 7 8 9
  2 3 4 5 6 7
  ...

  ---- OUTPUT -----
  A combination of numbers which has qtd of bets between the specified
  range (#bets >= n_betters_min and #bets <= n_betters_max).

  -----------------
  It takes around ~15 seconds to run (if compiled with the -O3 tag)
  ./lottery 10000000 60 6 4 2 500 < bet_list.txt

  In other words, it takes the 10M bets of size 6 and finds all
  combinations of 4 numbers which 2 <= x <= 500 betters betted on.

  This software displays only the first solution, but to change that
  just change the "main" method.
 */
int main(int argc, char *argv[]){
  int n_games = 100;
  int n_options = 60;
  int bet_size = 6;
  int target_bet_size = 6;
  int n_betters_min = 2;
  int n_betters_max = 50;

  if(argc > 1) n_games = strtol(argv[1], &argv[1], 10);
  if(argc > 2) n_options = strtol(argv[2], &argv[2], 10);
  if(argc > 3) bet_size = strtol(argv[3], &argv[3], 10);
  if(argc > 4) target_bet_size = strtol(argv[4], &argv[4], 10);
  if(argc > 5) n_betters_min = strtol(argv[5], &argv[5], 10);
  if(argc > 5) n_betters_max = strtol(argv[6], &argv[6], 10);

  auto start = high_resolution_clock::now(); 

  vector<set<short>> all_bets;
  for(int i = 0; i < n_games; i++){
    set<short> current_bet;
    for(int j = 0; j < bet_size; j++){
      short n;
      scanf("%hd", &n);
      current_bet.insert(n);
    }

    all_bets.push_back(current_bet);
  }

  auto stop = high_resolution_clock::now(); 
  auto duration = duration_cast<microseconds>(stop - start); 
  cout << "Time taken to read: " << (duration.count() / 1000000.0) << endl; 

  start = high_resolution_clock::now(); 
  vector<SearchResult> result = solve(all_bets, n_options, bet_size, target_bet_size, n_betters_min, n_betters_max);
  stop = high_resolution_clock::now(); 
  duration = duration_cast<microseconds>(stop - start); 
  cout << "Time taken to solve: " << (duration.count() / 1000000.0) << endl; 

  if(result.size() > 0){
    vector<vector<long long>> nCk_precomp = precompute_nCk(n_options, target_bet_size);

    cout << "Result: ";
    set<short> solution = decode_bet(result[0].encoded_bet, target_bet_size, nCk_precomp);
    for(auto it = solution.begin(); it != solution.end(); it++) cout << *it << " ";
    cout << "with " << result[0].count << " appearances" << endl;
  }else{
    cout << "No solutions found" << endl;
  }

  return 0;
}