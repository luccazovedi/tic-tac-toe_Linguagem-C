#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>

struct Jogador { // Declaração da estrutura do Jogador.
    char nome[50];
    int x, y;
};

struct Jogador j1;
struct Jogador j2;

struct Fila { // Declaração da estrutura da Fila.
    int x, y;
    char simbolo;
    struct Fila* proximo;
};

struct Fila* primeiro = NULL;
struct Fila* ultimo = NULL;

struct Pilha { // Declaração da estrutura da Pilha.
    int x, y;
    char simbolo;
    struct Fila* ponteiro;
} *topo, *topoAux, *temp;

int statusRetroceder = 0; // Utilizado para controlar a quantidade de retrocessos de uma partida.
int checagem = 0; // Utilizada para receber o retorno TRUE (1) or FALSE (0) das checagens do Computador.
int modoDeJogo = 0; // Utilizado para armazenar o Modo de Jogo.
int ordemJogador; // Utilizado para controlar a troca de turnos entre Jogadores.
int contadorDeJogadas = 0; // Utilizado para contar as jogadas.
int contadorDeJogadasAux; // Utilizado para realizar operações sem mudar o contador de jogadas oficial.
int escolha; // Utilizado para armazenar as escolhas dos Menus.
int escolhaDificuldade; // Utilizado para armazenar a escolha da dificuldade do Modo Computador.
char tabuleiro[3][3]; // Utilizado para armazenar as jogadas.

void menuModoDeJogo();
void menuNormal();
void menuComputador();
void jogadaComputador(int escolhaDificuldade);
void jogoDaVelha();
void menuRetrocederRodadas();
void reiniciarJogo();
void retrocederUmaRodada();
void retrocederRodadaQualquer();
void checarPosicao();
void checarEvento();
void inicializarTabuleiro();
void imprimirTabuleiro();
void inverterNome();
void jogadaComputadorAleatoria();
int jogadaComputadorChecagem();
int checagemComputadorLinhas();
int checagemComputadorColunas();
int checagemComputadorDiagonais();
void inserirNaFila(int x, int y, char simbolo);
void removerDaFila();
void imprimirFila();
void limparFila();
void criarPilha();
void inserirNaPilha(int x, int y, char simbolo);
void removerDaPilha();
void imprimirPilha();
void limparPilha();
void menuRepetirJogadas();
void repetirTodasJogadas();
void repetirJogadaPorJogada();
void repetirJogadaQualquer();

int main() {

    srand(time(NULL));

    menuModoDeJogo();

    return 0;
}

void menuModoDeJogo() { // Menu Principal, aqui são feitas as escolhas básicas.
    printf("\n[JOGO-DA-VELHA] Bem-vindo(a)! Escolha qual modo de jogo deseja jogar.");
    printf("\n(1) Normal (Jogador X Jogador);");
    printf("\n(2) Computador (Jogador X Computador);");
    printf("\n(3) Sair.");
    do {
        printf("\n>: ");
        scanf("%d", &escolha);
        if (escolha != 1 && escolha != 2 && escolha != 3) { // Tratamento de Excessão sobre digitação inválida.
            printf("\n[ERRO] Escolha inexistente! Tente novamente.\n");
        }
    } while (escolha != 1 && escolha != 2 && escolha != 3);

    if (escolha == 1) {
        modoDeJogo = 1;
        menuNormal();
    }
    else if (escolha == 2) {
        modoDeJogo = 2;
        menuComputador();
    }
    else {
        exit(0);
    }
}

void menuNormal() { // Menu do Modo de Jogo Normal (JxJ).
    criarPilha(); // Cria a pilha antes de iniciar uma nova partida.
    limparPilha(); // Limpa a pilha antes de iniciar uma nova partida.
    system("cls");
    printf("\n[MODO-NORNAL] O Modo Normal (JxJ) foi escolhido!");
    printf("\n[MODO-NORMAL] Qual o nome do Jogador 1? ");
    scanf("%s", &j1.nome);
    printf("[MODO-NORMAL]Qual o nome do Jogador 2? ");
    scanf("%s", &j2.nome);

    inicializarTabuleiro();
    system("cls");
    printf("\n[DICA] Ao digitar (-1 -1), o menu para retroceder jogadas se abre.\n");
    imprimirTabuleiro();
    jogoDaVelha();
}

