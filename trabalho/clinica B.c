#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

//cabeçalho de funções
ListaDupla* criar_lista();
NoAVL* criar_no_avl(Paciente paciente);
int altura_avl(NoAVL* no);
int fator_balanceamento(NoAVL* no);
NoAVL* rotacionar_direita(NoAVL* y);
NoAVL* rotacionar_esquerda(NoAVL* x);
NoAVL* inserir_avl(NoAVL* raiz, Paciente paciente);
void inserir_ordenado(ListaDupla* lista, Paciente paciente);
Paciente* buscar_lista(ListaDupla* lista, char* nome);
Paciente* buscar_avl(NoAVL* raiz, char* nome);
void exibir_paciente(Paciente* paciente);
void listar_pacientes_lista(ListaDupla* lista);
void listar_pacientes_avl(NoAVL* raiz);
void limpar_string(char* str);
void carregar_pacientes(ListaDupla* lista_m, NoAVL** raiz_l, char* nome_arquivo);
void cadastrar_paciente(ListaDupla* lista_m, NoAVL** raiz_l);
void alterar_registro(ListaDupla* lista_m, NoAVL* raiz_l);
void salvar_pacientes_moises(ListaDupla* lista, const char* nome_arquivo);
void salvar_pacientes_liz(NoAVL* raiz, FILE* arquivo);
void salvar_pacientes_liz_arquivo(NoAVL* raiz, const char* nome_arquivo);
void salvar_pacientes(ListaDupla* lista_m, NoAVL* raiz_l);
void menu_moises(ListaDupla* lista);
void menu_liz(NoAVL* raiz);
void menu_principal(ListaDupla* lista_m, NoAVL* raiz_l);
void limpar_tela();
//

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

    salvar_pacientes(lista_m, raiz_l);
    
    // Liberar memória (a ser implementado)
    return 0;
}

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

// Função para criar um paciente 
void cadastrar_paciente(ListaDupla* lista_m, NoAVL** raiz_l) {
    Paciente paciente;
    char sexo;

    printf("Digite o nome do paciente: ");
    scanf(" %[^\n]", paciente.nome);

    printf("Digite o sexo do paciente (M/F): ");
    scanf(" %c", &sexo);
    paciente.sexo = sexo;

    printf("Digite a data de nascimento (dd/mm/aaaa): ");
    scanf(" %s", paciente.nascimento);

    printf("Digite a data da última consulta (dd/mm/aaaa): ");
    scanf(" %s", paciente.ultima_consulta);

    if (sexo == 'M') {
        inserir_ordenado(lista_m, paciente);
        printf("Paciente cadastrado na lista do Moises.\n");
    } else if (sexo == 'F') {
        *raiz_l = inserir_avl(*raiz_l, paciente);
        printf("Paciente cadastrado na arvore da Liz.\n");
    } else {
        printf("Sexo inválido. Use 'M' para masculino ou 'F' para feminino.\n");
    }
}

// Função para alterar um registro de paciente
void alterar_registro(ListaDupla* lista_m, NoAVL* raiz_l){
    char nome[100];
    int menu;

    printf("Digite o nome do paciente que deseja alterar: ");
    scanf(" %[^\n]", nome);

    // Busca na lista do Moisés
    Paciente* paciente = buscar_lista(lista_m, nome);
    if (paciente != NULL) {
        printf("Paciente encontrado na lista do Moises.\n");
    } else {
        // Busca na árvore da Liz
        paciente = buscar_avl(raiz_l, nome);
        if (paciente != NULL) {
            printf("Paciente encontrado na arvore da Liz.\n");
        } else {
            printf("Paciente nzo encontrado.\n");
            return;
        }
    }

    setbuf(stdin, NULL);
    getchar();

    printf("O que deseja alterar?\n");
    printf("1. Nome\n");
    printf("2. Sexo\n");
    printf("3. Data de Nascimento\n");
    printf("4. Data da Última Consulta\n");
    printf("Sua escolha: ");
    scanf("%d", &menu);

    switch(menu){
        case 1:
            printf("Digite o novo nome: ");
            char novo_nome[100];
            scanf(" %[^\n]", novo_nome);
            strcpy(paciente->nome, novo_nome);

            setbuf(stdin, NULL);
            printf("Registro do paciente alterado com sucesso.\n");
        break;
        case 2:
            printf("Digite o novo sexo (M/F): ");
            char novo_sexo;
            scanf(" %c", &novo_sexo);
            if (novo_sexo == 'M' || novo_sexo == 'F') {
                paciente->sexo = novo_sexo;
            }

            setbuf(stdin, NULL);
            printf("Registro do paciente alterado com sucesso.\n");
        break;
        case 3:
            printf("Digite a nova data de nascimento (dd/mm/aaaa):");
            char nova_nascimento[11];
            scanf(" %s", nova_nascimento);
            strcpy(paciente->nascimento, nova_nascimento);

            setbuf(stdin, NULL);
            printf("Registro do paciente alterado com sucesso.\n");
        break;
        case 4:
            printf("Digite a nova data da última consulta (dd/mm/aaaa):");
            char nova_consulta[11];
            scanf(" %s", nova_consulta);
            strcpy(paciente->ultima_consulta, nova_consulta);

            setbuf(stdin, NULL);
            printf("Registro do paciente alterado com sucesso.\n");
        break;
        case 5:
            printf("Voltando ao menu principal.\n");
            limpar_tela();
        break;
        default:
            printf("Opção inválida.\n");
    } while(menu != 5);
}

