#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
// #include <climits>
using namespace std;

struct arco{
  int id;
  int nodo_partenza;
  int nodo;
  int peso;
  int peso_max;
  arco(){
    id=-1;
    nodo_partenza=-1;
    peso_max=-1;
    peso=0;
  }
};

struct informazione{
  int costo_totale;
  vector<int> stanze_passate;
  vector<int> archi_speciali_passati;
  vector<int> archi_speciali_passati_non_maxati;
};

struct nodo{
  vector<arco> adj;
  int arco_di_passaggio;
  int nodo_entrante;
  int costo;
  nodo(){
    costo=999999;
    nodo_entrante=-1;
  }
};

int main()
{
    
    ifstream in("input.txt");
    ofstream out("output.txt");
    int N,M;
    in >> N >> M;
    int vittoria;
    vector<nodo> grafo;
    vector<arco> archi;
    grafo.resize(N);

    //Lettura del grafo
    int f,t;
    for(int i=0;i<M;i++){
        arco s1;
        in>>f>>t;
        s1.nodo_partenza = f;
        s1.nodo = t;
        archi.push_back(s1);
        grafo[f].adj.push_back(s1);
        grafo[t].adj.push_back(s1);
    }
    out<< 0 << ' ' << M << endl;
    for (arco x: archi){
      out<< '-' << ' ' << x.nodo << ' ' << x.nodo_partenza << endl;
    }
    out << "***" << endl;
    int a = 0;
}