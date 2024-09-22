/********************************************************
Trabalho de Laboratório de programação 2 - Campo minado
            Aluno: Leandro Peres Sobreira
********************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct
{
    int num;    // Guarda o que tem na celula
    int v;      // Indica se a celula é visivel ou não
}tCell;

void freeGame(tCell **tab){
    free(tab[0]);
    free(tab);
    tab = NULL;
}

/*  Imprime o tabuleiro do jogo para o usuario.
    (Tabela, tamanho de linhas, tamanho de colunas)*/
void impGame(tCell **tab, int tamL, int tamC){
    printf("/");
    for (int c = 0; c < tamC; c++)
    {
        printf("---");
    }
    printf("\\\n");
    for (int l = 0; l < tamL; l++)
    {
        printf("|");
        for (int c = 0; c < tamC; c++)
        {
            if (tab[l][c].v == 0)
            {
                printf(" . ");
            } else if (tab[l][c].v == 2){
                printf(" # ");
            }else{
                if(tab[l][c].num == 0){
                    printf("   ");
                } else if(tab[l][c].num == -1){
                    printf(" B ");
                } else {
                    printf(" %d ", tab[l][c].num);
                }   
            }
            
        }
        printf("| %d\n", l+1);
    }
    printf("\\");
    for (int c = 0; c < tamC; c++)
    {
        printf("---");
    }
    printf("/\n ");
    for (int c = 0; c < tamC; c++)
    {
        printf(" %c ", c+97);
    }
    printf("\n");
    
}

/*  Zera a matriz que foi passada por parâmetro, alem de ocultar todos os espaços.
    (Tabela, tamanho de linhas, tamanho de colunas, quantidade de espaços não descobertos)*/
void zeraTab(tCell **tab, int tamL, int tamC, int *qEspNdescob){
    for (int l = 0; l < tamL; l++)
    {
        for (int c = 0; c < tamC; c++)
        {
            tab[l][c].num = 0;
            tab[l][c].v = 0;
        }
        *qEspNdescob = tamL * tamC;
    }
}

/*  Cria alertas ao redor da bomba a qual o local foi passado por parâmeto.
    (Tabela, tamanho de linhas, tamanho de colunas, posição da linha da bomba, posção da coluna da bomba)*/
void criaNum(tCell **tab, int tamL, int tamC, int pBombL, int pBombC){
    for (int l = pBombL - 1; l <= pBombL + 1; l++)
    {
        for (int c = pBombC - 1; c <= pBombC + 1; c++)
        {
            if(l >= 0 && c >= 0 && l < tamL && c < tamC)
            {
                if (tab[l][c].num != -1)
                {
                    tab[l][c].num++;
                }
            }
        }
    }
}

/*  Cria uma bomba em um local aleatório ja colocando o alerta em casas adjacentes, e se o local da bomba ja estiver ocupado ou seja o local que o usuario digitou chama ela mesma criando a bomba em outro local.
    (Tabela, tamanho de linhas, tamanho de colunas, posição da linha selecionada, posição da coluna selecionada)*/
void criaBomb(tCell **tab, int tamL, int tamC, int pSelecL, int pSelecC){
    int c = rand()%tamC;
    int l = rand()%tamL;

    if (tab[l][c].num >= 0 && (l != pSelecL || c != pSelecC))
    {
        tab[l][c].num = -1;
        criaNum(tab, tamL, tamC, l, c);
    } else {
        criaBomb(tab, tamL, tamC, pSelecL, pSelecC);
    }
}

/*  Revela o espaço selecionado e quando em branco revela tambem seus adjacentes.
    (Tabela, tamanho de linhas, tamanho de colunas, posição da linha selecionada, posição da coluna selecionada, ptr da quantidade de espaço não descobertos)*/
void revelar(tCell **tab, int tamL, int tamC, int pSelecL, int pSelecC, int *qEspNdescob){
    
    if(tab[pSelecL][pSelecC].v == 0){
        tab[pSelecL][pSelecC].v = 1;
        *qEspNdescob = *qEspNdescob -1;
        if(tab[pSelecL][pSelecC].num == 0){
            for (int l = pSelecL - 1; l <= pSelecL + 1; l++)
            {
                for (int c = pSelecC - 1; c <= pSelecC + 1; c++)
                {
                    if(l >= 0 && c >= 0 && l < tamL && c < tamC)
                    {
                        revelar(tab, tamL, tamC, l, c, qEspNdescob);
                    }
                }
            }
        }
    }
}   

/*  Pede para o usuario digitar as cordenadas em que ele vai jogar,, se for uma opção invalida pede outra cordenada.
    (ptr da posição L, ptr da posição C, tamamnho de linhas, tamalho de colunas)*/
