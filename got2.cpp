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
vector<string> delete_base;
set<string> created;
set<string> destroyed;
int best_move=0;
int mucca=0;

void connetti_stacca_grafi(){
  for (const auto& coppia : grafi) {
    for (const int& elemento : coppia.second) {
      set<int> differenceSet;
      set_difference(coppia.second.begin(), coppia.second.end(),
                          grafo[elemento].adj.begin(), grafo[elemento].adj.end(),
                          inserter(differenceSet, differenceSet.begin()));
      differenceSet.erase(elemento);
      for (int n: differenceSet) {
        grafo[elemento].adj.insert(n);
        grafo[n].adj.insert(elemento);
        created.insert("+ "+to_string(n)+" "+to_string(elemento));
        mucca++;
      }
    }

  }
}

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
      destroyed.clear();
      created.clear();
      int N,M;
      in >> N >> M;
      int vittoria;
      
      vector<arco> archi;
      best_move=0;
      mucca=0;
      
      delete_base.clear();
      
      grafo.resize(N);

      //Lettura del grafo
      int f,t;
      for(int i=0;i<M;i++){
          arco s1;
          in>>f>>t;
          s1.nodo_partenza = f;
          s1.nodo = t;
          archi.push_back(s1);
          delete_base.push_back("- " + to_string(f) + " " + to_string(t));
          best_move++;
          grafo[f].adj.insert(t);
          grafo[f].id_nodo=f;
          grafo[t].adj.insert(f);
          grafo[t].id_nodo=t;
      }
      for (nodo& x: grafo) {
        set<int> lista_adiacenti_copia= x.adj;
        for (int n_id: lista_adiacenti_copia) {
          set<int> differenceSet;
          set_difference(x.adj.begin(), x.adj.end(),
                              grafo[n_id].adj.begin(), grafo[n_id].adj.end(),
                              inserter(differenceSet, differenceSet.begin()));

          int mancanti = differenceSet.size()-1;
          int n_adj = x.adj.size()-1;
          if (mancanti>0 && n_adj/mancanti < 2) {
            x.adj.erase(n_id);
            grafo[n_id].adj.erase(x.id_nodo);
            destroyed.insert("- "+to_string(n_id) + ' ' + to_string(x.id_nodo));
            mucca++;
          }
          
        }
      }

      out << "0 " << best_move << endl;
      for (arco i: archi) {
        out << "- " << i.nodo << " " << i.nodo_partenza << endl;
      }
      out << "***" << endl;
      calcola_grafi();
      connetti_stacca_grafi();
      if (best_move > mucca) {
        out << created.size() << " " << destroyed.size() << endl;
        for (string i: created){
          out << i << endl;
        }
        for (string i: destroyed){
          out << i << endl;
        }
        out << "***" << endl;
      } 
      
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