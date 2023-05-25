#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
// #include <climits>
#include <iostream>
#include <map>
#include <set>

using namespace std;



struct arco{
  int id;
  int nodo_partenza;
  int nodo;
  arco(){
    id=-1;
    nodo_partenza=-1;
  }
};

struct informazione{
  int costo_totale;
  vector<int> stanze_passate;
  vector<int> archi_speciali_passati;
  vector<int> archi_speciali_passati_non_maxati;
};

struct nodo{
  set<int> adj;
  int id_grafo;
  int id_nodo;
  int nodo_entrante;
  bool visitato;
  nodo(){
    visitato=false;
    nodo_entrante=-1;
    id_grafo=-1;
    id_nodo=-1;
  }
};
//globall
int grafo_count = 0;
map<int, set<int>> grafi;
vector<nodo> grafo;

void calcola_grafi(){
  stack<int> coda;
  grafo_count = 0;
  for (nodo& x: grafo) {
    cout << "asd"<<endl;
    //se non visitato aggiungo gli adiacenti alla coda e aumento il grafo count di 1 che mi serve per inserire l'id
    if (!x.visitato){
      grafo_count++;
      grafi[grafo_count].insert(x.id_nodo);
      x.visitato = true;
      x.id_grafo = grafo_count;
      for(int v: x.adj){
        coda.push(v);
      }
    }
    //ciclo tutti gli adiacenti per salvarmi il grafo connesso! e settargli l'id
    while (!coda.empty()){
      int a = coda.top();
      coda.pop();
      nodo& n = grafo[a];
      if (n.visitato){
        continue;
      }
      n.visitato = true;
      grafi[grafo_count].insert(n.id_nodo);
      n.id_grafo = grafo_count;
      for(int v: n.adj){
        coda.push(v);
      }
    }
  }
}


int main()
{
    for (int i = 0; i < 20; ++i) {
      string inf = "input/input" + to_string(i) + ".txt";
      string outf = "output/output" + to_string(i) + ".txt";
      ifstream in(inf);
      ofstream out(outf);
      grafo_count = 0;
      grafi.clear();
      grafo.clear();
      int N,M;
      in >> N >> M;
      int vittoria;
      
      vector<arco> archi;
      vector<arco> created;
      vector<arco> destroyed;
      grafo.resize(N);

      //Lettura del grafo
      int f,t;
      for(int i=0;i<M;i++){
          arco s1;
          in>>f>>t;
          s1.nodo_partenza = f;
          s1.nodo = t;
          archi.push_back(s1);
          grafo[f].adj.insert(t);
          grafo[f].id_nodo=f;
          grafo[t].adj.insert(f);
          grafo[t].id_nodo=t;
      }
      for (nodo& x: grafo) {
        for (int n_id: x.adj) {
          set<int> differenceSet;
          set_difference(x.adj.begin(), x.adj.end(),
                              grafo[n_id].adj.begin(), grafo[n_id].adj.end(),
                              inserter(differenceSet, differenceSet.begin()));

          int mancanti = differenceSet.size();
          int n_adj = x.adj.size()-1;
          if (mancanti>0 && n_adj/mancanti < 2) {
            ///elliminare e andare avanti ricalcolare i grafi e proseguire con il capire se eliminare o collegare i grafi restanti
          }
          
        }
      }

      calcola_grafi();
      out << grafi.size();
    }


    /*
    for (const auto& coppia : grafi) {
        int chiave = coppia.first;                  // Ottieni la chiave della mappa
        const set<int>& setValori = coppia.second;  // Ottieni il set di valori associato alla chiave
        int size = setValori.size();
        int n_valori = size*(size-1)/2;
        // Cicla sugli elementi del set
        for (const int& valore : setValori) {
            std::cout << setValori.size() << " ";
        }

        std::cout << std::endl;
    }
    

    out<< 0 << ' ' << M << endl;
    for (arco x: archi){
      out<< '-' << ' ' << x.nodo << ' ' << x.nodo_partenza << endl;
    }
    out << "***" << endl;
    int a = 0;
    */
}