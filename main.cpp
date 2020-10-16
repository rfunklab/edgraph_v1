//-----------------------------------------------------------------------------
// File        : main.cpp
// Description : Uses edgraph class to calculate disruptiveness of some patents
//-----------------------------------------------------------------------------
// Copyright (c) 2012, Russell J. Funk and Jason Owen Smith
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the University of Michigan nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL RUSSELL J. FUNK OR JASON OWEN-SMITH BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Author    : Russell J. Funk
// Email     : funk@umich.edu
// Web page  : http://www-personal.umich.edu/~funk/index.html
// Location  : University of Michigan, Ann Arbor, Michigan
// Date      : December 15, 2012
// Reference : http://arxiv.org/abs/1212.3559 (pp. 6-8 and pp. 24-25)
//-----------------------------------------------------------------------------


// to compile and use

// g++ main.cpp edgraph.cpp -o edgraph

// ./edgraph vertices_time.csv edge_data_big.csv results.txt

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "edgraph_v1.h"

int main (int argc, char* argv[])
{
    
    // get the vertex and edgefile from the command line
    std::string vertexfile;
    std::string edgefile;
    std::string resultsfile;
    
    if (argc != 4) {
        std::cout << "usage: " << argv[0];
        std::cout << " [vertexfile] [edgefile] [resultsfile]" << std::endl;
        exit(EXIT_FAILURE);
    }
    else {
        vertexfile = argv[1];
        edgefile = argv[2];
        resultsfile = argv[3];
    }
    
    std::cout<<"Reading vertices and edges..."<<std::endl;
    
    // create a new edgraph object from files
    EDGraph g = EDGraph(vertexfile, edgefile);
    
    // some illustrative examples of the class and member functions, using three
    // patents, PageRank (USPTO patent 6,285,999), Axel's cotransformation
    // technique (USPTO patent 4,399,216), and Monsanto's glyphosate resistant
    // soybeans (USPTO patent 6,958,436)
    
    // highly disruptive technologies (those with larger positive values on the
    // measure) interrupt the flow of citations (i.e., the use) of their
    // predecessors, while highly amplifying technologies (those with larger
    // negative values on the measure) increase the flow of citations (i.e., use)
    // of their predecessors by complementing them and making them more valuable
    
    // the Axel patent is highly disruptive, PageRank is moderately disruptive,
    // while Monsanto is highly amplifying
    
    
    // disruptiveness of the PageRank patent in 2002, 2005, and 2010
    double pagerank95 = g.disrupt(6285999, 1995);
    double pagerank02 = g.disrupt(6285999, 2002);
    double pagerank05 = g.disrupt(6285999, 2005);
    double pagerank10 = g.disrupt(6285999, 2010);
    
    // disruptiveness of the Axel patent in 2010
    double axel10 = g.disrupt(4399216, 2010);
    
    // disruptiveness of Monsanto's glyphosate resistant soybean patent in 2010
    double monsanto10 = g.disrupt(6958436, 2010);
    
    // patents citing the PageRank patent as of 2003
    std::vector<unsigned long> gfor03 = g.citing(6285999, 2003);
    
    // output results
    std::cout<<"Citations to PageRank patent as of 2003"<<std::endl;
    for(std::vector<unsigned long>::iterator it1 = gfor03.begin();
        it1 != gfor03.end(); it1++) {
        std::cout<<*it1<<std::endl;
    }
    
    // patents citing the PageRank patent as of 2010
    std::vector<unsigned long> gfor10 = g.citing(6285999, 2010);
    
    // output results
    std::cout<<"Citations to PageRank patent as of 2010"<<std::endl;
    for(std::vector<unsigned long>::iterator it1 = gfor10.begin();
        it1 != gfor10.end(); it1++) {
        std::cout<<*it1<<std::endl;
    }
    
    // time associated with PageRank patent vertex
    std::cout << "PageRank time, i.e. issue date: " << g.time(6285999) << std::endl;
    
    // illustrate changes in disruptiveness over time for PageRank patent
    std::cout << "PageRank disruptiveness, 1995: " << pagerank95 << std::endl;
    std::cout << "PageRank disruptiveness, 2002: " << pagerank02 << std::endl;
    std::cout << "PageRank disruptiveness, 2005: " << pagerank05 << std::endl;
    std::cout << "PageRank disruptiveness, 2010: " << pagerank10 << std::endl;
    
    // illustrate changes in in-degree (i.e., citations) over time for PageRank
    // patent
    std::cout<<"PageRank in-degree, 1995: "<<g.indegree(6285999, 1995)<<std::endl;
    std::cout<<"PageRank in-degree, 2002: "<<g.indegree(6285999, 2002)<<std::endl;
    std::cout<<"PageRank in-degree, 2005: "<<g.indegree(6285999, 2005)<<std::endl;
    std::cout<<"PageRank in-degree, 2010: "<<g.indegree(6285999, 2010)<<std::endl;
    
    // illustrate changes in radicalness (i.e., in-degree weighted disruptiveness)
    // over time for PageRank patent
    std::cout<<"PageRank radicalness, 1995: "<<g.disrupt(6285999, 1995)*g.indegree(6285999, 1995)<<std::endl;
    std::cout<<"PageRank radicalness, 2002: "<<g.disrupt(6285999, 2002)*g.indegree(6285999, 2002)<<std::endl;
    std::cout<<"PageRank radicalness, 2005: "<<g.disrupt(6285999, 2005)*g.indegree(6285999, 2005)<<std::endl;
    std::cout<<"PageRank radicalness, 2010: "<<g.disrupt(6285999, 2010)*g.indegree(6285999, 2010)<<std::endl;
    
    // illustrate disruptiveness for Axel patent
    std::cout << "Axel disruptiveness, 2010: " << axel10 << std::endl;
    
    // illustrate disruptiveness for Axel patent
    std::cout << "Monsanto disruptiveness, 2010: " << monsanto10 << std::endl;
    
    // use this code to output year by year disruptiveness for all patents in list
    std::cout<<"Getting vertices..."<<std::endl;
    
    std::cout<<"Beginning calculations..."<<std::endl;
    
    // open file to export results
    std::ofstream results;
    results.open(resultsfile.c_str());
    
    std::vector<unsigned long> verts = g.vertices();
    
    for (int year = 1976; year <= 2010; year++) {
        
        for (std::vector<unsigned long>::iterator it1 = verts.begin();
             it1 != verts.end(); ++it1) {
            if (year >= g.time(*it1) && g.time(*it1) >= 1976) {
                
                std::cout<<*it1<<","; // vertex id (i.e., patent number)
                std::cout<<g.time(*it1)<<","; // vertex time (i.e., patent issue date)
                std::cout<<year<<","; // loop time (i.e., panel date)
                std::cout<<g.disrupt(*it1,year)<<","; // disruptiveness
                std::cout<<g.disrupt(*it1,year)*g.indegree(*it1,year)<<","; // radicalness
                std::cout<<g.indegree(*it1,year)<<std::endl; // indegree
                
                results<<*it1<<","; // vertex id (i.e., patent number)
                results<<g.time(*it1); // vertex time (i.e., patent issue date)
                results<<","; // vertex time (i.e., patent issue date)
                results<<year<<","; // loop time (i.e., panel date)
                results<<g.disrupt(*it1,year)<<","; // disruptiveness
                results<<g.disrupt(*it1,year)*g.indegree(*it1,year)<<","; // radicalness
                results<<g.indegree(*it1,year)<<std::endl; // indegree
                
            }
        }
    }
    
    // close results file
    results.close();
    
    return 0;
}


