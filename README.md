# edgraph_v1

This repository contains a C++ library and example program for computing the CD index (as introduced by Funk and Owen-Smith, 2017, see <http://www.cdindex.info> for more information) and a few related measures. 

# Compiling and running
The code only uses the Standard Template Library so it should compile very easily with your favorite C++ compiler, e.g.

`g++ main.cpp edgraph.cpp -o edgraph`

You can then run `edgraph` as follows.

`./edgraph vertices.csv edges.csv results.txt`

where `vertices.csv` is a csv file with two columns, one for the (integer) identifier for each vertex, `edges.csv` is an edge list, formatted as a csv file, with two columns, one for the "citing" and one for the "cited" vertex. 

# Notes
That this library was written before the final publication of the paper, and uses slightly different terminology ("disruptiveness" -> "CD index", "disruptive" -> "destabilizing", "amplifying" -> "consolidating", "radicalness" -> "mCD index"). 

Also note that this code was written by Funk ages ago, and while potentially useful, is not really actively maintained, though he is generally happy to answer questions (<rfunk@umn.edu>). There is more recent C/Python package available [here](https://github.com/russellfunk/cdindex). 