void menuComputador() { // Menu Principal do Modo de Jogo Computador (JxC).
    criarPilha(); // Cria a pilha antes de iniciar uma nova partida.
    limparPilha(); // Limpa a pilha antes de iniciar uma nova partida.
    system("cls");
    printf("\n[MODO-COMPUTADOR] O Modo Computador (JxC) foi escolhido!");
    printf("\n[MODO-COMPUTADOR] Qual o seu nome? ");
    scanf("%s", j1.nome);
    inverterNome(j1.nome);
    printf("\n[MODO-COMPUTADOR] Qual a dificuldade do Computador?");
    printf("\n(1) Iniciante;");
    printf("\n(2) Intermediário;");
    printf("\n(3) Avançado;");
    printf("\n(4) Experiente;");
    printf("\n(5) Especial.");
    do {
        printf("\n>: ");
        scanf("%d", &escolhaDificuldade);
        if (escolhaDificuldade != 1 && escolhaDificuldade != 2 && escolhaDificuldade != 3 && escolhaDificuldade != 4 && escolhaDificuldade != 5) {
            printf("\n[ERRO] Escolha de dificuldade inexistente! Tente novamente.\n");
        }
    } while (escolhaDificuldade != 1 && escolhaDificuldade != 2 && escolhaDificuldade != 3 && escolhaDificuldade != 4 && escolhaDificuldade != 5);

    inicializarTabuleiro();
    system("cls");
    printf("\n[DICA] Ao digitar (-1 -1), o menu para retroceder jogadas se abre.\n");
    imprimirTabuleiro();
    jogoDaVelha();
}

void jogadaComputador(int escolhaDificuldade) { // De acordo com a escolha da dificuldade, o computador realiza uma quantidade de checagens.
    if (escolhaDificuldade == 1) { // Iniciante. Joga de forma aleatória.
        jogadaComputadorAleatoria();
    }
    else if (escolhaDificuldade == 2) { // Intermediário. Checa apenas as linhas, caso não haja nada para bloquear, joga de forma aleatória.
        checagem = checagemComputadorLinhas();
        if (checagem == 0) {
            jogadaComputadorAleatoria();
        }
    }
    else if (escolhaDificuldade == 3) { // Avançado. Checa apenas as linhas e as colunas, caso não haja nada para bloquear, joga de forma aleatória.
        checagem = checagemComputadorLinhas();
        if (checagem == 0) {
            checagem = checagemComputadorColunas();
            if (checagem == 0) {
                jogadaComputadorAleatoria();
            }
        }
    }
    else if (escolhaDificuldade == 4) { // Experiente. Checa linhas, colunas e diagonais, caso não haja nada para bloquear, joga de forma aleatória.
        checagem = checagemComputadorLinhas();
        if (checagem == 0) {
            checagem = checagemComputadorColunas();
            if (checagem == 0) {
                checagem = checagemComputadorDiagonais();
                if (checagem == 0) {
                    jogadaComputadorAleatoria();
                }
            }
        }
    }
    else { // Especial. Antes de checar linhas, colunas e diagonais para se defender, ele as checa para atacar e, caso não haja nada para bloquear ou atacar, joga de forma aleatória.
        checagem = jogadaComputadorChecagem();
        if (checagem == 0) {
            checagem = checagemComputadorLinhas();
            if (checagem == 0) {
                checagem = checagemComputadorColunas();
                if (checagem == 0) {
                    checagem = checagemComputadorDiagonais();
                    if (checagem == 0) {
                        jogadaComputadorAleatoria();
                    }
                }
            }
        }
    }
}

