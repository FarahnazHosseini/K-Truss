//This code works on small datasets

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
using namespace std;

class graph {
public:
    int totalEdge;
    int totalNode;
    vector<int> csr;
    vector< vector <int > > supports;
    vector<int> degree;
    void setGraph(int, int);
    void Ktruss();
    void printSupport();
    friend void readFile(graph& g, string fileName);
    friend void printGraph(graph g);
    friend void printEdge(graph g);
};
//...........................
void graph::setGraph(int totalNode, int totalEdge) {

    this->totalNode = totalNode;
    this->totalEdge = totalEdge;
    csr.resize(totalNode + totalEdge, -1);
    degree.resize(totalNode, -1);
    supports.resize(totalEdge/2, vector<int>(3,-1));
}
//...........................
void graph::Ktruss(){
    
    int from,to,a,b,bi,startIndexA,endIndexA,startIndexB,endIndexB,count,s=0;
    for(int a=0;a<totalNode;a++){
      from=csr[a];
      to=csr[a+1];
      if(a==totalNode-1)
        to=csr.size(); 
      for(int bi=from;bi<to;bi++){//neighbors of a
             b=csr[bi];
             if(a<b)
                continue;
             count=0;
            //___________________
            startIndexA= csr[a];
            endIndexA=csr[a+1];
            if(a==totalNode-1)
                endIndexA=csr.size();
            startIndexB =csr[b];
            endIndexB =csr[b+1];
            if(b==totalNode-1){
               endIndexB=csr.size();
            }
            //___________________
            int i=startIndexA;
            int j=startIndexB;
            do
            {
                if(csr[i]==csr[j]){
                    count++;
                    i++;
                    j++;
                }
                else if(csr[i]<csr[j]){
                    i++;
                }
                else{
                    j++;
                }
            } while (i<endIndexA && j<endIndexB); 
            //cout<<" A: "<<a<<"B: "<<b<<" Support: " <<count<<endl;
             
            supports[s][0]=a;
            supports[s][1]=b;
            supports[s][2]=count;
            s++;     
        }
    }
}
//............................
void graph::printSupport(){
    for(int i=0;i<supports.size();i++){
        cout<<"node= "<<supports[i][0]<<"    "<<"node= "<<supports[i][1]<<" support: "<<supports[i][2]<<endl;
    }
}
//...........................
void printGraph(graph g) {
    cout << endl;
    int from, to;
    for (int i = 0;i < g.totalNode;i++) {
        from = g.csr[i];
        to=g.csr[i+1];
        if(i==g.totalNode-1){
            to=g.csr.size();
        }
        cout << i << ": ";
        for (int j = from;j < to;j++) {
            cout<< g.csr[j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    //printDegrees(g.degree);
}
//..........................
void readFile(graph& g, string fileName) {
    fstream p;
    int src, dest, totalEdge, totalNode, rowIndex = 0,lastSrc = -1;
    p.open(fileName);
    p >> totalNode >> totalEdge;
    g.setGraph(totalNode, totalEdge);
    int colIndex = totalNode;
    while (!p.eof()) {
        p >> src >> dest;   
        if (dest == -1) {
            rowIndex++;
            lastSrc = src;
            g.degree[src] = -1;
            continue;
        }
        else if (src != lastSrc) {
            g.csr[rowIndex] = colIndex;
            rowIndex++;
            lastSrc = src;
        }
        g.csr[colIndex] = dest;
        colIndex++;
        g.degree[src]++;
    }
    p.close();
   // for(int i=0;i<g.csr.size();i++){
    //    cout<<i<< " : " <<g.csr[i]<<endl;
   // }
}
//..........................................
int main() {
    graph g;
   string fileName = "network6.txt";
   readFile(g, fileName);
   printGraph(g);
   g.Ktruss();
   g.printSupport();
}
