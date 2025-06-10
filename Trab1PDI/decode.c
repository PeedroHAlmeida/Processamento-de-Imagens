/*
 * UNIFAL - Universidade Federal de Alfenas.                
 * BACHARELADO EM CIENCIA DA COMPUTACAO.                     
 * Trabalho: Decodificador do formato PGH                    
 * Professor: Luiz Eduardo da Silva
 * Aluno: Pedro Henrique de Almeida
 * Data: 10/06/2025                                          
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "huffman.h"
#include "imagelib.h"

#define OUTPUT_DIR "resultados/"
#define PATH_BUFFER_SIZE 4096 

void msg(char *s) {
    printf("\nDecoding files in PGH format");
    printf("\n----------------------------");
    printf("\nUsage: %s <image.pgh> [viewer] [-dot]\n\n", s);
    printf("  [viewer] : Opcional. 'eog', 'feh', etc. Padrão: 'eog'.\n");
    printf("  [-dot]   : Opcional. Gera uma imagem 'tree.svg' da árvore de Huffman.\n\n");
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        msg(argv[0]);
    }

    const char* viewer_cmd = "eog";
    char* input_file = NULL;
    bool generate_viz = false;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-dot") == 0) {
            generate_viz = true;
        } else if (input_file == NULL) {
            input_file = argv[i];
        } else {
            viewer_cmd = argv[i];
        }
    }

    if (input_file == NULL) {
        printf("Erro: Nenhum arquivo de entrada especificado.\n");
        msg(argv[0]);
    }

    // --- Preparação de Nomes de Arquivos e Pastas de Saída ---
    char base_name[256];
    
    // Extrai o nome base do arquivo de entrada (ex: de "imagens/foto.pgh" -> "foto")
    const char* basename_start = strrchr(input_file, '/');
    if (basename_start) {
        basename_start++;
    } else {
        basename_start = input_file;
    }
    strcpy(base_name, basename_start);
    char* dot_pos = strrchr(base_name, '.');
    if (dot_pos) {
        *dot_pos = '\0'; // Remove a extensão
    }

    // Cria o caminho para o subdiretório de saída
    char output_dir_path[512]; // Buffer de tamanho razoável, apenas para o caminho do diretório
    snprintf(output_dir_path, sizeof(output_dir_path), "%s%s", OUTPUT_DIR, base_name);

    // Cria as pastas de saída
    mkdir(OUTPUT_DIR, 0755); 
    mkdir(output_dir_path, 0755);

    printf("Pasta de saída: %s/\n", output_dir_path);

    // --- Leitura do Arquivo PGH e Decodificação ---
    FILE *fp = fopen(input_file, "rb");
    if (!fp) { /* ... */ return 1; }
    
    char line[256];
    int width, height, max_val, hist_size;
    int *histogram;
    Node *huffman_root;
    image *decoded_image;

    fgets(line, sizeof(line), fp);
    if (strncmp(line, "PH", 2) != 0) { /* ... */ return 1; }
    while (fgets(line, sizeof(line), fp) && line[0] == '#');
    sscanf(line, "%d %d", &width, &height);
    fgets(line, sizeof(line), fp);
    sscanf(line, "%d", &max_val);
    hist_size = max_val + 1;
    histogram = (int*)malloc(hist_size * sizeof(int));
    fread(histogram, sizeof(int), hist_size, fp);
    
    huffman_root = build_huffman_tree(histogram, hist_size);

    if (generate_viz) {
        char dot_path[PATH_BUFFER_SIZE];
        char svg_path[PATH_BUFFER_SIZE];
        
        // Construa os caminhos .dot e .svg diretamente dos componentes base
        snprintf(dot_path, sizeof(dot_path), "%s/tree.dot", output_dir_path);
        snprintf(svg_path, sizeof(svg_path), "%s/tree.svg", output_dir_path);

        generate_dot_file(huffman_root, dot_path);
        
        char dot_cmd[PATH_BUFFER_SIZE * 2 + 100]; // Buffer extra grande para o comando
        snprintf(dot_cmd, sizeof(dot_cmd), "dot -Tsvg \"%s\" -o \"%s\"", dot_path, svg_path);
        
        int ret = system(dot_cmd);
        
        if (ret == 0) {
            printf("Imagem da árvore salva como '%s'.\n", svg_path);
            char viz_cmd[PATH_BUFFER_SIZE + 100];
            snprintf(viz_cmd, sizeof(viz_cmd), "%s \"%s\" &", viewer_cmd, svg_path);
            system(viz_cmd);
        } else {
            fprintf(stderr, "Erro: Não foi possível gerar '%s'. Verifique se o GraphViz está instalado.\n", svg_path);
        }
    }
    
    decoded_image = img_create(width, height, max_val);
    int total_pixels = width * height;
    Node* current_node = huffman_root;
    unsigned char byte_buffer;
    int bit_pos = 7;
    fread(&byte_buffer, 1, 1, fp);
    for (int i = 0; i < total_pixels; i++) {
        while (current_node->left != NULL && current_node->right != NULL) {
            int bit = (byte_buffer >> bit_pos) & 1;
            bit_pos--;
            if (bit == 0) current_node = current_node->left;
            else current_node = current_node->right;
            if (bit_pos < 0) {
                bit_pos = 7;
                if(fread(&byte_buffer, 1, 1, fp) != 1 && i < total_pixels -1) break;
            }
        }
        decoded_image->data[i] = current_node->symbol;
        current_node = huffman_root;
    }

    // Salva a imagem PGM
    char pgm_path[PATH_BUFFER_SIZE];
    snprintf(pgm_path, sizeof(pgm_path), "%s/%s.pgm", output_dir_path, base_name);
    
    printf("Decodificação completa.\n");
    save_pgm(decoded_image, pgm_path);
    
    // Exibe a imagem PGM
    char img_cmd[PATH_BUFFER_SIZE + 100]; 
    snprintf(img_cmd, sizeof(img_cmd), "%s \"%s\" &", viewer_cmd, pgm_path);
    system(img_cmd);

    // Limpeza
    free(histogram);
    free_huffman_tree(huffman_root);
    img_free(decoded_image);
    fclose(fp);

    return 0;
}