// Função para salvar os pacientes da lista do Moisés em um arquivo
void salvar_pacientes_moises(ListaDupla* lista, const char* nome_arquivo) {
    FILE* arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar os pacientes do Moisés.\n");
        return;
    }

    NoLista* atual = lista->inicio;
    while (atual != NULL) {
        fprintf(arquivo, "%s, %c, %s, %s\n", atual->paciente.nome, atual->paciente.sexo, atual->paciente.nascimento, atual->paciente.ultima_consulta);
        atual = atual->proximo;
    }

    fclose(arquivo);
    printf("Pacientes do Moisés salvos em %s.\n", nome_arquivo);
}

// Função para salvar os pacientes da árvore da Liz em um arquivo
void salvar_pacientes_liz(NoAVL* raiz, FILE* arquivo) {
    if (raiz == NULL) return;

    salvar_pacientes_liz(raiz->esquerda, arquivo);
    fprintf(arquivo, "%s, %c, %s, %s\n", raiz->paciente.nome, raiz->paciente.sexo, raiz->paciente.nascimento, raiz->paciente.ultima_consulta);
    salvar_pacientes_liz(raiz->direita, arquivo);
}

// Função para salvar os pacientes da árvore da Liz em um arquivo
void salvar_pacientes_liz_arquivo(NoAVL* raiz, const char* nome_arquivo) {
    FILE* arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar os pacientes da Liz.\n");
        return;
    }

    salvar_pacientes_liz(raiz, arquivo);
    fclose(arquivo);
    printf("Pacientes da Liz salvos em %s.\n", nome_arquivo);
}

// Função para salvar todos os pacientes ao fechar o programa
void salvar_pacientes(ListaDupla* lista_m, NoAVL* raiz_l) {
    salvar_pacientes_moises(lista_m, "pacientes_moises.txt");
    salvar_pacientes_liz_arquivo(raiz_l, "pacientes_liz.txt");
}

// Função para exibir o menu de pacientes do Moisés
void menu_moises(ListaDupla* lista) {
    int opcao;
    char nome[100];

    setbuf(stdin, NULL);

    do {
        printf("\n--- Pacientes do Moises ---\n");
        printf("1. Consultar paciente\n");
        printf("2. Listar todos os pacientes\n");
        printf("3. Cadastrar paciente\n");
        printf("4. Alterar cadastro do paciente\n");
        printf("5. Voltar\n");
        printf("Sua escolha: ");
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
                cadastrar_paciente(lista, NULL);
                break;
            case 4:
                alterar_registro(lista, NULL);
                break;
            case 5:
                printf("Voltando ao menu principal.\n");
                limpar_tela();
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

    setbuf(stdin, NULL);

    do {
        printf("\n--- Pacientes da Liz ---\n");
        printf("1. Consultar paciente\n");
        printf("2. Listar todos os pacientes\n");
        printf("3. Cadastrar paciente\n");
        printf("4. Alterar cadastro do paciente\n");
        printf("5. Voltar\n");
        printf("Sua escolha: ");
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
                getchar();
                break;
            case 3:
                cadastrar_paciente(NULL, &raiz);
                break;
            case 4:
                alterar_registro(NULL, raiz);
                break;
            case 5:
                printf("Voltando ao menu principal.\n");
                limpar_tela();
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 5);
}

// Função para exibir o menu principal
void menu_principal(ListaDupla* lista_m, NoAVL* raiz_l) {
    int opcao;

    setbuf(stdin, NULL);

    do {
        printf("\n--- Menu Principal ---\n");
        printf("1. Pacientes do Moises\n");
        printf("2. Pacientes da Liz\n");
        printf("3. Finalizar programa\n");
        printf("Sua escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                limpar_tela();
                menu_moises(lista_m);
                break;
            case 2:
                limpar_tela();
                menu_liz(raiz_l);
                break;
            case 3:
                printf("Finalizando programa.\n");
                break;
            default:
                printf("Opção inválida.\n");
                limpar_tela();
        }
    } while (opcao != 3);
}

//função para limpar a tela, dependendo do sistema operacional
void limpar_tela(){
    #ifdef _WIN32
        system("cls"); 
    #else
        system("clear"); 
    #endif
}