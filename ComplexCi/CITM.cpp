//Algorithm of CI-TM in [Sen Pei, Xian Teng, Jeffrey Shaman, Flaviano Morone, and Hernán A. Makse. "Efficient collective influence maximization in cascading processes with first-order transitions." Scientific Reports 7,45240 (2017).]

	////////////////////////////////
#include<stdio.h>
#include<stdlib.h>
#include<cmath>
#include<time.h>
	//////////////////////////////////////


namespace citm
{

#define MAX_DEGREE 1000000
#define FILENAME_LENGHT 10000

	typedef long int int_t;

	typedef enum{ OFF, ON } off_on;
	typedef enum{ OUT, IN } out_in;

	//Node struct
	typedef struct{
		out_in n;//IN: remaining; OUT: removed
		off_on v;//OFF: uninfected; ON: infected
		double m;//threshold
		int deg, compNum;//degree
	}varNode;

	//functions for heap

	//Heap struct
	typedef struct{
		int VAL;
		int id;
	} vertex;
	typedef struct{
		int num_data;
		vertex *node;
	} Heap;

	//heapify
	void heapify(Heap *Heap_ptr, int index, int *heap_pos) {
		int left, right, max_index, temp_pos;
		vertex temp_node;

		left = 2 * index;
		right = left + 1;
		//Compare sons with father
		if (left < Heap_ptr->num_data) {
			if (Heap_ptr->node[left].VAL > Heap_ptr->node[index].VAL) {
				max_index = left;
			}
			else {
				max_index = index;
			}
			if (right < Heap_ptr->num_data) {
				if (Heap_ptr->node[right].VAL > Heap_ptr->node[max_index].VAL) {
					max_index = right;
				}
			}
			//IF necessary swap father and son
			if (max_index != index) {
				temp_node = Heap_ptr->node[index];
				temp_pos = heap_pos[Heap_ptr->node[index].id];

				heap_pos[Heap_ptr->node[index].id] = max_index;
				heap_pos[Heap_ptr->node[max_index].id] = temp_pos;

				Heap_ptr->node[index] = Heap_ptr->node[max_index];
				Heap_ptr->node[max_index] = temp_node;

				heapify(Heap_ptr, max_index, heap_pos);
			}
		}
	}

	void build_heap(Heap *Heap_ptr, int *heap_pos) {
		int i;
		//Build the heap starting with the father of the last element	
		for (i = (int)(Heap_ptr->num_data / 2); i > 0; --i)
			heapify(Heap_ptr, i, heap_pos);
	}
	//CITM
	/////////////////////////

	//get CITM for node i
	double get_CITM(int i, varNode *Node, int N, int_t **Graph, int L, int *queue, int *check, int *root) {
		int r, e, k, neigh, deg, node, rnode, qlength;
		double CITM;
		int s;
		CITM = Node[i].deg;

		if (Node[i].deg == 0 || L == 0) { return CITM; }
		else {
			qlength = 0;
			queue[0] = i;
			check[i] = ON;
			root[i] = 0;
			r = 0;
			qlength += 1;
			e = qlength;
			deg = Graph[i][0];
			for (k = 1; k <= deg; k++){
				neigh = Graph[i][k];
				if ((Node[neigh].n == IN) && (check[neigh] == OFF)){
					queue[qlength] = neigh;
					qlength += 1;
					check[neigh] = ON;
					root[neigh] = i;
				}
			}
			r = e;
			e = qlength;
			s = 1;
			while (r != qlength) {
				if (s <= L) {
					node = queue[r];
					rnode = root[node];
					deg = Graph[node][0];
					if (Node[node].m == 1){
						CITM += Node[node].deg - Node[node].m;
						for (k = 1; k <= deg; k++){
							neigh = Graph[node][k];
							if ((Node[neigh].n == IN) && (check[neigh] == OFF) && (Node[neigh].v == OFF)){
								queue[qlength] = neigh;
								qlength += 1;
								check[neigh] = ON;
								root[neigh] = node;
							}
						}
					}
				}
				r += 1;
				if (r == e) {
					s += 1;
					e = qlength;
				}
			}
			for (k = 0; k < qlength; k++){
				check[queue[k]] = OFF;
			}
			return CITM;
		}
	}

