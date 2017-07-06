import networkx as nx
import csv
import time
import heapq
import os
import sys
from ci import collective_influence

def chunks(l, n, modelID):
    """Yield successive n-sized chunks from l."""
    for i in range(0, len(l), n):
        genL = l[i:i + n]
        if len(genL)<n:
            genL.extend(["" for j in range(n-len(genL))])
        genL.insert(0, modelID)

        yield genL

if __name__ == '__main__':

    if len(sys.argv) < 7:

        print("%s %s %s %s %s %s %s" % ("ComplexCiPython.py","[input file]","[output folder]","[ballRadius]","[batchNum]","[outputNum]","[methodCentrality]"))
        input()
        sys.exit(0);

    ballRadius = int(sys.argv[3]);
    batchNum = int(sys.argv[4]);
    outputNum = int(sys.argv[5]);
    methodCentrality = int(sys.argv[6]);
    sourcePath = sys.argv[1]
    modelID = os.path.basename(sourcePath).split('.')[0]
    outputPath = os.path.join(sys.argv[2] , modelID +"_method_" + str(methodCentrality) + '.csv_out')

    print("sourcePath: %s outputPath: %s ballRadius: %s batchNum: %s outputNum: %s methodCentrality: %s" % (sourcePath,outputPath,ballRadius,batchNum,outputNum, methodCentrality))
    
    start_time = time.time()

    G=nx.Graph();

    with open(sourcePath) as f:
        f_csv = csv.reader(f);
        for row in f_csv:
            G.add_edge(row[0],row[1]);

    totalNode = G.number_of_nodes()
    print("%s nodes are Loaded: " % totalNode)

    print("--- %s seconds ---" % (time.time() - start_time))

    finalResult = [];


    while True:

        if methodCentrality == 0:
            ciMap = collective_influence(G,distance=ballRadius)
        elif methodCentrality == 1:
            ciMap = nx.degree_centrality(G)
        elif methodCentrality == 2:
            ciMap = nx.eigenvector_centrality(G)
        elif methodCentrality == 3:
            ciMap = nx.eigenvector_centrality_numpy(G)
        elif methodCentrality == 4:
            ciMap = nx.katz_centrality(G)
        elif methodCentrality == 5:
            ciMap = nx.katz_centrality_numpy(G)
        elif methodCentrality == 6:
            ciMap = nx.closeness_centrality(G)
        elif methodCentrality == 7:
            ciMap = nx.current_flow_closeness_centrality(G)
        elif methodCentrality == 8:
            ciMap = nx.betweenness_centrality(G)
        elif methodCentrality == 9:
            ciMap = nx.edge_betweenness_centrality(G)
        elif methodCentrality == 10:
            ciMap = nx.betweenness_centrality_subset(G)
        elif methodCentrality == 11:
            ciMap = nx.edge_betweenness_centrality_subset(G)
        elif methodCentrality == 12:
            ciMap = nx.current_flow_betweenness_centrality(G)
        elif methodCentrality == 13:
            ciMap = nx.edge_current_flow_betweenness_centrality(G)
        elif methodCentrality == 14:
            ciMap = nx.approximate_current_flow_betweenness_centrality(G)
        elif methodCentrality == 15:
            ciMap = nx.current_flow_betweenness_centrality_subset(G)
        elif methodCentrality == 16:
            ciMap = nx.edge_current_flow_betweenness_centrality_subset(G)
        elif methodCentrality == 17:
            ciMap = nx.communicability_betweenness_centrality(G)
        elif methodCentrality == 18:
            ciMap = nx.load_centrality(G)
        elif methodCentrality == 19:
            ciMap = nx.edge_load_centrality(G)
        elif methodCentrality == 20:
            ciMap = nx.subgraph_centrality(G)
        elif methodCentrality == 21:
            ciMap = nx.subgraph_centrality_exp(G)
        elif methodCentrality == 22:
            ciMap = nx.estrada_index(G)
        elif methodCentrality == 23:
            ciMap = nx.harmonic_centrality(G)
        elif methodCentrality == 24:
            ciMap = nx.local_reaching_centrality(G)
        elif methodCentrality == 25:
            ciMap = nx.global_reaching_centrality(G)
        elif methodCentrality == 26:
            ciMap = nx.pagerank(G)
        elif methodCentrality == 27:
            ciMap = nx.pagerank_numpy(G)
        elif methodCentrality == 28:
            ciMap = nx.pagerank_scipy(G)
        else:
            print("methodCentrality %s is not valid" % methodCentrality)
            input()
            sys.exit(0);
            

        maxCiNodeIt = heapq.nlargest(batchNum, ciMap, key=ciMap.get)
        G.remove_nodes_from(maxCiNodeIt);
        finalResult.extend(maxCiNodeIt);


        #print(maxCiNodeIt);
        print("modelID: %s, Left nodes: %s, Total nodes: %s, MaxCiValue: %s, MaxCiNode: %s" % (modelID, G.number_of_nodes(), totalNode, ciMap[maxCiNodeIt[0]], maxCiNodeIt[0]))


        if G.number_of_nodes()==1:
            finalResult.extend(G.nodes());
            break;

    with open(outputPath, "w", newline='') as csv_file:
        writer = csv.writer(csv_file, delimiter=',')
        for line in chunks(finalResult,outputNum, modelID):
            writer.writerow(line)


    print("--- %s seconds ---" % (time.time() - start_time))