#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class NodeLinkedList {

    public:
        int data_int;
        float data_float;
        NodeLinkedList* next_node;

        NodeLinkedList(int, float);
};

NodeLinkedList::NodeLinkedList(int new_data_int = 0, float new_data_float = 0.0f) {
    
    this->data_int = new_data_int;
    this->data_float = new_data_float;
    this->next_node = nullptr;
}

class LinkedList {
    
    public:
        NodeLinkedList* head_LinkedList;
        int size_LinkedList;

        LinkedList();
        void insert(int, float);
};

LinkedList::LinkedList() {

    head_LinkedList = nullptr;
    size_LinkedList = 0;
}

void LinkedList::insert(int new_data_int = 0, float new_data_float = 0.0f) {

    if (head_LinkedList == nullptr) {

        head_LinkedList = new NodeLinkedList(new_data_int, new_data_float);
    } else {

        NodeLinkedList* new_node = new NodeLinkedList(new_data_int, new_data_float);
        new_node->next_node = head_LinkedList;
        this->head_LinkedList = new_node;
    }
    size_LinkedList++;
}

class NodeBinaryHeap {

    public:
        float data_float;
        int data_int;
        NodeBinaryHeap(int, float);
};

NodeBinaryHeap::NodeBinaryHeap(int new_data_int = 0, float new_data_float = 0.0f) {

    this->data_int = new_data_int;
    this->data_float = new_data_float;
}

class BinaryHeap {
    
    public:
        vector<NodeBinaryHeap> heap_array;
        vector<NodeBinaryHeap*> auxiliar_pointers;
        int currrent_heap_size;
        int maximum_heap_size;

        int parent(int i) {return (i-1)/2;}
        int left_child(int i) {return (2*i + 1);}
        int right_child(int i) {return (2*i + 2);}

        BinaryHeap(int);
        void swap_heap_array(NodeBinaryHeap*, NodeBinaryHeap*);
        void swap_auxiliar_pointers(NodeBinaryHeap*&, NodeBinaryHeap*&);
        void heapify_up(int);
        void heapify_down(int);
        void insert(float, int);
        int extract_min();
};

BinaryHeap::BinaryHeap(int new_maximum) { 

    currrent_heap_size = 0;
    maximum_heap_size = new_maximum;
    heap_array.resize(new_maximum);
    auxiliar_pointers.resize(new_maximum);
}

void BinaryHeap::swap_heap_array(NodeBinaryHeap *x, NodeBinaryHeap *y) {

    NodeBinaryHeap temp = *x;
    *x = *y;
    *y = temp;
}

void BinaryHeap::swap_auxiliar_pointers(NodeBinaryHeap *&x, NodeBinaryHeap *&y) {

    NodeBinaryHeap* temp = x;
    x = y;
    y = temp;
}

void BinaryHeap::insert(float key, int value) {

    if (currrent_heap_size == maximum_heap_size) {

        cout << "\nOverflow: Could not insert key\n";
        return;
    }

    int index = currrent_heap_size;
    currrent_heap_size++;
    heap_array[index].data_float = key;
    heap_array[index].data_int = value;
    auxiliar_pointers[value-1] = &heap_array[index];

    while ((index != 0) && (heap_array[parent(index)].data_float > heap_array[index].data_float)) {

        swap_heap_array(&heap_array[index], &heap_array[parent(index)]);
        swap_auxiliar_pointers(auxiliar_pointers[index], auxiliar_pointers[parent(index)]);
        index = parent(index);
    }
}

void BinaryHeap::heapify_up(int i) {

    while (i != 0 && heap_array[parent(i)].data_float > heap_array[i].data_float) {

       swap_heap_array(&heap_array[i], &heap_array[parent(i)]);
       swap_auxiliar_pointers(auxiliar_pointers[i], auxiliar_pointers[parent(i)]);
       i = parent(i);
    }
}

