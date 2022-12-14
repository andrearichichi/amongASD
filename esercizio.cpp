#include <fstream>
#include <vector>
#include <stack>
using namespace std;


struct arco{
  int nodo;
  int peso;
  int ct;
};

struct nodo{
  vector<arco> adj;
  int costo;
  bool visited;
  nodo(){
    visited=false;
    costo=0;
  }
};

vector<nodo> grafo;
vector<nodo> grafo_stud;
int count=0;
arco impostore = {0,0,0};
arco studente = {0,0,0};
int fablab;

int main(void)
{
  ifstream in("input.txt");
  ofstream out("output.txt");
  int N,M,S;
  in >> N >> M >> S;
  in >> impostore.nodo >> studente.nodo >> fablab;

  grafo.resize(N);
  grafo_stud.resize(N);
  //Lettura del grafo
  for(int i=0;i<M;i++){
    arco s1;
    int f,t,j;
    in>>f>>t>>j;
    s1.nodo = t;
    s1.peso = j;
    s1.ct=0;
    grafo[f].adj.push_back(s1);
  }
  //creo un grafo uguale per lo studente
  grafo_stud.assign(grafo.begin(), grafo.end());
  stack<arco> st;
  
  //st lista di archi con peso e ct=peso totale raggiunto fin la
  st.push(impostore);
  while(!st.empty()){
    arco n=st.top();
    st.pop();
    //Se abbiamo giá visitato il nodo e il costo è gia inferiore o uguale, ignoriamolo.
    if(grafo[n.nodo].visited)
      if (grafo[n.nodo].costo <= n.ct)
        continue;

    grafo[n.nodo].visited=true;
    grafo[n.nodo].costo=n.ct;
    //Aumenta il contatore
    //Visita tutti i vicini
    for(arco v:grafo[n.nodo].adj){
      v.ct=v.peso+grafo[n.nodo].costo;
      st.push(v);        
    }
  }

  st.push(studente);
  while(!st.empty()){
    arco n=st.top();
    st.pop();
    //Se abbiamo giá visitato il nodo e il costo è gia inferiore o uguale, ignoriamolo.
    if(grafo_stud[n.nodo].visited)
      if (grafo_stud[n.nodo].costo <= n.ct)
        continue;

    grafo_stud[n.nodo].visited=true;
    grafo_stud[n.nodo].costo=n.ct;
    //Aumenta il contatore
    //Visita tutti i vicini
    for(arco v:grafo_stud[n.nodo].adj){
      v.ct=v.peso+grafo_stud[n.nodo].costo;
      st.push(v);        
    }
  }

  out<<count<<endl;
  return 0;
}

