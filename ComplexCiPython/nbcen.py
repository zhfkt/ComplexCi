from networkx import *
import scipy.sparse.linalg as lin
import scipy.sparse as sp
import numpy as np
import matplotlib.pylab as plt
import time
import glob
import sys
import csv
import heapq
import os



def hashi_eig_cen(G):

    S = nx.to_scipy_sparse_matrix(G, dtype='d')

    I = sp.identity(S.shape[0])
    I_minus_D = sp.lil_matrix(S.shape)

    for node,deg in enumerate(G.degree()):
        I_minus_D[node,node] = 1.0-deg[1]
    crazy = sp.bmat([[None,I],[I_minus_D,S]])
    eigenvalue, eigenvector = lin.eigs(crazy, k=1, which="LR")
  
    largest = abs(eigenvector[:len(G.nodes()),0].flatten().real)
    norm = np.sqrt(sum(x*x for x in largest))

    centrality = dict(zip(G,map(float,largest/norm)))
    return centrality;



def chunks(l, n, modelID):
    """Yield successive n-sized chunks from l."""
    for i in range(0, len(l), n):
        genL = l[i:i + n]
        if len(genL)<n:
            genL.extend(["" for j in range(n-len(genL))])
        genL.insert(0, modelID)

        yield genL


if __name__ == '__main__':

    if len(sys.argv) < 4:

        print ("python nbcen.py [cenMethod] [input path] [output folder]");
        input()
        sys.exit(0);


    cenMethod = int(sys.argv[1]);
    inputPath = sys.argv[2]
    modelID = os.path.basename(inputPath).split('.')[0]
    outputPath = os.path.join(sys.argv[3] , modelID +"_method_" + str(cenMethod) + '.csv_out')

    print("cenMethod: %s inputPath: %s modelID: %s outputPath: %s" % (cenMethod,inputPath,modelID,outputPath))

    start_time = time.time()

    G = nx.Graph()

    with open(inputPath) as f:
        f_csv = csv.reader(f);
        for row in f_csv:

            #row0int = int(row[0])
            #row1int = int(row[1])

            G.add_edge(row[0],row[1])
    
    print("Total Nodes: " ,len(G.nodes()));

    if cenMethod == 0 :
        centrality = hashi_eig_cen(G)
    else:
        centrality = nx.eigenvector_centrality_numpy(G);

    centrality = sorted(centrality.items(), key=lambda eachResult : eachResult[1], reverse=True)
    nodeCentrality = [cen[0] for cen in centrality]
    
    with open(outputPath, "w", newline='') as csv_file:
        writer = csv.writer(csv_file, delimiter=',')
        for line in chunks(nodeCentrality,500, modelID):
            writer.writerow(line)

    print("--- %s seconds ---" % (time.time() - start_time))
