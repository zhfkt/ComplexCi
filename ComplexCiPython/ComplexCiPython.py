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

    if len(sys.argv) < 6:

        print("%s %s %s %s %s %s" % ("ComplexCiPython.py","[input file]","[output folder]","[ballRadius]","[batchNum]","[outputNum]"))
        input()
        sys.exit(0);

    ballRadius = int(sys.argv[3]);
    batchNum = int(sys.argv[4]);
    outputNum = int(sys.argv[5]);
    sourcePath = sys.argv[1]
    modelID = os.path.basename(sourcePath).split('.')[0]
    outputPath = os.path.join(sys.argv[2] , modelID + '_output')

    print("sourcePath: %s outputPath: %s ballRadius: %s batchNum: %s outputNum: %s" % (sourcePath,outputPath,ballRadius,batchNum,outputNum))
    
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
        ciMap = collective_influence(G,distance=ballRadius)

        maxCiNodeIt = heapq.nlargest(batchNum, ciMap, key=ciMap.get)
        G.remove_nodes_from(maxCiNodeIt);
        finalResult.extend(maxCiNodeIt);


        #print(maxCiNodeIt);
        print("modelID: %s, Left nodes: %s, Total nodes: %s, MaxCiValue: %s, MaxCiNode: %s" % (modelID, G.number_of_nodes(), totalNode, ciMap[maxCiNodeIt[0]], maxCiNodeIt[0]))


        if G.number_of_nodes()==0:
            break;

    with open(outputPath, "w", newline='') as csv_file:
        writer = csv.writer(csv_file, delimiter=',')
        for line in chunks(finalResult,outputNum, modelID):
            writer.writerow(line)


    print("--- %s seconds ---" % (time.time() - start_time))