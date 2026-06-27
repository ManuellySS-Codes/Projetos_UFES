//Bibliotecas
#include <stdio.h>
#include <stdlib.h>

//Valores definidos
#define max_x 23
#define max_y 13
#define min_x 0
#define min_y 0
#define num_max_aliens 5
#define num_fileiras_alienigenas 2
#define Distancia_entre_aliens 4
#define Y_primeira_fileira_inimigos 1
#define Y_segunda_fileira_inimigos 4
#define X_primeiro_alien 1
#define X_inicial_Nave 10
#define Y_inicial_Nave 10
#define largura_nave 3
#define altura_nave 2
#define largura_aliens 3
#define altura_aliens 3
#define QTD_Vidas_inimigas 2

//Complexos ou structs
//Estao separados em tres porque um unico complexo ficaria desorganizado mas todos tem relacao com tJogo
typedef struct{
    int X_nave;
    int X_aliens;
    int X_tiro_jogador;
    int Y_tiro_jogador;
    int X_tiro_inimigo_1;
    int Y_tiro_inimigo_1;
    int X_tiro_inimigo_2;
    int Y_tiro_inimigo_2;
    int direcao_aliens;
    int posicao_do_extremo_do_ultimo_alien;
} tMovimentacao;

typedef struct{
    char nave_jogador[altura_nave][largura_nave];
    char nave_inimiga_1[altura_aliens][largura_aliens];
    char nave_inimiga_2[altura_aliens][largura_aliens];
    char tiro_inimigo;
    char tiro_jogador;
} tPersonagens;

typedef struct{
    char jogada;
    char arena[max_y][max_x];
    char MapaDeCalor[max_y][max_x];
    int inimigos[num_fileiras_alienigenas][num_max_aliens]; //2 ou 1 signigica aliens vivos e 0 aliens mortos
    int seletor_funcionalidade;
    int tiro_jogador_existe;
    int tiro_inimigo_1_existe;
    int tiro_inimigo_2_existe;
    int TipoAlienFileira1;
    int TipoAlienFileira2;
    int jogador_morreu;
    int num_aliens;
    int iteracoes;
    int qtd_tiros;
    int pontos;
    tMovimentacao coordenada;
    tPersonagens personagem;
} tJogo;


//FUNCOES 
//FUNCOES BASICAS OU INICIAIS
void ProcuraDiretorio(int argc, char *argv[], FILE * pSaida);
tJogo LeArquivosEGeraPrimeiroMapa(int argc, char *argv[], tJogo jogo, FILE * pSaida);
tJogo InicializaPrimeiraVersaoDaArena(FILE * pPersonagens,FILE * pConfig, FILE * pSaida, tJogo jogo);
tJogo PegaInformacoesDasConfiguracoes(FILE * pConfig, tJogo jogo);
tJogo PegaInformacoesDosPersonagens(FILE * pPersonagens, tJogo jogo);
tJogo InicializaLimitesDaArena(FILE* pSaida, tJogo jogo);
tJogo ColocaAliensNaArena( tJogo jogo);
tJogo PreenchePrimeiraFileiraDeAliens(int tipo_alien, tJogo jogo);
tJogo PreencheSegundaFileiraDeAliens(int tipo_alien, tJogo jogo);
tJogo PreencheComNaveJogador(tJogo jogo);
//FUNCOES DINAMICAS DO JOGO
tJogo RealizaJogo(tJogo jogo, FILE * pSaida);
tJogo LeJogada(tJogo jogo);
tJogo InicializaProximasVersoesArena(FILE * pSaida, tJogo jogo);
tJogo MovePersonagens(tJogo jogo);
tJogo LimpaArenaERecolocaAsBordas(FILE * pSaida, tJogo jogo);
//FUNCOES QUE TRATAM ESPECIFICAMENTE OS TIRO DO JOGADOR E AS CONSEQUENCIAS DE CADA UM
tJogo MoveTirosJogador(tJogo jogo);
int VerificaSeTiroVaiAtingirUmInimigo(tJogo jogo);
int VerificaSeTiroVaiAtingirOutroTiro(tJogo jogo); //PRECISO VER NOS CASOS O QUE ACONTECE EXATAMENTE    
tJogo RegistraQualInimigoFoiAtingido(tJogo jogo);
tJogo EliminaInimigosMortos(tJogo jogo);
int JogadorMorreu(tJogo jogo);
int JogadorGanhou(tJogo jogo);
//FUNCOES QUE TRATAM ESPECIFICAMENTE DOS TIROS INIMIGOS E AS CONSEQUENCIAS DE CADA UM
tJogo MoveTirosInimigos(tJogo jogo);
tJogo DizCoordenadaDoTiroInimigo(tJogo jogo);
int VerificaSeTiroVaiAtingirNave(tJogo jogo);
tJogo EliminaJogador(tJogo jogo);
//FUNCOES DE IMPRESSAO
void PrintaArenaEhPlacar(FILE* pSaida, tJogo jogo);
//Falta Implementar ainda
tJogo AtualizaMapaDeCalor(tJogo jogo);
void PrintaMapaDeCalor(FILE* pSaida, FILE* pSaida_Mapa, tJogo jogo);

//Funcao a Funcao
void ProcuraDiretorio(int argc, char *argv[], FILE * pSaida){
    //Informa erro se o usuario nao digitar um diretorio com base no tamanho do argumento passado pra main
    if(argc<=1){
        fprintf(pSaida,"ERRO: Informe o diretorio com os arquivos de jogo.\n");
        exit(0);
    }
}

tJogo LeArquivosEGeraPrimeiroMapa(int argc, char *argv[], tJogo jogo, FILE * pSaida){
    //eu acho que stream se aplica em alguma parte mas to em duvida
    //Abre os arquivos de configuaracao e personagens e verifica se eles existem e caso nao escreve no arquivo de saida que houve erro ao tentar ler

    FILE * pPersonagens;
    pPersonagens = fopen("personagens.txt", "r");

    if(!pPersonagens){
        fprintf(pSaida,"Erro ao tentar ler personagens.txt, caminho do diretorio: %s", argv[1]);
        exit(0);
    }

    FILE * pConfig;
    pConfig = fopen("config_inicial.txt", "r");

    if(!pConfig){
        fprintf(pSaida, "Erro ao tentar ler config_inicial.txt, caminho do diretorio: %s", argv[1]);
        exit(0);
    }
    jogo = InicializaPrimeiraVersaoDaArena(pPersonagens,pConfig, pSaida, jogo);
    return jogo;
}

