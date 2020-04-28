#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
using namespace std;


int main(){

    int s1, s2, *a1, *a2;

    //Data for A1
    cout << "Enter A1 size: "; cin >> s1;
    a1 = new int[s1];
    cout << "Enter A1 Elements: "; for(int i=0; i<s1; i++){cin >> a1[i];}

    //Data for A2
    cout << "Enter A2 Size: "; cin >> s2;
    a2 = new int[s2];
    cout << "Enter A2 Elements: "; for(int i=0; i<s2; i++){cin >> a2[i];}

    auto it = max_element(a1, a1+s1);
    int cs = (*it) + 1;
    vector<int> counting(cs);
    counting.assign(cs,0);

    for(int i=0; i<s1; i++){
        counting[a1[i]]++;
    }

    //Print according to a2
    int i = 0;
    for(; i<s2; i++){
        for(int j=0; j<counting[a2[i]]; j++){
            cout << a2[i] << " ";
        }
        counting[a2[i]] = 0;
    }

    //Print the rest
    for(int i=0; i<counting.size(); i++){
        for(int j=0; j<counting[i]; j++){
            cout << i << " ";
        }
    }

    return 0;
}
