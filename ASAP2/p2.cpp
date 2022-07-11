/*
----------------------------------
|   Projeto 2 ASA 2020/2021      |
|   Grupo al102                  |
|   Francisco Ribeiro - 95578    |
|   Diogo Lopes - 96732          |
----------------------------------
*/

#include <vector> 
#include <iostream>
#include <list>


using namespace std;

#define INFINITO 100000000
#define SUCCESS 0;
#define FAILURE -1;
#define IS_LESS_THAN_0(X) (X < 0 ? false : true)
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

struct Arco {
    int v ;         // vertice destino
    int fluxo ;      // fluxo
    int capacidade;          // capacidade
    int reverso ;       // indice do arco oposto na rede residual
    bool deadEnd;   // 1 se o arco nao tiver caminhos de aumento, 0 c.c
};
 
class Grafo {
    int V;              // numero total de vertices
    int *d ;        // lista de d de cada vertice
    vector<Arco> *adjacencias;  // vetor de adjacencias
    
    public:
        // construtor
        Grafo(int V) {  
            adjacencias = new vector<Arco>[V];
            this->V = V;
            d = new int[V];
        }
        // adicionar um novo arco
        void addArco(int u, int v, int capacidade) {
            Arco a{v, 0, capacidade, (int)adjacencias[v].size(), 0};
            Arco b{u, 0, 0, (int)adjacencias[u].size(), 0};
            adjacencias[u].push_back(a);  // aresta "a" vai de "u" para "v"
            adjacencias[v].push_back(b); 
        }
        // BFS - Complexidade Temporal O(V + E) = O(E)
        bool BFS(int fonte, int sumidouro) {
            int i = 0;
            while( i < V ) {
                d[i] = -1;      // todos os vertice comecam com distancia a -1
                i++;
            }
            d[fonte] = 0;       // distancia da fonte eh 0
            list<int> fila;     // cria se a queue
            fila.push_back(fonte);      // mete se la a fonte
            vector<Arco>::iterator iterador ;   // pointer normal de um Arco
            
            while (!fila.empty()) {
                int u = fila.front();       // o primeiro elemento eh a fonte e depois vai por largura
                fila.pop_front();           // retira se da queue
                for (iterador = adjacencias[u].begin(); iterador != adjacencias[u].end(); iterador++) {
                    Arco &e = *iterador;
                    if (d[e.v] < 0  && e.fluxo < e.capacidade) {    // se ainda n foi visitado e tem capacidade p mandar fluxo
                        d[e.v] = 1 + d[u];                      // ta apenas um nivel acima do seu "pai"
                        fila.push_back(e.v);                    // o "filho vai pa queue"
                    }
                }
            }
            return IS_LESS_THAN_0(d[sumidouro]); //< 0 ? false : true ; "ja chegamos ao sumidouro"
        }
    

        int sendFluxo(int u, int fluxo, int sumidouro, int *L) {
            if (u == sumidouro)     // fonte e ja o sumidouro
                return fluxo;
            // visitar todos os vertices adjacentes a 'u' aka todas as arestas
            for (; L[u] < (int) adjacencias[u].size(); L[u]++) {
                Arco &e = adjacencias[u][L[u]];

                // verifica se adjacencia direta tem altura de 'u'+1, o arco ainda nao foi saturado
                // e existem possiveis caminhos de aumento        
                if (d[e.v] == 1 + d[u] && e.fluxo < e.capacidade && (e.deadEnd == 0)) {
                    // obtem o fluxo minimo do caminho de aumento de 'u' ate 't'
                    int currentFlow = MIN(fluxo, e.capacidade - e.fluxo);  
                    int tempFlow = sendFluxo(e.v, currentFlow, sumidouro, L);
                    if (tempFlow >= 1) {
                        e.fluxo += tempFlow;
                        adjacencias[e.v][e.reverso].fluxo -= tempFlow;
                        return tempFlow;
                    }
                    else {  // nao existem mais caminhos de aumento por este arco aka cap==max
                        adjacencias[e.v][e.reverso].deadEnd = 1;
                        //cout << adjacencias[e.v][e.reverso].v << "->" << e.v << " is deadend\n"; 
                    }
                }
            }
            return SUCCESS;
        }
 
   
        /*  Algoritmo de Dinic para fluxo maximo: 
            Complexidade Temporal - O(V^2 * E)
        */
        int DinicFluxoMax(int fonte, int sumidouro) {
            if (fonte == sumidouro)             // condicao de erro: grafo com apenas 1 vertice
                return FAILURE;
            int total = 0;          // fluxo maximo
            // O( V*(E + VE) ), o ciclo tem no maximo V iteracoes
            while (BFS(fonte, sumidouro)) {  // O(E) 
                int *L = new int[V+1] {0}; // lista de vertices a visitar
                while (int fluxo = sendFluxo(fonte, INFINITO, sumidouro, L))   // O(V*E)
                    total += fluxo;
            }
            return total;
        }
};
 
int main() {
    int N, K;       
    int X, Y;
    int d, p, c;
    
    if(!scanf("%d %d", &N, &K))
        return 0;
    if ((N < 2) || (K < 0))    
        return 0;
    
    //cout << (N+2 + 2*K + 2*N) << "\n";
    // inicializar grafo
    Grafo Grafo(N+2);                   

    // processar o input
    for(int i = 1; i <= N; i++) {
        if(!scanf("%d %d", &X, &Y))
            return 0;
        Grafo.addArco(0, i, X );
        Grafo.addArco(i, N+1, Y );
    }
    for (int i = 0; i < K; i++) {
        if(!scanf("%d %d %d", &d, &p, &c))
            return 0;
        Grafo.addArco(d, p, c);
        Grafo.addArco(p, d, c);
    }

    // calcular o fluxo maximo 
    cout << Grafo.DinicFluxoMax(0, N+1) << "\n"; 
    return 0;
}