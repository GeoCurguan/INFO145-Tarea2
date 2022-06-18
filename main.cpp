#include <iostream>
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
void exhaustiveSearch(set<int> x, vector<set<int>> F){
    //Caso en que uno de los conjuntos sea igual al universo
    for (size_t i = 0; i < F.size(); i++){
        if(x == F[i]){
            cout << "MSC de tamaño 1 encontrado, correspondiente a S" << i+1 << endl;
            return;
        }
    }

}

int main(){
    // Set de ejemplos
    set<int> s1 = {1,3,2,4,5};
    set<int> s2 = {2,4};
    set<int> s3 = {3,4};
    set<int> s4 = {4,5};
    set<int>::iterator it;
    vector<set<int>> F;
    F.push_back(s1);
    F.push_back(s2);
    F.push_back(s3);
    F.push_back(s4);
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