void jogoDaVelha() { // Função que coordena o jogo.
    do {
        if (ordemJogador == 1) { // Se a ordem for igual a 1, significa que é a vez do J1.
            printf("\n[JOGO-DA-VELHA] %s, digite o posicionamento da jogada [X], [Y]: ", j1.nome); // Pede a posição no tabuleiro.
            scanf("%d %d", &j1.x, &j1.y); // Armazena na estrutura.
            checarPosicao(); // Checa a integridade da posição digitada.
            ordemJogador = 2; // Muda o turno.
            imprimirTabuleiro(); // Exibe o tabuleiro.
            checarEvento(); // Checa se houve vitória, derrota ou empate.
        }
        else if (ordemJogador == 2) {
            if (modoDeJogo == 1) {
                printf("\n[JOGO-DA-VELHA] %s, digite o posicionamento da jogada [X], [Y]: ", j2.nome);
                scanf("%d %d", &j2.x, &j2.y);
            }
            else { // Caso o Modo de Jogo seja Computador (JxC), quem faz a jogada é a função do mesmo.
                jogadaComputador(escolhaDificuldade);
            }
            checarPosicao();
            ordemJogador = 1;
            imprimirTabuleiro();
            checarEvento();
        }
    } while (ordemJogador <= 2); // Tudo isso ocorre enquanto a variável "ordemJogador" for menor ou igual a 2. Ela se torna igual a dois quando um jogador ganha, perde ou o jogo empata.

    imprimirPilha(); // Exibe a pilha.

    topoAux = topo; // Nessa etapa, a pilha já não possui importância alguma, pois poderia ser manipulada apenas no retrocesso dentro da partida.
    criarPilha();
    while (topoAux != NULL) { // Portanto, aqui é feita a sua inversão para ser passada na ordem correta para a fila.
        inserirNaPilha(topoAux->x, topoAux->y, topoAux->simbolo);
        topoAux = topoAux->ponteiro;
    }
    
    topoAux = topo; // A pilha copia seu conteúdo para a fila.
    while (topoAux != NULL) {
        inserirNaFila(topoAux->x, topoAux->y, topoAux->simbolo);
        topoAux = topoAux->ponteiro;
    }

    imprimirFila(); // Exibe a fila.

    printf("\n[JOGO-DA-VELHA] O jogo foi finalizado, o que deseja fazer?"); // Menu de Final de Partida.
    printf("\n(1) Exibir as jogadas da partida;");
    printf("\n(2) Jogar novamente;");
    printf("\n(3) Alterar o Modo de Jogo;");
    printf("\n(4) Sair.");
    do {
        printf("\n>: ");
        scanf("%d", &escolha);
        if (escolha != 1 && escolha != 2 && escolha != 3 && escolha != 4) { // Tratamento de Excessão sobre digitação inválida.
            printf("\n[ERRO] Escolha inexistente! Tente novamente.\n");
        }
     } while (escolha != 1 && escolha != 2 && escolha != 3 && escolha != 4);

    if (escolha == 1) {
        contadorDeJogadasAux = contadorDeJogadas; // O contador auxiliar é iniciado, pois é utilizado na lógica de repetição de jogada.
        menuRepetirJogadas();
    }
    else if (escolha == 2) { // Antes de jogar novamente, as estruturas de dados são limpas.
        limparFila();
        limparPilha();
        if (modoDeJogo == 1) {
            menuNormal();
        }
        else {
            menuComputador();
        }
    }
    else if (escolha == 3) {
        limparFila();
        menuModoDeJogo();
    }
    else {
        exit(0);
    }
}

void menuRetrocederRodadas() { // Menu de Retrocesso de Jogadas durante a Partida.
    char escolha;
    printf("\n[RETROCEDER-RODADA] Um atalho para o menu dentro do jogo foi utilizado.");
    printf("\n(1) Reiniciar a partida;");
    printf("\n(2) Voltar uma rodada;");
    printf("\n(3) Voltar uma quantidade de rodadas;");
    printf("\n(4) Sair.");
    printf("\n\n[AVISO] A regra permite a apenas um retrocesso de rodada, escolha com sabedoria.");
    do {
        printf("\n>: ");
        scanf("%d", &escolha);
        if (escolha != 1 && escolha != 2 && escolha != 3 && escolha != 4) { // Tratamento de Excessão sobre digitação inválida.
            printf("\n[ERRO] Escolha inexistente! Tente novamente.\n");
        }
    } while(escolha != 1 && escolha != 2 && escolha != 3 && escolha != 4);
    if (escolha == 1) {
        reiniciarJogo();
    }
    else if (escolha == 2) {
        retrocederUmaRodada();
    }
    else if (escolha == 3) {
        retrocederRodadaQualquer();
    }
    else {
        exit(0);
    }
}

void reiniciarJogo() { // Essa opção reinicia a partida e não é contada na regra.
    system("cls");
    printf("\n[SISTEMA] O jogo foi reiniciado.");
    limparPilha();
    inicializarTabuleiro();
    jogoDaVelha();
}

void retrocederUmaRodada() { // Essa opção volta para a jogada anterior.
    if (contadorDeJogadas < 1) {
        printf("\n[ERRO] Quantidade de rodadas insuficiente! Tente novamente.\n");
        jogoDaVelha();
    }
    if (ordemJogador == 1) { // Checa a ordem dos turnos, limpa o tabuleiro e diminui o contador.
        ordemJogador = 2;
        tabuleiro[j2.x][j2.y] = '-';
        contadorDeJogadas--;
    }
    else {
        ordemJogador = 1;
        tabuleiro[j1.x][j1.y] = '-';
        contadorDeJogadas--;
    }
    removerDaPilha(); // Remove a última jogada da pilha.
    printf("\n[SISTEMA] Uma rodada foi retrocedida.\n");
    statusRetroceder = 1; // Contabiliza o retrocesso.
    imprimirTabuleiro();
    jogoDaVelha();
}