void BinaryHeap::heapify_down(int index = 0) {

    bool done = false;
    int new_root = 0;

    while(!done) {

        new_root = index;

        if ((left_child(index) < currrent_heap_size) && (heap_array[left_child(index)].data_float < heap_array[index].data_float)) {

            new_root = left_child(index);
        }
        if ((right_child(index) < currrent_heap_size) && (heap_array[right_child(index)].data_float < heap_array[new_root].data_float)) {

            new_root = right_child(index);
        }
        if (new_root != index) {

            swap_heap_array(&heap_array[new_root], &heap_array[index]);
            swap_auxiliar_pointers(auxiliar_pointers[new_root], auxiliar_pointers[index]);
            index = new_root;
        } else {done = true;}
    }
}

int BinaryHeap::extract_min() { 

    if (currrent_heap_size <= 0) {return -1;}
    else if (currrent_heap_size == 1) {

        currrent_heap_size--;
        return heap_array[0].data_int;
    } 
    else {
    
        int root = heap_array[0].data_int;
        heap_array[0] = heap_array[currrent_heap_size-1];
        heap_array.pop_back();
        currrent_heap_size--;
        auxiliar_pointers[root] = nullptr;
        heapify_down();

        return root;
    }
}

class Graph {

    public:
        int number_vertex;
        vector<LinkedList> graph_representation;
        vector<int> parent;

        Graph();
        void swap_vector(NodeBinaryHeap *x, NodeBinaryHeap *y);
        void swap_auxiliar_pointers(NodeBinaryHeap *&, NodeBinaryHeap *&);
        void graph_builder_adjacency_list(string, bool);
        void dijkstra_heap(int);
        void dijkstra_vector(int);
        void prim_heap(int);
};

Graph::Graph() {

    number_vertex = 0;
}


void Graph::swap_vector(NodeBinaryHeap *x, NodeBinaryHeap *y) { // O(1)

    NodeBinaryHeap temp = *x;
    *x = *y;
    *y = temp;
}

void Graph::swap_auxiliar_pointers(NodeBinaryHeap *&x, NodeBinaryHeap *&y) { // O(1)

    NodeBinaryHeap* temp = x;
    x = y;
    y = temp;
}

void Graph::graph_builder_adjacency_list(string file_name, bool graph_weight = false) { // O(n+m)

    int vertex, adjancent_vertex;
    float edge_weight;
    ifstream infile(file_name);
    infile >> number_vertex;
    graph_representation.resize(number_vertex); // O(n)

    if (graph_weight == true) {

        while(infile >> vertex >> adjancent_vertex >> edge_weight) { // O(m)

            graph_representation[vertex-1].insert(adjancent_vertex, edge_weight);
            graph_representation[adjancent_vertex-1].insert(vertex, edge_weight);
        }
    } else if (graph_weight == false) {

        while(infile >> vertex >> adjancent_vertex) { // O(m)

            graph_representation[vertex-1].insert(adjancent_vertex);
            graph_representation[adjancent_vertex-1].insert(vertex);
        }
    }
    
    infile.close();
}