tJogo InicializaPrimeiraVersaoDaArena(FILE * pPersonagens,FILE * pConfig, FILE * pSaida, tJogo jogo){
    int linha=0, coluna=0;
    //Aproveito que essa funcao so roda uma vez para inicializar os valores da struct jogo com os valores que tem que ter inicialmente e resolver o problema de terem lixo da memoria nelas
    jogo.pontos=0;
    jogo.iteracoes=0;
    jogo.coordenada.X_nave= X_inicial_Nave;
    jogo.coordenada.X_aliens= X_primeiro_alien;
    jogo.coordenada.direcao_aliens =1;
    jogo.jogador_morreu=0;
    jogo.tiro_jogador_existe = 0;
    jogo.tiro_inimigo_1_existe=0;
    jogo.tiro_inimigo_2_existe=0;
    for(linha=0; linha<num_fileiras_alienigenas; linha++){
        for(coluna=0; coluna<num_max_aliens; coluna++){ //considerei o num_max_aliens com base no que eu acho que cabe na arena, mas se ele aumentar eu preciso trocar
            jogo.inimigos[linha][coluna]= QTD_Vidas_inimigas;
        }
    }
    //As seguintes funcoes sao responsaveis por pegar informacoes dos arquivos, preencher e inicializar a primeira versao da arena
    jogo = PegaInformacoesDosPersonagens(pPersonagens, jogo);
    jogo = PegaInformacoesDasConfiguracoes(pConfig, jogo);
    jogo = InicializaLimitesDaArena(pSaida, jogo);
    jogo = ColocaAliensNaArena(jogo);
    jogo = PreencheComNaveJogador(jogo);
    //essa funcao printa a arena inicial no arquivo de saida
    PrintaArenaEhPlacar(pSaida,jogo);
    return jogo;
}

tJogo PegaInformacoesDosPersonagens(FILE * pPersonagens,  tJogo jogo){
    //Funcao que preenche as informacoes relativas aos personagens e que vao fazer parte da struct jogo
    int i = 0, j = 0;
    //Trecho que preenche o tiro do jogador
    fscanf(pPersonagens, "%c\n", &jogo.personagem.tiro_jogador);
    //Trecho que preenche a nave do jogador na matriz onde vou guardar o caracteres que formam ela
    for( i= 0; i< altura_nave; i++){
        for(j = 0; j < largura_nave; j++){
            fscanf(pPersonagens, "%c", &jogo.personagem.nave_jogador[i][j]);
        }
        fscanf(pPersonagens,"\n");
    }
    //Trecho que preenche na struct o caracter do tiro do inimigo
    fscanf(pPersonagens, "%c\n", &jogo.personagem.tiro_inimigo);
    //Trecho que preenche o tipo 1 de inimigo numa matriz que guarda os caracteres dele
    for(i = 0; i < altura_aliens; i++){
        for(j = 0; j < largura_aliens; j++){
            fscanf(pPersonagens,"%c", &jogo.personagem.nave_inimiga_1[i][j]);
        }
        fscanf(pPersonagens,"\n");
    }
    //Trecho que preenche o tipo 2 de inimigo numa matriz que guarda os caracteres dele
    for(i= 0 ; i < altura_aliens; i++){
        for(j = 0; j < largura_aliens; j++){
            fscanf(pPersonagens,"%c", &jogo.personagem.nave_inimiga_2[i][j]);
        }
        fscanf(pPersonagens,"\n");
    }
    //Sempre que termino de usar um arquivo eu fecho porque pesquisei sobre e aparentemente podem ocorrer erros relativos a deixar aberto
    fclose(pPersonagens);
    return jogo;
}

tJogo PegaInformacoesDasConfiguracoes(FILE * pConfig, tJogo jogo){
    //Funcao que preenche as variaveis das structs com informacoes de configuracao do jogo
    int i = 0, j = 0;
    //o unico detalhe importante e que pode ser pedido na implementacao eh de trocar a leitura dos tipos de alien eu sei que a ordem eh essa pelo script mas talvez mude neh
    fscanf(pConfig, "%d\n%d %d %d", &jogo.seletor_funcionalidade, &jogo.num_aliens, &jogo.TipoAlienFileira1, &jogo.TipoAlienFileira2);
    //Sempre que termino de usar um arquivo eu fecho porque pesquisei sobre e aparentemente podem ocorrer erros relativos a deixar aberto
    fclose(pConfig);
    return jogo;
}

tJogo InicializaLimitesDaArena(FILE * pSaida, tJogo jogo){
    //Funcao que coloca as barras e underlines na borda da arena com base nos tamanhos dados pelo script
    //Esse for basicamente forma so o retangulo da arena e preenche o centro da matriz arena com espacos em branco
    int linha = 0, coluna = 0;
    for(linha = 0; linha < max_y; linha++){
        for(coluna = 0; coluna < max_x; coluna++){
            if(linha == 0 || (linha == (max_y-1) && coluna != 0 && coluna != (max_x - 1))){
                jogo.arena[linha][coluna] = '_';
            }else if((linha > 0 && coluna == 0) || (linha > 0 && coluna == (max_x - 1))){
                jogo.arena[linha][coluna] = '|';
            }else{
                jogo.arena[linha][coluna] = ' ';
            }
        }
    }
    return jogo;
}