void retrocederRodadaQualquer() { // Essa opção deixa o jogador escolher uma rodada para retroceder.
    int rodadas, i;
    do {
        printf("\n[RETROCEDER-RODADA] Escolha quantas rodadas deseja retroceder: ");
        scanf("%d", &rodadas);
        if (rodadas < 0 && rodadas > contadorDeJogadas) {
            printf("\n[ERRO] Quantidade de rodadas insuficiente! Tente novamente.\n");
        }
    } while(rodadas < 0 && rodadas > contadorDeJogadas);
    if (rodadas % 2 == 0) { // Checa o turno.
        ordemJogador = 1;
    }
    else {
        ordemJogador = 2;
    }
    contadorDeJogadas -= rodadas; // Altera o contador de jogadas de acordo com as rodadas retrocedidas.
    for (i = 0; i < rodadas; i++) { // Limpa o tabuleiro e a(s) última(s) jogada(s) da pilha.
        tabuleiro[topo->x][topo->y] = '-';
        removerDaPilha();
    }
    printf("\n[SISTEMA] O jogo foi retrocedido em %d rodadas.\n", rodadas);
    statusRetroceder = 1; // Contabiliza o retrocesso.
    imprimirTabuleiro();
    jogoDaVelha();
}

void checarPosicao() { // Função que checa a integridade das posições digitadas no tabuleiro.
    if (ordemJogador == 1) {
        if ((tabuleiro[j1.x][j1.y] != '-' && j1.x != -1 && j1.y != -1) || (j1.x < -1 || j1.x > 2 || j1.y < -1 || j1.y > 2)) {
            printf("\n[ERRO] %s, posicionamento ocupado ou inexistente.\n", j1.nome);
            jogoDaVelha();
        }
        else {
            if (j1.x == -1 && j1.y == -1) { // Código para entrar no Menu de Retrocesso de Jogada.
                if (statusRetroceder == 1) { // Se já houve um retrocesso, mostra uma mensagem de erro, pois apenas 1 retrocesso é permitido.
                    printf("\n[AVISO] O limite de retrocessos (1) foi excedido! Menu de retrocessos desligado.\n");
                    jogoDaVelha();
                }
                else {
                    menuRetrocederRodadas();
                }
            }
            tabuleiro[j1.x][j1.y] = 'X'; // Armazena a jogada no tabuleiro.
            contadorDeJogadas++; // Incrementa a quantidade de jogadas.
            //inserirNaFila(j1.x, j1.y, 'X');
            inserirNaPilha(j1.x, j1.y, 'X'); // Insere a jogada na pilha.
        }
    }
    else {
        if ((tabuleiro[j2.x][j2.y] != '-' && j2.x != -1 && j2.y != -1) || (j2.x < -1 || j2.x > 2 || j2.y < -1 || j2.y > 2)) {
            if (modoDeJogo == 1) {
                printf("\n[ERRO] %s, posicionamento ocupado ou inexistente.\n", j2.nome);
            }
            jogoDaVelha();
        }
        else {
            if (j2.x == -1 && j2.y == -1) {
                if (statusRetroceder == 1) {
                    printf("\n[AVISO] O limite de retrocessos (1) foi excedido! Menu de retrocessos desligado.\n");
                    jogoDaVelha();
                }
                else {
                    menuRetrocederRodadas();
                }
            }
            tabuleiro[j2.x][j2.y] = 'O';
            contadorDeJogadas++;
            //inserirNaFila(j2.x, j2.y, 'O');
            inserirNaPilha(j2.x, j2.y, 'O');
            if (modoDeJogo == 2) { // Se estiver no Modo Computador (JxC), é mostrada na tela a jogada dele.
                printf("\n[JOGO-DA-VELHA] %s digitou: [%d], [%d]\n", j2.nome, j2.x, j2.y);
            }
        }
    }
}

