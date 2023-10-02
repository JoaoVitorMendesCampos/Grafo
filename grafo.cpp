#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

class Graph {
public:
    // Construtor
    Graph(int numVertices) {
        this->numVertices = numVertices;
        adjMatrix.resize(numVertices, vector<int>(numVertices, 0));
        coordinates.resize(numVertices, make_pair(0, 0));
    }

    // Importar grafo de um arquivo de texto
    void importGraph(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Erro: Nao e possivel abrir o arquivo." << endl;
            return;
        }

        // Le se o gráfico é direcionado ou não
        string directedLine;
        getline(file, directedLine);
        isDirected = (directedLine == "direcionado");

        // Le o número de vertices
        file >> numVertices;
        adjMatrix.resize(numVertices, vector<int>(numVertices, 0));
        coordinates.resize(numVertices, make_pair(0, 0));

        // Le as coordenadas de vértice
        for (int i = 0; i < numVertices; i++) {
            int id;
            double x, y;
            file >> id >> x >> y;
            setCoordinates(id, x, y);
        }

        // Le o número de arestas
        int numEdges;
        file >> numEdges;

        // Le as arestas e os pesos
        for (int i = 0; i < numEdges; i++) {
            int source, dest, weight;
            file >> source >> dest >> weight;
            addEdge(source, dest, weight);
        }

        file.close();
    }

    // Exportar grafo para um arquivo de texto
    void exportGraph(const string& filename) {
        ofstream file(filename);

        if (!file.is_open()) {
            cout << "Erro: nao e possivel abrir o arquivo para exportacao." << endl;
            return;
        }

        file << (isDirected ? "direcionado" : "nao direcionado") << endl;
        file << numVertices << endl;

        // Escreve as coordenadas de vértice
        for (int i = 0; i < numVertices; i++) {
            file << i << " " << fixed << setprecision(2) << coordinates[i].first << " " << coordinates[i].second << endl;
        }

        // Escreve o número de arestas
        file << getNumEdges() << endl;

        // Escreve as arestas e os pesos
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                int weight = getEdgeWeight(i, j);
                if (weight > 0) {
                    file << i << " " << j << " " << weight << endl;
                }
            }
        }

        file.close();
    }

    // Exibir adjacências (matriz ou lista de adjacências)
    void displayAdjacencies() {
        cout << "Matriz de adjacencia:" << endl;
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                cout << adjMatrix[i][j] << " ";
            }
            cout << endl;
        }
    }

    // Verifique se um vértice é adjacente a outro
    bool isAdjacent(int vertex1, int vertex2) {
        return adjMatrix[vertex1][vertex2] > 0;
    }

    // Insere uma nova aresta
    void addEdge(int source, int dest, int weight) {
        if (source >= 0 && source < numVertices && dest >= 0 && dest < numVertices) {
            adjMatrix[source][dest] = weight;
            if (!isDirected) {
                adjMatrix[dest][source] = weight;
            }
        }
    }

    // Remove uma aresta
    void removeEdge(int source, int dest) {
        if (source >= 0 && source < numVertices && dest >= 0 && dest < numVertices) {
            adjMatrix[source][dest] = 0;
            if (!isDirected) {
                adjMatrix[dest][source] = 0;
            }
        }
    }

    // Edita as cordenadas de um vértice
    void setCoordinates(int vertex, double x, double y) {
        if (vertex >= 0 && vertex < numVertices) {
            coordinates[vertex] = make_pair(x, y);
        }
    }

    // Verifique o primeiro adjacente de um vértice
    int getFirstAdjacent(int vertex) {
        for (int i = 0; i < numVertices; i++) {
            if (adjMatrix[vertex][i] > 0) {
                return i;
            }
        }
        return -1; // vertice não adjacente
    }

    // Verifique o próximo adjacente de um vértice a partir de um adjacente informado
    int getNextAdjacent(int vertex, int prevAdjacent) {
        for (int i = prevAdjacent + 1; i < numVertices; i++) {
            if (adjMatrix[vertex][i] > 0) {
                return i;
            }
        }
        return -1; // No more adjacent vertices
    }

    // Consulte a lista completa de adjacentes
    vector<int> getAdjacents(int vertex) {
        vector<int> adjacents;
        for (int i = 0; i < numVertices; i++) {
            if (adjMatrix[vertex][i] > 0) {
                adjacents.push_back(i);
            }
        }
        return adjacents;
    }

    // Obtem o número de arestas no grafo
    int getNumEdges() {
        int count = 0;
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                if (adjMatrix[i][j] > 0) {
                    count++;
                }
            }
        }
        return count / (isDirected ? 1 : 2);
    }

    // Obtem o peso de uma aresta entre dois vértices