	//update Node after removal of i, inode records infected nodes
	void updateNode_CITM(int i, varNode *Node, int N, int_t **Graph, int L, int *queue, int *check, int *root, int *inode) {
		int r, e, k, neigh, deg, node, rnode, qlength, ilength;
		double CITM;
		int s;
		CITM = Node[i].deg;
		Node[i].v = ON;
		qlength = 0;
		queue[0] = i;
		check[i] = ON;
		root[i] = 0;
		r = 0;
		qlength += 1;
		e = qlength;
		inode[0] = i;
		ilength = 1;
		deg = Graph[i][0];
		for (k = 1; k <= deg; k++){
			neigh = Graph[i][k];
			if ((Node[neigh].n == IN) && (check[neigh] == OFF)){
				queue[qlength] = neigh;
				qlength += 1;
				check[neigh] = ON;
				root[neigh] = i;
			}
		}
		r = e;
		e = qlength;
		s = 1;
		while (r != qlength) {
			if (s <= L) {
				node = queue[r];
				rnode = root[node];
				deg = Graph[node][0];
				if (Node[node].m == 1){
					//node is infected
					Node[node].v = ON;
					inode[ilength] = node;
					ilength += 1;
					CITM += Node[node].deg - Node[node].m;
					for (k = 1; k <= deg; k++){
						neigh = Graph[node][k];
						if ((Node[neigh].n == IN) && (check[neigh] == OFF) && (Node[neigh].v == OFF)){
							queue[qlength] = neigh;
							qlength += 1;
							check[neigh] = ON;
							root[neigh] = node;
						}
					}
				}
			}
			r += 1;
			if (r == e) {
				s += 1;
				e = qlength;
			}
		}
		for (k = 0; k < qlength; k++){
			check[queue[k]] = OFF;
		}
		inode[0] = ilength - 1; // number of infected nodes
	}

	//get list of nodes to update and store them in queue[]
	void get_listNodeToUpdate_CITM(int i, varNode *Node, int N, int_t **Graph, int L, int *queue, int *check, int *inode){
		int r, e, k, neigh, deg, node, qlength, numinode;
		int s;
		numinode = inode[0];// number of infected nodes
		queue[0] = i;
		qlength = 1;
		r = 1;
		for (k = 1; k <= numinode; k++) {
			node = inode[k];
			queue[qlength] = node;
			qlength += 1;
			check[node] = ON;
		}
		e = qlength;
		s = 0;
		while (r != qlength){
			if (s <= L) {
				node = queue[r];
				deg = Graph[node][0];
				for (k = 1; k <= deg; k++) {
					neigh = Graph[node][k];
					if ((Node[neigh].n == IN) && (check[neigh] == OFF)) {
						queue[qlength] = neigh;
						qlength += 1;
						check[neigh] = ON;
					}
				}
			}
			r += 1;
			if (r == e) {
				s += 1;
				e = qlength;
			}
		}
		for (k = 0; k < qlength; k++){
			check[queue[k]] = OFF;
		}
		queue[0] = qlength - 1;//number of nodes to be updated
	}

