#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
using namespace std;

//funcion que imprime sets
void imprimirSets(set<int> x){
    set<int>::iterator ite;
    for (ite = x.begin(); ite != x.end(); ite++) {
        cout << *ite << " ";
    }
    cout << endl;
}
void imprimirVector(vector<set<int>> F){
    set<int> element;
    for (size_t i = 0; i < F.size(); i++){
        element.insert(F.at(i).begin(),F.at(i).end());
        imprimirSets(element);
        element.clear();
    }
}

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
    cout << "Comenzando solucion 1......" << endl;
    for(size_t j = 1; j < F.size(); j++){
        vector<int> msc = comb(F.size(),j,x,F);
        if (!msc.empty()){
            cout << "MSC encontrado de tamaño: " << msc.size() << endl;
            //En caso de que se quiera imprimir
            cout << "Los conjunto que lo forman son: " << endl;
            for(size_t i = 0; i != msc.size(); i++){
                cout << "S" << msc[i]+1 << "  ";
                imprimirSets(F[i]);
            }
            cout << endl;
            break;
        }
    }
    cout << "Fin Solucion 1" << endl << "-------------------------------------------------" << endl;
}

//Solucion 3
void greedAlgoritms(set<int> x, vector<set<int>> F){
    cout << "Comenzando solucion 3......" << endl;
    set<int> U = x;
    vector<set<int>> C;
    set<int> S;
    int mayor = 0;
    while( U.size() > 0){
        for(size_t j = 0; j < F.size(); j++){
            if (F.at(j).size() > mayor){
                S.clear();
                S.insert(F[j].begin(),F[j].end());
                mayor = F.at(j).size();
                    //F.erase()
                auto elem = F.begin() + j;
                if (elem != F.end()){
                    F.erase(elem);
                }
            }
        }
        mayor= 0;
        set<int>::iterator ite;
        for (ite = S.begin(); ite != S.end(); ite++) {
            U.erase(*ite);
        }
        C.push_back(S);
    }
    imprimirVector(C);
    cout << "Fin Solucion 3" << endl << "-------------------------------------------------" << endl;
}

int main(){
    // Set de ejemplos
    /*
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
*/
        // Set de ejemplos
    set<int> S1 = {1,2,3,4,5,6};
    set<int> S2= {5,6,8,9};
    set<int> S3= {1,4,7,10};
    set<int> S4 = {2,5,7,8,11};
    set<int> S5 = {3,6,9,12};
    set<int> S6 = {10,11};
    set<int>::iterator it;
    vector<set<int>> F;
    F.push_back(S1);
    F.push_back(S2);
    F.push_back(S3);
    F.push_back(S4);
    F.push_back(S5);
    F.push_back(S6);

    set<int> x;
    x = getUniverse(F);
    exhaustiveSearch(x,F);
    //imprimirVector(F);
    greedAlgoritms(x,F);
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