int getEdgeWeight(int source, int dest) {
    if (source >= 0 && source < numVertices && dest >= 0 && dest < numVertices) {
        return adjMatrix[source][dest];
    }
    return 0; // Retorne 0 se a aresta não existir ou os vértices estiverem fora dos limites
}


private:
    int numVertices;
    vector<vector<int>> adjMatrix;
    vector<pair<double, double>> coordinates;
    bool isDirected;
};

int main() {
    int numVertices;
    cout << "Insira o numero de vertices: ";
    cin >> numVertices;

    Graph graph(numVertices);

    // Menu para interações do usuário
    int choice;
    do {
        cout << "\nMenu:\n";
        cout << "1. Importar grafo de um arquivo\n";
        cout << "2. Exportar grafo para um arquivo\n";
        cout << "3. Exibir adjacencias\n";
        cout << "4. Verifique se um vertice e adjacente a outro\n";
        cout << "5. Insira uma nova aresta\n";
        cout << "6. Remove uma aresta\n";
        cout << "7. Editar a coordenada de um vertice\n";
        cout << "8. Verifique o primeiro adjacente de um vertice\n";
        cout << "9. Verifique o próximo adjacente de um vertice a partir de um adjacente informado\n";
        cout << "10. Consulte a lista completa de adjacentes\n";
        cout << "0. Sair\n";
        cout << "Digite sua escolha: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string filename;
                cout << "Digite o nome do arquivo para importar: ";
                cin >> filename;
                graph.importGraph(filename);
                break;
            }
            case 2: {
                string filename;
                cout << "Digite o nome do arquivo para exportar: ";
                cin >> filename;
                graph.exportGraph(filename);
                break;
            }
            case 3:
                graph.displayAdjacencies();
                break;
            case 4: {
                int vertex1, vertex2;
                cout << "Insira dois vertices: ";
                cin >> vertex1 >> vertex2;
                bool adjacent = graph.isAdjacent(vertex1, vertex2);
                cout << (adjacent ? "Os vertices sao adjacentes" : "Os vertices nao sao adjacentes") << endl;
                break;
            }
            case 5: {
                int source, dest, weight;
                cout << "Insira o vertice de origem, o vertice de destino e o peso: ";
                cin >> source >> dest >> weight;
                graph.addEdge(source, dest, weight);
                break;
            }
            case 6: {
                int source, dest;
                cout << "Insira o vertice de origem e o vertice de destino para remover a aresta: ";
                cin >> source >> dest;
                graph.removeEdge(source, dest);
                break;
            }
            case 7: {
                int vertex;
                double x, y;
                cout << "Insira o vertice e novas coordenadas(x, y): ";
                cin >> vertex >> x >> y;
                graph.setCoordinates(vertex, x, y);
                break;
            }
            case 8: {
                int vertex;
                cout << "Insira o vertice para verificar o primeiro adjacente: ";
                cin >> vertex;
                int firstAdjacent = graph.getFirstAdjacent(vertex);
                if (firstAdjacent != -1) {
                    cout << "Primeiro adjacente do vertice " << vertex << ": " << firstAdjacent << endl;
                } else {
                    cout << "Nenhum vertice adjacente." << endl;
                }
                break;
            }
            case 9: {
                int vertex, prevAdjacent;
                cout << "Insira o vertice e o adjacente anterior: ";
                cin >> vertex >> prevAdjacent;
                int nextAdjacent = graph.getNextAdjacent(vertex, prevAdjacent);
                if (nextAdjacent != -1) {
                    cout << "Próximo adjacente do vertice " << vertex << " de " << prevAdjacent << ": " << nextAdjacent << endl;
                } else {
                    cout << "Nao ha mais vertices adjacentes." << endl;
                }
                break;
            }
            case 10: {
                int vertex;
                cout << "Insira o vertice para consultar a lista completa de adjacentes: ";
                cin >> vertex;
                vector<int> adjacents = graph.getAdjacents(vertex);
                cout << "Adjacentes do vertice " << vertex << ": ";
                for (int adj : adjacents) {
                    cout << adj << " ";
                }
                cout << endl;
                break;
            }
            case 0:
                cout << "Saindo do programa.\n";
                break;
            default:
                cout << "Escolha invalida. Por favor, tente novamente.\n";
        }
    } while (choice != 0);

    return 0;
}
