#include <fstream>
#include <vector>
#include <stack>
using namespace std;


struct arco{
  int nodo_partenza;
  int nodo;
  int peso;
  int ct;
};

struct nodo{
  vector<arco> adj;
  int nodo_entrante;
  int costo;
  bool visited;
  nodo(){
    visited=false;
    costo=0;
    nodo_entrante=-1;
  }
};

vector<nodo> grafo;
vector<nodo> grafo_stud;
arco impostore = {-1,0,0,0};
arco studente = {-1,0,0,0};
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
    s1.nodo_partenza = f;
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
    grafo[n.nodo].nodo_entrante=n.nodo_partenza;
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
    grafo_stud[n.nodo].nodo_entrante=n.nodo_partenza;
    //Aumenta il contatore
    //Visita tutti i vicini
    for(arco v:grafo_stud[n.nodo].adj){
      v.ct=v.peso+grafo_stud[n.nodo].costo;
      st.push(v);        
    }
  }
  int vittoria;
  vittoria = grafo[fablab].costo < grafo_stud[fablab].costo ? 1 : grafo[fablab].costo > grafo_stud[fablab].costo ? 2 : 0;
  vector<int> stanze_imp;
  

  nodo n;
  stanze_imp.push_back(fablab);
  n=grafo[fablab];
  while(n.nodo_entrante!=-1 && n.nodo_entrante!=0){
    stanze_imp.push_back(n.nodo_entrante);
    n=grafo[n.nodo_entrante];
  }

  out<<vittoria<<endl;
  out<<grafo[fablab].costo << " " << grafo_stud[fablab].costo<<endl;
  
  return 0;
}

