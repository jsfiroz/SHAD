set(program
ei-triangle-count
ei-page-rank
ei-sssp
ei-vertex_nomination
ei-random_walk
)


foreach(p ${program})
  add_executable(${p} ${p}.cc)
  target_link_libraries(${p} ${SHAD_RUNTIME_LIB} runtime)
endforeach(p)