void selecCord(int *pSelecL, int *pSelecC, int tamL, int tamC){
    *pSelecL = 0;
    *pSelecC = 0;

    printf("Digite a linha desejada: ");
    scanf("%d", pSelecL);
    *pSelecL = *pSelecL -1;
    
    printf("Digite a coluna desejada: ");
    scanf(" %c", pSelecC);
    *pSelecC = *pSelecC - 97;

    if (*pSelecL < 0 || *pSelecL >= tamL || *pSelecC < 0 || *pSelecC >= tamC)
    {
        printf("Cordenada invalida!\n");
        selecCord(pSelecL, pSelecC, tamL, tamC);
    }
}

/*  Pede para o usuario digitar as cordenadas desejadas, se nela estiver um espaço não revalado coloca uma bandeira, se já tiver uma bandeira retira ela, se for um espaço revelado a função chama ela mesma
    (Tabela, tamanho de linhas, tamanho de colunas, ptr da posição da linha selecionada, ptr da posição da coluna selecionada)*/
void bandeira(tCell **tab, int tamL, int tamC, int *pSelectL, int *pSelectC){
    selecCord(pSelectL, pSelectC, tamL, tamC);
    if (tab[*pSelectL][*pSelectC].v != 1){
        if(tab[*pSelectL][*pSelectC].v == 0){
            tab[*pSelectL][*pSelectC].v = 2;
        }else{
            tab[*pSelectL][*pSelectC].v = 0;
        }
    }else{
        printf("Posicao ja revelada!\n");
        bandeira(tab, tamL, tamC, pSelectL, pSelectC);
    }
}

/*  Cria o tabuleiro do jogo */
tCell ** criaTab(int tamL, int tamC){
        
    tCell **tab = malloc(sizeof(tCell *) * tamL); // Matriz principal do jogo.
    tab[0] = malloc(sizeof(tCell) * tamL * tamC);
    for (int i = 1; i < tamL; i++)
    {
        tab[i] = tab[0] + (i * tamC);
    }

    return tab;
}

/*Inicia o jogo criando um novo tabuleiro*/
void initGame(tCell **tab, int tamL, int tamC, int *pSelecL, int *pSelecC, int *qEspNdescob, int qBomb){

    zeraTab(tab, tamL, tamC, qEspNdescob);
    impGame(tab, tamL, tamC);

    selecCord(pSelecL, pSelecC, tamL, tamC);

    for (int i = 0; i < qBomb; i++)
    {
        criaBomb(tab, tamL, tamC, *pSelecL, *pSelecC);
    }

    revelar(tab, tamL, tamC, *pSelecL, *pSelecC, qEspNdescob);
}

void opicoes(tCell **tab, int *tamL, int *tamC, int *qBomb, int *fNewTab){
    char opc;
    printf("Deseja mudar as configuracoes do tabuleiro? (Isso ira criar um novo jogo) (s/n): ");
    scanf(" %c", &opc);
    
    if (opc == 's' || opc == 'S')
    {
        freeGame(tab);
        *fNewTab = 1;
        printf("Digite a quantidade de linhas: ");
        scanf("%d", tamL);

        printf("Digite a quantidade de colunas: ");
        scanf("%d", tamC);
    
        printf("Digite a quantidade de bombas: ");
        scanf("%d", qBomb);
    }
    
}

/*  Salva a configuração das opções no arquivo "saveConfig.dat" e salva o jogo no arquivo "save.dat".
    (Tabela, tamanho de linhas, tamanho de colunas, quantidade de bombas, tempo decorrido, tempo pausado)*/
void salvar(tCell **tab, int tamL, int tamC, int qBomb, int qEspNdescob, clock_t timeD, clock_t timeP){
    
    FILE *saveConfig = fopen("saveConfig.dat", "wb"); //O arquivo é aberto no modo escrita em binario.
    if (saveConfig == NULL) //Verificação se o arquivo abriu corretamente.
    {
        printf("ERRO! - O arquivo saveConfig.dat nao pode ser aberto!\n");
    }
    fwrite(&tamL, sizeof(tamL), 1, saveConfig);
    fwrite(&tamC, sizeof(tamC), 1, saveConfig);
    fwrite(&qBomb, sizeof(qBomb), 1, saveConfig);
    fclose(saveConfig);

    clock_t timeSave = timeD - timeP;
    FILE *save = fopen("save.dat", "wb"); //O arquivo é aberto no modo escrita em binario.
    if (save == NULL)
    {
        printf("ERRO! - O arquivo save.dat nao pode ser aberto!\n");
    }
    
    for (int l = 0; l < tamL; l++) //Salvando a matriz principal do jogo.
    {
        for (int c = 0; c < tamC; c++)
        {
            fwrite(&tab[l][c].num, sizeof(tab[l][c].num), 1, save);
            fwrite(&tab[l][c].v, sizeof(tab[l][c].v), 1, save);
        }
    }
    fwrite(&qEspNdescob, sizeof(qEspNdescob), 1, save);
    fwrite(&timeSave, sizeof(timeSave), 1, save);
    fclose(save);
}