void checarEvento() { // Função que checa os eventos de vitória, derrota e empate.
    if (ordemJogador == 2) {
        if (tabuleiro[0][0] == 'X' && tabuleiro[0][1] == 'X' && tabuleiro[0][2] == 'X') {
            ordemJogador = 3; // Há um campeão.
        }
        else if (tabuleiro[1][0] == 'X' && tabuleiro[1][1] == 'X' && tabuleiro[1][2] == 'X') {
            ordemJogador = 3; // Há um campeão.
        }
        else if (tabuleiro[2][0] == 'X' && tabuleiro[2][1] == 'X' && tabuleiro[2][2] == 'X') {
            ordemJogador = 3; // Há um campeão.
        }

        else if (tabuleiro[0][0] == 'X' && tabuleiro[1][0] == 'X' && tabuleiro[2][0] == 'X') {
            ordemJogador = 3; // Há um campeão.
        }
        else if (tabuleiro[0][1] == 'X' && tabuleiro[1][1] == 'X' && tabuleiro[2][1] == 'X') {
            ordemJogador = 3; // Há um campeão.
        }
        else if (tabuleiro[0][2] == 'X' && tabuleiro[1][2] == 'X' && tabuleiro[2][2] == 'X') {
            ordemJogador = 3; // Há um campeão.
        }

        else if (tabuleiro[0][0] == 'X' && tabuleiro[1][1] == 'X' && tabuleiro[2][2] == 'X') {
            ordemJogador = 3; // Há um campeão.
        }
        else if (tabuleiro[0][2] == 'X' && tabuleiro[1][1] == 'X' && tabuleiro[2][0] == 'X') {
            ordemJogador = 3; // Há um campeão.
        }
        
        if (ordemJogador == 3) {
            system("cls");
            printf("\n[JOGO-DA-VELHA] Fim de jogo! O vencedor se chama: %s!\n", j1.nome);
        }
    }
    else if (ordemJogador == 1) {
        if (tabuleiro[0][0] == 'O' && tabuleiro[0][1] == 'O' && tabuleiro[0][2] == 'O') {
            ordemJogador = 3; // Há um campeão.
        }
        else if (tabuleiro[1][0] == 'O' && tabuleiro[1][1] == 'O' && tabuleiro[1][2] == 'O') {
            ordemJogador = 3; // Há um campeão.
        }
        else if (tabuleiro[2][0] == 'O' && tabuleiro[2][1] == 'O' && tabuleiro[2][2] == 'O') {
            ordemJogador = 3; // Há um campeão.
        }

        else if (tabuleiro[0][0] == 'O' && tabuleiro[1][0] == 'O' && tabuleiro[2][0] == 'O') {
            ordemJogador = 3; // Há um campeão.
        }
        else if (tabuleiro[0][1] == 'O' && tabuleiro[1][1] == 'O' && tabuleiro[2][1] == 'O') {
            ordemJogador = 3; // Há um campeão.
        }
        else if (tabuleiro[0][2] == 'O' && tabuleiro[1][2] == 'O' && tabuleiro[2][2] == 'O') {
            ordemJogador = 3; // Há um campeão.
        }

        else if (tabuleiro[0][0] == 'O' && tabuleiro[1][1] == 'O' && tabuleiro[2][2] == 'O') {
            ordemJogador = 3; // Há um campeão.
        }
        else if (tabuleiro[0][2] == 'O' && tabuleiro[1][1] == 'O' && tabuleiro[2][0] == 'O') {
            ordemJogador = 3; // Há um campeão.
        }

        if (ordemJogador == 3) {
            system("cls");
            if (modoDeJogo == 1) {
                printf("\n[JOGO-DA-VELHA] Fim de jogo! O vencedor se chama: %s!\n", j2.nome);
            }
            else {
                printf("\n[JOGO-DA-VELHA] Vergonhoso, %s! %s lhe derrotou!\n", j1.nome, j2.nome);
            }
        }
    }

    if (contadorDeJogadas == 9 && ordemJogador != 3) {
            system("cls");
            printf("\n[JOGO-DA-VELHA] Fim de jogo! Foi um belo duelo, no entanto, houve um empate.\n");
            ordemJogador = 4; // Há um empate.
    }
}

void inicializarTabuleiro() { // Função que limpa o tabuleiro e as variáveis de turno, quantidade de jogadas e contagem de retrocessos.
    int i, j;
    for (i = 0; i < 3; i++) {
       for (j = 0; j < 3; j++) {
            tabuleiro[i][j] = '-';
        } 
    }

    ordemJogador = 1;
    contadorDeJogadas = 0;
    statusRetroceder = 0;
}

void imprimirTabuleiro() { // Função que exibe o tabuleiro.
    int i, j;
    printf("\n");
    printf("   0 1 2\n");
    printf("   _ _ _\n");
    for (i = 0; i < 3; i++) {
        printf("%d |", i);
       for (j = 0; j < 3; j++) {
            printf("%c ", tabuleiro[i][j]);
        }
        printf("\n");
    }
}

void inverterNome() { // Função que inverte o nome do J1 para dá-lo ao Computador no Modo Computador (JxC).
    strcpy(j2.nome, j1.nome);
    int tamanhoNome = strlen(j2.nome);
    int i, j, aux;

    for (i = 0, j = tamanhoNome - 1; i < j; i++, j--) {
        aux = j2.nome[i];
        j2.nome[i] = j2.nome[j];
        j2.nome[j] = aux;
    }
}

void jogadaComputadorAleatoria() { // Função em que o Computador jogada de forma aleatória, sem nenhuma checagem.
    j2.x = rand() % 3;
    j2.y = rand() % 3;
}

