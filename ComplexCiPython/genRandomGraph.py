import networkx
import sys

if len(sys.argv) < 2:

	print ("python genRandomGraph.py [output folder]");
	input()
	sys.exit(0);

outputPath = sys.argv[1]

genList=[(100000,2),(100000,3),(1000000,2),(1000000,3)]


for i,(node,mean_degree) in enumerate(genList):

	print(i, (node,mean_degree))

	G=networkx.fast_gnp_random_graph(node,mean_degree/node)
	networkx.write_edgelist(G, outputPath + "/genRandomGraph_"+str(i)+".csv", data=False , delimiter=',')




