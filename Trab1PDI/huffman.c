#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

// A função create_node e build_huffman_tree permanecem as mesmas...

Node* create_node(int symbol, int frequency, Node* left, Node* right) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        perror("Failed to allocate memory for Huffman node");
        exit(EXIT_FAILURE);
    }
    newNode->symbol = symbol;
    newNode->frequency = frequency;
    newNode->left = left;
    newNode->right = right;
    return newNode;
}

Node* build_huffman_tree(int *histogram, int size) {
    Node* nodes[size];
    int node_count = 0;
    for (int i = 0; i < size; i++) {
        nodes[node_count++] = create_node(i, histogram[i], NULL, NULL);
    }

    while (node_count > 1) {
        int min1_idx = -1, min2_idx = -1;
        for (int i = 0; i < node_count; i++) {
            if (nodes[i] != NULL) {
                if (min1_idx == -1 || nodes[i]->frequency < nodes[min1_idx]->frequency) {
                    min2_idx = min1_idx;
                    min1_idx = i;
                } else if (min2_idx == -1 || nodes[i]->frequency < nodes[min2_idx]->frequency) {
                    min2_idx = i;
                }
            }
        }

        Node* left = nodes[min1_idx];
        Node* right = nodes[min2_idx];
        int combined_freq = left->frequency + right->frequency;
        Node* parent = create_node(-1, combined_freq, left, right);

        nodes[min1_idx] = parent;
        nodes[min2_idx] = nodes[node_count - 1];
        node_count--;
    }

    return nodes[0];
}

// *** LÓGICA DE VISUALIZAÇÃO ADAPTADA E ADICIONADA ABAIXO ***

/**
 * @brief Função recursiva auxiliar para desenhar os nós e as arestas da árvore.
 * @param dot Ponteiro para o arquivo .dot aberto.
 * @param node O nó atual a ser desenhado.
 */
void draw_links_recursive(FILE* dot, Node* node) {
    if (node == NULL) {
        return;
    }

    // Define o nó no formato do GraphViz.
    // Usamos o endereço de memória do nó (%p) como seu ID único.
    if (node->left == NULL && node->right == NULL) { // É um nó folha
        fprintf(dot, "\t\"%p\" [shape=record, label=\"px=%d|fq=%d\", color=RED];\n", (void*)node, node->symbol, node->frequency);
    } else { // É um nó interno
        fprintf(dot, "\t\"%p\" [shape=circle, label=\"%d\"];\n", (void*)node, node->frequency);
    }

    // Desenha a aresta para o filho esquerdo
    if (node->left) {
        draw_links_recursive(dot, node->left);
        fprintf(dot, "\t\"%p\" -> \"%p\" [label=0];\n", (void*)node, (void*)node->left);
    }

    // Desenha a aresta para o filho direito
    if (node->right) {
        draw_links_recursive(dot, node->right);
        fprintf(dot, "\t\"%p\" -> \"%p\" [label=1];\n", (void*)node, (void*)node->right);
    }
}

// Implementação da nova função declarada no .h
void generate_dot_file(Node* root, const char* filename) {
    FILE* dot = fopen(filename, "w");
    if (!dot) {
        perror("Failed to open .dot file for writing");
        return;
    }

    fprintf(dot, "digraph HuffmanTree {\n");
    fprintf(dot, "\tnode [fontname=\"Arial\"];\n");
    
    if (root == NULL) {
        fprintf(dot, "\tEmpty_Tree [label=\"Árvore Vazia\"];\n");
    } else {
        draw_links_recursive(dot, root);
    }
    
    fprintf(dot, "}\n");
    fclose(dot);
    printf("Arquivo de visualização '%s' gerado com sucesso.\n", filename);
}

// A função free_huffman_tree permanece a mesma...
void free_huffman_tree(Node *root) {
    if (root == NULL) return;
    free_huffman_tree(root->left);
    free_huffman_tree(root->right);
    free(root);
}