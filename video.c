#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Define a structure for a graph node
typedef struct Node {
    int id;
    struct Node* next;
} Node;

// Define a structure for the graph
typedef struct Graph {
    int numVertices;
    Node** adjLists;
    int* inDegree;
} Graph;

// Define a structure for a player
typedef struct Player {
    int x;
    int y;
} Player;

// Function declarations
Graph* createGraph(int vertices);
void addEdge(Graph* graph, int src, int dest);
void topologicalSort(Graph* graph, void (*taskHandler[])(Player*));
void freeGraph(Graph* graph);

// Task handler functions
void handleInput(Player* player);
void updatePlayerPosition(Player* player);
void checkCollisions(Player* player);
void render(Player* player);

// Utility function to create a new node
Node* createNode(int id) {
    Node* newNode = malloc(sizeof(Node));
    newNode->id = id;
    newNode->next = NULL;
    return newNode;
}

int main() {
    // Initialize the player
    Player player = {5, 5};

    // Create a graph with 4 tasks
    Graph* graph = createGraph(4);

    // Add dependencies (edges)
    addEdge(graph, 0, 1); // Input -> Update Position
    addEdge(graph, 1, 2); // Update Position -> Check Collisions
    addEdge(graph, 2, 3); // Check Collisions -> Render

    // Task handler array (corrected function pointer syntax)
    void (*taskHandler[])(Player*) = {
        handleInput,
        updatePlayerPosition,
        checkCollisions,
        render
    };

    // Perform topological sorting to determine the order of execution
    printf("Executing game loop tasks in topological order:\n");
    topologicalSort(graph, taskHandler);

    // Free the graph
    freeGraph(graph);
    return 0;
}

Graph* createGraph(int vertices) {
    Graph* graph = malloc(sizeof(Graph));
    graph->numVertices = vertices;
    graph->adjLists = malloc(vertices * sizeof(Node*));
    graph->inDegree = calloc(vertices, sizeof(int));

    for (int i = 0; i < vertices; i++) {
        graph->adjLists[i] = NULL;
    }
    return graph;
}

void addEdge(Graph* graph, int src, int dest) {
    Node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
    graph->inDegree[dest]++;
}

void topologicalSort(Graph* graph, void (*taskHandler[])(Player*)) {
    int* stack = malloc(graph->numVertices * sizeof(int));
    int stackTop = -1;

    // Push all vertices with in-degree 0 to the stack
    for (int i = 0; i < graph->numVertices; i++) {
        if (graph->inDegree[i] == 0) {
            stack[++stackTop] = i;
        }
    }

    int count = 0;
    int* topOrder = malloc(graph->numVertices * sizeof(int));

    while (stackTop >= 0) {
        int u = stack[stackTop--];
        topOrder[count++] = u;

        Node* temp = graph->adjLists[u];
        while (temp != NULL) {
            int v = temp->id;
            if (--graph->inDegree[v] == 0) {
                stack[++stackTop] = v;
            }
            temp = temp->next;
        }
    }

    if (count != graph->numVertices) {
        printf("There exists a cycle in the graph\n");
        free(topOrder);
        free(stack);
        return;
    }

    // Example player initialization
    Player player = {5, 5};

    // Execute tasks in topologically sorted order
    for (int i = 0; i < count; i++) {
        printf("Executing task %d\n", topOrder[i]);
        taskHandler[topOrder[i]](&player);
    }

    free(topOrder);
    free(stack);
}

void freeGraph(Graph* graph) {
    for (int i = 0; i < graph->numVertices; i++) {
        Node* temp = graph->adjLists[i];
        while (temp) {
            Node* next = temp->next;
            free(temp);
            temp = next;
        }
    }
    free(graph->adjLists);
    free(graph->inDegree);
    free(graph);
}

// Task handler implementations
void handleInput(Player* player) {
    printf("Handling input...\n");
    player->x += 1; // Move player to the right
}

void updatePlayerPosition(Player* player) {
    printf("Updating player position to (%d, %d)...\n", player->x, player->y);
}

void checkCollisions(Player* player) {
    printf("Checking collisions at position (%d, %d)...\n", player->x, player->y);
}

void render(Player* player) {
    printf("Rendering player at position (%d, %d)...\n", player->x, player->y);
}
