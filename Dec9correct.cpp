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
    vector< vector <int> >TriangleList;
    vector< vector <int > > commonNeighbor;
    vector<int> degree;
    void setGraph(int, int);
    void preKtruss();
    void Ktruss();
    void calcKtruss(vector <int> &);
    int getEdgeInx(int, int);
    void printSupport();
    void printTriangles();
    void updateNeighbours(int, vector <int> &);
    void printGraph();
    void deletEdge(int, int);
    int findMin();
    void setFromAndTo(int node, int& from, int& to);
    friend void readFile(graph& g, string fileName);
};
//...........................
void graph::setGraph(int totalNode, int totalEdge) {
    this->totalNode = totalNode;
    this->totalEdge = totalEdge;
    csr.resize(totalNode + totalEdge, -1);
    degree.resize(totalNode, -1);
    supports.resize(totalEdge, vector<int>(4, -1));
    commonNeighbor.resize(totalEdge / 2, vector<int>(0));
}
//........................
int graph::getEdgeInx(int a, int b) {
    for (int i = 0;i < supports.size();i++) {
        if ((supports[i][0] == a && supports[i][1] == b) || (supports[i][0] == b && supports[i][1] == a))
            return i;
    }
    return -1;
}
//............................
void graph::setFromAndTo(int node, int& from, int& to) {
    from = csr[node];
    if (node == totalNode - 1)
        to = csr.size();
    else
    {
         to = csr[node + 1];
         if(to==-1){
            to=from+degree[node];
         }
    }
       
}
//............................
void graph::calcKtruss(vector <int> & updateList) {// calculating the support of edges which are impacted of removing other eges 
    int from, to, a, b, bi, fromA, toA, edgeID, fromB, toB, count, s = 0;
    for (int E = 0;E < updateList.size();E++) {    
            edgeID = updateList[E];
            a=supports[edgeID][0];
            b=supports[edgeID][1];
            if (edgeID == -1 || supports[edgeID][2] == -1)
                continue;
            count = 0;
            setFromAndTo(a, fromA, toA);
            setFromAndTo(b, fromB, toB);
            int i = fromA, j = fromB;
            do
            {
                if (csr[i] == csr[j]) {
                    count++;
                    commonNeighbor[s].push_back(csr[i]);
                    i++;
                    j++;
                }
                else if (csr[i] < csr[j]) 
                    i++;
                else 
                    j++;
            } while (i < toA && j < toB);
            supports[edgeID][2] = count;
        }
        updateList.resize(0);
}
//...........................
void graph::deletEdge(int a, int b) {
    int from, to, i;
    setFromAndTo(a, from, to);
    for (i = from;i < to;i++) {
        if (csr[i] == b) {
            csr[i] = -1;
            break;
        }
    }
    //____________________
    setFromAndTo(b, from, to);
    for (i = from;i < to;i++) {
        if (csr[i] == a) {
            csr[i] = -1;
            break;
        }
    }
}
//............................
void graph::updateNeighbours(int edgeID, vector <int> &updateList) {//when an edge is removing it updateds the common neighbors a, b
    int a, b, Edge;
    a = supports[edgeID][0];
    b = supports[edgeID][1];
    for (int i = 0; i < commonNeighbor[edgeID].size(); i++)
    {
        Edge = getEdgeInx(a, commonNeighbor[edgeID][i]);
        if (Edge != -1 && supports[Edge][2] > 0)
            updateList.push_back(Edge);
        Edge = getEdgeInx(b, commonNeighbor[edgeID][i]);
        if(Edge!=-1 && supports[Edge][2] > 0)
            updateList.push_back(Edge);
    }
}
//............................
int graph::findMin() {
    int min = totalEdge;
    for (int i = 0;i < supports.size();i++) {
        if (min > supports[i][2] && supports[i][2] !=-1)
            min = supports[i][2];
    }
    return min;
}
//............................
void graph::Ktruss() {
    int remainEdge = (totalEdge / 2), l = 1, levelIndex, edgeID, edgeInx, src, dest, a, b, from, to;
    int Triangle = 1, minTriangele=0,lastMin=0,newMin=0;
    vector <int > currrentList;
    vector <int>  updateList;
    do {
        for (int edgeID = 0;edgeID < supports.size();edgeID++) {
            if (supports[edgeID][2] <= newMin && supports[edgeID][2]!=-1) {//removing edges
                currrentList.push_back(edgeID);
                supports[edgeID][3] = Triangle;
                supports[edgeID][2] = -1;
                remainEdge--;
                updateNeighbours(edgeID,updateList);               
                a = supports[edgeID][0];
                b = supports[edgeID][1];
                deletEdge(a, b);
            }
        }
        calcKtruss(updateList); 
        newMin = findMin();
        if (newMin > lastMin)
        {
            Triangle = newMin;
            TriangleList.push_back(currrentList);
            currrentList.resize(0);
            lastMin=newMin ;
        }
        else
        {
            Triangle = Triangle;         
        }
    } while (remainEdge > 0);
}
//............................
void graph::preKtruss() {
    int from, to, a, b, bi, fromA, toA, fromB, toB, count, edgeID = 0;
    for (int a = 0;a < totalNode;a++) {
        //cout<<a<<endl;
        setFromAndTo(a, from, to);
        for (int bi = from;bi < to;bi++) {
            b = csr[bi];
            //cout<<"\t"<<b<<endl;
            if (a < b)
                continue;
            count = 0;
            //___________________
            setFromAndTo(a, fromA, toA);
            setFromAndTo(b, fromB, toB);
            int i = fromA;
            int j = fromB;
            do
            {
                if (csr[i] == csr[j]) {
                    count++;
                    commonNeighbor[edgeID].push_back(csr[i]);
                    i++;
                    j++;
                }
                else if (csr[i] < csr[j])
                    i++;
                else 
                    j++;
            } while (i < toA && j < toB);            
            supports[edgeID][0] = a;
            supports[edgeID][1] = b;
            supports[edgeID][2] = count;
            supports[edgeID][3] = 0;
            //if(count>0)// tedad common neighbors
                 //cout<<"("<<a<<"__"<<b<<")="<<count<<"\t";
                 //for(int c=0;c<commonNeighbor[edgeID].size();c++)
                  //  cout<<commonNeighbor[edgeID][c]<<" , ";
                //cout<<endl;
            edgeID++;
        }
    }
}
//............................
void graph::printSupport() {
    for (int i = 0;i < supports.size();i++) {
        cout << i << " :\t" << supports[i][0] << "---" << supports[i][1] << "\t\tsupport: " << supports[i][2] << "\tk-truss: " << supports[i][3]  << endl;
    }
}
//...........................
void graph::printTriangles(){
    int edge;
    for(int i=0;i<TriangleList.size();i++){
        cout<<"\n___________number of Triangles:  "<<i<<"   ___________\n";
        for(int j=0;j<TriangleList[i].size();j++){
            edge=TriangleList[i][j];
            cout<<TriangleList[i][j]<<"("<<supports[edge][0]<<"__"<<supports[edge][1]<<")"<<" , ";
        }
        cout<<endl;
    }
}
//...........................
void graph::printGraph() {
    int from, to,noden=0;
    for (int i = 0;i < totalNode;i++) {
        setFromAndTo(i, from, to);
       if(csr[i]==-1){
            continue;
        //    cout<<"csr[i]"<<csr[i]<<endl;
        }
        cout << i << ": ";

        for (int j = from;j < to;j++) {
           cout << csr[j] << " ";
        }
        cout << endl;
        //noden++;
    }
}
//..........................
void readFile(graph& g, string fileName) {
    fstream p;
    int src, dest, totalEdge, totalNode, rowIndex = 0, lastSrc = -1;
    p.open(fileName);
    p >> totalNode >> totalEdge;
    g.setGraph(totalNode, totalEdge);
    int colIndex = totalNode;
    while (!p.eof()) {
        p >> src >> dest;
        //cout<<"("<<src<<", "<<dest<<")"<<endl;
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
}
//..........................................
int main() {
    graph g;
    string fileName = "data5000Test.txt";
    readFile(g, fileName);
    //g.printGraph();
    clock_t start = clock();
    g.preKtruss();
    g.Ktruss();
    clock_t end = clock();
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    cout << "\n" << "cpu_time_used  " << cpu_time_used << "\n";
    //g.printSupport();
   // g.printTriangles();
}
