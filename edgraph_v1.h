//-----------------------------------------------------------------------------
// File        : edgraph_v1.h
// Description : Includes class and member function declarations for edgraph
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

#ifndef _EDGRAPH_H
#define _EDGRAPH_H

class EDGraph {
    
private:
    
    // input vertex file name
    std::string vertexfile_;
    std::map<unsigned long, unsigned long> *gvertex_;
    
    // input edge list file name
    std::string edgefile_;
    
    // for storing the two columns of the edge list; this method sacrifices
    // memory (by having two copies of the same list) for the sake of speed
    // since both columns are sorted for fast lookups
    std::multimap<unsigned long, unsigned long> *gcited_;
    std::multimap<unsigned long, unsigned long> *gciting_;
    
public:
    
    // edgraph (evolving directed graph) type
    EDGraph(std::string vertexfile, std::string edgefile);
    ~EDGraph();
    
    // return all vertices from the vertex list
    std::vector<unsigned long> vertices();
    
    // return citations made by a particular vertex (backward in time)
    std::vector<unsigned long> cited(unsigned long focalvertex);
    
    // return citations to a particular vertex (forward in time)
    std::vector<unsigned long>
    citing(unsigned long focalvertex, unsigned long endtime);
    
    // return time associated with particular vertex
    unsigned long time(unsigned long focalvertex);
    
    // return the out-degree of a particular vertex (does not vary with time)
    unsigned long outdegree(unsigned long focalvertex);
    
    // return the in-degree of a particular vertex
    unsigned long indegree(unsigned long focalvertex, unsigned long endtime);
    
    // return normalized disruptiveness (Funk & Owen-Smith, 2012: pp. 6-8;
    // see http://arxiv.org/abs/1212.3559 for download)
    double disrupt(unsigned long focalvertex, unsigned long endtime);
    
};

#endif
