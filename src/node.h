#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"

struct Node{
    Vector2 position_vec;
    Vector2 velocity_vec;
    Vector2 force_vec;
    char anchored;
};

struct Connection {
    struct Node* a;
    struct Node* b;
    float maxsize;
    float elastic_coefficient;
};

struct NodeGroup {
    size_t node_amount;
    struct Node* nodes;

    size_t conn_amount;
    struct Connection* conns;
};

void UpdateNodeGroup(struct NodeGroup* nodegroup,float deltatime);