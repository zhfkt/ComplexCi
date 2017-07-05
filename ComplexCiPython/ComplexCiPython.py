import networkx as nx
import csv
import time
import heapq
from ci import collective_influence

def chunks(l, n, modelID):
    """Yield successive n-sized chunks from l."""
    for i in range(0, len(l), n):
        genL = l[i:i + n]
        if len(genL)<500:
            genL.extend(["" for j in range(500-len(genL))])
        genL.insert(0, modelID)

        yield genL

if __name__ == '__main__':

    start_time = time.time()

    G=nx.Graph();
    batchNum = 2500;
    outputNum = 500;
    #sourcePath = r'C:\Users\冯贶天\Source\Repos\ComplexCi\data\524038a.csv'
    sourcePath = r'C:\Users\冯贶天\Source\Repos\ComplexCi\data\networks\real2.csv'
    modelID = sourcePath.split('\\')[-1].split('.')[0]
    outputPath = sourcePath + '_output'

    with open(sourcePath) as f:
        f_csv = csv.reader(f);
        for row in f_csv:
            G.add_edge(row[0],row[1]);

    totalNode = G.number_of_nodes()
    print("%s nodes are Loaded: " % totalNode)

    print("--- %s seconds ---" % (time.time() - start_time))

    finalResult = [];


    while True:
        ciMap = collective_influence(G)

        maxCiNodeIt = heapq.nlargest(batchNum, ciMap, key=ciMap.get)
        G.remove_nodes_from(maxCiNodeIt);
        finalResult.extend(maxCiNodeIt);


        #print(maxCiNodeIt);
        print("modelID: %s, Left nodes: %s, Total nodes: %s, MaxCiNode: %s" % (modelID, G.number_of_nodes(), totalNode, maxCiNodeIt[0]))


        if G.number_of_nodes()==0:
            break;

    with open(outputPath, "w") as csv_file:
        writer = csv.writer(csv_file, delimiter=',')
        for line in chunks(finalResult,outputNum, modelID):
            writer.writerow(line)


    print("--- %s seconds ---" % (time.time() - start_time))