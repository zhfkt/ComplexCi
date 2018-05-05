#import networkx as nx

from graph_tool.all import *
#import graph_tool.centrality
import csv
import time
import heapq
import os
import sys

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

    G=Graph(directed=False);

    int_vertex = {}
    vertex_int = G.new_vertex_property("int")

    with open(sourcePath) as f:
        f_csv = csv.reader(f);
        for row in f_csv:

            row0int = int(row[0])
            row1int = int(row[1])

            if row0int in int_vertex: 
                v0 = int_vertex[row0int]
            else:
                v0 = G.add_vertex()
                vertex_int[v0] = row0int
                int_vertex[row0int] = v0

            if row1int in int_vertex:
                v1 = int_vertex[row1int]
            else:
                v1 = G.add_vertex()
                vertex_int[v1] = row1int
                int_vertex[row1int] = v1

            G.add_edge(v0,v1);

    totalNode = G.num_vertices()
    print("%s nodes are Loaded: " % totalNode)

    print("--- %s seconds ---" % (time.time() - start_time))

    finalResult = [];

    #while True:

	algo_start_time = time.time()
	
	if methodCentrality == 0:
		cenMap = graph_tool.centrality.pagerank(G)
	elif methodCentrality == 1:
		cenMap = graph_tool.centrality.katz(G)
	elif methodCentrality == 2:
		cenMap = graph_tool.centrality.closeness(G)
	elif methodCentrality == 3:
		cenMap, ep = graph_tool.centrality.betweenness(G)
	else:
		print("methodCentrality %s is not valid" % methodCentrality)
		input()
		sys.exit(0);
	
	print("--- algo uses %s seconds ---" % (time.time() - algo_start_time))
	
	resultPartial = []
	for item in G.vertices():
		resultPartial.append((item, cenMap[item]))

	resultPartialTop = heapq.nlargest(batchNum, resultPartial, key=lambda x: x[1])
	#print(resultPartial)
	resultVertexArray = [nodeIndiceAndValue[0] for nodeIndiceAndValue in resultPartialTop]

	print("modelID: %s, Left Edges %s, Left nodes: %s, Total nodes: %s, resultValue: %s, resultVertex: %s" % (modelID,len(list(G.edges())) , G.num_vertices(), totalNode, cenMap[resultVertexArray[0]], vertex_int[resultVertexArray[0]]) )
	finalResult.extend([vertex_int[eachVertex] for eachVertex in resultVertexArray]);

	#for eachV in resultVertexArray:
	#	G.clear_vertex(eachV)

	#G.remove_vertex(resultVertexArray);

	#if G.num_vertices()==0:
	#	break

    with open(outputPath, "w", newline='') as csv_file:
        writer = csv.writer(csv_file, delimiter=',')
        for line in chunks(finalResult,outputNum, modelID):
            writer.writerow(line)

    print("--- %s seconds ---" % (time.time() - start_time))
