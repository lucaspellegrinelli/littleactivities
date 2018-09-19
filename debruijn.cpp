#include <iostream>
#include <vector>

std::vector<int> de_bruijin(int k, int n){
  int a[k * n], l = 1;
  for(int i = 0; i < k * n; i++) a[i] = 0;
  std::vector<int> sequence;

  while(true){
    for(int i = 0; n % l == 0 && i < l; i++)
      sequence.push_back(a[i]);
    
    for(int i = l; i < n; i++)
      a[i] = a[i - l];
    
    for(l = n; l > 0 && a[l - 1] >= k - 1; l--);

    if(l == 0)
      return sequence;
    
    a[l - 1]++;
  }
}

int main() {
  for(int i : de_bruijin(2, 3)){
    std::cout << i << " ";
  }
}
