#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
using namespace std;

string readStr(const char* name){
    ifstream is(name);
    if(!is){
        cerr<<"Conversion error:: readStr()\n"
              "  Nie można otworzyć pliku: "<<name<<endl;
        return "";
    }

    string line;
    string out;
    ostringstream os(out);
    while(is.good()){
        getline(is,line);
        os<<line<<endl;
    }
    out = os.str();
    is.close();

    return out;
}

string readList(const char* name){
    ifstream is(name);
    if(!is){
        cerr<<"Conversion error:: readList()\n"
              "  Nie można otworzyć pliku: "<<name<<endl;
        return "";
    }

    int node = 0;
    int edge = 0;
    string line;
    string value;
    string out;
    ostringstream os(out);
    while(is.good()){
        getline(is,line);
        istringstream isl(line);
        do {
            getline(isl, value, ' ');
            if(value != ""){
                os<<node<<'>'<<value<<"w1\n";
                ++edge;
            }
        } while(!isl.eof());
        ++node;
    }
    out = os.str();

    is.close();
    return "1 " + to_string(node) + " " + to_string(edge) + "\n" + out ;
}

string readListWeighted(const char* name){
    ifstream is(name);
    if(!is){
        cerr<<"Conversion error:: readListWeighted()\n"
              "  Nie można otworzyć pliku: "<<name<<endl;
        return "";
    }

    int node = 0;
    int edge = 1;
    string line;
    string value;
    string out;
    string weights;
    string weightvalue;
    ostringstream os(out);

    getline(is,weights);
    istringstream count(weights);
    for(char c; !count.eof(); ){
        count.get(c);
        if(c == ' ') ++edge;
    }
    
    istringstream wghts(weights);
    for(int i=0; i<edge; ){
        getline(is,line);
        istringstream isl(line);
        if(!is){
            cerr<<"Conversion error:: readListWeighted()\n"
                "  Lista zawiera mniej węzłów niż tablica wag: "<<i<<endl;
            edge = i;
            break;
        }
        do {
            getline(isl, value, ' ');
            if(value != ""){
                getline(wghts,weightvalue, ' ');
                os<<node<<'>'<<value<<'w'<<weightvalue<<endl;
                ++i;
            }
        } while(!isl.eof());
        ++node;
    }

    getline(is,line);
    if(is){
        cerr<<"Conversion error:: readListWeighted()\n"
            "  Lista zawiera więcej węzłów niż tablica wag"<<endl;
    }

    out = os.str();

    is.close();
    return "1 " + to_string(node) + " " + to_string(edge) + "\n" + out ;
}


bool saveStr(const char* name, string input){
    ofstream os(name);
    if(!os){
        cerr<<"Conversion error:: saveStr()\n"
              "  Nie można utworzyć pliku: "<<name<<endl;
        return 0;
    }
    os<<input;
    os.close();
    return 1;
}

bool saveList(const char* name, string input){
    ofstream os(name);
    if(!os){
        cerr<<"Conversion error:: saveList()\n"
              "  Nie można utworzyć pliku: "<<name<<endl;
        return 0;
    }

    istringstream is(input);
    string w;
    int a, b;
    bool is_directed;
    int nodes;
    int edges;
    
    getline(is, w, ' ');
    is_directed = (w == "0" ? 0 : 1);
    
    getline(is, w, ' ');
    nodes = stoi(w);
    string* list[nodes];
    for(int i=0; i<nodes; ++i){
        list[i] = new string;
    }
    
    getline(is, w, '\n');
    edges = stoi(w);

    for(int i=0; i<edges; ++i){
        if(w == ""){
            return 0;
        }

        getline(is, w, '>');
        a = stoi(w);

        getline(is, w, 'w');
        b = stoi(w);

        list[a]->append(to_string(b));
        list[a]->append(" ");
        if(!is_directed){
            list[b]->append(to_string(a));
            list[b]->append(" ");
        }

        getline(is, w, '\n');
    }
    
    for(int i=0; i<nodes-1; ++i){
        if(list[i]->size()) list[i]->pop_back();
        os<<*list[i]<<endl;
    }
    os<<*list[nodes-1];

    os.close();
    return 1;
}

bool saveListWeighted(const char* name, string input){
    ofstream osf(name);
    if(!osf){
        cerr<<"Conversion error:: saveList()\n"
              "  Nie można utworzyć pliku: "<<name<<endl;
        return 0;
    }

    string out;
    ostringstream os(out);
    istringstream is(input);
    string w;
    int a, b;
    bool is_directed;
    int nodes;
    int edges;
    
    getline(is, w, ' ');
    is_directed = (w == "0" ? 0 : 1);
    
    getline(is, w, ' ');
    nodes = stoi(w);
    string* list[nodes];
    for(int i=0; i<nodes; ++i){
        list[i] = new string;
    }
    
    getline(is, w, '\n');
    edges = stoi(w);

    for(int i=0; i<edges; ++i){
        if(w == ""){
            return 0;
        }

        getline(is, w, '>');
        a = stoi(w);

        getline(is, w, 'w');
        b = stoi(w);

        list[a]->append(to_string(b));
        list[a]->append(" ");
        if(!is_directed){
            list[b]->append(to_string(a));
            list[b]->append(" ");
        }

        getline(is, w, '\n');
        osf<<w<<' ';
    }
    
    for(int i=0; i<nodes-1; ++i){
        if(list[i]->size()) list[i]->pop_back();
        os<<*list[i]<<endl;
    }
    os<<*list[nodes-1];
    out = os.str();
    out.pop_back();

    osf.seekp(-1, osf.cur);
    osf<<endl<<out;
    osf.close();
    return 1;
}

/*
int main(){
    cout<<readList("graf1.txt");
}
*/