	//get influencers and corresponding CITM values
	int **get_influencers_CITM(varNode *Node, int N, int_t **Graph, double *threshold, int L) {
		int i, j, cnt, cnt1, toBeRemoved, currentNode, pos_currentNode, NumNodesToUpdate;
		int *heap_pos, *queue, *check, *root, *inode, *listNodeToUpdate;
		int **listInfluencers;
		Heap heap;

		queue = (int *)calloc(N + 1, sizeof(int));
		check = (int *)calloc(N + 1, sizeof(int));
		root = (int *)calloc(N + 1, sizeof(int));
		inode = (int *)calloc(N + 1, sizeof(int));
		listNodeToUpdate = (int *)calloc(N + 1, sizeof(int));

		listInfluencers = (int **)calloc(N + 1, sizeof(int *));
		heap.num_data = (N + 1);
		heap.node = (vertex *)calloc(heap.num_data, sizeof(vertex));
		heap_pos = (int *)calloc(heap.num_data, sizeof(int));

		//Init Node variables
		for (i = 1; i <= N; i++) {
			Node[i].n = IN;
			Node[i].v = OFF;
			Node[i].deg = Graph[i][0];
			Node[i].m = threshold[i];
			listInfluencers[i] = (int *)calloc(3, sizeof(int));
		}

		for (i = 1; i <= N; i++) {
			heap.node[i].VAL = get_CITM(i, Node, N, Graph, L, queue, check, root);
			heap.node[i].id = i;
			heap_pos[i] = i;
		}
		build_heap(&heap, heap_pos);

		cnt = 1;
		cnt1 = N;
		while (heap.num_data > 1) {
			toBeRemoved = heap.node[1].id;
			listInfluencers[cnt][1] = toBeRemoved;
			listInfluencers[cnt][2] = heap.node[1].VAL;

			//update
			updateNode_CITM(toBeRemoved, Node, N, Graph, L, queue, check, root, inode);
			Node[toBeRemoved].n = OUT;
			for (i = 1; i <= Graph[toBeRemoved][0]; i++){
				int neigh = Graph[toBeRemoved][i];
				Node[neigh].deg--;
				Node[neigh].m = Node[neigh].m - 1;
			}
			//Swap first and last
			heap_pos[heap.node[heap.num_data - 1].id] = 1;
			heap_pos[heap.node[1].id] = heap.num_data;
			heap.node[1] = heap.node[heap.num_data - 1];
			heap.num_data--;
			heapify(&heap, 1, heap_pos);

			for (i = 1; i <= inode[0]; i++) {
				Node[inode[i]].n = OUT;
				for (j = 1; j <= Graph[inode[i]][0]; j++) {
					int neigh = Graph[inode[i]][j];
					Node[neigh].deg--;
					Node[neigh].m = Node[neigh].m - 1;
				}
				int pos = heap_pos[inode[i]];
				heap_pos[heap.node[heap.num_data - 1].id] = pos;
				heap_pos[heap.node[pos].id] = heap.num_data;
				heap.node[pos] = heap.node[heap.num_data - 1];
				heap.num_data--;
				listInfluencers[cnt1][1] = inode[i];
				listInfluencers[cnt1][2] = 0;
				cnt1--;
				heapify(&heap, pos, heap_pos);
			}

			//Find nodes to recalculate
			get_listNodeToUpdate_CITM(toBeRemoved, Node, N, Graph, L + 1, listNodeToUpdate, check, inode);
			NumNodesToUpdate = listNodeToUpdate[0];

			//Recalculate CITM for relevant nodes
			for (j = 1; j <= NumNodesToUpdate; j++) {
				currentNode = listNodeToUpdate[j];
				pos_currentNode = heap_pos[currentNode];
				heap.node[pos_currentNode].VAL = get_CITM(currentNode, Node, N, Graph, L, queue, check, root);
				heapify(&heap, pos_currentNode, heap_pos);
			}
			cnt++;
		}

		free(queue);
		free(check);
		free(root);
		free(inode);
		free(listNodeToUpdate);
		free(heap.node);
		free(heap_pos);
		return listInfluencers;
	}
	//Generate ER networks
	int **generateERnet(int N, double p){
		int i, j;
		int node1, node2, existlink, flag;
		int link = (int)(N*p*(N - 1) / 2), avedeg = (int)((N - 1)*p);
		int **tempnet = (int **)calloc(N, sizeof(int *));
		int *cnt = (int *)calloc(N, sizeof(int));
		for (i = 0; i<N; i++){
			tempnet[i] = (int *)calloc(20 * avedeg, sizeof(int));
			cnt[i] = 0;
		}
		for (i = 0; i<link; i++){
			existlink = 0;
			flag = 0;
			while (flag != 1){
				node1 = rand() % N;
				node2 = rand() % N;
				existlink = 0;
				flag = 0;
				for (j = 0; j<cnt[node1]; j++){
					if (tempnet[node1][j] == node2){
						existlink = 1;
						break;
					}
				}
				if (existlink != 1 && node1 != node2){
					tempnet[node1][cnt[node1]] = node2;
					tempnet[node2][cnt[node2]] = node1;
					cnt[node1]++;
					cnt[node2]++;
					flag = 1;
				}
			}
		}

		int **Graph = (int **)calloc(N + 1, sizeof(int *));
		Graph[0] = (int *)calloc(1, sizeof(int));
		Graph[0][0] = N;
		for (i = 0; i<N; i++){
			Graph[i + 1] = (int *)calloc(cnt[i] + 1, sizeof(int));
			Graph[i + 1][0] = cnt[i];
			for (j = 0; j<cnt[i]; j++){
				Graph[i + 1][j + 1] = tempnet[i][j] + 1;
			}
		}

		free(tempnet);
		free(cnt);
		return Graph;
	}

