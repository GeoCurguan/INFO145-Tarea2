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

void imprimirVectorInt(vector<int> F){
    for (size_t i = 0; i < F.size(); i++){
        cout << F.at(i);
    }
    cout<<endl;
}

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
    set<int>::iterator ite;
    if (F.size() < 10){
    for (size_t i = 0; i < F.size(); i++){
        element.insert(F.at(i).begin(),F.at(i).end());
        imprimirSets(element);
        element.clear();
        }
    }
    cout<< "Tamaño: "<<F.size()<<endl ;
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
    cout << "Fin Solucion 1" << endl;
}

set<int> getIntersection(set<int> x, vector<set<int>> F){
    set<int> inter;
    set<int> setComp;
    //Buscando los elementos que se encuentren en un solo conjunto
    set_intersection(F[0].begin(),F[0].end(),F[1].begin(),F[1].end(), inserter(inter, inter.begin()));
    set_symmetric_difference(F[0].begin(),F[0].end(),F[1].begin(),F[1].end(), inserter(setComp, setComp.begin()));
    set<int> aux;
    for (size_t i = 2; i < F.size(); i++){
        aux.clear();
        aux.insert(setComp.begin(),setComp.end());
        setComp.clear();
        set_intersection(aux.begin(),aux.end(),F[i].begin(),F[i].end(), inserter(inter, inter.begin()));
        set_symmetric_difference(aux.begin(),aux.end(),F[i].begin(),F[i].end(), inserter(setComp, setComp.begin()));
    }
    setComp.clear();
    set_difference(x.begin(),x.end(),inter.begin(),inter.end(), inserter(setComp, setComp.begin()));
    return setComp;
}

//Solucion 2
void optimizedSearch(set<int> x, vector<set<int>> F){
    cout << "Comenzando solucion 2......" << endl;
    if(F.size() > 1){
        set<int> setComp = getIntersection(x,F);
        int mscSize = 0;
        string solMscPrint = "";
        //setComp : Tiene los elementos que solo estan en un conjunto
        for(auto k = setComp.begin(); k!= setComp.end(); k++){
            size_t c = 0;
            //aca deberia buscar todos los sets que tengan el elemento que solo está en un solo conjunto
            while(c != F.size() && F[c].find(*k) == F[c].end()) c++;
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
                //Print menor a 60 para no generar un flood en la consola
                if(msc.size() + mscSize < 60){
                    for(size_t i = 0; i != msc.size(); i++){
                        solMscPrint = solMscPrint + "S" + to_string(msc[i]+1) + " ";
                        //imprimirSets(F[msc[i]]);
                    }
                    cout << solMscPrint << endl;
                }
                break;
            }
        }
    }
    cout << "Fin Solucion 2" << endl;
}

//Solucion 3 -> algoritmo implementado del pseudocodigo
void greedAlgoritms(set<int> x, vector<set<int>> F){
    cout << "Comenzando solucion 3......" << endl;
    set<int> U = x;
    vector<set<int>> C;
    set<int>::iterator ite;
    set<int> S;
    int mayor = 0;
    int countDiferencias = 0;
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
            }
        }
        i = 0;
        mayor= 0;
        for (ite = S.begin(); ite != S.end(); ite++) {
            U.erase(*ite);
        }
        C.push_back(S);
    }
    imprimirVector(C);
    cout << "Fin Solucion 3" << endl;
}

//Solución 4 -> Mejora n°1

/*
void OptimizedGreedAlgoritms(set<int> x, vector<set<int>> F){
    cout << "Comenzando solucion 4......" << endl;
    set<int> U = x;
    vector<set<int>> C;
    set<int>::iterator ite;
    set<int> S;
    // Buscar elementos que solo esten en un conjunto
    if(F.size() > 1){
        set<int> setComp = getIntersection(x,F);
        //Agregar los conjuntos que tengan un elemento
        while(setComp.size() > 0){
            for(size_t j = 0; j < F.size(); j++){
                for (int k : setComp){
                    if (F.at(j).count(k)){
                        if ( C.size() >0 && *find(C.begin(),C.end(), F.at(j)) == F.at(j)){
                            setComp.erase(k);
                        }else{
                            S.insert(F[j].begin(),F[j].end());
                            for (ite = S.begin(); ite != S.end(); ite++) {
                                U.erase(*ite);
                            }
                            C.push_back(S);
                            S.clear();
                            setComp.erase(k);
                            }
                        break;

                    }
                }
            }
        }
    }
    int mayor = 0;
    int countDiferencias = 0;
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
            }
        }
        mayor= 0;
        for (ite = S.begin(); ite != S.end(); ite++) {
            U.erase(*ite);
        }
        C.push_back(S);
    }
    imprimirVector(C);

    cout << "Fin Solucion 4" << endl;
}
*/

set <int> unionDeConjuntos(vector<set<int>> F){
    set <int> Union;
    set <int> AC;
    set <int> AC2;
    AC.insert(F[0].begin(),F[0].end());

    for (size_t i = 1;i < F.size(); i++){
        set_union(AC.begin(),AC.end(),F[i].begin(),F[i].end(), inserter(Union, Union.begin()));
    }
    return Union;
}