void Graph::dijkstra_vector(int start_vertex) {
    
    vector<float> distance_vector(number_vertex, 1000000000000.0f);
    vector<int> parent_vector(number_vertex, 0);
    LinkedList explored_set;
    vector<NodeBinaryHeap> discovered_set;
    vector<NodeBinaryHeap*> auxiliar_pointers (number_vertex);
    NodeLinkedList* adjancent_vertex;
    int next_explored_vertex, index_min;

    distance_vector[start_vertex-1] = 0.0f;
    discovered_set.push_back(NodeBinaryHeap(start_vertex, 0.0f));
    auxiliar_pointers[start_vertex-1] = &discovered_set[discovered_set.size()-1];
    
    while(explored_set.size_LinkedList != number_vertex) {

        index_min = 0;
        
        for (int i = 0; i < discovered_set.size(); i++) {

            if (discovered_set[index_min].data_float > discovered_set[i].data_float) {

                index_min = i;
            }
        }
             
        next_explored_vertex = discovered_set[index_min].data_int;
        explored_set.insert(next_explored_vertex, distance_vector[next_explored_vertex-1]);
        swap_vector(&discovered_set[index_min], &discovered_set[discovered_set.size()-1]);
        swap_auxiliar_pointers(auxiliar_pointers[index_min], auxiliar_pointers[discovered_set.size()-1]);
        auxiliar_pointers[discovered_set.size()-1] = nullptr;
        discovered_set.pop_back();
        
        auxiliar_pointers[index_min] = nullptr;
        adjancent_vertex = graph_representation[next_explored_vertex-1].head_LinkedList;
        
        for (int i = 0; i < graph_representation[next_explored_vertex-1].size_LinkedList; i++) {
            
            if (adjancent_vertex->data_float < 0.0f) {

                cout << "The library still doesn't implement shortest paths with negative weights.";
                return;
            }
            
            else if (distance_vector[adjancent_vertex->data_int-1] > distance_vector[next_explored_vertex-1] + adjancent_vertex->data_float) {
                
                distance_vector[adjancent_vertex->data_int-1] = distance_vector[next_explored_vertex-1] + adjancent_vertex->data_float;
                parent_vector[adjancent_vertex->data_int-1] = next_explored_vertex;

                if (auxiliar_pointers[adjancent_vertex->data_int-1] == nullptr) {

                    discovered_set.push_back(NodeBinaryHeap(adjancent_vertex->data_int, distance_vector[adjancent_vertex->data_int-1]));
                    auxiliar_pointers[adjancent_vertex->data_int-1] = &discovered_set[discovered_set.size()-1];
                } else {

                    auxiliar_pointers[adjancent_vertex->data_int-1]->data_float = distance_vector[next_explored_vertex-1] + adjancent_vertex->data_float;
                }
            }

            adjancent_vertex = adjancent_vertex->next_node;
        }
    }
    
    cout << distance_vector[9] << ", " << distance_vector[19] << ", " << distance_vector[29]<< ", " << distance_vector[39]<< ", " << distance_vector[49]<< ", " << distance_vector[59] << '\n';

    int count = 1;

    
    for(const auto& i: distance_vector) {

        cout << "(" << count << ", " << parent_vector[count] << ", " << i << ")  \n";
        count++;
    }
}

void Graph::dijkstra_heap(int start_vertex) {
    
    vector<float> distance_vector(number_vertex, 1000000000000.0f);
    vector<int> parent_vector(number_vertex, 0);
    LinkedList explored_set;
    BinaryHeap discovered_set(number_vertex);
    NodeLinkedList* adjancent_vertex;
    int next_explored_vertex;
    
    distance_vector[start_vertex-1] = 0.0f;
    discovered_set.insert(0.0f, start_vertex);


    while(explored_set.size_LinkedList != number_vertex) {
        
        next_explored_vertex = discovered_set.extract_min();
        explored_set.insert(next_explored_vertex, distance_vector[next_explored_vertex-1]);
        adjancent_vertex = graph_representation[next_explored_vertex-1].head_LinkedList;

        for (int i = 0; i < graph_representation[next_explored_vertex-1].size_LinkedList; i++) {

            if (adjancent_vertex->data_float < 0.0f) {

                cout << "The library still doesn't implement shortest paths with negative weights.";
                return;
            }
            
            else if (distance_vector[adjancent_vertex->data_int-1] > distance_vector[next_explored_vertex-1] + adjancent_vertex->data_float) {
                
                distance_vector[adjancent_vertex->data_int-1] = distance_vector[next_explored_vertex-1] + adjancent_vertex->data_float;
                parent_vector[adjancent_vertex->data_int-1] = next_explored_vertex;

                if (discovered_set.auxiliar_pointers[adjancent_vertex->data_int-1] == nullptr) {

                    discovered_set.insert(distance_vector[adjancent_vertex->data_int-1], adjancent_vertex->data_int);
                } else {

                    discovered_set.auxiliar_pointers[adjancent_vertex->data_int-1]->data_float = distance_vector[next_explored_vertex-1] + adjancent_vertex->data_float;
                    discovered_set.heapify_up(&*discovered_set.auxiliar_pointers[adjancent_vertex->data_int-1] - &discovered_set.heap_array[0]);
                }
            }

            adjancent_vertex = adjancent_vertex->next_node;
        }
    }
    
    cout << distance_vector[9] << ", " << distance_vector[19] << ", " << distance_vector[29]<< ", " << distance_vector[39]<< ", " << distance_vector[49]<< ", " << distance_vector[59] << '\n';
    
    int count = 1;
    
    
    for(const auto& i: distance_vector) {

        cout << "(" << count << ", " << parent_vector[count-1] << ", " << i << ")  \n";
        count++;
    }
}

