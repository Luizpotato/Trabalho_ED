#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para armazenar os dados de um paciente
typedef struct {
    char nome[100];
    char sexo;
    char nascimento[11]; // Formato: dd/mm/aaaa
    char ultima_consulta[11]; // Formato: dd/mm/aaaa
} Paciente;

// Estrutura de um nó da lista duplamente encadeada
typedef struct NoLista {
    Paciente paciente;
    struct NoLista* proximo;
    struct NoLista* anterior;
} NoLista;

// Estrutura da lista duplamente encadeada
typedef struct {
    NoLista* inicio;
    NoLista* fim;
} ListaDupla;

// Estrutura de um nó da árvore AVL
typedef struct NoAVL {
    Paciente paciente;
    struct NoAVL* esquerda;
    struct NoAVL* direita;
    int altura;
} NoAVL;

// Função para criar uma nova lista vazia
ListaDupla* criar_lista() {
    ListaDupla* lista = (ListaDupla*)malloc(sizeof(ListaDupla));
    if (lista == NULL) {
        printf("Erro ao alocar memória para a lista.\n");
        exit(1);
    }
    lista->inicio = NULL;
    lista->fim = NULL;
    return lista;
}

// Função para criar um novo nó da árvore AVL
NoAVL* criar_no_avl(Paciente paciente) {
    NoAVL* novo_no = (NoAVL*)malloc(sizeof(NoAVL));
    if (novo_no == NULL) {
        printf("Erro ao alocar memória para o nó da árvore.\n");
        exit(1);
    }
    novo_no->paciente = paciente;
    novo_no->esquerda = NULL;
    novo_no->direita = NULL;
    novo_no->altura = 1;
    return novo_no;
}

// Função para calcular a altura de um nó da árvore AVL
int altura_avl(NoAVL* no) {
    if (no == NULL) return 0;
    return no->altura;
}

// Função para calcular o fator de balanceamento de um nó da árvore AVL
int fator_balanceamento(NoAVL* no) {
    if (no == NULL) return 0;
    return altura_avl(no->esquerda) - altura_avl(no->direita);
}

// Função para rotacionar à direita (rotação simples à direita)
NoAVL* rotacionar_direita(NoAVL* y) {
    NoAVL* x = y->esquerda;
    NoAVL* T2 = x->direita;

    x->direita = y;
    y->esquerda = T2;

    y->altura = 1 + (altura_avl(y->esquerda) > altura_avl(y->direita) ? altura_avl(y->esquerda) : altura_avl(y->direita));
    x->altura = 1 + (altura_avl(x->esquerda) > altura_avl(x->direita) ? altura_avl(x->esquerda) : altura_avl(x->direita));

    return x;
}

// Função para rotacionar à esquerda (rotação simples à esquerda)
NoAVL* rotacionar_esquerda(NoAVL* x) {
    NoAVL* y = x->direita;
    NoAVL* T2 = y->esquerda;

    y->esquerda = x;
    x->direita = T2;

    x->altura = 1 + (altura_avl(x->esquerda) > altura_avl(x->direita) ? altura_avl(x->esquerda) : altura_avl(x->direita));
    y->altura = 1 + (altura_avl(y->esquerda) > altura_avl(y->direita) ? altura_avl(y->esquerda) : altura_avl(y->direita));

    return y;
}

