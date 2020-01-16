# A-and-RBFS-with-Hanoi
Using A* and RBFS with created heuristic to traverse a binary move tree in a Towers of Hanoi game to find optimal path.

##Files##
  aStar.cpp
  OldaStar.cpp
  RBFS.cpp
  Makefile
  README

##Compile & run on UNIX system##
    
    $ make britain
    $ ./astarb
    $./rbfsb
    $ make clean
    
## A* Notes ##
  f'(n) = g(n) + h'(n)   //Hueristic function, approximates the true evaluation of a node.
  g(n)  = distance from parent. "Cost of getting from initial state to current node."
  h'(n) = estimate of the cost of getting from current state to goal state.
  h(n)  = (2^d - 1) optimal moves

  OPEN[] = List of 'open' nodes. (generated but not processed) The initial state is first.
          #NOTE# If open is empty, report failure.

  CLOSED[]=List of nodes that have already been examined (had their successors generated)


  BESTNODE = Curent best node (Lowest f' value on open)

  OLD = A successor of BESTNODE which is already in OPEN
      #NOTE# iff path is cheaper than current best path to old throw away 'successor',
           update old's parent to "BESTNODE", propogate new cost to all successors and their successors.

  CLOSED_OLD = a successor of BESTNODE which has already been CLOSED
  
  Contributers:
  Britain Mackenzie - aStar.cpp, OldaStar.cpp, RBFS.cpp
  Andrew Brooks - Makefile, README, aStar.cpp
  
  
