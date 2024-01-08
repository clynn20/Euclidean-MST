#include<iostream>
#include<cstdlib>
#include<cmath>
#include<algorithm>
#include<fstream>

using namespace std;

struct Vertex{
	int x;
	int y;
	int num; // assign each vertex a number 
};

struct Edge{
	int src; // the assigned number of the source vertex
	int des; // the assigned number of the destination vertex
	int wt;  // the euclidean distance between source vertex and destination vertex
};

struct Graph{
	int V; // number of vertex in graph
	int E; // number of edge in graph
	Edge* edge; 
};

struct Subset{
	int parent;
	int rank;
};

// euclidean distance between vertex(x1,y1) and vertex(x2,y2)
int dist(int x1,int y1,int x2,int y2){
	return round(sqrt((x1-x2)*(x1-x2) +(y1-y2)*(y1-y2)));
}
// create graph
Graph* create_graph(int V, int E){
	Graph* graph = new Graph;
	graph -> V = V; 
	graph -> E = E;
	graph -> edge = new Edge[E];
	return graph;
}

// fill the edge src, des and wt
void fill_edge_info(Graph* graph,Vertex* v_arr){
	int i =0;// index for edge array
	int V = graph->V;
	for(int r=0;r<V;r++){
		for(int c=0;c<V;c++){
			if(r<c){
				// use a 1D edge array to store the info
				// from low assigned# to high assigned#
				graph->edge[i].src = v_arr[r].num;
				graph->edge[i].des = v_arr[c].num;
				graph->edge[i].wt = dist(v_arr[r].x,v_arr[r].y,v_arr[c].x,v_arr[c].y);
				i++;
			}
		}
	}
}

//Path compression, find set of an element i 
int find(Subset* subsets,int i){
	// find root and make root as parent of i 
	if(subsets[i].parent!=i){
		subsets[i].parent = find(subsets,subsets[i].parent);
	}
	return subsets[i].parent;
}

// Union by rank, union the two sets of a and b 
void Union(Subset* subsets,int a,int b){
	int a_root = find(subsets,a);
	int b_root = find(subsets,b);

	// attach the smaller rank tree under root of high rank tree
	if(subsets[a_root].rank < subsets[b_root].rank){
		// make b as parent of a
		subsets[a_root].parent = b_root;
	}
	else if(subsets[a_root].rank > subsets[b_root].rank){
		// make a as parent of b 
		subsets[b_root].parent = a_root;
	}
	else{
		// make any tree child of other tree
		subsets[b_root].parent = a_root;
		// increase the rank
		subsets[a_root].rank++;
	}
}

// merge arr function 
void merge_arr(Edge* array,int left,int mid,int right){
	int n1 = mid-left+1;
	int n2 = right-mid;
	Edge left_arr[n1];
	Edge right_arr[n2];
	for(int i=0;i<n1;i++){
		left_arr[i] = array[left+i];
	}
	for(int j=0;j<n2;j++){
		right_arr[j]=array[mid+1+j];
	}
	int i=0;
	int j=0;
	int k=left;
	while(i<n1 && j<n2){
		if(left_arr[i].wt<=right_arr[j].wt){
			array[k]=left_arr[i];
			i++;
		}
		else{
			array[k]=right_arr[j];
			j++;
		}
		k++;
	}
	while(i<n1){
		array[k]=left_arr[i];
		i++;
		k++;
	}
	while(j<n2){
		array[k]=right_arr[j];
		j++;
		k++;
	}
}
//mergesort from hw1
void mergesort(Edge* array,int left,int right){
	if(left<right){
		int mid = left+(right-left)/2;
		mergesort(array,left,mid);
		mergesort(array,mid+1,right);
		merge_arr(array,left,mid,right);
	}
}

// Kruskal algorithm to find the mst
void kruskal(Graph* graph){
	int e = 0;//index used for taken edge 
	int i = 0;//index used for graph sorted edge 
	Edge taken[graph->V-1];
	// sort the edge in increasing order by edge's weight
	mergesort(graph->edge,0,graph->E-1);
	// create V subsets
	Subset* subsets = new Subset[(graph->V)];
	// make set
	for(int v=0;v < graph->V;v++){
		subsets[v].parent = v;
		subsets[v].rank = 0;
	}
	// mst takes V-1 edge 
	while(e < (graph->V)-1 && i < graph->E){
		Edge next_edge = graph->edge[i];
		i++;
		int a = find(subsets,next_edge.src);
		int b = find(subsets,next_edge.des);
		// a and b are not in the same set, i.e. no cycle detected
		 if(a!=b){
			taken[e] = next_edge;
			Union(subsets,a,b);
			e++;
		}
	}
	
	cout << "Edge in Mst" << endl;
	int cost =0;
	for(int p=0;p<e;p++){
		cout << "v#"<<taken[p].src << "--" << "v#" << taken[p].des << " distance: " << taken[p].wt << endl;
		cost = cost + taken[p].wt;
	}
	cout << "	Mst total distance " << cost << endl;
	cout << endl;

}
// print the vertex array 
void print_v_arr(Vertex* v_arr,int V){	
	cout <<" vertex info check" << endl;
	for(int k=0;k<V;k++){
		cout << v_arr[k].x << " "<< v_arr[k].y << "  #" << v_arr[k].num << endl;
	}
}
// function to print the edge array
void print_e_arr(Graph* graph,int E){
	cout << "edge info check before sort" << endl;
	for(int k=0;k<E;k++){
		cout << "v#" << graph->edge[k].src << " to v#" <<graph->edge[k].des <<" dist:" << graph->edge[k].wt << endl;
	}
}

int main(){
	fstream infile;
	infile.open("graph.txt");
	int V,E,cases;
	if(infile.is_open()){
			infile >> cases;
			for(int i=0;i<cases;i++){
				infile >> V;	
				E = (V*(V-1))/2;
				Vertex* v_arr = new Vertex [V];
				Graph* graph = create_graph(V,E);
				// read in vertices info
				for(int r=0;r<V;r++){
					infile >> v_arr[r].x;
					infile >> v_arr[r].y;
					v_arr[r].num = r;
				}
				//print_v_arr(v_arr,V); for check only
				fill_edge_info(graph,v_arr);
				//print_e_arr(graph,E); for check only
				cout << "Test cases: " << i+1 << endl;
				kruskal(graph);
			
			}

		infile.close();
	}
	else{
		cout <<"Cannot open graph.txt" << endl;
	}

	return 0;
}
