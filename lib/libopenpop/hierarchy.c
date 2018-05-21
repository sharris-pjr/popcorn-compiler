/*
 * Provides hierarchy abstractions for threads executing in Popcorn Linux.
 *
 * Copyright Rob Lyerly, SSRG, VT, 2018
 */

#include "libgomp.h"
#include "hierarchy.h"

global_info_t popcorn_global;
node_info_t popcorn_node[MAX_POPCORN_NODES];

void hierarchy_init_node(int nid)
{
  popcorn_node[nid].remaining = popcorn_node[nid].threads;
  popcorn_node[nid].has_leader = false;
  gomp_barrier_init(&popcorn_node[nid].bar, popcorn_node[nid].threads);
}

bool hierarchy_select_leader_optimistic(int nid)
{
  bool expected = false;
  bool leader = __atomic_compare_exchange_n(&popcorn_node[nid].has_leader,
                                            &expected,
                                            true,
                                            false,
                                            MEMMODEL_ACQ_REL,
                                            MEMMODEL_RELAXED);
  return leader;
}

bool hierarchy_select_leader_synchronous(int nid)
{
  size_t rem = __atomic_add_fetch(&popcorn_node[nid].remaining,
                                  -1,
                                  MEMMODEL_ACQ_REL);
  if(rem) return false;
  else
  {
    __atomic_store_n(&popcorn_node[nid].has_leader, true, MEMMODEL_RELEASE);
    return true;
  }
}

void hierarchy_leader_cleanup(int nid)
{
  __atomic_store_n(&popcorn_node[nid].remaining,
                   popcorn_node[nid].threads,
                   MEMMODEL_RELEASE);
  __atomic_store_n(&popcorn_node[nid].has_leader,
                   false,
                   MEMMODEL_RELEASE);;
}

