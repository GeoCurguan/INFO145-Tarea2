#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <ctime>
#include <chrono>
#include <thread>
#include <fstream>
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
    int mscSize = F.size();
    vector<int> solMsc;
    string solMscPrint = "";
    for(size_t j = F.size()-1; j != 0; j--){
        vector<int> msc = comb(F.size(),j,x,F);
        if(!msc.empty() && msc.size() <= mscSize){
            solMsc = msc;
            solMscPrint = "";
            for(size_t i = 0; i != msc.size(); i++){
                solMscPrint = solMscPrint + "S" + to_string(msc[i]+1) + " ";
            }
            mscSize = msc.size();
        }
        msc.clear();
    }
    cout << "MSC encontrado de tamaño: " << mscSize << endl;
    //Print menor a 60 para no generar un flood en la consola
    if(mscSize < 60){
        cout << solMscPrint << endl;
    }
    cout << "Fin Solucion 1" << endl << "-------------------------------------------------" << endl;
}
//Solucion 2
void optimizedSearch(set<int> x, vector<set<int>> F){
    cout << "Comenzando solucion 2......" << endl;
    if(F.size() > 1){
        set<int> inter;
        set<int> setComp;
        //Buscando los elementos que se encuentren en un solo conjunto
        set_intersection(F[0].begin(),F[0].end(),F[1].begin(),F[1].end(), inserter(inter, inter.begin()));
        set_symmetric_difference(F[0].begin(),F[0].end(),F[1].begin(),F[1].end(), inserter(setComp, setComp.begin()));
        set<int> aux;
        for (size_t i = 2; i < F.size(); i++)
        {
            aux.clear();
            aux.insert(setComp.begin(),setComp.end());
            setComp.clear();
            set_intersection(aux.begin(),aux.end(),F[i].begin(),F[i].end(), inserter(inter, inter.begin()));
            set_symmetric_difference(aux.begin(),aux.end(),F[i].begin(),F[i].end(), inserter(setComp, setComp.begin()));
        }
        setComp.clear();
        set_difference(x.begin(),x.end(),inter.begin(),inter.end(), inserter(setComp, setComp.begin()));
        int mscSize = 0;
        string solMscPrint = "";
        //setComp : Tiene los elementos que solo estan en un conjunto
        for(auto k = setComp.begin(); k!= setComp.end(); k++){
            size_t c = 0;
            //aca deberia buscar todos los sets que tengan el elemento que solo está en un solo conjunto
            while(c != F.size() && F[c].find(*k) == F[c].end()) c++;
            //imprimirSets(F[c]);
            //cout << c << endl;
            //cout << solMscPrint << endl;
            //imprimirSets(F[c]);
            if(c != F.size()){
                solMscPrint = solMscPrint + "S" + to_string(c+1) + " ";
                //cout << c+1 << "/" << setSol << endl;
                set<int>::iterator iter;
                set<int>::iterator del;
                for(iter=F[c].begin(); iter!=F[c].end(); iter++){
                    x.erase(x.find(*iter));
                    for(size_t j=0;j < F.size(); j++){
                        if(j != c)((del = find(F[j].begin(), F[j].end(), *iter)) == F[j].end()) ? del : F[j].erase(del);
                    }
                }
                F[c].clear();
                mscSize++;
            }
        }
        for(size_t j = 1; j < F.size(); j++){
            vector<int> msc = comb(F.size(),j,x,F);
            //Si entra al if, es porque encontro un MSC
            if (!msc.empty()){
                cout << "MSC encontrado de tamaño: " << msc.size()+mscSize << endl;
                //En caso de que se quiera imprimir
                for(size_t i = 0; i != msc.size(); i++){
                    solMscPrint = solMscPrint + "S" + to_string(msc[i]+1) + " ";
                    imprimirSets(F[msc[i]]);
                }
                //Print menor a 60 para no generar un flood en la consola
                if(msc.size()+mscSize < 60){
                    cout << solMscPrint << endl;
                }
                //Como encontro un msc, no tiene sentido seguir buscando...
                break;
            }
        }
    }
}