tJogo PreenchePrimeiraFileiraDeAliens(int tipo_alien, tJogo jogo){
    //Funcao que preenche a primeira fileira de aliens ela sempre eh assionada e sempre preenche a fileira inteira porque mais tarde
    //Na parte dos tiros e mortes eu preferi apagar os mortos sempre antes de reprintar a matriz ao inves de atualizar aqui e baguncar minha funcao que tava boa
    int i = 0, j = 0, inicio_inimigo = 0, alien = 0;
    //detalhe importante eh que eu atualizo o tipo alien da fileira antes de chamar a funcao
    if(tipo_alien == 1){
        for(alien = 0; alien<jogo.num_aliens; alien++){
            for(i = 0; i < altura_aliens; i++){
                for(j = 0; j < largura_aliens; j++){
                    jogo.arena[Y_primeira_fileira_inimigos + i][jogo.coordenada.X_aliens + inicio_inimigo + j] = jogo.personagem.nave_inimiga_1[i][j];
                }
            }
            inicio_inimigo += Distancia_entre_aliens;
        }
    }else{
        for(alien = 0; alien < jogo.num_aliens; alien++){
            for(i = 0; i < altura_aliens; i++){
                for(j = 0; j < largura_aliens; j++){
                    jogo.arena[Y_primeira_fileira_inimigos + i][jogo.coordenada.X_aliens + inicio_inimigo + j] = jogo.personagem.nave_inimiga_2[i][j];
                }
            }
            inicio_inimigo += Distancia_entre_aliens;
        }
    }
    return jogo;
}

tJogo PreencheSegundaFileiraDeAliens(int tipo_alien, tJogo jogo){
    //Preenche a segunda fileira ou fileira mais proxima da nave do jogador com base tambem em tipoalien e no mesmo principio da anterior de sempre preencher toda fileira
    int i = 0, j = 0, inicio_inimigo= 0, alien=0;
    if(tipo_alien == 1){
        for(alien = 0; alien < jogo.num_aliens; alien++){
            for(i = 0; i < altura_aliens; i++){
                for(j = 0; j < largura_aliens; j++){
                    jogo.arena[Y_segunda_fileira_inimigos + i][jogo.coordenada.X_aliens + inicio_inimigo + j] = jogo.personagem.nave_inimiga_1[i][j];
                }
            }
            inicio_inimigo += Distancia_entre_aliens;
        }
    }else{
        for(alien = 0; alien < jogo.num_aliens; alien++){
            for(i = 0; i < altura_aliens; i++){
                for(j = 0; j < largura_aliens; j++){
                    jogo.arena[Y_segunda_fileira_inimigos + i][jogo.coordenada.X_aliens + inicio_inimigo + j] = jogo.personagem.nave_inimiga_2[i][j];
                }
            }
            inicio_inimigo += Distancia_entre_aliens;
        }
    }
    return jogo;
}

tJogo PreencheComNaveJogador(tJogo jogo){
    //Funcao que preenche a nave do jogador na matriz da arena ela so precisa saber o Xnave que eh a coordenada horizontal mais a direita do jogador atualizado
    int i = 0, j = 0;

    for(i = 0; i < altura_nave; i++){
        for(j = 0; j < largura_nave; j++){
            jogo.arena[Y_inicial_Nave + i][jogo.coordenada.X_nave + j] = jogo.personagem.nave_jogador[i][j];
        }
    }
    return jogo;
}

tJogo ColocaAliensNaArena(tJogo jogo){
    //Funcao que orquestra como os tipodes de aliens vao ser inseridos em cada fileira alienigena
    int fileira_inimigo = 0, i = 0, j = 0, tipo_alien = 0;
    //O tipoaleinFileira1 ou 2 sao preenchidos la quando importo os dados de configuracao me baseando na ordem fixa que eles sao passados
    if(jogo.TipoAlienFileira1 == 1 && jogo.TipoAlienFileira2 == 2){
        tipo_alien = 1;
        jogo = PreenchePrimeiraFileiraDeAliens(tipo_alien, jogo);
        tipo_alien = 2;
        jogo = PreencheSegundaFileiraDeAliens(tipo_alien, jogo);
    }else if(jogo.TipoAlienFileira1 == 2 && jogo.TipoAlienFileira2 == 1){
        tipo_alien = 2;
        jogo = PreenchePrimeiraFileiraDeAliens(tipo_alien, jogo);
        tipo_alien = 1;
        jogo = PreencheSegundaFileiraDeAliens(tipo_alien, jogo);
    }else{
        //esse else foi adicionado so pra eu mesma mapear meus erros nao veio do script
        printf("ERRO NA ENTRADA DOS PARAMETROS DAS CONFIGURACOES ALIENIGENAS\n");
        exit(0);
    }
    return jogo;
}

void PrintaArenaEhPlacar(FILE* pSaida, tJogo jogo){
    //Funcao que printa a matriz da arena e a linha de placar
    int linha = 0, coluna = 0;
    //Os pontos e iteracoes sao atualizados em outras funcoes
    fprintf(pSaida, "Pontos: %d | Iteracoes: %d\n", jogo.pontos, jogo.iteracoes);
    //Esse for so printa a arena inteira mas ela tem seus elementos preenchidos em outras funcoes
    for(linha = 0; linha < max_y; linha++){
        for(coluna = 0; coluna < max_x; coluna++){
            fprintf(pSaida, "%c", jogo.arena[linha][coluna]);
        }
        fprintf(pSaida, "\n");
    }
}

void PrintaMapaDeCalor(FILE* pSaida_Mapa, FILE* pSaida, tJogo jogo){
    //Essa funcao soh printa o mapa de calor se o seletor de config for 0 se nao for ela nao printa nada
    //De qualquer modo o mapa de calor sempre vai existir o que vai ser decidido eh se ele vai ser printado ou nao
    int linha = 0, coluna = 0;
    //Por enquanto ta assim porque o professor pediu pra implementar na sala
    if(jogo.seletor_funcionalidade == 0){
        fprintf(pSaida_Mapa, "Ainda a ser implementado!\n");
    }
    /*
    for(linha = 0; linha < max_y; linha++){
        for(coluna = 0; coluna < max_x; coluna++){
            fprintf(pSaida, "%02d ", jogo.MapaDeCalor[linha][coluna]);
        }
        fprintf(pSaida, "\n");
    }
    */
}