// Função para inserir um paciente na árvore AVL
NoAVL* inserir_avl(NoAVL* raiz, Paciente paciente) {
    if (raiz == NULL) return criar_no_avl(paciente);

    if (strcmp(paciente.nome, raiz->paciente.nome) < 0) {
        raiz->esquerda = inserir_avl(raiz->esquerda, paciente);
    } else if (strcmp(paciente.nome, raiz->paciente.nome) > 0) {
        raiz->direita = inserir_avl(raiz->direita, paciente);
    } else {
        printf("Erro: Já existe um paciente com o nome %s.\n", paciente.nome);
        return raiz; // Nomes iguais não são permitidos
    }

    raiz->altura = 1 + (altura_avl(raiz->esquerda) > altura_avl(raiz->direita) ? altura_avl(raiz->esquerda) : altura_avl(raiz->direita));

    int balanceamento = fator_balanceamento(raiz);

    // Casos de desbalanceamento
    if (balanceamento > 1 && strcmp(paciente.nome, raiz->esquerda->paciente.nome) < 0) {
        return rotacionar_direita(raiz);
    }
    if (balanceamento < -1 && strcmp(paciente.nome, raiz->direita->paciente.nome) > 0) {
        return rotacionar_esquerda(raiz);
    }
    if (balanceamento > 1 && strcmp(paciente.nome, raiz->esquerda->paciente.nome) > 0) {
        raiz->esquerda = rotacionar_esquerda(raiz->esquerda);
        return rotacionar_direita(raiz);
    }
    if (balanceamento < -1 && strcmp(paciente.nome, raiz->direita->paciente.nome) < 0) {
        raiz->direita = rotacionar_direita(raiz->direita);
        return rotacionar_esquerda(raiz);
    }

    return raiz;
}

// Função para inserir um paciente na lista duplamente encadeada (Z-A)
void inserir_ordenado(ListaDupla* lista, Paciente paciente) {
    NoLista* novo_no = (NoLista*)malloc(sizeof(NoLista));
    if (novo_no == NULL) {
        printf("Erro ao alocar memória para o nó da lista.\n");
        exit(1);
    }
    novo_no->paciente = paciente;
    novo_no->proximo = NULL;
    novo_no->anterior = NULL;

    if (lista->inicio == NULL) {
        lista->inicio = novo_no;
        lista->fim = novo_no;
    } else {
        NoLista* atual = lista->inicio;
        while (atual != NULL && strcmp(atual->paciente.nome, paciente.nome) > 0) {
            atual = atual->proximo;
        }

        if (atual == lista->inicio) {
            novo_no->proximo = lista->inicio;
            lista->inicio->anterior = novo_no;
            lista->inicio = novo_no;
        } else if (atual == NULL) {
            novo_no->anterior = lista->fim;
            lista->fim->proximo = novo_no;
            lista->fim = novo_no;
        } else {
            novo_no->proximo = atual;
            novo_no->anterior = atual->anterior;
            atual->anterior->proximo = novo_no;
            atual->anterior = novo_no;
        }
    }
}

// Função para buscar um paciente na lista duplamente encadeada
Paciente* buscar_lista(ListaDupla* lista, char* nome) {
    NoLista* atual = lista->inicio;
    while (atual != NULL) {
        if (strcmp(atual->paciente.nome, nome) == 0) {
            return &(atual->paciente);
        }
        atual = atual->proximo;
    }
    return NULL;
}

// Função para buscar um paciente na árvore AVL
Paciente* buscar_avl(NoAVL* raiz, char* nome) {
    if (raiz == NULL) return NULL;

    if (strcmp(nome, raiz->paciente.nome) < 0) {
        return buscar_avl(raiz->esquerda, nome);
    } else if (strcmp(nome, raiz->paciente.nome) > 0) {
        return buscar_avl(raiz->direita, nome);
    } else {
        return &(raiz->paciente);
    }
}

// Função para exibir os dados de um paciente
void exibir_paciente(Paciente* paciente) {
    if (paciente != NULL) {
        printf("Nome: %s\n", paciente->nome);
        printf("Sexo: %c\n", paciente->sexo);
        printf("Data de Nascimento: %s\n", paciente->nascimento);
        printf("Última Consulta: %s\n", paciente->ultima_consulta);
    } else {
        printf("Paciente não encontrado.\n");
    }
}

// Função para listar todos os pacientes da lista duplamente encadeada
void listar_pacientes_lista(ListaDupla* lista) {
    NoLista* atual = lista->inicio;
    if (atual == NULL) {
        printf("Nenhum paciente cadastrado.\n");
        return;
    }

    printf("\n--- Lista de Pacientes (Moisés) ---\n");
    while (atual != NULL) {
        exibir_paciente(&(atual->paciente));
        printf("\n");
        atual = atual->proximo;
    }
}