int jogadaComputadorChecagem() { // Função em que o Computador checa se pode atacar em qualquer local.
    if ((tabuleiro[0][0] == 'O' && tabuleiro[0][1] == 'O' && tabuleiro[0][2] == '-') || (tabuleiro[0][0] == 'O' && tabuleiro[0][2] == 'O' && tabuleiro[0][1] == '-') || (tabuleiro[0][1] == 'O' && tabuleiro[0][2] == 'O' && tabuleiro[0][0] == '-')) {
        j2.x = 0;
        j2.y = rand() % 3;
        return 1;
    }
    else if ((tabuleiro[1][0] == 'O' && tabuleiro[1][1] == 'O' && tabuleiro[1][2] == '-') || (tabuleiro[1][0] == 'O' && tabuleiro[1][2] == 'O' && tabuleiro[1][1] == '-') || (tabuleiro[1][1] == 'O' && tabuleiro[1][2] == 'O' && tabuleiro[1][0] == '-')) {
        j2.x = 1;
        j2.y = rand() % 3;
        return 1;
    }
    else if ((tabuleiro[2][0] == 'O' && tabuleiro[2][1] == 'O' && tabuleiro[2][2] == '-') || (tabuleiro[2][0] == 'O' && tabuleiro[2][2] == 'O' && tabuleiro[2][1] == '-') || (tabuleiro[2][1] == 'O' && tabuleiro[2][2] == 'O' && tabuleiro[2][0] == '-')) {
        j2.x = 2;
        j2.y = rand() % 3;
        return 1;
    }
    else if ((tabuleiro[0][0] == 'O' && tabuleiro[1][0] == 'O' && tabuleiro[2][0] == '-') || (tabuleiro[0][0] == 'O' && tabuleiro[2][0] == 'O' && tabuleiro[1][0] == '-') || (tabuleiro[1][0] == 'O' && tabuleiro[2][0] == 'O' && tabuleiro[0][0] == '-')) {
        j2.x = rand() % 3;
        j2.y = 0;
        return 1;
    }
    else if ((tabuleiro[0][1] == 'O' && tabuleiro[1][1] == 'O' && tabuleiro[2][1] == '-') || (tabuleiro[0][1] == 'O' && tabuleiro[2][1] == 'O' && tabuleiro[1][1] == '-') || (tabuleiro[1][1] == 'O' && tabuleiro[2][1] == 'O' && tabuleiro[0][1] == '-')) {
        j2.x = rand() % 3;
        j2.y = 1;
        return 1;
    }
    else if ((tabuleiro[0][2] == 'O' && tabuleiro[1][2] == 'O' && tabuleiro[2][2] == '-') || (tabuleiro[0][2] == 'O' && tabuleiro[2][2] == 'O' && tabuleiro[1][2] == '-') || (tabuleiro[1][2] == 'O' && tabuleiro[2][2] == 'O' && tabuleiro[0][2] == '-')) {
        j2.x = rand() % 3;
        j2.y = 2;
        return 1;
    }
    else {
        return 0;
    }
}

int checagemComputadorLinhas() { // Função em que o Computador checa as linhas.
    if ((tabuleiro[0][0] == 'X' && tabuleiro[0][1] == 'X' && tabuleiro[0][2] == '-') || (tabuleiro[0][0] == 'X' && tabuleiro[0][2] == 'X' && tabuleiro[0][1] == '-') || (tabuleiro[0][1] == 'X' && tabuleiro[0][2] == 'X' && tabuleiro[0][0] == '-')) {
        j2.x = 0;
        j2.y = rand() % 3;
        return 1;
    }
    else if ((tabuleiro[1][0] == 'X' && tabuleiro[1][1] == 'X' && tabuleiro[1][2] == '-') || (tabuleiro[1][0] == 'X' && tabuleiro[1][2] == 'X' && tabuleiro[1][1] == '-') || (tabuleiro[1][1] == 'X' && tabuleiro[1][2] == 'X' && tabuleiro[1][0] == '-')) {
        j2.x = 1;
        j2.y = rand() % 3;
        return 1;
    }
    else if ((tabuleiro[2][0] == 'X' && tabuleiro[2][1] == 'X' && tabuleiro[2][2] == '-') || (tabuleiro[2][0] == 'X' && tabuleiro[2][2] == 'X' && tabuleiro[2][1] == '-') || (tabuleiro[2][1] == 'X' && tabuleiro[2][2] == 'X' && tabuleiro[2][0] == '-')) {
        j2.x = 2;
        j2.y = rand() % 3;
        return 1;
    }
    else {
        return 0;
    }
}

