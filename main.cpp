#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <ctime>
#include <chrono>
#include <thread>
using namespace std;


void setsLectura();
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
//Solucion 2

void optimizedSearch(set<int> x, vector<set<int>> F){
    cout << "Comenzando solucion 2......" << endl;
    //Buscando los elementos que se encuentren en un solo conjunto
    if(F.size() > 1){
        set<int> inter;
        set<int> setComp;
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
        //Por si se quiere imprimir todos los elementos que estan en mas de un conjunto
        //imprimirSets(inter);
        setComp.clear();
        set_difference(x.begin(),x.end(),inter.begin(),inter.end(), inserter(setComp, setComp.begin()));
        int mscSize = 0;
        string solMscPrint = "";
        //setComp : Tiene los elementos que solo estan en un conjunto
        for(auto k = setComp.begin(); k!= setComp.end(); k++){
            size_t c = 0;
            //aca deberia buscar todos los sets que tengan el elemento que solo está en un solo conjunto
            while(c != F.size() && F[c].find(*k) == F[c].end()) c++;
            solMscPrint = solMscPrint + "S" + to_string(c+1) + " ";
            //imprimirSets(F[c]);
            if(c != F.size()){
                set<int>::iterator iter;
                set<int>::iterator del;
            for(iter=F[c].begin(); iter!=F[c].end(); iter++){
                x.erase(x.find(*iter));
                for(size_t j=0;j < F.size(); j++){
                    if(j != c)((del = find(F[j].begin(), F[j].end(), *iter)) == F[j].end()) ? del : F[j].erase(del);
                }
            }
            F.erase(F.begin() + c);
            }
            mscSize++;
        }
        //Si se cumple, se aplica la primera optimizacion
        for(size_t j = 1; j < F.size(); j++){
            vector<int> msc = comb(F.size(),j,x,F);
            if (!msc.empty()){
                cout << "MSC encontrado de tamaño: " << msc.size()+mscSize << endl;
                //En caso de que se quiera imprimir
                for(size_t i = 0; i != msc.size(); i++){
                    solMscPrint = solMscPrint + "S" + to_string(msc[i]+1) + " ";
                    ///imprimirSets(F[msc[i]]);
                }
                if(msc.size()+mscSize < 60){
                    cout << solMscPrint << endl;
                }
                //cout << endl;
                break;
            }
        }
        //

    }
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
        S.clear();
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
    */
    set<int> S1 = {1,2,3,4,5,6};
    //for(auto i=S1.begin();i!=S1.end();i++) cout << &i << endl;
    set<int> S2= {5,6,8,9};
    set<int> S3= {1,4,7,10};
    set<int> S4 = {2,3,5,7,8,11};
    set<int> S5 = {3,6,9,12};
    set<int> S6 = {10,11};
    set<int> A1 = {13,14,15,16,17,18};
    //for(auto i=S1.begin();i!=S1.end();i++) cout << &i << endl;
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

    int val = 1;
    int val_2 = 2;
    int val_3 = 3;
    int* ptr_val = &val;
    set<int*> T;
    T.insert(ptr_val);
    ptr_val = &val_2;
    T.insert(ptr_val);
    ptr_val = &val_3;
    T.insert(ptr_val);
    /*
    for(auto i=T.begin();i!=T.end();i++) cout << "Direccion: " << *i << " / Valor: " << **i << endl;
    cout << "---------------------\n";
    set<int*> R;
    R.insert(&val_2);
    R.insert(&val_3);
    for(auto i=R.begin();i!=R.end();i++) cout << "Direccion: " << *i << " / Valor: " << **i << endl;
    */
    //F[1].erase(F[1].find(3));
    /*set<int> intersect;
    set_intersection(S1.begin(),S1.end(),S2.begin(),S2.end(), inserter(intersect, intersect.begin()));
    set<int> unionLessIntersect;
    set<int> test = {1,2,3,4,8,9};
    set_symmetric_difference(test.begin(),test.end(),S3.begin(),S3.end(), inserter(unionLessIntersect, unionLessIntersect.begin()));
    */
    
    //auto pos = S6.find(10);
    //cout << *pos << endl;
    //exhaustiveSearch(x,F);

    set<int> x;
    x = getUniverse(F);
    //imprimirSets(x);
    auto start = std::chrono::high_resolution_clock::now();
    //optimizedSearch(x,F);
    
    OptimizedGreedAlgoritms(x,F);
    //std::this_thread::sleep_for(std::chrono::seconds (3));
    auto end = std::chrono::high_resolution_clock::now();
    auto int_s = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::chrono::duration<double, std::milli> float_ms = end - start;
    std::cout << "funcSleep() elapsed time is " << float_ms.count() << " milliseconds" << std::endl;
    setsLectura();
    /*it = s1.find(4);
    bool isIn = s1.find(3) != s1.end();
    cout << isIn << endl;
    it = s1.end();
    cout << *it;
    */
    //Iteracion sobre un set
    /*
    for (auto i = unionLessIntersect.begin(); i != unionLessIntersect.end(); i++)
    {
        cout << *i << " ";
    }
    */
}

void setsLectura(){
 string line ="";
 string sub;
  ifstream myfile ("lectura.txt");
  string space_delimiter = " ";
  vector <set<int>> F;
  set <int> A;
  size_t pos = 0;
  int c = 1;
  if (myfile.is_open())
  {
    while ( getline (myfile,line) && c<2 )
    {
      //cout << line << '\n';
        cout<<line << "\n";
        while ((pos = line.find(space_delimiter)) != string::npos ) {
        sub = line.substr(0, pos);
        cout << stoi(sub) ;
        //cout << stoi(sub);
        //A.insert(sub);
        //F.push_back(A);
        line.erase(0, pos + space_delimiter.length());
    }
    size_t pos = 0;
    
    }
    myfile.close();
    cout<<"en"<<endl;
    imprimirVector(F);
   
  }

  else cout << "Unable to open file"; 

}