tJogo RealizaJogo(tJogo jogo, FILE * pSaida){
    //Funcao que faz o jogo acontecer de certa forma
    int morreu = 0, ganhou = 0;
    //antes de tudo se verifica se o jogador ganhou ou morreu na jogada anterior
    ganhou = JogadorGanhou(jogo);
    morreu = JogadorMorreu(jogo);
    //Pelos casos de teste eh pra priorizar quando o jogador ganha por isso essa verificacao e a decisao vem com isso primeiro 
    //Mesmo se ganhar e morrer acontecer na mesma rodada vai dar que o jogador ganhou
    if(ganhou == 1){
        fprintf(pSaida, "Parabens! Voce eliminou todas as naves alienigenas e venceu!\n");
        fclose(pSaida);
        exit(0);
    }else if(morreu == 1){
        fprintf(pSaida,"As naves alienigenas te eliminaram! Fim de jogo.\n");
        fclose(pSaida);
        exit(0);
    }
    //Depois se nenhum dos dois aconteceu se le a proxima jogada e continua o jogo
    jogo = LeJogada(jogo);
    if(jogo.jogada == 'a' || jogo.jogada == 'd' || jogo.jogada == 's' || jogo.jogada == ' ' ){
        if(morreu == 0 && ganhou == 0){
            jogo = InicializaProximasVersoesArena(pSaida, jogo);
        } 
    }else{
        //Esse print novamente eh pra eu saber quando uma jogada diferente do comum foi lida
        //ajudou a consertar algumas coisas do buffer e quando o documento acabava e o jogo nao por erro meu caia aqui
        printf("Digite uma jogada valida\n");
        fclose(pSaida);
        exit(0);
    }
    return jogo;
}

tJogo LeJogada(tJogo jogo){
    //Essa funcao le a jogada que vai vir da entrada padrao com ./ex . entrada.txt no terminal
    if(scanf("%c", &jogo.jogada) == 1){
        if(jogo.jogada == '\n'){ //esse if serve porque as vezes tava lendo errado principalmente quando a jogada era o space e o scanf ignorava dai eu coloquei essa verificacao pra ter certeza que leu e excluir os \n
            if(scanf("%c", &jogo.jogada) != 1){
                printf("acabaram as jogadas\n");
            }
        }
    }else{
        printf("acabaram as jogadas\n");
        exit(0);
    }
    return jogo;
}

int JogadorMorreu(tJogo jogo){
    //Funcao que verifica se a flag jogador morreu foi atividada com 1
    if(jogo.jogador_morreu == 1){
        return 1;
    }else{
        return 0;
    }
}

int JogadorGanhou(tJogo jogo){
    //Funcao que verifica se tem algum alien vivo
    //A verificacao tem por base a matriz inimigos que eu usei pra mapear os vivos e as quantidades de vidas em varias funcoes posteriores e inicializei em InicializaPrimeiraVersaoDaArena
    int i = 0, j = 0;
    for(i = 0; i < num_fileiras_alienigenas; i++){
        for(j = 0; j < jogo.num_aliens; j++){
            if(jogo.inimigos[i][j] > 0){
                return 0; //se encontra alguma nave com mais de 0 vidas significa que nao foram todas que morreram entao o jogador nao ganhou
            }
        }
    }
    return 1; //se todas as naves da matriz inimigos tiverem vida menor ou igual a zero nao vai entrar no if e vai sair do for e vir pra ca pois significa que morreu tudo
}

tJogo InicializaProximasVersoesArena(FILE * pSaida, tJogo jogo){
    //Funcao que inicializa proximas versoes da Arena soh se RealizaJogo nao detectou vitoria nem derrota e nem jogadas invalidas
    jogo.iteracoes++;
    //O padrao que eu escolhi foi Limpar a arena toda e recolocar os personagens e os tiros e eliminar os inimigos a cada iteracao
    //Desse jeito nao tenho problema de rastro quando alguma coisa muda de posicao talvez nao seja mais pratico mas eu achei melhor de detectar os erros tambem
    jogo = LimpaArenaERecolocaAsBordas(pSaida,jogo);
    jogo = MovePersonagens(jogo);
    jogo = MoveTirosJogador(jogo);
    //verfica nas posicoes mais atualizadas se houve colisao de inimigo e tiro precisei disso pra quando a nave se move pra cima do tiro contar como colisao tambem
    if(jogo.tiro_jogador_existe == 1){
        if(VerificaSeTiroVaiAtingirUmInimigo(jogo)){
            jogo = RegistraQualInimigoFoiAtingido(jogo);
            jogo.tiro_jogador_existe = 0;
            jogo.qtd_tiros--;
        }
    }
    jogo = EliminaInimigosMortos(jogo);
    //Botei essa funcao depois de eliminar os inimigos mortos pra nao ocorrer de tiros sairem de naves que morreram na mesma iteracao
    jogo = MoveTirosInimigos(jogo);
    PrintaArenaEhPlacar(pSaida, jogo);
    return jogo;
}

tJogo LimpaArenaERecolocaAsBordas(FILE * pSaida, tJogo jogo){
    //Funcao responsavel por limpar o conteudo da arena antes de adicionar os outros elementos atualizados
    int linha=0, coluna=0;
    for(linha=0; linha<max_y; linha++){
        for(coluna =0; coluna<max_x; coluna++){
            jogo.arena[linha][coluna] = ' ';
        }
    }
    //aproveito pra depois de limpar colocar as bordas aqui mesmo
    jogo = InicializaLimitesDaArena(pSaida, jogo);
    return jogo;
}