//Combinación de k-conjuntos, y que ademas escoge la mejor combinación de estos k-conjuntos
vector<set<int>> comb2(int N, int K,set<int> x, vector<set<int>> F)
{
    string bitmask(K, 1);
    bitmask.resize(N, 0);
    vector<int> A;
    vector <set<int>> Im;
    vector <set<int>> MejorUnion;
    int comparador = 0;
    set<int> c;
    set<int> Union;
    do {
        set<int> s;
        for (int i = 0; i < N; ++i)
        {
            if (bitmask[i]){
                s.insert(F[i].begin(),F[i].end());
                A.push_back(i);
                }
        }
        for (size_t i = 0; i < A.size(); i++){
            c.insert(F.at(A[i]).begin(),F.at(A[i]).end());
            Im.push_back(c);
            c.clear();
        }
        Union = unionDeConjuntos(Im);
        if (comparador < Union.size()){
            MejorUnion = Im;
            comparador = Union.size();
        }
        Union.clear();
        s.clear();
        A.clear();
        Im.clear();
    } while (prev_permutation(bitmask.begin(), bitmask.end()));
    return MejorUnion;
}


// Función que realiza la mejora (1) y (2) del gready algorithms
void OptimizedGreedAlgoritmsV3(set<int> x, vector<set<int>> F, int k){
    cout << "Comenzando solucion 4......" << endl;
    set<int> U = x;
    vector<set<int>> C;
    set<int>::iterator ite;
    set<int> S;
    int mayor = 0;
    int countDiferencias = 0;
    size_t i=0;

    if (k > 1){
        cout<< "k = "<<k  << endl;
        vector <set<int>> ELEM;
        ELEM = comb2(F.size(),k,x,F);
        set <int> conjuntosUnidos = unionDeConjuntos(ELEM);
        for (ite = conjuntosUnidos.begin(); ite != conjuntosUnidos.end(); ite++) {
            U.erase(*ite);
        }
        C = ELEM;
    }
    if(F.size() > 1){
        set<int> setComp = getIntersection(x, F);
        while(setComp.size() > 0){
            for(size_t j = 0; j < F.size(); j++){
                for (int k : setComp){
                    if (F.at(j).count(k)){
                        if ( C.size() >0 && *find(C.begin(),C.end(), F.at(j)) == F.at(j)){
                            setComp.erase(k);
                        }else{
                            S.insert(F[j].begin(),F[j].end());
                            for (ite = S.begin(); ite != S.end(); ite++) {
                                U.erase(*ite);
                            }
                            C.push_back(S);
                            S.clear();
                            setComp.erase(k);
                            }
                        break;

                    }
                }
            }
        }

    }
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
            }
        }
        mayor= 0;
        for (ite = S.begin(); ite != S.end(); ite++) {
            U.erase(*ite);
        }
        C.push_back(S);
        }
    imprimirVector(C);
    cout << "Fin Solucion 4.2" << endl;
}

