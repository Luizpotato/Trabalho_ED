#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    char nome[100];
    char sexo;
    char nascimento[11]; // Formato: dd/mm/aaaa
    char ultimaConsulta[11]; // Formato: dd/mm/aaaa
} Paciente;

// Estrutura de um nó da árvore AVL (para pacientes femininos)
typedef struct AVLNode {
    Paciente paciente;
    struct AVLNode *esquerda;
    struct AVLNode *direita;
    int altura;
} AVLNode;

// Estrutura de um nó da lista duplamente encadeada (para pacientes masculinos)
typedef struct ListNode {
    Paciente paciente;
    struct ListNode *proximo;
    struct ListNode *anterior;
} ListNode;

// Função para criar um novo nó da árvore AVL
AVLNode* criarNoAVL(Paciente paciente) {
    AVLNode* novoNo = (AVLNode*)malloc(sizeof(AVLNode));
    if (novoNo == NULL) {
        printf("Erro ao alocar memória para o nó AVL.\n");
        exit(1);
    }
    novoNo->paciente = paciente;
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    novoNo->altura = 1; // Novo nó é adicionado como folha, altura 1
    return novoNo;
}

// Função para criar um novo nó da lista duplamente encadeada
ListNode* criarNoLista(Paciente paciente) {
    ListNode* novoNo = (ListNode*)malloc(sizeof(ListNode));
    if (novoNo == NULL) {
        printf("Erro ao alocar memória para o nó da lista.\n");
        exit(1);
    }
    novoNo->paciente = paciente;
    novoNo->proximo = NULL;
    novoNo->anterior = NULL;
    return novoNo;
}

// Função para ler o arquivo e separar os pacientes
void lerArquivo(const char* nomeArquivo, AVLNode** raizAVL, ListNode** cabecaLista) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        exit(1);
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), arquivo)) {
        // Remover o caractere de nova linha no final da linha
        linha[strcspn(linha, "\n")] = '\0';

        // Verificar se a linha está no formato esperado
        if (strlen(linha) < 10 || linha[0] != '<' || linha[strlen(linha) - 1] != '>') {
            printf("Formato inválido na linha: %s\n", linha);
            continue;
        }

        // Extrair os campos da linha
        Paciente paciente;
        int result = sscanf(linha, "<%[^,], %c, %[^,], %[^>]>", paciente.nome, &paciente.sexo, paciente.nascimento, paciente.ultimaConsulta);
        if (result != 4) {
            printf("Erro ao ler os dados do paciente na linha: %s\n", linha);
            continue;
        }

        // Inserir na estrutura correta com base no sexo
        if (paciente.sexo == 'F') {
            // Inserir na árvore AVL (a ser implementada)
            printf("Paciente feminino adicionado: %s\n", paciente.nome);
            // AVLNode* novoNo = criarNoAVL(paciente);
            // *raizAVL = inserirAVL(*raizAVL, novoNo); // Função de inserção na AVL a ser implementada
        } else if (paciente.sexo == 'M') {
            // Inserir na lista duplamente encadeada (a ser implementada)
            printf("Paciente masculino adicionado: %s\n", paciente.nome);
            // ListNode* novoNo = criarNoLista(paciente);
            // *cabecaLista = inserirLista(*cabecaLista, novoNo); // Função de inserção na lista a ser implementada
        } else {
            printf("Sexo inválido para o paciente: %s\n", paciente.nome);
        }
    }

    fclose(arquivo);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo_pacientes.txt>\n", argv[0]);
        return 1;
    }

    AVLNode* raizAVL = NULL; // Raiz da árvore AVL para pacientes femininos
    ListNode* cabecaLista = NULL; // Cabeça da lista duplamente encadeada para pacientes masculinos

    // Ler o arquivo e separar os pacientes
    lerArquivo(argv[1], &raizAVL, &cabecaLista);

    // Aqui você pode continuar com a implementação do menu e outras funcionalidades

    return 0;
}