// Função para listar todos os pacientes da árvore AVL (em ordem A-Z)
void listar_pacientes_avl(NoAVL* raiz) {
    if (raiz == NULL) return;

    listar_pacientes_avl(raiz->esquerda);
    exibir_paciente(&(raiz->paciente));
    printf("\n");
    listar_pacientes_avl(raiz->direita);
}

// Função para remover caracteres indesejados (aspas e < >)
void limpar_string(char* str) {
    int i, j = 0;
    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] != '<' && str[i] != '>') {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}

// Função para carregar os pacientes do arquivo TXT
void carregar_pacientes(ListaDupla* lista_m, NoAVL** raiz_l, char* nome_arquivo) {
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), arquivo)) {
        limpar_string(linha);

        Paciente paciente;
        sscanf(linha, "%[^,], %c, %[^,], %s", paciente.nome, &paciente.sexo, paciente.nascimento, paciente.ultima_consulta);

        if (paciente.sexo == 'M') {
            inserir_ordenado(lista_m, paciente);
        } else if (paciente.sexo == 'F') {
            *raiz_l = inserir_avl(*raiz_l, paciente);
        }
    }

    fclose(arquivo);
}

// Função para exibir o menu de pacientes do Moisés
void menu_moises(ListaDupla* lista) {
    int opcao;
    char nome[100];

    do {
        printf("\n--- Pacientes do Moisés ---\n");
        printf("1. Consultar paciente\n");
        printf("2. Listar todos os pacientes\n");
        printf("3. Cadastrar paciente\n");
        printf("4. Alterar cadastro do paciente\n");
        printf("5. Voltar\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite o nome do paciente: ");
                scanf(" %[^\n]", nome);
                Paciente* paciente = buscar_lista(lista, nome);
                exibir_paciente(paciente);
                break;
            case 2:
                listar_pacientes_lista(lista);
                break;
            case 3:
                printf("nao implementado");
                break;
            case 4:
                printf("Alterar cadastro (não implementado).\n");
                break;
            case 5:
                printf("Voltando ao menu principal.\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 5);
}

// Função para exibir o menu de pacientes da Liz
void menu_liz(NoAVL* raiz) {
    int opcao;
    char nome[100];

    do {
        printf("\n--- Pacientes da Liz ---\n");
        printf("1. Consultar paciente\n");
        printf("2. Listar todos os pacientes\n");
        printf("3. Cadastrar paciente\n");
        printf("4. Alterar cadastro do paciente\n");
        printf("5. Voltar\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite o nome do paciente: ");
                scanf(" %[^\n]", nome);
                Paciente* paciente = buscar_avl(raiz, nome);
                exibir_paciente(paciente);
                break;
            case 2:
                printf("\n--- Lista de Pacientes (Liz) ---\n");
                listar_pacientes_avl(raiz);
                break;
            case 3:
                printf("cadastrar paciente(não implementado).\n");
                break;
            case 4:
                printf("Alterar cadastro (não implementado).\n");
                break;
            case 5:
                printf("Voltando ao menu principal.\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 5);
}

// Função para exibir o menu principal
void menu_principal(ListaDupla* lista_m, NoAVL* raiz_l) {
    int opcao;

    do {
        printf("\n--- Menu Principal ---\n");
        printf("1. Pacientes do Moisés\n");
        printf("2. Pacientes da Liz\n");
        printf("3. Finalizar programa\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                menu_moises(lista_m);
                break;
            case 2:
                menu_liz(raiz_l);
                break;
            case 3:
                printf("Finalizando programa.\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 3);
}

// Função principal
int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo.txt>\n", argv[0]);
        return 1;
    }

    ListaDupla* lista_m = criar_lista();
    NoAVL* raiz_l = NULL;

    carregar_pacientes(lista_m, &raiz_l, argv[1]);

    menu_principal(lista_m, raiz_l);

    // Liberar memória (a ser implementado)
    return 0;
}