/*  Carrega as configurações do jogo (tamanho do tabuleiro e quantidade de bombas).
    (ptr tamanho de linhas, ptr tamanho de colunas, ptr quantidade de bombas)*/
void carregarConfig(int *tamL, int *tamC, int *qBomb){
    FILE *saveConfig = fopen("saveConfig.dat", "rb");
    if (saveConfig == NULL)
    {
        printf("ERRO! - O arquivo saveConfig.dat nao pode ser aberto!\n");
    }

    fread(tamL, sizeof(*tamL), 1, saveConfig);
    fread(tamC, sizeof(*tamC), 1, saveConfig);
    fread(qBomb, sizeof(*qBomb), 1, saveConfig);
    printf("Quantidade de bombas carregadas: %d\n", *qBomb);

    fclose(saveConfig);
}

/*  Carrega o quego que foi salvo.
    (Tabela, tamanho de linhas, tamanho de colunas, ptr quantidade de espaços não descobertos, ptr tempo salvo)*/
void carregar(tCell **tab, int tamL, int tamC, int *qEspNdescob, clock_t *timeSave){

    FILE *save = fopen("save.dat", "rb");
    if (save == NULL)
    {
        printf("ERRO! - O arquivo save.dat nao pode ser aberto!\n");
    }

    for (int l = 0; l < tamL; l++)
    {
        for (int c = 0; c < tamC; c++)
        {
            fread(&tab[l][c].num, sizeof(tab[l][c].num), 1, save);
            fread(&tab[l][c].v, sizeof(tab[l][c].v), 1, save);
        }
    }
    fread(qEspNdescob, sizeof(*qEspNdescob), 1, save);
    fread(timeSave, sizeof(*timeSave), 1, save);
    fclose(save);
}

/*  Menu principal do jogo, pausa o cronometro*/
void menu(tCell **tab, int *tamL, int *tamC, int *pSelecL, int *pSelecC, int *qBomb, int *qEspNdescob, int *gameRunnig, int *fNewTab, int *fCarregar, clock_t timeD, clock_t timeP, clock_t *timeSave){
    char opc;
    printf(
        "C - Continuar;\n"
        "A - Ajuda;\n"
        "O - Opcoes;\n"
        "S - Salvar;\n"
        "Z - Carregar;\n"
        "X - Sair;\n"
        "O que deseja fazer? "
    );
    scanf(" %c", &opc);

    switch (opc)
    {
    case 'C':
    case 'c':
        break;
    case 'A':
    case 'a':
        printf(
            "Objetivo: Descubra todas as celulas sem minas, usando dicas numericas que indicam quantas minas estao ao redor de cada celula.\n"
            "Numeros: Cada numero (de 1 a 8) indica quantas minas estao nas celulas adjacentes.\n"
            "Revelando Celulas: Digite as cordenadas da tabela para revelar. Celulas vazias revelarao áreas maiores, enquanto numeros darao pistas sobre a localizaçao das minas.\n"
            "Marcar Minas: Coloque bandeiras para marcar uma celula que voce suspeita ser uma mina.\n"
            "Vencer: Revele todas as celulas sem minas para ganhar. Se voce cavar uma mina, o jogo acaba!\n"
            );
        break;
    case 'O':
    case 'o':
        opicoes(tab, tamL, tamC, qBomb, fNewTab);
        break;
    case 'S':
    case 's':
        salvar(tab, *tamL, *tamC, *qBomb, *qEspNdescob, timeD, timeP);
        break;
    case 'Z':
    case 'z':
        *fCarregar = 1;
        break;
    case 'X':
    case 'x':
        *gameRunnig = 0;
        break;
    
    default:
        printf("Opcao invalida!\n");
        menu(tab, tamL, tamC, pSelecL, pSelecC, qBomb, qEspNdescob, gameRunnig, fNewTab, fCarregar, timeD, timeP, timeSave);
        break;
    }
}