int checagemComputadorColunas() { // Função em que o Computador checa as colunas.
    if ((tabuleiro[0][0] == 'X' && tabuleiro[1][0] == 'X' && tabuleiro[2][0] == '-') || (tabuleiro[0][0] == 'X' && tabuleiro[2][0] == 'X' && tabuleiro[1][0] == '-') || (tabuleiro[1][0] == 'X' && tabuleiro[2][0] == 'X' && tabuleiro[0][0] == '-')) {
        j2.x = rand() % 3;
        j2.y = 0;
        return 1;
    }
    else if ((tabuleiro[0][1] == 'X' && tabuleiro[1][1] == 'X' && tabuleiro[2][1] == '-') || (tabuleiro[0][1] == 'X' && tabuleiro[2][1] == 'X' && tabuleiro[1][1] == '-') || (tabuleiro[1][1] == 'X' && tabuleiro[2][1] == 'X' && tabuleiro[0][1] == '-')) {
        j2.x = rand() % 3;
        j2.y = 1;
        return 1;
    }
    else if ((tabuleiro[0][2] == 'X' && tabuleiro[1][2] == 'X' && tabuleiro[2][2] == '-') || (tabuleiro[0][2] == 'X' && tabuleiro[2][2] == 'X' && tabuleiro[1][2] == '-') || (tabuleiro[1][2] == 'X' && tabuleiro[2][2] == 'X' && tabuleiro[0][2] == '-')) {
        j2.x = rand() % 3;
        j2.y = 2;
        return 1;
    }
    else {
        return 0;
    }
}

int checagemComputadorDiagonais() { // Função em que o Computador checa as diagonais.
    if ((tabuleiro[0][0] == 'X' && tabuleiro[1][1] == 'X' && tabuleiro[2][2] == '-') || (tabuleiro[0][0] == 'X' && tabuleiro[2][2] == 'X' && tabuleiro[1][1] == '-') || (tabuleiro[1][1] == 'X' && tabuleiro[2][2] == 'X' && tabuleiro[0][0] == '-')) {
        j2.x = rand() % 3;
        j2.y = j2.x;
        return 1;
    }
    else if (tabuleiro[0][2] == 'X' && tabuleiro[1][1] == 'X' && tabuleiro[2][0] == '-') {
        j2.x = 2;
        j2.y = 0;
        return 1;
    }
    else if (tabuleiro[0][2] == 'X' && tabuleiro[2][0] == 'X' && tabuleiro[1][1] == '-') {
        j2.x = j2.y = 1;
        return 1;
    }
    else if (tabuleiro[1][1] == 'X' && tabuleiro[2][0] == 'X' && tabuleiro[0][2] == '-') {
        j2.x = 0;
        j2.y = 2;
        return 1;
    }
    else {
        return 0;
    }
}

void inserirNaFila(int x, int y, char simbolo) { // Função de inserção na Fila.
    struct Fila* temp = (struct Fila*)malloc(sizeof(struct Fila));
    temp->x = x;
    temp->y = y;
    temp->simbolo = simbolo;
    temp->proximo = NULL;
    if (primeiro == NULL && ultimo == NULL) {
        primeiro = ultimo = temp;
        return;
    }
    ultimo->proximo = temp;
    ultimo = temp;
}

void removerDaFila() { // Função de remoção da Fila.
    struct Fila* temp = primeiro;
    if (primeiro == NULL) {
        printf("\n[SISTEMA] Fila vazia.");
        return;
    }
    if (primeiro == ultimo) {
        primeiro = ultimo = NULL;
    }
    else {
        primeiro = primeiro->proximo;
    }
    free(temp);
}

void imprimirFila() { // Função de impressão da Fila.
    struct Fila* temp = primeiro;
    printf("\n[SISTEMA] Elementos da Fila:\n");
    while (temp != NULL) {
        printf("[%d-%d-%c]", temp->x, temp->y, temp->simbolo);
        printf("\n");
        temp = temp->proximo;
    }
}

void limparFila() { // Função de limpeza da Fila.
    int i;
    for (i = 0; i < contadorDeJogadas; i++) {
        removerDaFila();
    }
}

void criarPilha() { // Função de criação da Pilha.
    topo = NULL;
}

void inserirNaPilha(int x, int y, char simbolo) { // Função de inserção na Pilha.
    if (topo == NULL) {
        topo = (struct Pilha*)malloc(1 * sizeof(struct Pilha));
        topo->ponteiro = NULL;
        topo->x = x;
        topo->y = y;
        topo->simbolo = simbolo;
    }
    else {
        temp = (struct Pilha*)malloc(1 * sizeof(struct Pilha));
        temp->ponteiro = topo;
        temp->x = x;
        temp->y = y;
        temp->simbolo = simbolo;
        topo = temp;
    }
}

