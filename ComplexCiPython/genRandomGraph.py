import networkx
import sys

if len(sys.argv) < 2:

	print ("python genRandomGraph.py [output folder]");
	input()
	sys.exit(0);

outputPath = sys.argv[1]

G=networkx.erdos_renyi_graph(100000,3/100000)
networkx.write_edgelist(G, outputPath + "/genRandomGraph.csv", data=False , delimiter=',')


