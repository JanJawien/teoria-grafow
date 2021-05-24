#include <iostream>
#include <fstream>
#include <string>
#include "graph.cpp"   // Klasy i metody
#include "convert.cpp" // read___(), save____()
using namespace std;

int main(){
    string graphSchem;

    graphSchem = readListWeighted("graf1/graf1.txt");   // czytaj graf z pliku w postaci rozszerzonej listy sasiedztw
    if(graphSchem == ""){ return 1; }                   // jeśli nie znaleziono pliku, zakończ program (błąd 1)

    Graph g;
    g.strToGraph(graphSchem, "Graf 1"); // zbuduj graf g na podstawie stringa

    cout<<g.bellmanFord(0, 0);  // wykonaj algorytm Bellmana-Forda na grafie, i wypisz wynik w postaci dwóch tablic na konsoli

    graphSchem = g.bellmanFord(0, 1);               // wykonaj algorytm Bellmana-Forda na grafie, i zapisz wynik w postaci stringa opisującego
                                                    //  znalezione drzewo minimalnych ścieżek

    saveStr("graf1/graf1_tree_str.txt", graphSchem);     // zapisz znalezione drzewo w pliku, w postaci stringa
    saveList("graf1/graf1_tree_list.txt", graphSchem);   // zapisz znalezione drzewo w pliku, w postaci listy sąsiedztw  

    Graph g_tree;                                    
    g_tree.strToGraph(graphSchem, "Drzewo ścieżek grafu 1");    // zbuduj graf drzewa minimalnych ścieżek

    ofstream os("graf1/graf1_tree_print.txt");  
    if(!os){ return 2; }                        // jeśli nie udało się otworzyć pliku, zakończ program (błąd 2)
    g_tree.print(os);                            // wydrukuj drzewo w czytelnym formacie w pliku 
    os.close();  

    return 0;  
}


///////// Drugi przykładowy program - czyta graf z pliku graf2/graf2.txt i wykonuje te same działania.
/*
    Graph g;
    string graphSchem = readStr("graf2/graf2.txt");
    if(graphSchem == ""){ return 1; }
    g.strToGraph(graphSchem, "Graf 2");

    cout<<g.bellmanFord(0);
    graphSchem = g.bellmanFord(0, 1);

    Graph g_tree;
    g_tree.strToGraph(graphSchem, "Drzewo ścieżek grafu 2");
    
    saveStr("graf2/graf2_tree_str.txt", graphSchem);     
    saveList("graf2/graf2_tree_list.txt", graphSchem);  

    ofstream os("graf2/graf2_tree_print.txt");
    if(!os){ return 2; }
    g_tree.print(os);
    os.close();

    return 0;
*/