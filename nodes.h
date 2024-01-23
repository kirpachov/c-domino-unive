//
// Created by sasha on 23/01/24.
//

#ifndef C_DOMINO_UNIVE_NODES_H
#define C_DOMINO_UNIVE_NODES_H

#include "nodes.h"

struct Node {
  struct Domino* domino;

  bool is_vert; // true if vertical, false if horizontal

  unsigned long int id; // Incremental id

  struct Node* top_left;
  struct Node* top_right;
  struct Node* bottom_left;
  struct Node* bottom_right;
};

#define TOP_LEFT 1
#define TOP_RIGHT 2
#define BOTTOM_LEFT 3
#define BOTTOM_RIGHT 4

#endif //C_DOMINO_UNIVE_NODES_H