/* TODO
 - If I don't want to supply a default value for undefined values, I
 have to return a pointer. (http://www.functionx.com/cpp/examples/returnpointer.htm)
 (http://stackoverflow.com/questions/2639255/c-return-a-null-object-if-search-result-not-found)
 - Related: need a better way for returning undefined values (e.g., indegree might return 0 when actually undefined)
 
 - serialization
 - need to use templating!!!!
 - inline?
 - code review from someone
 - boost python interface
 - find other c++ code examples
 - After extensive testing, I have found that the program can be sped up
 immensely by using integer ids. Thus, I suggest a few options: (1) require a
 vertex list for hashing, (2) make the default assume integer ids but allow an
 option for strings [perhaps templating?].
 - Think very hard about using an std::vector<pair<int, float> >, and using
 std::lower_bound, std::upper_bound, and/or std::equal_range to look up values.
 This approach offers speed improvements (tested) though not nearly as much as
 using integer ids.
 - Produce basic documentation
 - Add alternative input/constructor methods (e.g., from map, from file)
 - Add radicalness with better weighting
 - Get a good test data set (e.g., all relevant cites to a sample of top 10,000)
 - Pass delimiter from command line
 - Allow interactive or command line mode
 - Default constructor
 - Add expanded measure support
 
 - Typedefs are essential
 - demo-TUNGraph.cpp -- example undirected graph (see constructor method TUNGraph::New()
 - graph.cpp (in SNAP) defines TUNGraph
 - graph.h (in SNAP) on TUNGraph
 - /// Static constructor that returns a pointer to the graph. Call: PUNGraph Graph = TUNGraph::New().
 static PUNGraph New() { return new TUNGraph(); }
 
 - GNU plot support (see SNAP)
 
 */

