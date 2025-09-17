#include "node.h"
#include "raymath.h"

void UpdateConnForces(struct Connection* conn){
    Vector2 deltav = Vector2Subtract(conn->b->position_vec,conn->a->position_vec);
    float size = Vector2Length(deltav);
    deltav = Vector2Scale(deltav,(conn->elastic_coefficient*(size - conn->maxsize))/size);
    
    if (size <= conn->maxsize) return;
    
    conn->a->force_vec = Vector2Add(conn->a->force_vec,deltav);
    conn->b->force_vec = Vector2Subtract(conn->b->force_vec,deltav);
    
}

void ZeroNodeForce(struct Node* node){
    node->force_vec = Vector2Zero();
}

void ApplyNodeGravity(struct Node* node, float gravity_force){
    node->force_vec = Vector2Add(node->force_vec ,(Vector2) {0,gravity_force});
}

void UpdateNodeVelAndPos(struct Node* node,float deltatime){
    node->velocity_vec = Vector2Add(node->velocity_vec, Vector2Scale(node->force_vec,deltatime));
    node->position_vec = Vector2Add(node->position_vec, Vector2Scale(node->velocity_vec,deltatime));
}

void UpdateNodeGroup(struct NodeGroup* nodegroup,float deltatime){
    for (int node_ind = 0; node_ind < nodegroup->node_amount; node_ind++){
        ZeroNodeForce(&nodegroup->nodes[node_ind]);
        ApplyNodeGravity(&nodegroup->nodes[node_ind],100.0f);
    }
    for (int conn_ind = 0; conn_ind < nodegroup->conn_amount; conn_ind++){
        UpdateConnForces(&nodegroup->conns[conn_ind]);            
    }
    for (int node_ind = 0; node_ind < nodegroup->node_amount; node_ind++){
        if (nodegroup->nodes[node_ind].anchored) continue;
        UpdateNodeVelAndPos(&nodegroup->nodes[node_ind],deltatime);
    }
}