//Solucion 3
void greedAlgoritms(set<int> x, vector<set<int>> F){
    cout << "Comenzando solucion 3......" << endl;
    set<int> U = x;
    vector<set<int>> C;
    set<int>::iterator ite;
    set<int> S;
    int mayor = 0;
    int countDiferencias = 0;
    //int dif = 0;
    size_t i = 0;
    while( U.size() > 0){
        for(size_t j = 0; j < F.size(); j++){
            countDiferencias = 0;
            for (int r : F.at(j)) {
                if (U.count(r)){
                   countDiferencias++;
                }
            }
            if (countDiferencias > mayor){
                S.clear();
                S.insert(F[j].begin(),F[j].end());
                mayor = countDiferencias;
                //i = j;
                //dif = countDiferencias;
                //cout<< countDiferencias;
            }
           
        }
        //cout<<"S seleccionado, con N-difereneicas: " << dif <<endl;
        //imprimirSets(S);
        /*        
        auto elem = F.begin() + i;
        if (elem != F.end()){
            F.erase(elem);
                }
        */

        i = 0;
        mayor= 0;

        for (ite = S.begin(); ite != S.end(); ite++) {
            U.erase(*ite);
       
        }
        
        C.push_back(S);
        
    }
    imprimirVector(C);
    cout << "Fin Solucion 3" << endl << "-------------------------------------------------" << endl;
}
//Solución 4
void OptimizedGreedAlgoritms(set<int> x, vector<set<int>> F){
    cout << "Comenzando solucion 4......" << endl;

    //Iterador
    int k= 0;
    set<int>::iterator ite;
    //Algortimo presentado en clases
    set<int> U = x;
    vector<set<int>> C;
    set<int> S;
    int count = 0;
    while( U.size() > 0 && k < 1){

        for(size_t j = 0; j < F.size(); j++){
            for (ite = F.at(j).begin(); ite != F.at(j).end(); ite++) {
                cout << *ite << " ";
                //if ()
            }
            cout <<endl;
                }

        set<int>::iterator ite;
        for (ite = S.begin(); ite != S.end(); ite++) {
            U.erase(*ite);
        }
        C.push_back(S);
        S.clear();
        k++;
    }
    imprimirVector(C);

    cout << "Fin Solucion 4" << endl << "-------------------------------------------------" << endl;
}


vector <set<int>> setsLectura(){
    string line ="";
    string sub;
    ifstream myfile ("lectura.txt");
    vector <set<int>> F;
    if (myfile.is_open()){
        while ( getline (myfile,line)){
            int i = 0;
            string text = "";
            set <int> A;
            while(i < line.length()){
                text = "";
                while(i < line.length() && (int)line[i] != 32 && (int)line[i] != 13){
                    text = text + line[i];
                    i++;
                }
                //cout << "-------\n";
                if(text.length() != 0){
                    A.insert(stoi(text));
                }
                i++;
            }
            F.push_back(A);
            A.clear();
        }
        myfile.close();
    }
    return F;
}

int main(){
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
    //set<int> x;
    //x = getUniverse(F);
    //exhaustiveSearch(x,F);
    // Set de ejemplos

    set<int> S1 = {1,2,3,4,5,6};
    set<int> S2= {5,6,8,9};
    set<int> S3= {1,4,7,10};
    set<int> S4 = {2,3,5,7,8,11};
    set<int> S5 = {3,6,9,12};
    set<int> S6 = {10,11};
    set<int> A1 = {13,14,15,16,17,18};
    set<int> A2= {13,14,15,16,17,18,19,20};
    set<int> A3= {21,22};
    set<int> A4 = {22,23,24};
    set<int> A5 = {19,20,24};
    set<int>::iterator it;
    vector<set<int>> F;
    F.push_back(S1);
    F.push_back(S2);
    F.push_back(S3);
    F.push_back(S4);
    F.push_back(S5);
    F.push_back(S6);
    F.push_back(A1);
    F.push_back(A2);
    F.push_back(A3);
    F.push_back(A4);
    */
    //auto pos = S6.find(10);
    //cout << *pos << endl;
    /*vector <set<int>> F = setsLectura();
    set<int> x;
    x = getUniverse(F);
    cout << "Universo :";
    imprimirSets(x);
    //imprimirSets(x);
    auto start = std::chrono::high_resolution_clock::now();
    //exhaustiveSearch(x,F);
    optimizedSearch(x,F);
    //greedAlgoritms(x,F);
    //OptimizedGreedAlgoritms(x,F);
    //std::this_thread::sleep_for(std::chrono::seconds (3));
    auto end = std::chrono::high_resolution_clock::now();
    auto int_s = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::chrono::duration<double, std::milli> float_ms = end - start;
    std::cout << "funcSleep() elapsed time is " << float_ms.count() << " milliseconds" << std::endl;

*/
     srand(time(NULL));
    int num=1+rand()%(100);
    int cantidadSets = 1 + rand()%(100/2);


    cout << num << endl;

    

    
    /*for (size_t i = 1; i < vec.size(); i++) {
        cout << vec.at(i) << "; ";
    }*/

    vector <set<int>> efx;
    for (int i = 0; i <= cantidadSets; i++){
        set<int> rands = {};
        int tamanoSets = 1 + rand()%(100/2);
        for(int j = 0; j <= tamanoSets; j++){
                int numra = 1 + rand()%(num);
                rands.insert(numra);
        }
        efx.push_back(rands);
        rands.clear();
        
    }


    set<int> y;
    y = getUniverse(efx);

    imprimirSets(y);

    cout << "------------------------------------------------ " << endl;

    imprimirVector(efx);

    cout << "------------------------------------------------ " << endl;


    //exhaustiveSearch(y, efx);
    optimizedSearch(y, efx);
}
