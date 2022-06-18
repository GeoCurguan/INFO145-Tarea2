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

int comb(int N, int K,set<int> x, vector<set<int>> F)
{
    string bitmask(K, 1); // K leading 1's
    bitmask.resize(N, 0); // N-K trailing 0's
    // print integers and permute bitmask
    int cant = 0;
    int mayor = x.size();
    do {
        set<int> s;
        for (int i = 0; i < N; ++i) // [0..N-1] integers
        {
            if (bitmask[i]){
                s.insert(F[i].begin(),F[i].end());
                //cout << " " << i;
                cant++;
                if(x == s)
                { 
                    cout << endl << "MSC encontrado "<<cant<<" " << endl;
                    if (cant < mayor){
                        cout << cant << " - "<< mayor <<endl;
                        mayor = cant;
                    }
                }
            }
        }
        cant=0;
        //cout << endl;
        //cout << "menor " << mayor <<endl;
        s.clear();
    } while (prev_permutation(bitmask.begin(), bitmask.end()));
return mayor;
}

void exhaustiveSearch(set<int> x, vector<set<int>> F){
    //Caso en que uno de los conjuntos sea igual al universo
    int menor = x.size();
    int cant = 0;
    int retorno;
    for(size_t j = 1; j < F.size(); j++){
       retorno = comb(F.size(),j,x,F);
       if (retorno < menor){
        menor = retorno;
       }
    }
    cout << menor << endl;
    //comb(4,2,x,F);

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