/*  Mini menu com 3 possibilidades, cavar, colocar bandeira ou abrir menu principal*/
void jogada(tCell **tab, int *tamL, int *tamC, int *pSelecL, int *pSelecC, int *qBomb, int *qEspNdescob, int *gameRunnig, int *fNewTab, int *fCarregar, clock_t *timeD, clock_t *timeP, clock_t *timeSave){
    char opc;
    printf("O que deseja fazer? (C - Cavar, B - Bandeira, M - Menu): ");
    scanf(" %c", &opc);

    switch (opc)
    {
    case 'C':
    case 'c':
        selecCord(pSelecL, pSelecC, *tamL, *tamC);
        revelar(tab, *tamL, *tamC, *pSelecL, *pSelecC, qEspNdescob);
        break;
    case 'B':
    case 'b':
        bandeira(tab, *tamL, *tamC, pSelecL, pSelecC);
        break;
    case 'M':
    case 'm':
        *timeD = clock();
        menu(tab, tamL, tamC, pSelecL, pSelecC, qBomb, qEspNdescob, gameRunnig, fNewTab, fCarregar, *timeD, *timeP, timeSave);
        *timeP += clock() - *timeD;
        break;
    
    default:
        printf("Opcao invalida!\n");
        jogada(tab, tamL, tamC, pSelecL, pSelecC, qBomb, qEspNdescob, gameRunnig, fNewTab, fCarregar, timeD, timeP, timeSave);
    }
}

int main(){
    tCell **tab = NULL; // Matriz principal do jogo
    int tamC = 10;      // Variavel que guarda o tamanho de colunas que a matriz tem.
    int tamL = 10;      // Variavel que guarda o tamanho de linhas que a matriz tem.
    int qBomb = 10;     // Variavel que guarda a quantidade de bombas.
    int pSelecL;        // Variavel que guarda a posição da linha selecionada pelo usuario.
    int pSelecC;        // Variavel que guarda a posição da coluna selecionada pelo usuario.
    int qEspNdescob;    // Variavel que guarda a quantidade de espaços não descobertos. 
    int fNewTab = 0;    // Flag que indica quando um novo tabubeiro deve ver criado;
    int fCarregar = 0;  // Flag que indica quando o jogo precisa ser carregado;
    int gameRunnig = 1; // Flag que faz o loop principal girar.
    clock_t timeD;      // Variavel que armazena o tempo decorrido;
    clock_t timeP;      // Variavel que armazena o tempo pausado;
    clock_t timeSave=0; // Variavel que armazena o tempo da partida salva;


    timeP = clock();
    carregarConfig(&tamL, &tamC, &qBomb);

    srand(time(NULL));

    tab = criaTab(tamL, tamC);

    printf("|  Tempo: 00:00:000 |\n");
    initGame(tab, tamL, tamC, &pSelecL, &pSelecC, &qEspNdescob, qBomb);

    while (gameRunnig)
    {
        timeD = clock() + timeSave;
        printf("\n|  Tempo: %02d:%02d:%03d  |\n", ((timeD-timeP)/1000)/60, ((timeD-timeP)/1000)%60, (timeD-timeP)%1000);
        impGame(tab, tamL, tamC);
        jogada(tab, &tamL, &tamC, &pSelecL, &pSelecC, &qBomb, &qEspNdescob, &gameRunnig, &fNewTab, &fCarregar, &timeD, &timeP, &timeSave);
        if(fNewTab){
            tab = criaTab(tamL, tamC);
            initGame(tab, tamL, tamC, &pSelecL, &pSelecC, &qEspNdescob, qBomb);
            timeP = clock();
            timeSave = 0;
            fNewTab = 0;
        }
        if (fCarregar)
        {
            carregarConfig(&tamL, &tamC, &qBomb);
            tab = criaTab(tamL, tamC);
            carregar(tab, tamL, tamC, &qEspNdescob, &timeSave);
            timeP = clock();
            fCarregar = 0;
        }
        

        if(tab[pSelecL][pSelecC].num == -1 && tab[pSelecL][pSelecC].v == 1){
            for (int l = 0; l < tamL; l++)
            {
                for (int c = 0; c < tamC; c++)
                {
                    tab[l][c].v = 1;
                }
            }
            impGame(tab, tamL, tamC);
            int opc;
            printf("BOOOOOOM! voce perdeu quer jogar novamente? (s/n): ");
            scanf(" %c", &opc);

            if (opc == 's' || opc == 'S')
            {
                initGame(tab, tamL, tamC, &pSelecL, &pSelecC, &qEspNdescob, qBomb);
                timeP = clock();
                timeSave = 0;
            }else{
                gameRunnig = 0;
            }
            
        }else if(qEspNdescob <= qBomb){
            int opc;
            printf("PARABENS VOCE GANHOU!! quer jogar novamente? (s/n): ");
            scanf(" %c", &opc);

            if (opc == 's' || opc == 'S')
            {
                initGame(tab, tamL, tamC, &pSelecL, &pSelecC, &qEspNdescob, qBomb);
                timeP = clock();
                timeSave = 0;
            }else{
                gameRunnig = 0;
            }
        }
    }
    
    freeGame(tab);
    printf("Obrigado por jogar!\n");
    return 0;
}