vector <set<int>> readFile(string file){
    string line ="";
    string sub;
    ifstream myfile (file);
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

vector <set<int>> randSets(){
    vector <set<int>> efx;
    srand(time(NULL));
    int num=1+rand()%(100);
    int cantidadSets = 1 + rand()%(100/2);
    int probElemUnic = (cantidadSets * 0.05);

    if(probElemUnic < 1){
        probElemUnic = probElemUnic + 1;
    }
    //cout  << cantidadSets << "; " << probElemUnic << endl;
    int c = 0;
    int elecSet = 0 + rand()%(cantidadSets);
    int insernum = 1 + rand()%(num);
    vector <int> elemunic;
    //cout << insernum << endl;
    //cout << "elemento unico: " << insernum << endl;
    for (int i = 0; i <= cantidadSets; i++){
        set<int> rands = {};
        int tamanoSets = 1 + rand()%(100/2);  //FALTA CONTROLAR DEL TODO LA CANTIDAD DE ELEMENTOS UNICOS QUE TIENE CADA SET
        for(int j = 0; j <= tamanoSets; j++){
                int numra = 1 + rand()%(num);
                //cout << numra << endl;
                //cout << elecSet << endl;
                if((c <= probElemUnic) && (elecSet == i)){
                    for(size_t i = 0; i != efx.size(); i++){
                        for(auto itr = efx[i].begin(); itr != efx[i].end(); itr++){
                            if(*itr == insernum){
                                efx[i].erase(insernum);
                            }
                    }
                            /*if(efx[i].find(insernum) == efx[i].end()){
                            efx[i].erase(insernum);
                            imprimirSets(efx[i]);
                            }*/
                    }
                    rands.insert(insernum);
                    elemunic.push_back(insernum);
                    elecSet = i + rand()%(cantidadSets);
                    insernum = 1 + rand()%(num);
                    //cout << "nuevo elemento unico: " << insernum << endl;
                        c = c + 1;
                }
                else if(numra != insernum){
                    if(elemunic.size() > 0 && !(find(elemunic.begin(), elemunic.end(), numra) != elemunic.end())){
                        rands.insert(numra);
                    }
                }
        }
        efx.push_back(rands);
        rands.clear();
    }
    return efx;
}

int main(int argc, char **argv){
    if(argc != 3){
	    cout << " Debe ejecutarse como ./tarea e k (e=0->Tiempo de Busqueda, e=1->Tamaño de solucion)" << endl;
	    return EXIT_FAILURE;
	}
    int ent = atoi(argv[1]);
    int k = atoi(argv[2]);
    if(ent != 0 && ent != 1 && k < 0){
        cout << "0 = Tiempo de Busqueda, 1 = Tamaño de solucion, k debe ser mayor a 0" << endl;
        return EXIT_FAILURE;
    }else if(k < 0){
        cout << "k debe ser mayor a 0";
        return EXIT_FAILURE;
    }
    if(ent == 0){
        vector <set<int>> F = readFile("pmed1.txt");
        set<int> X = getUniverse(F);

        auto start = std::chrono::high_resolution_clock::now();
        exhaustiveSearch(X,F);
        auto end = std::chrono::high_resolution_clock::now();
        auto int_s = std::chrono::duration_cast<std::chrono::seconds>(end - start);
        std::chrono::duration<double, std::milli> float_ms = end - start;
        cout << "Tiempo de busqueda exhaustiva:  " << float_ms.count() << " milliseconds" << endl;
        cout << endl << "-------------------------------------------------" << endl;

        start = std::chrono::high_resolution_clock::now();
        optimizedSearch(X,F);
        end = std::chrono::high_resolution_clock::now();
        float_ms = end - start;
        cout << "Tiempo de busqueda exhaustiva optimizada:  " << float_ms.count() << " milliseconds";
        cout << endl << "-------------------------------------------------" << endl;

        start = std::chrono::high_resolution_clock::now();
        greedAlgoritms(X,F);
        end = std::chrono::high_resolution_clock::now();
        float_ms = end - start;
        cout << "Tiempo de busqueda algoritmo greedy clasico:  " << float_ms.count() << " milliseconds";
        cout << endl << "-------------------------------------------------" << endl;

        start = std::chrono::high_resolution_clock::now();
        OptimizedGreedAlgoritmsV3(X,F,k);
        end = std::chrono::high_resolution_clock::now();
        float_ms = end - start;
        cout << "Tiempo de busqueda algoritmo greedy optimizado:  " << float_ms.count() << " milliseconds";
        cout << endl << "-------------------------------------------------" << endl;

        F = readFile("pmed38.txt");
        X = getUniverse(F);
        cout << "Iniciando experimentacion con greedy para más conjuntos..." << endl;
        start = std::chrono::high_resolution_clock::now();
        greedAlgoritms(X,F);
        end = std::chrono::high_resolution_clock::now();
        float_ms = end - start;
        cout << "Tiempo de busqueda algoritmo greedy clasico:  " << float_ms.count() << " milliseconds";
        cout << endl << "-------------------------------------------------" << endl;

        start = std::chrono::high_resolution_clock::now();
        OptimizedGreedAlgoritmsV3(X,F,k);
        end = std::chrono::high_resolution_clock::now();
        float_ms = end - start;
        cout << "Tiempo de busqueda algoritmo greedy optimizado:  " << float_ms.count() << " milliseconds";
        cout << endl << "-------------------------------------------------" << endl;

    }else{
        vector <set<int>> F = randSets();
        set<int> X = getUniverse(F);

        auto start = std::chrono::high_resolution_clock::now();
        exhaustiveSearch(X,F);
        auto end = std::chrono::high_resolution_clock::now();
        auto int_s = std::chrono::duration_cast<std::chrono::seconds>(end - start);
        std::chrono::duration<double, std::milli> float_ms = end - start;
        cout << "Tiempo de busqueda exhaustiva:  " << float_ms.count() << " milliseconds" << endl;
        cout << endl << "-------------------------------------------------" << endl;

        start = std::chrono::high_resolution_clock::now();
        optimizedSearch(X,F);
        end = std::chrono::high_resolution_clock::now();
        float_ms = end - start;
        cout << "Tiempo de busqueda exhaustiva optimizada:  " << float_ms.count() << " milliseconds";
        cout << endl << "-------------------------------------------------" << endl;

        start = std::chrono::high_resolution_clock::now();
        greedAlgoritms(X,F);
        end = std::chrono::high_resolution_clock::now();
        float_ms = end - start;
        cout << "Tiempo de busqueda algoritmo greedy clasico:  " << float_ms.count() << " milliseconds";
        cout << endl << "-------------------------------------------------" << endl;

        start = std::chrono::high_resolution_clock::now();
        OptimizedGreedAlgoritmsV3(X,F,k);
        end = std::chrono::high_resolution_clock::now();
        float_ms = end - start;
        cout << "Tiempo de busqueda algoritmo greedy optimizado:  " << float_ms.count() << " milliseconds";
        cout << endl << "-------------------------------------------------" << endl;
    }
}
