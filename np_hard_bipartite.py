import sys
import timeit
import networkx as nx
import matplotlib.pyplot as plt
from networkx.algorithms import bipartite

start = timeit.default_timer()


if len(sys.argv) > 1:
	input_file = sys.argv[1]
else:
	input_file = "input_bipartite_2.txt"

output_file = 'output.txt'

data_in = open (input_file, 'r')
data_out = open(output_file, 'w')

temp_1 = data_in.readline().strip().split()

print 'this is the raw reading: ', temp_1

number_of_vertices, number_of_edges = temp_1[0], temp_1[1]

print 'this is the values assigned to seperate variables: ', number_of_vertices, number_of_edges

edge_matrix = []


for i in range(int(number_of_edges)):
	temp_2 = data_in.readline().strip().split()
	edge_matrix.append(temp_2)

print 'these are the edges in edge_matrix'
for item in edge_matrix:
	print item

Graph_1 = nx.Graph()

'''
# this is not usful since the nodes are being created by adding the edges
for x in range(int(number_of_vertices)):
	Graph_1.add_node(x + 1)
'''

for edge in edge_matrix:
	Graph_1.add_edge(edge[0], edge[1])

nodes_in_graph = Graph_1.nodes()
print 'this are the nodes in the graph: '
nodes_in_graph.sort()
print nodes_in_graph

#edges_in_graph = Graph_1.edges().sort()	# this doesnt work
#print Graph_1.edges().sort()	# this doesnt work
# so i am doing it this way
edges_in_graph = Graph_1.edges()
print 'these are the edges in the graph: '
edges_in_graph.sort()
print edges_in_graph

print 'these are the neighbors of each node in the graph: '
for x in range(int(number_of_vertices)):
	item = str(x+1)
	neighbors_in_graph = Graph_1.neighbors(item)
	neighbors_in_graph.sort()
	print neighbors_in_graph

#print type(item)	# returns the data type of the variable

# this was to test how to implement this
'''
nodes = nx.dfs_edges(Graph_1, '1')
print 'these are the DFS nodes: '
print nodes.next()
print nodes.next()
print nodes.next()
print nodes.next()
print nodes.next()
print nodes.next()
print nodes.next()
print nodes.next()
print nodes.next()
'''


# this was just to see how it works, probably not usful in this project
#print nx.is_connected(Graph_1)


print 'is graph bipartite? ', nx.is_bipartite(Graph_1)

if nx.is_bipartite(Graph_1):

	c = bipartite.color(Graph_1)
	print 'this is the dictionary of the bipartite graph'
	print c

	bottom_nodes, top_nodes = bipartite.sets(Graph_1)
	print list(top_nodes)
	print list(bottom_nodes)

	partition1 = list(top_nodes)
	partition2 = list(bottom_nodes)
	crossEdgeCount = 0
	for i in range(0, len(partition1)):
		crossEdgeCount += len(Graph_1.neighbors(partition1[i]))

print 'this is where we start printing to output file'
# this is the printing of the results to the screen and to the output file
stop = timeit.default_timer()

print (stop - start)/1000
data_out.write(str((stop - start)/1000))

data_out.write('\r')

print crossEdgeCount
data_out.write(str(crossEdgeCount))

data_out.write('\r')

for item in top_nodes:
	print item,
	data_out.write(item + ' ')

print '\r'
data_out.write('\r')

for item in bottom_nodes:
	print item,
	data_out.write(item + ' ')



#nx.min_cut(Graph_1, '1', '12')
nx.draw(Graph_1)
plt.show()


