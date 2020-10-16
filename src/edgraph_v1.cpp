//-----------------------------------------------------------------------------
// File        : edgraph_v1.cpp
// Description : Includes class and member functions for edgraph
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

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "edgraph_v1.h"

// defines edgraph (evolving directed graph) class
EDGraph::EDGraph(std::string vertexfile, std::string edgefile) {
    
    // look for vertexfile and load vertices
    
    // TODO: transform this to a "read vertices" function that can load vertices
    //       from generic sources, e.g., SQL, Python
    
    gvertex_ = new std::map<unsigned long, unsigned long>;
    
    vertexfile_ = vertexfile;
    
    std::string line;
    std::ifstream vfin;
    
    // try to open the file
    vfin.open(vertexfile_.c_str(), std::ios_base::in|std::ios_base::binary);
    if (!vfin.is_open()) {
        std::cout << "could not open the file '" << vertexfile_ << "'" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // parses the file and extracts comma-separated vertex-id,time pairs
    while (getline(vfin, line)) {
        std::istringstream linestream(line);
        std::string item;
        
        unsigned long vertex;
        unsigned long time;
        
        getline(linestream, item, ',');
        vertex = atoi(item.c_str());
        getline(linestream, item);
        time = atoi(item.c_str());
        
        gvertex_->insert(std::pair<unsigned long, unsigned long>(vertex, time));
    }
    
    // close the vertex file
    vfin.close();
    
    
    // look for edgefile and load edges
    
    // TODO: transform this to a "read edges" function that can load edges from
    //       generic sources, e.g., SQL, Python
    
    
    gcited_ = new std::multimap<unsigned long, unsigned long>;
    gciting_ = new std::multimap<unsigned long, unsigned long>;
    
    edgefile_ = edgefile;
    
    // obtain citing and cited multimaps
    std::ifstream efin;
    
    // try to open the file
    efin.open(edgefile_.c_str(), std::ios_base::in|std::ios_base::binary);
    if (!efin.is_open()) {
        std::cout << "could not open the file '" << edgefile_ << "'" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // parses the file and extracts comma-separated vertex-id,vertex-id pairs
    while (getline(efin, line)) {
        std::istringstream linestream(line);
        std::string item;
        
        unsigned long vertexa;
        unsigned long vertexb;
        
        getline(linestream, item, ',');
        vertexa = atoi(item.c_str());
        getline(linestream, item);
        vertexb = atoi(item.c_str());
        
        gcited_->insert(std::pair<unsigned long, unsigned long>(vertexa, vertexb));
        gciting_->insert(std::pair<unsigned long, unsigned long>(vertexb, vertexa));
    }
    
    // close the edge file
    efin.close();
}

// return time associated with particular vertex
unsigned long EDGraph::time(unsigned long focalvertex) {
    unsigned long time = gvertex_->find(focalvertex)->second;
    return time;
}


// return all vertices from the vertex list
std::vector<unsigned long> EDGraph::vertices(){
    
    std::vector<unsigned long> vertices;
    
    for (std::map<unsigned long,unsigned long>::iterator it1 = gvertex_->begin();
         it1 != gvertex_->end(); ++it1) {
        vertices.push_back(it1->first);
    }
    return vertices;
}

// return citations made by a particular vertex (backward in time)
std::vector<unsigned long> EDGraph::cited(unsigned long focalvertex){
    
    std::pair<std::multimap<unsigned long,unsigned long>::iterator,
    std::multimap<unsigned long,unsigned long>::iterator> it1;
    
    it1 = gcited_->equal_range(focalvertex);
    std::vector<unsigned long> cited;
    
    for (std::multimap<unsigned long,unsigned long>::iterator it2 = it1.first;
         it2 != it1.second; ++it2) {
        cited.push_back((*it2).second);
    }
    return cited;
}

// return citations to a particular vertex (forward in time)
std::vector<unsigned long> EDGraph::citing(unsigned long focalvertex,
                                           unsigned long endtime) {
    
    std::pair<std::multimap<unsigned long,unsigned long>::iterator,
    std::multimap<unsigned long,unsigned long>::iterator> it1;
    
    it1 = gciting_->equal_range(focalvertex);
    std::vector<unsigned long> citing;
    
    for (std::multimap<unsigned long,unsigned long>::iterator it2 = it1.first;
         it2 != it1.second; ++it2) {
        
        if (EDGraph::time((*it2).second) <= endtime) {
            citing.push_back((*it2).second);
        }
    }
    return citing;
}

// return the out-degree of a particular vertex (does not vary with time)
unsigned long EDGraph::outdegree(unsigned long focalvertex){
    return gcited_->count(focalvertex);
}

// return the in-degree of a particular vertex
unsigned long EDGraph::indegree(unsigned long focalvertex,
                                unsigned long endtime){
    return EDGraph::citing(focalvertex, endtime).size();
}

// return normalized disruptiveness (Funk & Owen-Smith, 2012: pp. 6-8;
// see http://arxiv.org/abs/1212.3559 for download)
double EDGraph::disrupt(unsigned long focalvertex, unsigned long endtime) {
    
    // collect all forward citations to focal vertex
    std::vector<unsigned long> f_i = EDGraph::citing(focalvertex, endtime);
    
    // collect all backward citations made by focal vertex
    std::vector<unsigned long> b = EDGraph::cited(focalvertex);
    
    // collect all unique forward citations of the backward citations made by the
    // focal vertex
    std::vector<unsigned long> b_i;
    
    for (std::vector<unsigned long>::iterator it1 = b.begin(); it1 != b.end();
         ++it1) {
        std::vector<unsigned long> vb_i = EDGraph::citing(*it1, endtime);
        b_i.insert( b_i.end(), vb_i.begin(), vb_i.end() );
    }
    std::sort( b_i.begin(), b_i.end() ); // keep unique elements b_i only
    b_i.erase( std::unique( b_i.begin(), b_i.end() ), b_i.end() );
    
    // create vector i of all forward citations to the focal vertex and/or its
    // backward citations
    std::vector<unsigned long> i;
    
    i.reserve( f_i.size() + b_i.size() ); // preallocate memory
    i.insert( i.end(), f_i.begin(), f_i.end() ); // add f_i
    i.insert( i.end(), b_i.begin(), b_i.end() ); // add b_i
    
    std::sort( i.begin(), i.end() ); // keep unique elements of i only
    i.erase( std::unique( i.begin(), i.end() ), i.end() );
    
    // calculate numerator (raw disruptiveness)
    std::vector<double> d_raw;
    
    // sort f_i for binary search
    std::sort (f_i.begin(), f_i.end());
    
    // sort b_i for binary search
    std::sort (b_i.begin(), b_i.end());
    
    // iterate over all i's
    
    double n_i = 0.0;
    
    unsigned long focaltime = EDGraph::time(focalvertex);
    for (std::vector<unsigned long>::iterator it1 = i.begin(); it1 != i.end();
         ++it1) {
        
        // calculate n_i with only those vertexs following focal vertex
        if (EDGraph::time(*it1) > focaltime){
            n_i++;
            
            double f_fcite = std::binary_search(f_i.begin(), f_i.end(), *it1);
            double b_fcite = std::binary_search(b_i.begin(), b_i.end(), *it1);
            d_raw.push_back(-2.0*f_fcite*b_fcite + f_fcite);
        }
    }
    
    // sum elements of d_raw
    double d_raw_sum = 0.0;
    
    for (std::vector<double>::iterator it1 = d_raw.begin(); it1 != d_raw.end();
         ++it1)
        d_raw_sum += *it1;
    
    // NOTE: pathways can be counted using
    //       double pathcount = i.size()*b.size()*2;
    
    // calculate final measure
    double d = d_raw_sum/n_i;
    
    return d;
    
}

// destructor
EDGraph::~EDGraph(){
}

// TODO: add multimap destructors (comment out)
