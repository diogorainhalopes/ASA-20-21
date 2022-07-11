/*
----------------------------------
|   Projeto 1 ASA 2020/2021      |
|   Grupo al102                  |
|   Francisco Ribeiro - 95578    |
|   Diogo Lopes - 96732          |
----------------------------------
*/

#include <iostream>
#include <vector> 
#include <algorithm>
#include <queue>
using namespace std;

#define SUCCESS 1;
#define FAILURE 0;

// vertex data structure
struct vertex {
    int val;        // value
    int inc_edges;  // incoming edges
    int d;          // distance from source   
};

// Graph data structure
struct Graph {
    vector<vertex> vertexL;     // vertex list
    vector<vector<int>> Adj;    // adjacency list
};


int V;      // number of vertexes in the graph
int E;      // number of edges in the graph
Graph G;    // Graph
vector<int> sources;    //  sources in the graph
int S = 0;      // number of sources

/* Function to process the input from the stdin 
    Time complexity: O(V + E)
*/
int processInput() {
    if (!scanf("%d %d", &V, &E))
        return FAILURE;
    if ((V < 2) || (E < 0)) {       // Condition to execute the program.
        return FAILURE;
    }
    //cout << "Size: " << (V+E) << endl;
    G.vertexL = vector<vertex>();       
    G.Adj = vector<vector<int>>(V, vector<int>());
    sources = vector<int>();
    

    //  Loop to create the graph without edges and create a temporary vector with all
    //  possible sources (all vertexes in the beginning).
    for(int i = 0; i < V; i++){             
        G.vertexL.push_back({i+1, 0, 0});
        sources.push_back(i+1);
    }

    //  Loop to create the edges of the graph. If a vertex has an incoming edge is removed
    //  from the temporary sources vector (its value becomes 0).
    for(int i = 0; i < E; i++){
        int a, b;
        if(scanf("%d %d", &a, &b));
        G.Adj[a-1].push_back(b);
        G.vertexL[b-1].inc_edges++; // increments the number of incoming edges for the given vertex
        sources[b-1] = 0;
    }

    return SUCCESS;
}

/*  Kahn's algorithm for topological sort
    Modified to return the longest possible distance in the graph
    Time complexity: O(V + E)
*/
int topSortMaxDistance() {
    int P = 0;      // Greatest distance in the graph 
    queue<int> q;   // Queue to store vertexes without incoming edges
    int count = 0;
    // Loop to place the sources in the queue
    for (int i = 0; i < V; i++) {
        if (sources[i]) {
            q.push(sources[i]);
            S++;
        }
    }

    // While the queue has unvisited edges, it keeps searching for vertexes
    while (!q.empty()) {

        int u = q.front();
        q.pop();
        
        // Iterates through all adjacent vertexes until it finds one without incoming edges
        // Once one is found, its distance becomes its predecessor's plus 1
        for (int i = 0; i < (int)G.Adj[u-1].size(); i++) {
            if(--G.vertexL[G.Adj[u-1][i]-1].inc_edges == 0) {
                q.push(G.Adj[u-1][i]);
                G.vertexL[G.Adj[u-1][i]-1].d = G.vertexL[u-1].d + 1;
                if(G.vertexL[G.Adj[u-1][i]-1].d > P)
                    P = G.vertexL[G.Adj[u-1][i]-1].d;
            }
        }
        count++;
    }

    if (count != V) {
        cout << "ERROR: Graph has cycles\n";
        return -1;
    }

    return P + 1;   // P + 1 to return the number of vertexes instead of edges.
}


int main() {

    if (!processInput()) {
        cout << "ERROR:  #V must be greater or equal to 2\n\t#E must be greater or equal to 0\n";
        return 0;
    }
    int D = topSortMaxDistance();
    if (D == -1)
        return 0;
    cout << S << " " << D << endl; 

    sources.clear();
    sources.shrink_to_fit();
    for(int i = 0; i < (int)G.Adj.size(); i++) {
        G.Adj[i].clear();
        G.Adj[i].shrink_to_fit();
    }
    G.vertexL.clear();
    G.vertexL.shrink_to_fit();
   
    return 0;
}