tJogo MovePersonagens(tJogo jogo){
    //Funcao que movimenta os personagens de acordo com a jogada que comanda essa iteracao
    if(jogo.jogada == 'a'){ //jogada a move pra esquerda entao o X diminui
        if(jogo.coordenada.X_nave > min_x+1){ //tomo cuidado a cada movimento pra nao ultrapassar as bordas da arena
            jogo.coordenada.X_nave--;
            if(jogo.jogador_morreu == 0){
                jogo = PreencheComNaveJogador(jogo);
            }        
        }else{
            if(jogo.jogador_morreu == 0){
                jogo = PreencheComNaveJogador(jogo);
            }
        }
    }else if(jogo.jogada == 'd'){ //jogada d move a nave pra direita entao o X aumenta
        if(jogo.coordenada.X_nave + 1 < (max_x - largura_nave)){ //verifico antes de iterar o x se ele vai ultrapassar o limite da arena
            jogo.coordenada.X_nave++;
            if(jogo.jogador_morreu == 0){
                jogo = PreencheComNaveJogador(jogo);
            }
        }else{
            if(jogo.jogador_morreu == 0){
                jogo = PreencheComNaveJogador(jogo);
            }        
        }
    }else if(jogo.jogada == 's' || jogo.jogada == ' '){ //tanto no S quando no space o jogador soh fica parado mesmo
        if(jogo.jogador_morreu == 0){ //Se o jogador morrer eh pra preencher a ultima iteracao sem ele
            jogo = PreencheComNaveJogador(jogo);
        }
    }
    //Essa parte da funcao basicamente soh controla o movimento constante que os aliens fazem grupalmente
    //direcao 2 siginifica que x vai decrementando entao os aliens vao pra esquerda direcao 1 significa que x vai incrementando 1 entao eles vao pra direita ata acalncar a parede
    if((jogo.iteracoes % 2) != 0){//se a iteracao for impar ele vai precisar movimentar a nave
        jogo.coordenada.posicao_do_extremo_do_ultimo_alien = jogo.coordenada.X_aliens + jogo.num_aliens * largura_aliens + (jogo.num_aliens - 1);
        if(jogo.coordenada.direcao_aliens == 1){
            if((jogo.coordenada.posicao_do_extremo_do_ultimo_alien) < (max_x - 1)){
                jogo.coordenada.X_aliens++;
                jogo = ColocaAliensNaArena(jogo);
            }else{
                jogo.coordenada.direcao_aliens = 2;
                jogo.coordenada.X_aliens--;
                jogo = ColocaAliensNaArena(jogo);
            }
        }else if(jogo.coordenada.direcao_aliens == 2){
            if(jogo.coordenada.X_aliens  > X_primeiro_alien){
                jogo.coordenada.X_aliens--;
                jogo = ColocaAliensNaArena(jogo);
            }else{
                jogo.coordenada.direcao_aliens = 1;
                jogo.coordenada.X_aliens++;
                jogo = ColocaAliensNaArena(jogo);
            }
        }
    }else{
        jogo = ColocaAliensNaArena(jogo);
    }
    return jogo;
}

tJogo MoveTirosJogador(tJogo jogo){
    //Funca que move os tiros do jogador e faz varias verificacoes em relacao as consequencias dessa acao acho que eh a mais complexa do negocio
    //Essa funcao ta meio redundante se eu tiver tempo livre vou encurtar ela soh preciso tomar cuidado pra manter a mesma logica e nao dar erro desnecessario
    int acertou_nave_inimiga = 0, i = 0, j = 0, acertou_tiro_inimigo = 0, X_tiro=0, Y_tiro=0;
    if(jogo.jogada == ' '){
        if(jogo.tiro_jogador_existe == 0){ //se o tiro nao existe a funcao cria ele quando a jogada eh space
            jogo.tiro_jogador_existe = 1;
            jogo.qtd_tiros++;
            jogo.coordenada.Y_tiro_jogador = Y_inicial_Nave - 1;
            jogo.coordenada.X_tiro_jogador = jogo.coordenada.X_nave + 1;
            jogo.arena[jogo.coordenada.Y_tiro_jogador][jogo.coordenada.X_tiro_jogador] = jogo.personagem.tiro_jogador;
        }else if(jogo.tiro_jogador_existe == 1){ //se a jogada eh space mas o tiro jah existe a funcao soh movimenta ele
            jogo.coordenada.Y_tiro_jogador--; //como o tiro jogador sobe a arena o y dele decrementa pra movimentar
            if(jogo.coordenada.Y_tiro_jogador > 0){ //tem que tomar cuidado com as bordas
                acertou_nave_inimiga = VerificaSeTiroVaiAtingirUmInimigo(jogo); //Verifica se o tiro vai acertar um inimigo
                if(acertou_nave_inimiga == 0){ //Se nao acertou uma nave inimiga e o proximo caracter eh um space ou um tiro do inimigo o tiro do jogador se sobrepoe
                    if(jogo.arena[jogo.coordenada.Y_tiro_jogador][jogo.coordenada.X_tiro_jogador] == ' ' || jogo.arena[jogo.coordenada.Y_tiro_jogador][jogo.coordenada.X_tiro_jogador] == jogo.personagem.tiro_inimigo){
                        jogo.arena[jogo.coordenada.Y_tiro_jogador][jogo.coordenada.X_tiro_jogador] = jogo.personagem.tiro_jogador;
                    }
                }else if(acertou_nave_inimiga == 1){
                    jogo = RegistraQualInimigoFoiAtingido(jogo); //registra na matriz inimigos a perda da vida o ganho dos pontos e os outros porens 
                    jogo.qtd_tiros--;
                    jogo.tiro_jogador_existe = 0;
                    jogo.coordenada.Y_tiro_jogador = -1; //-1 eh um valor facil de mapear erro
                    jogo.coordenada.X_tiro_jogador = -1;
                }       
            }else{
                jogo.qtd_tiros--;
                jogo.tiro_jogador_existe = 0;
                jogo.coordenada.Y_tiro_jogador = -1;
                jogo.coordenada.X_tiro_jogador = -1;
            }
        }
    }else{ //Mesma logica de movimentar o tiro mas sem precisar que a jogada seja um space
        if(jogo.tiro_jogador_existe == 1){
            jogo.coordenada.Y_tiro_jogador--;
            if(jogo.coordenada.Y_tiro_jogador > 0){
                acertou_nave_inimiga = VerificaSeTiroVaiAtingirUmInimigo(jogo);
                if(acertou_nave_inimiga == 0){
                    if(jogo.arena[jogo.coordenada.Y_tiro_jogador][jogo.coordenada.X_tiro_jogador] == ' ' || jogo.arena[jogo.coordenada.Y_tiro_jogador][jogo.coordenada.X_tiro_jogador] == jogo.personagem.tiro_inimigo){
                        jogo.arena[jogo.coordenada.Y_tiro_jogador][jogo.coordenada.X_tiro_jogador] = jogo.personagem.tiro_jogador;
                    }
                }else if(acertou_nave_inimiga == 1){
                    jogo = RegistraQualInimigoFoiAtingido(jogo);
                    jogo.qtd_tiros--;
                    jogo.tiro_jogador_existe = 0;
                    jogo.coordenada.Y_tiro_jogador = -1;
                    jogo.coordenada.X_tiro_jogador = -1;
                }       
            }else{
                jogo.qtd_tiros--;
                jogo.tiro_jogador_existe = 0;
                    jogo.coordenada.Y_tiro_jogador = -1;
                    jogo.coordenada.X_tiro_jogador = -1;
            }
        }
    }
    //preciso ver como eh esse mapa de calor pra saber ateh onde eh considerado que o tiro foi e onde eu coloco isso
    //vou implementar primeiro aqui e depois da funcao dos tiros inimigos pra nao errar muito
    X_tiro = jogo.coordenada.X_tiro_jogador;
    Y_tiro = jogo.coordenada.Y_tiro_jogador;
    //jogo = AtualizaMapaDeCalor(jogo, X_tiro, Y_tiro);
    return jogo;
}

