#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <ctime>
#include <set>
using namespace std;


////////////////////////////////////// Node //
class Node{
    int index;
    int count; //amt of neighbours
    vector<Node*> neighbours;

public:
    Node(int idx=0, int cnt=0):index(idx),count(cnt){
        neighbours.clear();
    }

    void setIndex(int idx){ index = idx; }
    int getCount(){ return count; }
    int getIndex(){ return index; }
    Node* getNeighbour(int idx){ return neighbours[idx]; }

    void print(ostream&os);
    bool addNeighbour(Node* nghbr);
};

void Node::print(ostream&os){
    
    os<<index<<".: neighbours: ";
    for(int i=0; i<count; ++i){
        os<<neighbours[i]->getIndex()<<",";
    }
    os<<" "<<count<<" in total"<<endl;
}

bool Node::addNeighbour(Node* nghbr){
    for(int i=0; i<count; ++i){
        if(neighbours[i] == nghbr) return 0;
    }

    neighbours.push_back(nghbr);
    ++count;
    return 1;
}


////////////////////////////////////// Edge //
class Edge{
    Node* start;
    Node* end;
    int weight;
    bool is_directed;

public:
    Edge(Node* from=NULL, Node* to=NULL, int wght=-1, bool dir=0)
        :start(from),end(to),weight(wght),is_directed(dir){};

    Node* getStart(){ return start; }
    Node* getEnd(){ return end; }
    int getWeight(){ return weight; }
    void setWeight(int a){ weight = a; }

    void print(ostream&os);
};

void Edge::print(ostream&os){
    os<<start->getIndex()<<"->"<<end->getIndex()<<", weight = "<<weight<<(is_directed ? ", " : ", not ")<<"directed"<<endl;
}


///////////////////////////////////// Graph //
class Graph{
    string name;
    bool is_directed;
    int node_count;
    int edge_count;
    Node* nodes;
    Edge* edges;

public:
    Graph(string n="untitled", bool dir=0, Node* np=NULL, Edge* ep=NULL):
    name(n),is_directed(dir),nodes(np),edges(ep){
        node_count = (np == NULL ? 0 : sizeof(np)/sizeof(Node*));
        edge_count = (ep == NULL ? 0 : sizeof(ep)/sizeof(Edge*));
    }
    ~Graph(){
        if(nodes) delete[] nodes;
        if(edges) delete[] edges;
    }

    int getNodeCount(){ return node_count; }
    int getEdgeCount(){ return edge_count; }
    void setName(string n){ name = n; }

    void print(ostream& os);
    bool setWeights(int* input, int size);
    void randomizeWeights(int a, int b);
    void randomizeWeights(int a);
    void randomizeWeights();
    bool strToGraph(string input, string inname="untitled");
    string graphToStr();

    string bellmanFord(int start, bool want_str=0);
};

void Graph::print(ostream& os){
    os<<"=== Graph.print() =================="<<endl<<endl;
    os<<"GRAPH NAME: "<<name<<endl<<endl;
    os<<"NODE COUNT: "<<node_count<<endl;
    os<<"EDGE COUNT: "<<edge_count<<endl;
    os<<"DIRECTED:   "<<(is_directed ? "true" : "false")<<endl<<endl;

    os<<"NODES: "<<endl;
    for(int i=0; i<node_count; ++i){
        nodes[i].print(os);
    }
    os<<endl;

    os<<"EDGES: "<<endl;
    for(int i=0; i<edge_count; ++i){
        edges[i].print(os);
    }
    os<<endl;
    os<<"===================================="<<endl<<endl;
}

bool Graph::setWeights(int* input, int size){
    if(size != edge_count){
        cerr<<"Graph method error:: setWeights()\n"
              "  Podana lista zawiera złą ilość danych\n"
              "  Oczekiwana ilość danych: "<<edge_count<<"\n"
              "  Otrzymana ilość danych: "<<size<<endl;
        return 0;
    }

    for(; size>0; --size){
        edges[size-1].setWeight(input[size-1]);
    }
    return 1;
}

void Graph::randomizeWeights(int a, int b){
    if(a > b){ swap(a, b); }
    srand(time(NULL));
    for(int i=0; i<edge_count; ++i){
        edges[i].setWeight(rand()%(b-a) + a);
    }
}

void Graph::randomizeWeights(int a){
    if(a<1){ a = 10; }
    randomizeWeights(0, a);
}

void Graph::randomizeWeights(){
    randomizeWeights(0, 10);
}

bool Graph::strToGraph(string input, string inname){
    istringstream is(input);
    string w;
    int a, b;

    name = inname;
    
    getline(is, w, ' ');
    is_directed = (w == "0" ? 0 : 1);
    
    getline(is, w, ' ');
    if(stoi(w) < 0){
        cerr<<"Graph method error:: strToGraph()\n"
              "  Przekazana liczba węzłów jest niepoprawna: "<<stoi(w)<<endl;
        return 0;
    }
    node_count = stoi(w);
    nodes = new Node[node_count];
    for(int i=0; i<node_count; ++i){
        nodes[i].setIndex(i);
    }
    
    getline(is, w, '\n');
    if(stoi(w) < 0){
        cerr<<"Graph method error:: strToGraph()\n"
              "  Przekazana liczba krawędzi jest niepoprawna: "<<stoi(w)<<endl;
        return 0;
    }
    edge_count = stoi(w);
    edges = new Edge[stoi(w)];

    for(int i=0; i<edge_count; ++i){
        getline(is, w, '>');
        if(is.eof()){
            cerr<<"Graph method error:: strToGraph()\n"
                "  Przekazano za mało krawędzi: "<<i<<endl;
            return 0;
        }

        a = stoi(w);
        if(a < 0 || a >= node_count){
            cerr<<"Graph method error:: strToGraph()\n"
                "  Przekazano niepoprawnie opisaną krawędź w linii: "<<i+2<<endl;
            return 0;
        }

        getline(is, w, 'w');
        b = stoi(w);
        if(b < 0 || b >= node_count){
            cerr<<"Graph method error:: strToGraph()\n"
                "  Przekazano niepoprawnie opisaną krawędź w linii: "<<i+2<<endl;
            return 0;
        }

        nodes[a].addNeighbour(&nodes[b]);
        if(is_directed == 0) nodes[b].addNeighbour(&nodes[a]);

        getline(is, w, '\n');
        edges[i] = *(new Edge(&nodes[a], &nodes[b], stoi(w), is_directed));
    }
    return 1;
}

