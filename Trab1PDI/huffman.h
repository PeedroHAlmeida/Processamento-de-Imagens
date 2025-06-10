#ifndef HUFFMAN_H
#define HUFFMAN_H

// A estrutura Node permanece a mesma
typedef struct Node {
    int symbol;
    int frequency;
    struct Node *left;
    struct Node *right;
} Node;

Node* build_huffman_tree(int *histogram, int size);
void free_huffman_tree(Node *root);

/**
 * @brief Gera um arquivo no formato .dot para visualização da árvore de Huffman.
 * * @param root O nó raiz da árvore de Huffman.
 * @param filename O nome do arquivo .dot a ser criado.
 */
void generate_dot_file(Node* root, const char* filename);

#endif // HUFFMAN_H