int VerificaSeTiroVaiAtingirUmInimigo(tJogo jogo){
    //Funcao que verifica se tiro vai alcancar o corpo de algum dos aliens
    int i = 0, X_comeco_alien_i = 0, X_fim_aliens_i = 0,acertou = 0;
    //Primeiro tem que verificar se o tiro ta na fileira dos aliens
    if((jogo.coordenada.Y_tiro_jogador >= Y_primeira_fileira_inimigos && jogo.coordenada.Y_tiro_jogador <= (Y_primeira_fileira_inimigos + 1)) || (jogo.coordenada.Y_tiro_jogador >= Y_segunda_fileira_inimigos && jogo.coordenada.Y_tiro_jogador <= (Y_segunda_fileira_inimigos + 1))){
        //Se o tiro ta na fileira eh so verificar se ele ta no corpo de algum dos aliens
        if(jogo.coordenada.X_tiro_jogador >= jogo.coordenada.X_aliens){
            for(i = 0; i <= jogo.num_aliens - 1; i++){
                X_comeco_alien_i = jogo.coordenada.X_aliens + i * Distancia_entre_aliens; //cada aliens de uma fileira tem um comeco e um fim que define o corpinho dele
                X_fim_aliens_i = X_comeco_alien_i + (largura_aliens - 1); //usando essas coordenadas e sabendo o y do tiro eu sei quando ele vai invadir um dos aliens
                if(jogo.coordenada.X_tiro_jogador >= X_comeco_alien_i && jogo.coordenada.X_tiro_jogador <= X_fim_aliens_i){
                    acertou = 1;
                    //acaba a verificacao se descobriu que acertou pra nao ficar repetindo
                    break;
                }
            }
        }
    }
    return acertou;
}

int VerificaSeTiroVaiAtingirOutroTiro(tJogo jogo){
    //vou averiguar a necessidade dessa funcao nos casos testes, acho que so precisa que na colisao o tiro amigo se sobressaia
    if(jogo.arena[jogo.coordenada.Y_tiro_jogador][jogo.coordenada.X_tiro_jogador] == jogo.personagem.tiro_inimigo){
        return 1;
    }
    return 0;
}

tJogo RegistraQualInimigoFoiAtingido(tJogo jogo){   
    //Funcao responsavel por registrar qual inimigo especificamente foi atingido e atualizar a matriz inimigos em questao de vidas e tambem os pontos que o jogador recebe por acertar
    int i = 0, X_comeco_alien_i = 0, X_fim_aliens_i = 0;
    int fileira = -1; //inicializando com -1 para saber quando nao entrar em nenhum if que diz qual a linha do morto
    //Sempre primeiro verifico o Y ou fileiras
    if (jogo.coordenada.Y_tiro_jogador >= Y_primeira_fileira_inimigos && jogo.coordenada.Y_tiro_jogador <= (Y_primeira_fileira_inimigos + 1)) {
        fileira = 0;
    } else if (jogo.coordenada.Y_tiro_jogador >= Y_segunda_fileira_inimigos && jogo.coordenada.Y_tiro_jogador <= (Y_segunda_fileira_inimigos + 1)) {
        fileira = 1;
    }
    //Depois de saber a fileira onde o tiro esta eu posso verificar exatamente qual alien ele acertou
    if(fileira != -1){
        for(i = 0; i < jogo.num_aliens; i++){ // uso aqui a mesma logica de comeco e fim do corpinho de cada alien
            //com esse for eu sei o alien i que morreu e tambem uso o i pra atualizar a matriz
            X_comeco_alien_i = jogo.coordenada.X_aliens + (i * Distancia_entre_aliens);
            X_fim_aliens_i = X_comeco_alien_i + (largura_aliens - 1);
            if (jogo.coordenada.X_tiro_jogador >= X_comeco_alien_i && jogo.coordenada.X_tiro_jogador <= X_fim_aliens_i) {
                jogo.inimigos[fileira][i]--; //diminui das vidas do alien que morreu
                if(jogo.inimigos[fileira][i] <= QTD_Vidas_inimigas - 2){ //Se o alien ficou com 0 vidas ou menos ele morreu e da tres pontos pro jogador
                    jogo.pontos = jogo.pontos + 3;
                }else if (jogo.inimigos[fileira][i] == QTD_Vidas_inimigas - 1){ //Se o alien ainda tem uma vida sobrando ele nao morreu ent o jogador ganha soh um ponto
                    jogo.pontos = jogo.pontos + 1;
                }
                //breka quando acha o alien que foi atingido
                break; 
            }
        }
    }
    return jogo;
}

