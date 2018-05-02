//===------------------------------------------------------------*- C++ -*-===//
//
//                                     SHAD
//
//      The Scalable High-performance Algorithms and Data Structure Library
//
//===----------------------------------------------------------------------===//
//
// Copyright 2017 Pacific Northwest National Laboratory
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not
// use this file except in compliance with the License. You may obtain a copy
// of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations
// under the License.
//
//===----------------------------------------------------------------------===//

#include <atomic>
#include <fstream>
#include <iostream>
#include <mutex>
#include <sstream>
#include <utility>

#include "shad/data_structures/array.h"
#include "shad/data_structures/set.h"
#include "shad/extensions/graph_library/algorithms/sssp.h"
#include "shad/extensions/graph_library/edge_index.h"
#include "shad/runtime/runtime.h"
#include "shad/util/measure.h"
#include "shad/extensions/graph_library/local_edge_index.h"

void random_walk(shad::EdgeIndex<size_t, size_t>::ObjectID GID) {
   auto GraphPtr = shad::EdgeIndex<size_t, size_t>::GetPtr(GID);
   size_t num_vertices = GraphPtr->Size();

  /*choose a  random vertex to start walk from*/
  size_t currentSeed = rand() % num_vertices;
  size_t previousNode = currentSeed;

  /*Create a global id to hold the newly-explored graph*/
  auto exploredGraph = shad::EdgeIndex<size_t, size_t>::Create(1);
  while (true) {
    size_t currentNodeDegree = GraphPtr->GetDegree(currentSeed);
    if (currentNodeDegree > 0 ) {
      previousNode = currentSeed;
      /*Choose one neighbor*/
      size_t nextSeedIndex = rand() % currentNodeDegree; 
      /*How do I know what is the size of each entry of edgelist?*/
      //shad::DefaultEdgeIndexStorage* neighborList = NULL;
      // neighborList = new  shad::DefaultEdgeIndexStorage[currentNodeDegree];
      // std::vector<shad::DefaultEdgeIndexStorage, currentNodeDegree> neighborList;
// ::NeighborListStorageT
//GetNeighbors(previousNode, );
      // currentSeed = ; 
    }
  }
}

// The GraphReader expects an input file with the first line containing no of vertices and no of edges and subsequent lines with 1 edge per line
shad::EdgeIndex<size_t, size_t>::ObjectID GraphReader(std::ifstream &GFS) {
  std::string line;
  unsigned long EdgeNumber, VertexNumber;

  std::getline(GFS, line);

  std::istringstream headlineStream(line);

  headlineStream >> VertexNumber >> EdgeNumber;
  

  auto eiGraph = shad::EdgeIndex<size_t, size_t>::Create(VertexNumber);
  shad::rt::Handle handle;

  std::vector<size_t> edges;
  size_t source, destination, prev_source = 0;
  
  /*read the first edge*/
  std::getline(GFS, line);
  std::istringstream firstLineStream(line);
  firstLineStream >> source >> destination;
  std::cout << source << " " << destination << std::endl;
  edges.push_back(destination);
  prev_source = source;

  for (size_t i = 1L; i < EdgeNumber; i++) {
    std::getline(GFS, line);
    std::istringstream lineStream(line);
    lineStream >> source >> destination;
    std::cout << source << " " << destination << std::endl;
    if (source == prev_source) {
      edges.push_back(destination);
    } else {
      eiGraph->AsyncInsertEdgeList(handle, prev_source, edges.data(), edges.size());
      edges.clear();
      edges.push_back(destination);
      prev_source = source;
    }
  }
  shad::rt::waitForCompletion(handle);
  return eiGraph->GetGlobalID();
}

void printHelp(const std::string programName) {
  std::cerr << "Usage: " << programName << " --inpath FILENAME [options]\n"
            << "Options:\n"
            << "        --seed SEED (default 123)\n"
            << "        --num_runs NUM_RUNS (default 10)\n"
            << "        --num_seeds NUM_SEEDD (default 5)\n"
            << std::endl;
}

namespace shad {
int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Missing required argument --inpath FILENAME\n";
    printHelp(argv[0]);
    std::exit(-1);
  }
  // input parameters
  std::string inpath = "";  // required
 
  unsigned argIndex = 1;
  while (argIndex < argc and argv[argIndex][0] == '-') {
    std::string arg(argv[argIndex]);
    if (arg == "--inpath") {
      ++argIndex;
      inpath = std::string(argv[argIndex]);
      ++argIndex;
    }
  }
  if (inpath == "") {
    std::cerr << "Missing required argument --inpath FILENAME\n";
    printHelp(argv[0]);
    std::exit(-1);
  }
  shad::EdgeIndex<size_t, size_t>::ObjectID OID(-1);
  auto loadingTime = shad::measure<std::chrono::seconds>::duration([&]() {
      // The GraphReader expects an input file in METIS dump format
      std::ifstream inputFile;
      inputFile.open(inpath.c_str(), std::ifstream::in);
      OID = GraphReader(inputFile);
    });

  auto GraphPtr = shad::EdgeIndex<uint64_t, uint64_t>::GetPtr(OID);
  // size_t neighborListSize = GraphPtr->getSizeInBytes(0);

  using EdgeStorageType = shad::EdgeIndex<uint64_t, uint64_t>::EdgeStorage;
  std::vector<EdgeStorageType> neighborList(GraphPtr->GetDegree(0)); 
  //auto buf = GraphPtr->GetLocalBuf(0);
  GraphPtr->GetNeighbors(0, neighborList.data());
  
  // By default, neighborlist is returned as a localset.
  // We need an iterator to get access to each member.
  for (auto it = neighborList.begin(); it < neighborList.end(); it++) {
    
  }
  // neighborList.PrintAllElements();
  // buf.PrintAllElements();
 // for (auto it = buf.begin(); it < buf.end(); it++) {
  //   std::cout << *it.getEntry() << std::endl;
  // }

  std::cout << "Graph loaded in " << loadingTime.count() << std::endl; 
  return 0;
}
}  // namespace shad