	//get adjacency list
	int_t **makeRandomGraph(const char *NETWORK, int_t N) {
		int_t i, j, node;
		int_t *deg, **adj_list;
		int n;
		char line[MAX_DEGREE], *start;
		FILE *list;

		deg = (int_t *)calloc(N + 1, sizeof(int_t));
		adj_list = (int_t **)calloc(N + 1, sizeof(int_t *));
		adj_list[0] = (int_t *)calloc(1, sizeof(int_t));
		//count number of neighbours
		list = fopen(NETWORK, "r");
		i = 1;
		while (fgets(line, MAX_DEGREE, list) != NULL) {
			start = line;
			while (sscanf(start, "%ld%n", &node, &n) == 1) {
				start += n;
				deg[i]++;
			}
			adj_list[i] = (int_t *)calloc(deg[i], sizeof(long int));
			deg[i]--;
			i++;
		}
		fclose(list);
		list = fopen(NETWORK, "r");
		i = 1;
		//fill adjacency list
		while (fgets(line, MAX_DEGREE, list) != NULL) {
			start = line;
			j = 0;
			while (sscanf(start, "%ld%n", &node, &n) == 1) {
				start += n;
				adj_list[i][j] = node;
				j++;
			}
			adj_list[i][0] = deg[i];
			i++;
		}
		fclose(list);
		free(deg);
		return adj_list;
	}


	//get number of nodes from file
	int_t get_num_nodes(const char *NETWORK) {
		int_t node, N;
		int n;
		char line[MAX_DEGREE], *start;
		FILE *list;
		list = fopen(NETWORK, "r");
		N = 0;
		while (fgets(line, MAX_DEGREE, list) != NULL) {
			start = line;
			while (sscanf(start, "%ld%n", &node, &n) == 1) {
				start += n;
			}
			N++;
		}
		fclose(list);
		return N;
	}


	/*
	int main() {
		srand((unsigned int)time(NULL));
		int N = 100;//number of nodes
		double p = 5.0 / N;//link probability
		int **Graph = generateERnet(N, p);//create an ER network
		varNode *Node;
		int **listInfluencers;
		int *threshold;
		threshold = (int *)calloc(N + 1, sizeof(int));
		for (int i = 1; i <= N; i++)
			threshold[i] = 0.5;//the fractional threshold is 0.5
		Node = (varNode *)calloc(N + 1, sizeof(varNode));
		int L = 3;//length of search in CI-TM
		listInfluencers = get_influencers_CITM(Node, N, Graph, threshold, L);
		fprintf(stdout, "Node ID\tCI-TM value\n");
		for (int i = 1; i <= N; i++)
			fprintf(stdout, "%d\t\t%d\n", listInfluencers[i][1], listInfluencers[i][2]);
		return 0;
	}
	*/	

}