tJogo EliminaInimigosMortos(tJogo jogo){
    //Funcao que elimina os aliens mortos da matriz arena com base na matriz de inimigos e a quantidade de vidas que cada um tem
    int i = 0, j = 0, x_morto = 0;

    for(i = 0; i < num_fileiras_alienigenas ; i++){
        for(j = 0; j < jogo.num_aliens; j++){
            if(jogo.inimigos[i][j] <= 0){ //faz pra todos os aliens sem vida na matriz inimigos
                x_morto = (jogo.coordenada.X_aliens) + (j * Distancia_entre_aliens);
                //A logica eh bem bruta eh so apagar a regiao dos aliens mortos ja que eu sei a dimensao deles
                if(i == 0){ 
                    jogo.arena[Y_primeira_fileira_inimigos][x_morto] = ' ';
                    jogo.arena[Y_primeira_fileira_inimigos][x_morto + 1] = ' ';
                    jogo.arena[Y_primeira_fileira_inimigos][x_morto + 2] = ' ';
                    jogo.arena[Y_primeira_fileira_inimigos + 1][x_morto] = ' ';
                    jogo.arena[Y_primeira_fileira_inimigos + 1][x_morto + 1] = ' ';
                    jogo.arena[Y_primeira_fileira_inimigos + 1][x_morto + 2] = ' ';
                }else{
                    jogo.arena[Y_segunda_fileira_inimigos + 1][x_morto] = ' ';
                    jogo.arena[Y_segunda_fileira_inimigos + 1][x_morto + 1] = ' ';
                    jogo.arena[Y_segunda_fileira_inimigos + 1][x_morto + 2] = ' ';
                    jogo.arena[Y_segunda_fileira_inimigos][x_morto] = ' ';
                    jogo.arena[Y_segunda_fileira_inimigos][x_morto + 1] = ' ';
                    jogo.arena[Y_segunda_fileira_inimigos][x_morto + 2] = ' ';
                }
            }
        }
    }
    return jogo;
}

tJogo MoveTirosInimigos(tJogo jogo){
    //Funcao que move os tiros dos inimigos e gera eles quando eh necessario
    int acertou_nave_jogador = 0;
    //Essa parte da funcao soh eh responsavel por mover os tiros em si
    //Separei em tiro 1 e tiro 2 para ter um controle melhor das posicoes dde cada um
    if(jogo.tiro_inimigo_1_existe == 1){
        acertou_nave_jogador = VerificaSeTiroVaiAtingirNave(jogo);
        if(jogo.coordenada.Y_tiro_inimigo_1 < max_y-1){ //tomando cuidado primeiro pro tiro nao passar da arena
            if(acertou_nave_jogador == 0){
                jogo.arena[jogo.coordenada.Y_tiro_inimigo_1][jogo.coordenada.X_tiro_inimigo_1] = jogo.personagem.tiro_inimigo;
            }else{
                jogo.jogador_morreu = 1;
                jogo = EliminaJogador(jogo);
            }
        }else{
            jogo.tiro_inimigo_1_existe = 0;
            jogo.qtd_tiros--;
        }
        jogo.coordenada.Y_tiro_inimigo_1++;//incremento no final pro tiro nao sair em um y a mais depois de ser criado
    }
    if(jogo.tiro_inimigo_2_existe == 1){
        if(jogo.coordenada.Y_tiro_inimigo_2 < max_y-1){
            acertou_nave_jogador = VerificaSeTiroVaiAtingirNave(jogo);
            if(acertou_nave_jogador == 0){
                jogo.arena[jogo.coordenada.Y_tiro_inimigo_2][jogo.coordenada.X_tiro_inimigo_2] = jogo.personagem.tiro_inimigo;
            }else{
                jogo.jogador_morreu = 1;
                jogo = EliminaJogador(jogo);
            }
        }else{
            jogo.tiro_inimigo_2_existe = 0;
            jogo.qtd_tiros--;
        }
        jogo.coordenada.Y_tiro_inimigo_2++;
    }
    //Essa eh a parte da funcao que realmente cria os tiros
    if(jogo.iteracoes % 3 == 0 && jogo.iteracoes % 2 != 0 && jogo.qtd_tiros < 3){ //Condicoes para que um tiro inimigo deva ocorrer
        if(jogo.tiro_inimigo_1_existe == 0){
            jogo.tiro_inimigo_1_existe = 1;
            jogo = DizCoordenadaDoTiroInimigo(jogo); 
            jogo.qtd_tiros++;
        }else if (jogo.tiro_inimigo_2_existe == 0){
            jogo.tiro_inimigo_2_existe = 1;
            jogo = DizCoordenadaDoTiroInimigo(jogo); 
            jogo.qtd_tiros++;
        }
    }
    return jogo;
}