void Graph::prim_heap(int start_vertex) {
    
    vector<float> distance_vector(number_vertex, 1000000000000.0f);
    vector<int> parent_vector(number_vertex, 0);
    LinkedList explored_set;
    BinaryHeap discovered_set(number_vertex);
    NodeLinkedList* adjancent_vertex;
    int next_explored_vertex;
    ofstream mst_file ("MST.txt");

    distance_vector[start_vertex-1] = 0.0f;
    discovered_set.insert(0.0f, start_vertex);
    mst_file << number_vertex << "\n";

    while(explored_set.size_LinkedList != number_vertex) {
        
        distance_vector[discovered_set.heap_array[0].data_int-1] = discovered_set.heap_array[0].data_float;
        next_explored_vertex = discovered_set.extract_min();
        explored_set.insert(next_explored_vertex, distance_vector[next_explored_vertex-1]);
        adjancent_vertex = graph_representation[next_explored_vertex-1].head_LinkedList;

        for (int i = 0; i < graph_representation[next_explored_vertex-1].size_LinkedList; i++) {

            if (adjancent_vertex->data_float < 0.0f) {

                cout << "The library still doesn't implement shortest paths with negative weights.";
                return;
            } else if (discovered_set.auxiliar_pointers[adjancent_vertex->data_int-1] == nullptr) {

                discovered_set.insert(distance_vector[adjancent_vertex->data_int-1], adjancent_vertex->data_int);
                parent_vector[adjancent_vertex->data_int-1] = next_explored_vertex;
            } else {

                discovered_set.auxiliar_pointers[adjancent_vertex->data_int-1]->data_float = adjancent_vertex->data_float;
                discovered_set.heapify_up(&*discovered_set.auxiliar_pointers[adjancent_vertex->data_int-1] - &discovered_set.heap_array[0]);
                parent_vector[adjancent_vertex->data_int-1] = next_explored_vertex;
            }

            adjancent_vertex = adjancent_vertex->next_node;
        }
        
        if (mst_file.is_open()) {
            mst_file << next_explored_vertex << " " << discovered_set.heap_array[0].data_int << " " << discovered_set.heap_array[0].data_float << "\n";
        }
    }
    float total_weight = 0.0f;
    for (const auto &i: distance_vector) {
        total_weight += i;
    }
    mst_file << total_weight;
    mst_file.close();

}

int main() {
    
    Graph test;
    clock_t start, end;
    double cpu_time_used;
    /*
    test.graph_builder_adjacency_list("grafo_W_1_1.txt", true);
    test.dijkstra_heap(10);
    test.graph_builder_adjacency_list("grafo_W_2_1.txt", true);
    test.dijkstra_heap(10);
    test.graph_builder_adjacency_list("grafo_W_3_1.txt", true);
    test.dijkstra_heap(10);
    test.graph_builder_adjacency_list("grafo_W_4_1.txt", true);
    test.dijkstra_heap(10);
    test.graph_builder_adjacency_list("grafo_W_5_1.txt", true);
    test.dijkstra_heap(10);
    */

    test.graph_builder_adjacency_list("grafo_W_1_1.txt", true);
    test.dijkstra_vector(10);
    /*
    test.graph_builder_adjacency_list("grafo_W_2_1.txt", true);
    test.dijkstra_vector(10);
    test.graph_builder_adjacency_list("grafo_W_3_1.txt", true);
    test.dijkstra_vector(10);
    test.graph_builder_adjacency_list("grafo_W_4_1.txt", true);
    test.dijkstra_vector(10);
    test.graph_builder_adjacency_list("grafo_W_5_1.txt", true);
    test.dijkstra_vector(10);
    */


    start = clock();
    
    end = clock();
    cpu_time_used = ((double) (end-start)) / CLOCKS_PER_SEC;

    cout << "Tempo gasto: " << cpu_time_used << '\n';

    return 0;
}