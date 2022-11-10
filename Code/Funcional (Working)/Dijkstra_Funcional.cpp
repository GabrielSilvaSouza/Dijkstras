#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <utility>
#include <bits/stdc++.h>
#include <tuple>
#include <typeinfo>
#include <numeric>

using namespace std;


class Graph { 

    int numberVertex; 

    vector< tuple <int, double> > * graphLL;  

public:

    Graph(string file_name) 
    {

        ifstream infile(file_name); 
        int numberVertex;
        infile >> numberVertex;
        infile.close();

        this->numberVertex = numberVertex;

        graphLL = new vector< tuple <int, double> > [numberVertex+1];
    }


    void graphBuilderAdjacencyVector(string file_name) 
    {
        int Vertex_a, Vertex_b;
        double weight;
        ifstream infile(file_name); 
        infile >> weight;

        while(infile >> Vertex_a >> Vertex_b >> weight ) 
        {
        
            if (weight < 0)
            {
                cout << "There are negative values, thus Dijkstra cannot be performed..." << endl;
                cout << "Terminating program...." << endl;
                exit(0);
                
            }

            graphLL[Vertex_a].push_back(make_tuple(Vertex_b,weight));
            graphLL[Vertex_b].push_back(make_tuple(Vertex_a,weight));
            
        }

    } 

    void dijkstra(int start) 
    {

        priority_queue < tuple <double, int>, vector<tuple<double,int>>, greater< tuple<double, int> >  > heapgraph;
        
        vector<double> dist(numberVertex+1, numeric_limits<double>::infinity());
        vector<double> cost(numberVertex+1, numeric_limits<double>::infinity());

        tuple<int, double> u;

        list<int> explored;

        dist[start] = 0;
        cost[start] = 0;

        double sum = 0.0f;
        int count = -1;

        while (explored.size() != dist.size())
        {

            for (const auto& v : graphLL[start]) 
            {

                if (dist[get<0>(v)] > (dist[start] + get<1>(v)))
                {
                        
                        dist[get<0>(v)] = dist[start] + get<1>(v);
                        heapgraph.push(make_tuple(dist[get<0>(v)], get<0>(v)));

                }

            }

            explored.push_back(start);
            u = heapgraph.top();
            start = get<1>(u);
            heapgraph.pop();
            
        }

        cout << "__________________________________" << endl;
        cout << "|" << "Vertice" << "  |  " << "Distancia do vertice"  << "|" << endl;
        for(const auto& i: dist) {
            
            count++;
            cout << "       " <<  count <<  "  |   " <<  i << "                    |" << endl;
            
        }

        for(int i = 1; i < cost.size(); i++) {
            sum += cost[i];
        }

        cout << "MST: " << sum << endl;


    }


    void dijkstra_vector(int start) 
    {

        vector<double> dist(numberVertex+1, numeric_limits<double>::infinity());
        vector<double> cost(numberVertex+1, 0.0);
        list<int> explored, found;

        double sum = 0.0f;
        int count = -1;

        explored.push_back(start);
        found.push_back(start);

        dist[start] = 0;
        cost[start] = 0;
        
        while (explored.size() != dist.size())
        {

            for (const auto& v : graphLL[start]) 
            {

                if (std::find(explored.begin(), explored.end(), get<0>(v) ) == explored.end()) 
                {
                    found.push_back(get<0>(v));
                }
                
                if (dist[get<0>(v)] > (dist[start] + get<1>(v))) 
                {
                    dist[get<0>(v)] = dist[start] + get<1>(v);
                }
                
                if (cost[get<0>(v)] >= get<1>(v)) 
                {
                    cost[get<0>(v)] = get<1>(v);

                } 

            }
   
            start = found.front();
            explored.push_back(found.front());
            found.pop_front();

        }

        
        cout << "__________________________________" << endl;
        cout << "|" << "Vertice" << "  |  " << "Distancia do vertice"  << "|" << endl;
        for(const auto& i: dist) {

            count++;
                
            cout << "       " <<  count <<  "  |   " <<  i << "                    |" << endl;
            
        }

        for(int i = 1; i < cost.size(); i++) {
            sum += cost[i];
        }
        cout << "MST: " << sum << endl;

    }

};



int main() {



    Graph u("grafo_W_1_1.txt"); 

	u.graphBuilderAdjacencyVector("grafo_W_1_1.txt");

    clock_t start, end;
    double cpu_time_used;


    cout << "Dijkstra with heap" << endl;
    start = clock();

    u.dijkstra(10);
    u.dijkstra_vector(10);

    end = clock();
    cpu_time_used = ((double) (end-start)) / CLOCKS_PER_SEC;

    cout << "Tempo gasto: " << cpu_time_used << '\n' << endl;

    
    return 0;

}