string Graph::graphToStr(){
    ostringstream oss;

    oss<<(is_directed ? "1 " : "0 ");
    oss<<node_count;
    oss<<" ";
    oss<<edge_count;
    oss<<"\n";

    for(int i=0; i<edge_count; ++i){
        oss<<edges[i].getStart()->getIndex();
        oss<<">";
        oss<<edges[i].getEnd()->getIndex();
        oss<<"w";
        oss<<edges[i].getWeight();
        oss<<"\n";
    }

    return oss.str();
}

string Graph::bellmanFord(int start, bool want_str){
    if(edges == NULL || nodes == NULL){ return ""; }
    if(start < 0){ start = 0; }

    string str = "";
    ostringstream os(str);
    int values[node_count];       //dv
    int parentIDs[node_count];    //pv
    bool is_changed = 0;
    
    for(int i=0; i<node_count; ++i){
        values[i] = -1;           // waga -1 oznacza węzeł nieodwiedzony
    }
    values[start] = 0;
    parentIDs[start] = -1;        // rodzic -1 oznacza węzeł startowy

    // Tyle razy, ile jest węzłów (ostatnia iteracja do sprawdzenia występowania cyklu ujemnego)
    for(int i=0; i<node_count; ++i){
        // Dla każdej krawędzi
        for(int j=0; j<edge_count; ++j){
            // Jeśli startowy węzeł nie został odwiedzony, pomiń
            if(values[edges[j].getStart()->getIndex()] == -1){ continue; }

            // Jeśli końcowy węzeł krawędzi nie został odwiedzony ||
            //    || jego waga jest większa od wagi węzła startowego plus wagi krawędzi
            if(values[edges[j].getEnd()->getIndex()] == -1 || 
               values[edges[j].getEnd()->getIndex()] > values[edges[j].getStart()->getIndex()] + edges[j].getWeight()){   
                // Przypisz nową, mniejszą wagę węzłowi końcowemu
                values[edges[j].getEnd()->getIndex()] = values[edges[j].getStart()->getIndex()] + edges[j].getWeight();
                // Ustaw odpowiedniego rodzica
                parentIDs[edges[j].getEnd()->getIndex()] = edges[j].getStart()->getIndex();
                is_changed = 1;
            }
        }
        // Jeśli nic nie zostało zmienione, wyjdź z pętli - algorytm jest gotowy do zwrócenia wyniku
        if(!is_changed){ break; }
        // Jeśli algorytm wykonał |V|ty krok, w którym zaszła zmiana w tablicach
        if(is_changed && i == node_count-1){ 
            // Cykl ujemny
            cerr<<"Graph method error:: bellmanFord()\n"
                "  W następującym grafie występuje cykl ujemny: "<<name<<"\n"
                "  Algorytm ukończył pracę, ale zwrócony wynik jest najprawdopodobniej niepoprawny."<<endl;
            break;
        }
        is_changed = 0;
    }

    if(!want_str){
        // Zwrócenie tablic opisujących wynik
        os<<"dv = [ ";
        for(int i=0; i<node_count; ++i){
            os<<values[i]<<" ";
        }
        os<<"]\n";

        os<<"pv = [ ";
        for(int i=0; i<node_count; ++i){
            os<<parentIDs[i]<<" ";
        }
        os<<"]\n";
    } else {
        // Zwrócenie drzewa minimalnych ścieżek
        int w;
        os<<"1 "<<node_count<<" "<<node_count-1<<"\n";

        for(int i=0; i<node_count; ++i){
            if(parentIDs[i] == -1){ continue; }
            for(int j=0; j<edge_count; ++j){
                if(edges[j].getStart()->getIndex() == parentIDs[i] && edges[j].getEnd()->getIndex() == i){
                    w = edges[j].getWeight();
                    break;
                }
            }
            os<< parentIDs[i] <<'>'<< i <<'w'<< w <<endl;
        }
    }

    str = os.str();
    return str;
}



///////////////////////////////// functions //

/*
////////////////////////////////////// main //
int main(){
    Graph g;

    string GraphSchem = 
    "0 6 9\n"
    "0>1w4\n"
    "0>2w3\n"
    "1>2w1\n"
    "1>3w2\n"
    "2>3w4\n"
    "2>4w3\n"
    "3>4w2\n"
    "3>5w1\n"
    "4>5w6\n"
    "";

    //cout<<GraphSchem<<endl;
    g.strToGraph(GraphSchem, "test");
    //g.print(cout);
    
    cout<<g.GraphToStr()<<endl;
    g.randomizeWeights(0, 11);
    cout<<g.GraphToStr()<<endl;
}
*/