void removerDaPilha() { // Função de remoção da Pilha.
    topoAux = topo;
    if (topoAux == NULL) {
        printf("[SISTEMA] Pilha vazia!\n");
        return;
    }
    else {
        topoAux = topoAux->ponteiro;
        free(topo);
        topo = topoAux;
    }
}

void imprimirPilha() { // Função de impressão da Pilha.
    topoAux = topo;
    if (topo == NULL) {
        printf("[SISTEMA] Pilha vazia.");
        return;
    }
    printf("\n[SISTEMA] Elementos da Pilha:\n");
    while (topoAux != NULL) {
        printf("[%d-%d-%c]", topoAux->x, topoAux->y, topoAux->simbolo);
        printf("\n");
        topoAux = topoAux->ponteiro;
    }
}

void limparPilha() { // Função de limpeza da Pilha.
    topoAux = topo;
    while (topoAux != NULL) {
        topoAux = topo->ponteiro;
        free(topo);
        topo = topoAux;
        topoAux = topoAux->ponteiro;
    }
    free(topoAux);
    topo = NULL;
}

void menuRepetirJogadas() { // Menu de Repetição de Jogadas.
    printf("\n[REPETIR-JOGADAS] Escolha como deseja exibir as jogadas:");
    printf("\n(1) Todas as jogadas de uma vez;");
    printf("\n(2) Passar jogada por jogada;");
    printf("\n(3) Escolher uma jogada qualquer;");
    printf("\n(4) Sair.");
    do {
        printf("\n>: ");
        scanf("%d", &escolha);
        if (escolha != 1 && escolha != 2 && escolha != 3 && escolha != 4) { // Tratamento de Excessão sobre digitação inválida.
            printf("\n[ERRO] Escolha inexistente! Tente novamente.\n");
        }
    } while (escolha != 1 && escolha != 2 && escolha != 3 && escolha != 4);
    if (escolha == 1) {
        repetirTodasJogadas();
    }
    else if (escolha == 2) {
        repetirJogadaPorJogada();
    }
    else if (escolha == 3) {
        repetirJogadaQualquer();
    }
    else {
        exit(0);
    }
}

void repetirTodasJogadas() { // Essa opção repete todas as jogadas de uma só vez.
    system("cls");
    inicializarTabuleiro();
    struct Fila* temp = primeiro;
    while (temp != NULL) {
        tabuleiro[temp->x][temp->y] = temp->simbolo;
        imprimirTabuleiro();
        temp = temp->proximo;
    }
    menuRepetirJogadas();
}

void repetirJogadaPorJogada() { // Essa opção repete uma jogada de cada vez, com a opção de voltar ou passar ao longo delas.
    int i, j = 0;
    inicializarTabuleiro();
    struct Fila* temp = primeiro;
    do {
        do {
            printf("\n[REPETIR-JOGADAS] (0) Voltar para a jogada anterior - (1) Passar para a jogada posterior.");
            printf("\n>: ");
            scanf("%d", &escolha);
            if ((escolha != 0 && escolha != 1) || (escolha == 0 && j < 1)) { // Tratamento de Excessão sobre digitação inválida.
                printf("\n[ERRO] Escolha inexistente! Tente novamente.\n");
            }
        } while((escolha != 0 && escolha != 1) || (escolha == 0 && j < 1));
        if (escolha == 1) {
            j++;
        }
        else {
            j--;
        }
        temp = primeiro;
        for (i = 0; i < j; i++) {
            tabuleiro[temp->x][temp->y] = temp->simbolo;
            temp = temp->proximo;
        }
        system("cls");
        imprimirTabuleiro();
        inicializarTabuleiro();
    } while(temp != NULL);
    menuRepetirJogadas();
}

void repetirJogadaQualquer() { // Essa opção repete uma jogada específica.
    int i, rodada;
    do {
        printf("\n[REPETIR-JOGADAS] Digite qual rodada deseja repetir: ");
        scanf("%d", &rodada);
        if (rodada < 0 || rodada > contadorDeJogadasAux) { // Tratamento de Excessão sobre digitação inválida.
            printf("\n[ERRO] Rodada inexistente! Tente novamente.\n");
        }
    } while(rodada < 0 || rodada > contadorDeJogadasAux);
    inicializarTabuleiro();
    struct Fila* temp = primeiro;
    for (i = 0; i < rodada; i++) {
        tabuleiro[temp->x][temp->y] = temp->simbolo;
        temp = temp->proximo;
    }
    system("cls");
    imprimirTabuleiro();
    menuRepetirJogadas();
}