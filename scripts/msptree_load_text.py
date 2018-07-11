#import msprime
from testutils import *
import sys
import io
#/Users/jaredgalloway/Documents/UndergraduateThesis/UnsimplifiedTable
nodes_U = open("/Users/jaredgalloway/Documents/UndergraduateThesis/UnsimplifiedTable/NodeTable.txt","r")
edges_U = open("/Users/jaredgalloway/Documents/UndergraduateThesis/UnsimplifiedTable/EdgeTable.txt","r")
mutations_U = open("/Users/jaredgalloway/Documents/UndergraduateThesis/UnsimplifiedTable/MutationTable.txt","r")
sites_U = open("/Users/jaredgalloway/Documents/UndergraduateThesis/UnsimplifiedTable/SiteTable.txt","r")

nodes_S = open("/Users/jaredgalloway/Documents/UndergraduateThesis/SimplifiedTable/NodeTable.txt","r")
edges_S = open("/Users/jaredgalloway/Documents/UndergraduateThesis/SimplifiedTable/EdgeTable.txt","r")
mutations_S = open("/Users/jaredgalloway/Documents/UndergraduateThesis/SimplifiedTable/MutationTable.txt","r")
sites_S = open("/Users/jaredgalloway/Documents/UndergraduateThesis/SimplifiedTable/SiteTable.txt","r")

#Nodes = msprime.parse_nodes(nodes_U,base64_metadata=False)

tr_U = msprime.load_text(nodes=nodes_U, edges=edges_U, 
		       sites=sites_U, mutations=mutations_U,
		       base64_metadata=False)

tr_S = msprime.load_text(nodes=nodes_S, edges=edges_S, 
		       sites=sites_S, mutations=mutations_S,
		       base64_metadata=False)

#print(type(tr))
unsimplified = tr_U.trees()
simplified = tr_S.trees()


for i in unsimplified:
	print(i.draw(format="unicode"))
	print(i.interval)

#print("------------------- SIMPLIFIED TREES --------------------"+"\n")

for i in simplified:
	print(i.draw(format="unicode"))
	print(i.interval)


