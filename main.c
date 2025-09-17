#include "node.h"
#include "raylib.h"
#include <stdlib.h>
#include <time.h>

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

struct NodeGroup makesheet(float x,float y,int wc,int hc,float spacing,float elastic_coefficient){
    struct NodeGroup nodegroup;
    nodegroup.node_amount = wc*hc;
    nodegroup.nodes = malloc(sizeof(struct Node) * nodegroup.node_amount);

    nodegroup.conn_amount = 2 * wc * hc - hc - wc;
    nodegroup.conns = malloc(sizeof(struct Connection) * nodegroup.conn_amount);

    int conn_ind = 0;
    for (int i = 0; i < nodegroup.node_amount; i++){
            int xn = i%wc;
            int yn = i/wc;
            nodegroup.nodes[i] = (struct Node) {
                (Vector2){(float) spacing*xn + x, (float) spacing*yn + y},
                Vector2Zero(),
                Vector2Zero(),
                0
            };
            if (rand() % 100 == 0 ) nodegroup.nodes[i].anchored = 1;

            if (xn < wc - 1) {
                nodegroup.conns[conn_ind++] = (struct Connection) {
                    &nodegroup.nodes[i],
                    &nodegroup.nodes[i+1],
                    spacing,
                    elastic_coefficient
                };
            }
            if (yn < hc - 1) {
                nodegroup.conns[conn_ind++] = (struct Connection) {
                    &nodegroup.nodes[i],
                    &nodegroup.nodes[i+wc],
                    spacing,
                    elastic_coefficient
                };
            }
    }

    return nodegroup;
};

int main(void)
{   
    srand(time(NULL)); 

    struct NodeGroup mainsheet = makesheet(50.0f,50.0f,200,200,3.0f,1000.0f);
    
    // Initialization
    const int screenWidth = 700;
    const int screenHeight = 700;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - input keys");
    
    
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    int draw_amount = 0;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        UpdateNodeGroup(&mainsheet,GetFrameTime());
    
        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            for (int i = 0; i < mainsheet.conn_amount; i++){
                DrawLineV(mainsheet.conns[i].a->position_vec, mainsheet.conns[i].b->position_vec, BROWN);
            }
    
        EndDrawing();
    }

    // De-Initialization
    CloseWindow();        // Close window and OpenGL context
    
    return 0;
}

