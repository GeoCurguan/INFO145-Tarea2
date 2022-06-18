#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
using namespace std;

//Funcion que retorna el universo de elementos
set<int> getUniverse(vector<set<int>> F){
    set<int> x(F[0]);
    for(size_t i = 1; i<F.size(); i++){
        x.insert(F[i].begin(),F[i].end());
    }
    return x;
}

//Solucion 1

vector<int> comb(int N, int K,set<int> x, vector<set<int>> F)
{
    string bitmask(K, 1);
    bitmask.resize(N, 0);
    vector<int> A;
    do {
        set<int> s;
        for (int i = 0; i < N; ++i)
        {
            if (bitmask[i]){
                s.insert(F[i].begin(),F[i].end());
                A.push_back(i);
                if(x == s) return A;
            }
        }
        s.clear();
        A.clear();
    } while (prev_permutation(bitmask.begin(), bitmask.end()));
    return A;
}

void exhaustiveSearch(set<int> x, vector<set<int>> F){
    for(size_t j = 1; j < F.size(); j++){
        vector<int> msc = comb(F.size(),j,x,F);
        if (!msc.empty()){
            cout << "MSC encontrado de tamaño: " << msc.size() << endl;
            cout << "Los conjunto que lo forman son: " << endl;
            for(size_t i = 0; i != msc.size(); i++)
                cout << "S" << msc[i] << "  ";
            cout << endl;
            break;
        }
    }
}

int main(){
    // Set de ejemplos
    set<int> A = {1,2,3};
    set<int> B = {2,3,4,5};
    set<int> C = {4,5,6};
    set<int> D = {3,5,7};
    set<int> E = {7,8};
    set<int>::iterator it;
    vector<set<int>> F;
    F.push_back(A);
    F.push_back(B);
    F.push_back(C);
    F.push_back(D);
    F.push_back(E);
    set<int> x;
    x = getUniverse(F);
    exhaustiveSearch(x,F);
    /*it = s1.find(4);
    bool isIn = s1.find(3) != s1.end();
    cout << isIn << endl;
    it = s1.end();
    cout << *it;
    */
    //Iteracion sobre un set


    /*
    for (auto i = x.begin(); i != x.end(); i++)
    {
        cout << *i << " ";
    }
    */
}