tJogo DizCoordenadaDoTiroInimigo(tJogo jogo){
    //Funcao responsavel por mapear qual eh a nave viva mais proxima do jogador pra dizer onde o tiro deve surgir
    int coluna=0, Tem_nave_viva_na_fileira_mais_a_frente=0, X_de_cada_nave=0, menor= 3000, i=0, diferenca_horizontal_entre_naves_e_aliens=0, X_cabeca_de_cada_nave=0;
    if(jogo.tiro_inimigo_1_existe == 1 && jogo.tiro_inimigo_2_existe == 0){
        //Se tiver alien vivo na fileira mais proxima do jogador eu priorizo isso ent o y do tiro eh a primeira coisa que defino
        for(coluna=0; coluna<jogo.num_aliens; coluna++){
            if(jogo.inimigos[1][coluna] > 0){
                Tem_nave_viva_na_fileira_mais_a_frente = 1;
                jogo.coordenada.Y_tiro_inimigo_1 = Y_segunda_fileira_inimigos + altura_aliens;
                break;
            }else{
                Tem_nave_viva_na_fileira_mais_a_frente = 0;
                jogo.coordenada.Y_tiro_inimigo_1 = Y_primeira_fileira_inimigos + altura_aliens;
            }
        }
        //Depois de saber o Y tem que descobrir o X do tiro pra isso eu sei que mais perto significa menor distancia
        for(i=0; i<=jogo.num_aliens-1; i++){
            if(jogo.inimigos[Tem_nave_viva_na_fileira_mais_a_frente][i]>0){
                //Calculo do X de cada nave
                X_de_cada_nave = (jogo.coordenada.X_aliens + (i * Distancia_entre_aliens));
                X_cabeca_de_cada_nave = X_de_cada_nave + 1;
                diferenca_horizontal_entre_naves_e_aliens = (jogo.coordenada.X_nave - X_cabeca_de_cada_nave);

                if(diferenca_horizontal_entre_naves_e_aliens < 0){ //pra usar soh o modolo da diferenca e nao ter problema com a conta anterior
                    diferenca_horizontal_entre_naves_e_aliens = diferenca_horizontal_entre_naves_e_aliens * -1;
                }

                if(diferenca_horizontal_entre_naves_e_aliens <= menor){//salva a menor diferenca ou menor distancia horizontal entre alien e nave
                    menor = diferenca_horizontal_entre_naves_e_aliens;
                    if((jogo.iteracoes +1)% 2!=0){//Esse if eh pra quando a nave vai se movimentar na iteracao seguinte dai eu ajusto pro tiro nao ficar um X atradasado se esse for o caso
                        jogo.coordenada.X_tiro_inimigo_1 = X_cabeca_de_cada_nave;
                    }else{
                        jogo.coordenada.X_tiro_inimigo_1 = X_cabeca_de_cada_nave-1;
                    }            
                }
            }
        }
    }else if (jogo.tiro_inimigo_1_existe == 1 && jogo.tiro_inimigo_2_existe == 1){ //Mesma coisa mas pra quando o tiro 2 vai surgir
        for(coluna=0; coluna<jogo.num_aliens; coluna++){
            if(jogo.inimigos[1][coluna] >0){
                Tem_nave_viva_na_fileira_mais_a_frente = 1;
                jogo.coordenada.Y_tiro_inimigo_2 = Y_segunda_fileira_inimigos + altura_aliens;
                break;
            }else{
                Tem_nave_viva_na_fileira_mais_a_frente = 0;
                jogo.coordenada.Y_tiro_inimigo_2 = Y_primeira_fileira_inimigos + altura_aliens;
            }
        }
        for(i=0; i<jogo.num_aliens; i++){
            if(jogo.inimigos[Tem_nave_viva_na_fileira_mais_a_frente][i]>0){
                X_de_cada_nave = (jogo.coordenada.X_aliens + (i * Distancia_entre_aliens));
                X_cabeca_de_cada_nave = X_de_cada_nave + 1;
                diferenca_horizontal_entre_naves_e_aliens = (jogo.coordenada.X_nave - X_cabeca_de_cada_nave);

                if(diferenca_horizontal_entre_naves_e_aliens < 0){
                    diferenca_horizontal_entre_naves_e_aliens = diferenca_horizontal_entre_naves_e_aliens * -1;
                }

                if(diferenca_horizontal_entre_naves_e_aliens <= menor){
                    menor = diferenca_horizontal_entre_naves_e_aliens;
                    if((jogo.iteracoes +1)% 2!=0){
                        jogo.coordenada.X_tiro_inimigo_2 = X_cabeca_de_cada_nave;
                    }else{
                        jogo.coordenada.X_tiro_inimigo_2 = X_cabeca_de_cada_nave-1;
                    }            
                }
            }
        }
    }
    return jogo;
}

int VerificaSeTiroVaiAtingirNave(tJogo jogo){
    //Funcao que verifica se o tiro inimigo vai acertar o jogador
    if(jogo.coordenada.Y_tiro_inimigo_1+1 == Y_inicial_Nave && jogo.coordenada.X_tiro_inimigo_1 >= jogo.coordenada.X_nave && jogo.coordenada.X_tiro_inimigo_1 <= (jogo.coordenada.X_nave+2)){
        return 1;
    }else if(jogo.coordenada.Y_tiro_inimigo_2 == Y_inicial_Nave && jogo.coordenada.X_tiro_inimigo_2 >= jogo.coordenada.X_nave && jogo.coordenada.X_tiro_inimigo_2 <= jogo.coordenada.X_nave+2){
        return 1;
    }else{
        return 0;
    }
}

tJogo EliminaJogador(tJogo jogo){
    //Funcao que elimina a nave inimiga da matriz arena antes de declarar o jogo como perdido
    jogo.arena[Y_inicial_Nave][jogo.coordenada.X_nave] = ' ';
    jogo.arena[Y_inicial_Nave][jogo.coordenada.X_nave+1] = ' ';
    jogo.arena[Y_inicial_Nave][jogo.coordenada.X_nave+2] = ' ';
    jogo.arena[Y_inicial_Nave + 1][jogo.coordenada.X_nave] = ' ';
    jogo.arena[Y_inicial_Nave + 1][jogo.coordenada.X_nave+1] = ' ';
    jogo.arena[Y_inicial_Nave + 1][jogo.coordenada.X_nave+2] = ' ';
    return jogo;
}
/*tJogo AtualizaMapaDeCalor(tJogo jogo, int X_tiro, int Y_tiro){
    //Funcao que atualiza os dados do mapa de calor
    jogo.MapaDeCalor[X_tiro][Y_tiro]++;
    if(jogo.MapaDeCalor[X_tiro][Y_tiro] > 99){
        jogo.MapaDeCalor[X_tiro][Y_tiro] = 99;
    }
    return jogo;
}*/
int main(int argc, char *argv[]){
    tJogo jogo;
    //Abrindo os arquivos de saida
    FILE * pSaida;
    FILE * pSaida_Mapa;
    pSaida = fopen("saida/saida.txt", "w");
    pSaida_Mapa = fopen("saida/arquivo_saida.txt", "w");

    ProcuraDiretorio(argc, argv, pSaida);
    jogo = LeArquivosEGeraPrimeiroMapa(argc, argv, jogo, pSaida);
    //Loop que realiza o jogo ateh acontecer alguma coisa em realiza jogo que acaba a partida
    while(1){
        jogo = RealizaJogo(jogo, pSaida);
    }
    return 0;
}
