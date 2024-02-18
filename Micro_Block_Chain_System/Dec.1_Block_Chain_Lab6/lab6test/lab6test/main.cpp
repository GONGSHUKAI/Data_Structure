#include "structure.h"
#define MAX 9999999

void printPath(string v, unordered_map <string, double> distTo, unordered_map <string, string> edgeTo){
    stack <string> pathway;
    for (const auto &pair : distTo){
        if (pair.first != v && pair.second < MAX){
            cout << "Account " << v << " to Account " << pair.first << endl;
            cout << "Shortest path: " << pair.second << endl;
            cout << "Pathway: ";
            pathway.push(pair.first);
            string mid = edgeTo[pair.first];
            while (mid != ""){//Backtracing
                pathway.push(mid);
                mid = edgeTo[mid];
            }
            while (!pathway.empty()){
                cout << pathway.top();
                pathway.pop();
                if (!pathway.empty()) cout << " -> ";
            }
            cout << endl;
            cout << endl;
        }
    }
}

void Dijkstra(string v){//Runtime Error, Complexity O(n^2)
    clock_t startTime = clock();//count time
    //Search for the shortest path from account v to all other accounts
    unordered_map <string, bool> visit;
    unordered_map <string, double> distTo;//Store shortest path from v to other accounts
    unordered_map <string, string> edgeTo;//Store closest account from the end of path
    string u;//account name
    double w;//weight
    
    //Initialize
    for (const auto & pair : graph){
        visit[pair.first] = false;
        distTo[pair.first] = MAX;
        edgeTo[pair.first] = "";
    }
    distTo[v] = 0;
    //Add one account to the Shortest-Path-Tree at a time
    for (const auto &pair : graph){
        double min = MAX;
        for (const auto &pair2 : graph){
            if (!visit[pair2.first] && distTo[pair2.first] < min){
                u = pair2.first;
                min = distTo[pair2.first];
            }
        }
        visit[u] = true;
        //u is the closest account to v
        //visit account u;
        //relax every outEdge from u
        for (const auto &pair3 : graph[u].outEdges){
            w = pair3.second.totalAmount;//weight of edge <u, pair3.first>
            if (!visit[pair3.first] && w < MAX && distTo[u] + w < distTo[pair3.first]){
                //pair3.first haven't been visited
                //w is not ∞
                //edge can be relaxed
                distTo[pair3.first] = distTo[u] + w;//relax edge
                edgeTo[pair3.first] = u;
            }
        }
    }
    printPath(v, distTo, edgeTo);
    clock_t endTime = clock();
    cout << "Dijkstra Runtime: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
}

void Dijkstra2(string v){//Complexity O(nlogn)
    clock_t startTime = clock();//count time
    //Search for the shortest path from account v to all other accounts
    unordered_map <string, bool> visit;
    unordered_map <string, double> distTo;//Store shortest path from v to other accounts
    unordered_map <string, string> edgeTo;//Store closest account from the end of path
    string u;//account name
    double w;//weight
    
    priority_queue<pair<double, string>, vector<pair<double, std::string>>, greater<pair<double, string>>> pq;
    
    for (const auto& pair : graph){
        visit[pair.first] = false;
        distTo[pair.first] = MAX;
        edgeTo[pair.first] = "";
    }
    distTo[v] = 0;
    pq.push({0, v});
    while (!pq.empty()){
        auto node = pq.top();
        pq.pop();
        u = node.second;
        if (visit[u]) continue;//Skip account u if u has been visited
        visit[u] = true;

        for (const auto& edge : graph.at(u).outEdges) {
            double weight = edge.second.totalAmount;
            if (!visit[edge.first] && weight < MAX && distTo[u] + weight < distTo[edge.first]) {
                distTo[edge.first] = distTo[u] + weight;
                edgeTo[edge.first] = u;
                pq.push({distTo[edge.first], edge.first});
            }
        }
    }
    printPath(v, distTo, edgeTo);
    clock_t endTime = clock();
    cout << "Dijkstra Runtime: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
}

bool DescendComp(const pair <string, int> &pair1, const pair <string, int> &pair2){
    return pair1.second > pair2.second;
}

void degreeAnalysis(unsigned long long k){
    /*
    Calculate the average outdegree and indegree of the account graph.
    Show the top k accounts with the highest degree or indegree.
    */
    double outdegreeSum = 0;
    double indegreeSum = 0;
    vector <pair<string, int>> outdegrees;
    vector <pair<string, int>> indegrees;

    for (const auto &pair : graph){
        outdegreeSum += pair.second.outEdges.size();
        outdegrees.push_back({pair.first, pair.second.outEdges.size()});
        indegreeSum += pair.second.inEdges.size();
        indegrees.push_back({pair.first, pair.second.inEdges.size()});
    }
    double aveOutdegree = outdegreeSum / graph.size();
    double aveIndegree = indegreeSum / graph.size();
    sort(outdegrees.begin(), outdegrees.end(), DescendComp);
    sort(indegrees.begin(), indegrees.end(), DescendComp);
    
    cout << "Average Outdegree of the graph: " << aveOutdegree << endl;
    cout << "Average Indegree of the graph: " << aveIndegree << endl;
    cout << endl;
    cout << "k accounts with largest outdegrees: " << endl;
    for (int i = 0 ; i < k ; i++){
        cout << "Account Name: " << outdegrees[i].first << " outdegrees: " << outdegrees[i].second << endl;
    }
    cout << endl;
    cout << "k accounts with largest indegrees: " << endl;
    for (int i = 0 ; i < k ; i++){
        cout << "Account Name: " << indegrees[i].first << " indegrees: " << indegrees[i].second << endl;
    }
    cout << endl;
}


int main(){
    readBlock();
    readTransactions("tx_data_part1_v2");
    //Dijkstra2("1EKGMGUmVK4Mpbnnh3UysJvT3Z5fywrv5A");
    degreeAnalysis(3);
    return 0;
}

