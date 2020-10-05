#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <set>

using namespace std;

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
vector<vector<int>> precompute_nCk(int n_options, int bet_size){
  vector<vector<int>> precomp_mat(n_options);
  for(int i = 0; i < n_options; i++){
    vector<int> precomp_option;
    for(int j = 0; j <= bet_size; j++){
      precomp_option.push_back(n_choose_k(i, j));
    }

    precomp_mat[i] = precomp_option;
  }

  return precomp_mat;
}

// Takes a bet and encodes it into an integer
long long encode_bet(set<short> bet, vector<vector<int>> nCk_precomp){
  int result = 0;
  int k = bet.size();
  for(auto it = bet.rbegin(); it != bet.rend(); it++){
    result += nCk_precomp[*it][k--];
  }

  return result;
}

// Takes an integer and decodes it into a bet
set<short> decode_bet(long long bet_id, int bet_size){
  int choice = bet_size - 1;
  while (n_choose_k(choice, bet_size) < bet_id) {
    choice++;
  }

  set<short> result;
  for (; choice >= 0; choice--) {
    long long c_choose_bs = n_choose_k(choice, bet_size);
    if (c_choose_bs <= bet_id) {
      bet_id -= c_choose_bs;
      bet_size--;
      result.insert(choice);
    }
  }

  return result;
}

// Takes all the bets and returns a bet that no one betted on
set<short> solve(vector<set<short>> all_bets, int n_options, int bet_size){
  int bet_options_count = n_choose_k(n_options, bet_size);
  vector<int> bet_options(bet_options_count);
  fill(bet_options.begin(), bet_options.end(), 0);

  vector<vector<int>> nCk_precomp = precompute_nCk(n_options, bet_size);

  for(set<short> bet : all_bets){
    long long encoded_bet = encode_bet(bet, nCk_precomp);
    bet_options[encoded_bet]++;
  }

  for(int i = 0; i < bet_options_count; i++){
    if(bet_options[i] == 0){
      return decode_bet(i, bet_size);
    }
  }

  return decode_bet(0, bet_size);
}

/*
  ---- INPUT -----
  ./lottery [n_options=60] [bet_size=6] < [bet_list].txt

  Where "n_options" is the maximum number each player
  has available to them when betting, "bet size" is how
  many numbers they can choose and "bet_list" is the
  filename of a file containing all the bets with each
  number separated by spaces and bets separated by line.

  > bet_list_example.txt
  0 1 2 3 4 5
  0 1 2 3 4 6
  0 1 2 3 4 7
  4 5 6 7 8 9
  2 3 4 5 6 7
  ...

  ---- OUTPUT -----
  A combination of numbers which has no bets on. If said
  combination does not exist, the lexographic first sequence
  possible is returned.

  ---- TIME COMPLEXITY ANALYSIS ----
  The time complexity of this algorithm is
  O(m * k^2 + n * k + (m! / (k! * (m - k)!)))

  Where
    'n' = Number of bets
    'k' = Size of each bet
    'm' = Options available to choose from in each bet
  
  O(m * k^2) comes from the matrix precomputing where we loop
  through each possible bet number and for each of them (O(m)),
  through each position in the bet it can be in (O(k)) and for
  each of these combinations, we calculate nCr with it in O(k).
  Nesting all this together we end up with O(m * k^2)

  O(n * k) is because when calling 'solve', we loop through
  each bet (O(n)), for each bet we need to encode it by looping
  through each number in the bet (O(k)) and each encoding is
  now O(1) with the pre computing of the nCr matrix.

  O(m! / (k! * (m - k)!)) is because after encoding each bet,
  we need to loop over all possible combinations of bets, which
  is simply m-choose-k, with is calculated with this formula.

  Since we execute all of these in sequence, we add them up.

  For the base example, values used were
    'n' = 10000000  (10 million bets)
    'k' = 6         (numbers in each bet)
    'm' = 60        (options available to choose from in each bet)
  
  We have that
  60 * 6^2 + 10000000 * 6 + (60! / (6! * (60 - 6)!)) = 110066020

  Assuming 10^8 operations takes approximatelly 1 second, this much
  operations should take approx 110066020 / 10^8 = 1.1 seconds without
  taking the reading of the file into account.
*/
int main(int argc, char *argv[]){
  int n_options = 60;
  int bet_size = 6;

  if(argc > 1) n_options = strtol(argv[1], &argv[1], 10);
  if(argc > 2) bet_size = strtol(argv[2], &argv[2], 10);

  short n;
  set<short> current_bet;
  vector<set<short>> all_bets;
  while(cin >> n){
    current_bet.insert(n);

    if(current_bet.size() == bet_size){
      set<short> complete_bet = current_bet;
      all_bets.push_back(complete_bet);
      current_bet.clear();
    }
  }

  cout << "Result: ";
  set<short> result = solve(all_bets, n_options, bet_size);
  for(auto it = result.begin(); it != result.end(); it++){
    cout << *it << " ";
  }
  cout << endl;

  return 0;
}