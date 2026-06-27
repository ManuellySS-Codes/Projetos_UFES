//Bibliotecas
#include <stdio.h>
#include <stdlib.h>

//Valores definidos
#define max_x 23
#define max_y 13
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
tJogo LimpaArena(tJogo jogo);
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

tJogo InicializaPrimeiraVersaoDaArena(FILE * pPersonagens,FILE * pConfig, FILE * pSaida, tJogo jogo ){
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
//Ate aqui ta bem modularizado
tJogo PegaInformacoesDosPersonagens(FILE * pPersonagens,  tJogo jogo){
    //PREENCHE AS VARIAVEIS RELATIVAS AOS PERSONAGENS
    int i=0, j=0;
    //TIRO JOGADOR
    fscanf(pPersonagens, "%c\n", &jogo.personagem.tiro_jogador);
    //NAVE JOGADOR
    for(i=0; i<altura_nave; i++){
        for(j=0; j<largura_nave; j++){
            fscanf(pPersonagens,"%c",&jogo.personagem.nave_jogador[i][j]);
        }
        fscanf(pPersonagens,"\n");
    }
    //TIRO INIMIGO
    fscanf(pPersonagens, "%c\n", &jogo.personagem.tiro_inimigo);
    //PRIMEIRO TIPO DE ALIEN OU INIMIGO
    for(i=0; i<altura_aliens; i++){
        for(j=0; j<largura_aliens; j++){
            fscanf(pPersonagens,"%c", &jogo.personagem.nave_inimiga_1[i][j]);
        }
        fscanf(pPersonagens,"\n");
    }
    //SEGUNDO TIPO DE ALIEN OU INIMIGO
    for(i=0; i<altura_aliens; i++){
        for(j=0; j<largura_aliens; j++){
            fscanf(pPersonagens,"%c", &jogo.personagem.nave_inimiga_2[i][j]);
        }
        fscanf(pPersonagens,"\n");
    }
    //TEM QUE LEMBRAR DE FECHAR OS ARQUIVOS DEPOIS DE USAR PRA NAO DAR PROBLEMA DEPOIS
    fclose(pPersonagens);
    return jogo;
}

tJogo PegaInformacoesDasConfiguracoes(FILE * pConfig, tJogo jogo ){
    //Preenche as variaveis das structs com informações vindas dos arquivos abertos na funçao anterior
    int i=0, j=0;
    fscanf(pConfig, "%d\n%d %d %d", &jogo.seletor_funcionalidade, &jogo.num_aliens, &jogo.TipoAlienFileira1, &jogo.TipoAlienFileira2);

    //Fechando os arquivos de configuracoes e personagens depois de extrair as informações que eu queria deles, faço isso pra nao dar problema neles
    //Nao vou fechar saida agora porque nela a gente vai escrever as arenas
    fclose(pConfig);
    return jogo;
}

tJogo InicializaLimitesDaArena(FILE * pSaida, tJogo jogo ){
    int linha=0, coluna=0;
    for(linha=0; linha<max_y; linha++){
        for(coluna =0; coluna<max_x; coluna++){
            if(linha == 0 || (linha == (max_y-1) && coluna != 0 && coluna != (max_x-1))){
                jogo.arena[linha][coluna] = '_';
            }else if((linha > 0 && coluna==0) || (linha >0 && coluna == (max_x-1))){
                jogo.arena[linha][coluna] = '|';
            }else{
                jogo.arena[linha][coluna] = ' ';
            }
        }
    }
    return jogo;
}

tJogo PreenchePrimeiraFileiraDeAliens(int tipo_alien, tJogo jogo){
    int i=0, j=0, inicio_inimigo= 0, alien=0;
    if(tipo_alien ==1){
        for(alien=0; alien<jogo.num_aliens; alien++){
            for(i=0; i<altura_aliens; i++){
                for(j=0; j<largura_aliens; j++){
                    jogo.arena[Y_primeira_fileira_inimigos+i][jogo.coordenada.X_aliens+inicio_inimigo+j]= jogo.personagem.nave_inimiga_1[i][j];
                }
            }
            inicio_inimigo += Distancia_entre_aliens;
        }
    }else{
        for(alien=0; alien<jogo.num_aliens; alien++){
            for(i=0; i<altura_aliens; i++){
                for(j=0; j<largura_aliens; j++){
                    jogo.arena[Y_primeira_fileira_inimigos+i][jogo.coordenada.X_aliens+inicio_inimigo+j]= jogo.personagem.nave_inimiga_2[i][j];
                }
            }
            inicio_inimigo += Distancia_entre_aliens;
        }
    }
return jogo;
}

tJogo PreencheSegundaFileiraDeAliens(int tipo_alien, tJogo jogo){
    int i=0, j=0, inicio_inimigo= 0, alien=0;
    if(tipo_alien ==1){
        for(alien=0; alien<jogo.num_aliens; alien++){
            for(i=0; i<altura_aliens; i++){
                for(j=0; j<largura_aliens; j++){
                    jogo.arena[Y_segunda_fileira_inimigos+i][jogo.coordenada.X_aliens+inicio_inimigo+j]= jogo.personagem.nave_inimiga_1[i][j];
                }
            }
            inicio_inimigo += Distancia_entre_aliens;
        }
    }else{
        for(alien=0; alien<jogo.num_aliens; alien++){
            for(i=0; i<altura_aliens; i++){
                for(j=0; j<largura_aliens; j++){
                    jogo.arena[Y_segunda_fileira_inimigos+i][jogo.coordenada.X_aliens+inicio_inimigo+j]= jogo.personagem.nave_inimiga_2[i][j];
                }
            }
            inicio_inimigo += Distancia_entre_aliens;
        }
    }
return jogo;
}

tJogo PreencheComNaveJogador(tJogo jogo){
    int i=0, j=0;
        for(i=0; i<altura_nave; i++){
            for(j=0; j<largura_nave; j++){
                jogo.arena[Y_inicial_Nave+i][jogo.coordenada.X_nave+j] = jogo.personagem.nave_jogador[i][j];
            }
        }
    return jogo;
}

tJogo ColocaAliensNaArena(tJogo jogo ){
    int fileira_inimigo=0, i=0, j=0, tipo_alien=0;
    if(jogo.TipoAlienFileira1 ==1 && jogo.TipoAlienFileira2 ==2){
        tipo_alien = 1;
        jogo = PreenchePrimeiraFileiraDeAliens(tipo_alien, jogo);
        tipo_alien = 2;
        jogo = PreencheSegundaFileiraDeAliens(tipo_alien, jogo);
    }else if(jogo.TipoAlienFileira1 ==2 && jogo.TipoAlienFileira2 ==1){
        tipo_alien = 2;
        jogo = PreenchePrimeiraFileiraDeAliens(tipo_alien, jogo);
        tipo_alien = 1;
        jogo = PreencheSegundaFileiraDeAliens(tipo_alien, jogo);
    }else{
        printf("ERRO NA ENTRADA DOS PARAMETROS DAS CONFIGURACOES ALIENIGENAS\n");
        exit(0);
    }
    return jogo;
}

void PrintaArenaEhPlacar(FILE* pSaida, tJogo jogo ){
    int linha=0, coluna=0;

    fprintf(pSaida,"Pontos: %d | Iteracoes: %d\n", jogo.pontos, jogo.iteracoes);

    for(linha=0; linha<max_y; linha++){
        for(coluna =0; coluna<max_x; coluna++){
            fprintf(pSaida, "%c", jogo.arena[linha][coluna]);
        }
        fprintf(pSaida, "\n");
    }
}

void PrintaMapaDeCalor(FILE* pSaida_Mapa, FILE* pSaida, tJogo jogo ){
    if(jogo.seletor_funcionalidade ==0){
        fprintf(pSaida_Mapa,"Ainda a ser implementado!\n");
    }
}

tJogo RealizaJogo(tJogo jogo, FILE * pSaida){
    int morreu=0, ganhou=0;
    
    jogo = LeJogada(jogo);
        ganhou = JogadorGanhou(jogo);
        morreu = JogadorMorreu(jogo);
    if(ganhou == 1){
            fprintf(pSaida, "Parabens! Voce eliminou todas as naves alienigenas e venceu!\n");
            fclose(pSaida);
            exit(0);
    }else if(morreu ==1){
            fprintf(pSaida,"As naves alienigenas te eliminaram! Fim de jogo.\n");
            fclose(pSaida);
            exit(0);
    }

    if(jogo.jogada == 'a' || jogo.jogada =='d' || jogo.jogada =='s' || jogo.jogada ==' ' ){
        if(morreu == 0 && ganhou == 0){
            jogo = InicializaProximasVersoesArena(pSaida, jogo);
        } 
    }else{
        printf("Digite uma jogada válida\n");
        fclose(pSaida);
        exit(0);
    }
    return jogo;
}

tJogo LeJogada(tJogo jogo){
    if(scanf("%c", &jogo.jogada)==1){
        if(jogo.jogada == '\n'){
            if(scanf("%c", &jogo.jogada)!=1){
                printf("acabaram as jogadas\n");
            }
        }
    }else{
        printf("acabaram as jogadas\n");
        exit(0);
    }
        //printf("%c\n", jogo.jogada);
        return jogo;
}


int JogadorMorreu(tJogo jogo){
    int i=0;
    if(jogo.jogador_morreu ==1){
        return 1;
    }else{
        return 0;
    }
}

int JogadorGanhou(tJogo jogo){
    int i=0, j=0;
    for(i = 0; i < num_fileiras_alienigenas; i++){
        for(j = 0; j < jogo.num_aliens; j++){
            if(jogo.inimigos[i][j] > 0){
                return 0;
            }
        }
    }

    return 1;
}

tJogo InicializaProximasVersoesArena(FILE * pSaida, tJogo jogo){
    jogo.iteracoes++;
    jogo = LimpaArena(jogo);
    jogo = MovePersonagens(jogo);
    jogo = MoveTirosJogador(jogo);
    //verfica nas posicoes mais atualizadas se houve colisao de inimigo e tiro precisei disso pra quando a nave se move pra cima do tiro contar como colisao tambem
    if (jogo.tiro_jogador_existe == 1) {
        if (VerificaSeTiroVaiAtingirUmInimigo(jogo)) {
            jogo = RegistraQualInimigoFoiAtingido(jogo);
            jogo.tiro_jogador_existe = 0;
            jogo.qtd_tiros--;
        }
    }
    jogo = EliminaInimigosMortos(jogo);
    jogo = MoveTirosInimigos(jogo);
    PrintaArenaEhPlacar(pSaida, jogo);
    return jogo;
}

tJogo LimpaArena(tJogo jogo){
    int linha=0, coluna=0;
    for(linha=0; linha<max_y; linha++){
        for(coluna =0; coluna<max_x; coluna++){
            jogo.arena[linha][coluna] = ' ';
        }
    }
    for(linha=0; linha<max_y; linha++){
        for(coluna =0; coluna<max_x; coluna++){
            if(linha == 0 || (linha == (max_y-1) && coluna != 0 && coluna != (max_x-1))){
                jogo.arena[linha][coluna] = '_';
            }else if((linha > 0 && coluna==0) || (linha >0 && coluna == (max_x-1))){
                jogo.arena[linha][coluna] = '|';
            }else{
                jogo.arena[linha][coluna] = ' ';
            }
        }
    }
    return jogo;
}

tJogo MovePersonagens(tJogo jogo){
    if(jogo.jogada =='a'){
        if(jogo.coordenada.X_nave >1){
            jogo.coordenada.X_nave--;
            if(jogo.jogador_morreu == 0){
                jogo = PreencheComNaveJogador(jogo);
            }        
        }else{
            if(jogo.jogador_morreu == 0){
                jogo = PreencheComNaveJogador(jogo);
            }
        }
    }else if(jogo.jogada =='d'){
        if(jogo.coordenada.X_nave+1 < (max_x-largura_nave)){
            jogo.coordenada.X_nave++;
            if(jogo.jogador_morreu == 0){
                jogo = PreencheComNaveJogador(jogo);
            }
        }else{
            if(jogo.jogador_morreu == 0){
                jogo = PreencheComNaveJogador(jogo);
            }        
        }
    }else if(jogo.jogada == 's' || jogo.jogada == ' '){
            if(jogo.jogador_morreu == 0){
                jogo = PreencheComNaveJogador(jogo);
            }
    }

    //direcao 2 siginifica que x vai decrementando, entao os aliens vao pra esquerda, posicao 1 significa que x vai incrementando 1 entao eles vao pra direita ata acalncar a parede
    if((jogo.iteracoes % 2) !=0){
        jogo.coordenada.posicao_do_extremo_do_ultimo_alien = jogo.coordenada.X_aliens +jogo.num_aliens*largura_aliens + (jogo.num_aliens-1);
        if(jogo.coordenada.direcao_aliens==1){
            if(( jogo.coordenada.posicao_do_extremo_do_ultimo_alien) < (max_x-1)){
                jogo.coordenada.X_aliens++;
                jogo = ColocaAliensNaArena(jogo);
            }else{
                jogo.coordenada.direcao_aliens=2;
                jogo.coordenada.X_aliens--;
                jogo = ColocaAliensNaArena(jogo);
            }
        }else if(jogo.coordenada.direcao_aliens==2){
            if(jogo.coordenada.X_aliens  > X_primeiro_alien){
                jogo.coordenada.X_aliens--;
                jogo = ColocaAliensNaArena(jogo);
            }else{
                jogo.coordenada.direcao_aliens=1;
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
    int acertou_nave_inimiga=0, i=0, j=0, acertou_tiro_inimigo=0;
   if(jogo.jogada == ' '){
        if(jogo.tiro_jogador_existe == 0){
            jogo.tiro_jogador_existe = 1;
            jogo.qtd_tiros++;
            jogo.coordenada.Y_tiro_jogador = Y_inicial_Nave - 1;
            jogo.coordenada.X_tiro_jogador = jogo.coordenada.X_nave + 1;
            jogo.arena[jogo.coordenada.Y_tiro_jogador][jogo.coordenada.X_tiro_jogador] = jogo.personagem.tiro_jogador;
        }else if(jogo.tiro_jogador_existe == 1){
            jogo.coordenada.Y_tiro_jogador--;
            if(jogo.coordenada.Y_tiro_jogador>0){
                    acertou_nave_inimiga = VerificaSeTiroVaiAtingirUmInimigo(jogo);
                    if(acertou_nave_inimiga == 0){
                        if(jogo.arena[jogo.coordenada.Y_tiro_jogador][jogo.coordenada.X_tiro_jogador] == ' '){
                            jogo.arena[jogo.coordenada.Y_tiro_jogador][jogo.coordenada.X_tiro_jogador] = jogo.personagem.tiro_jogador;
                        }
                    }else if(acertou_nave_inimiga ==1){
                        jogo = RegistraQualInimigoFoiAtingido(jogo);
                        jogo.qtd_tiros--;
                        jogo.tiro_jogador_existe=0;
                        jogo.coordenada.Y_tiro_jogador = -1;
                        jogo.coordenada.X_tiro_jogador = -1;
                    }      
 
            }else{
                jogo.qtd_tiros--;
                jogo.tiro_jogador_existe=0;
                jogo.coordenada.Y_tiro_jogador = -1;
                jogo.coordenada.X_tiro_jogador = -1;
            }
        }
    }else{
        if(jogo.tiro_jogador_existe == 1){
            jogo.coordenada.Y_tiro_jogador--;
            if(jogo.coordenada.Y_tiro_jogador>0){
                    acertou_nave_inimiga = VerificaSeTiroVaiAtingirUmInimigo(jogo);
                    if(acertou_nave_inimiga ==0 && acertou_tiro_inimigo==0){
                        jogo.arena[jogo.coordenada.Y_tiro_jogador][jogo.coordenada.X_tiro_jogador] = jogo.personagem.tiro_jogador;
                    }else if(acertou_nave_inimiga ==1){
                        jogo = RegistraQualInimigoFoiAtingido(jogo);
                        jogo.qtd_tiros--;
                        jogo.tiro_jogador_existe=0;
                    }       
            }else{
                jogo.qtd_tiros--;
                jogo.tiro_jogador_existe=0;
            }
        }
    }
    return jogo;
}

int VerificaSeTiroVaiAtingirUmInimigo(tJogo jogo){
    //preciso fazer essa verificacao baseada na posicao das coordenadas de cada nave alienigena;
    int i=0, X_comeco_alien_i=0, X_fim_aliens_i=0,acertou=0;
    if((jogo.coordenada.Y_tiro_jogador >= Y_primeira_fileira_inimigos && jogo.coordenada.Y_tiro_jogador <= (Y_primeira_fileira_inimigos + 1)) || (jogo.coordenada.Y_tiro_jogador >= Y_segunda_fileira_inimigos && jogo.coordenada.Y_tiro_jogador <= (Y_segunda_fileira_inimigos + 1))){
        if(jogo.coordenada.X_tiro_jogador >= jogo.coordenada.X_aliens){
            for(i=0; i<= jogo.num_aliens-1; i++){
            X_comeco_alien_i = jogo.coordenada.X_aliens + i * Distancia_entre_aliens;
            X_fim_aliens_i = X_comeco_alien_i + (largura_aliens - 1);

            if(jogo.coordenada.X_tiro_jogador >= X_comeco_alien_i && jogo.coordenada.X_tiro_jogador <= X_fim_aliens_i){
                acertou = 1;
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
    //no sistema de pontos se uma nave teve seu tiro interceptado por mim ela perde uma vida e eu ganhou um ponto mas preciso saber qual nave atirou ainda
    //Preciso Que cada tiro tenha uma nave correspondente pra eu poder tirar a vida dela, vou implementar isso quando souber fazer os tiros das naves que PORRAAAAAAAAAAAAAAAAAAAAAAAAAA
    /*jogo.inimigos[y_nave_atiradora][x_nave_atiradora]--;
    if(jogo.inimigos[y_nave_atiradora][x_nave_atiradora]!= 0){
        jogo.pontos++;
    }
    */
}

tJogo RegistraQualInimigoFoiAtingido(tJogo jogo){   
    int i = 0, X_comeco_alien_i = 0, X_fim_aliens_i = 0;
    int fileira = -1; //inicializando com -1 para saber quando nao entrar em nenhum if que diz qual a linha do morto

    if (jogo.coordenada.Y_tiro_jogador >= Y_primeira_fileira_inimigos && jogo.coordenada.Y_tiro_jogador <= (Y_primeira_fileira_inimigos + 1)) {
        fileira = 0;
    } else if (jogo.coordenada.Y_tiro_jogador >= Y_segunda_fileira_inimigos && jogo.coordenada.Y_tiro_jogador <= (Y_segunda_fileira_inimigos + 1)) {
        fileira = 1;
    }

    if (fileira != -1) {
        for (i = 0; i < jogo.num_aliens; i++) {
            X_comeco_alien_i = jogo.coordenada.X_aliens + (i * Distancia_entre_aliens);
            X_fim_aliens_i = X_comeco_alien_i + (largura_aliens - 1);

            if (jogo.coordenada.X_tiro_jogador >= X_comeco_alien_i && jogo.coordenada.X_tiro_jogador <= X_fim_aliens_i) {
                jogo.inimigos[fileira][i]--;
                if (jogo.inimigos[fileira][i] == QTD_Vidas_inimigas - 2) {
                    jogo.pontos = jogo.pontos + 3;
                } else if (jogo.inimigos[fileira][i] == QTD_Vidas_inimigas - 1) {
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
    int i=0, j=0, x_morto=0;

    for(i=0; i<num_fileiras_alienigenas ; i++){
        for(j=0; j<jogo.num_aliens; j++){
            if(jogo.inimigos[i][j]<=0){
                x_morto = (jogo.coordenada.X_aliens)+(j * Distancia_entre_aliens);
                if(i==0){
                    jogo.arena[Y_primeira_fileira_inimigos][x_morto] = ' ';
                    jogo.arena[Y_primeira_fileira_inimigos][x_morto+1] = ' ';
                    jogo.arena[Y_primeira_fileira_inimigos][x_morto+2] = ' ';
                    jogo.arena[Y_primeira_fileira_inimigos + 1][x_morto] = ' ';
                    jogo.arena[Y_primeira_fileira_inimigos + 1][x_morto+1] = ' ';
                    jogo.arena[Y_primeira_fileira_inimigos + 1][x_morto+2] = ' ';
                }else{
                    jogo.arena[Y_segunda_fileira_inimigos+1][x_morto] = ' ';
                    jogo.arena[Y_segunda_fileira_inimigos+1][x_morto+1] = ' ';
                    jogo.arena[Y_segunda_fileira_inimigos+1][x_morto+2] = ' ';
                    jogo.arena[Y_segunda_fileira_inimigos][x_morto] = ' ';
                    jogo.arena[Y_segunda_fileira_inimigos][x_morto+1] = ' ';
                    jogo.arena[Y_segunda_fileira_inimigos][x_morto+2] = ' ';
                }
            }
        }
    }
    
    return jogo;
}
//preciso consertar essa parte pra parar de sair tiro de uma nave que ja morreu
tJogo MoveTirosInimigos(tJogo jogo){
    int acertou_nave_jogador=0;
    //MOVIMENTA OS TIROS inimigos
        if(jogo.tiro_inimigo_1_existe ==1){
        acertou_nave_jogador = VerificaSeTiroVaiAtingirNave(jogo);
        if(jogo.coordenada.Y_tiro_inimigo_1 < max_y-1){
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
        jogo.coordenada.Y_tiro_inimigo_1++;
    }
    if(jogo.tiro_inimigo_2_existe ==1){
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
    //CRIA OS TIROS INIMIGOS
    if (jogo.iteracoes % 3 == 0 && jogo.iteracoes % 2 != 0 && jogo.qtd_tiros < 3) {
        if (jogo.tiro_inimigo_1_existe == 0) {
            jogo.tiro_inimigo_1_existe = 1;
            jogo = DizCoordenadaDoTiroInimigo(jogo); 
            jogo.qtd_tiros++;
        } else if (jogo.tiro_inimigo_2_existe == 0) {
            jogo.tiro_inimigo_2_existe = 1;
            jogo = DizCoordenadaDoTiroInimigo(jogo); 
            jogo.qtd_tiros++;
        }
    }
    return jogo;
}
tJogo DizCoordenadaDoTiroInimigo(tJogo jogo){
    int coluna=0, Tem_nave_viva_na_fileira_mais_a_frente=0, X_de_cada_nave=0, menor= 3000, i=0, diferenca_horizontal_entre_naves_e_aliens=0;
    if(jogo.tiro_inimigo_1_existe == 1 && jogo.tiro_inimigo_2_existe == 0){
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
        for(i=0; i<=jogo.num_aliens-1; i++){
            if(jogo.inimigos[Tem_nave_viva_na_fileira_mais_a_frente][i]>0){
                X_de_cada_nave = jogo.coordenada.X_aliens + i * Distancia_entre_aliens;

                diferenca_horizontal_entre_naves_e_aliens = jogo.coordenada.X_nave - X_de_cada_nave;
                if(diferenca_horizontal_entre_naves_e_aliens < 0){
                    diferenca_horizontal_entre_naves_e_aliens = diferenca_horizontal_entre_naves_e_aliens * -1;
                }
                if(diferenca_horizontal_entre_naves_e_aliens <= menor){
                    menor = diferenca_horizontal_entre_naves_e_aliens;
                    if((jogo.iteracoes +1)% 2!=0){
                        jogo.coordenada.X_tiro_inimigo_1 = X_de_cada_nave+1;
                    }else{
                        jogo.coordenada.X_tiro_inimigo_1 = X_de_cada_nave;
                    }            
                }
            }
        }
    }else if (jogo.tiro_inimigo_1_existe == 1 && jogo.tiro_inimigo_2_existe == 1){
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

            X_de_cada_nave = (jogo.coordenada.X_aliens+1) + (Distancia_entre_aliens * i);
            diferenca_horizontal_entre_naves_e_aliens = jogo.coordenada.X_nave - X_de_cada_nave;
            if(diferenca_horizontal_entre_naves_e_aliens < 0){
                diferenca_horizontal_entre_naves_e_aliens = diferenca_horizontal_entre_naves_e_aliens * -1;
            }
            if(diferenca_horizontal_entre_naves_e_aliens <= menor){
                menor = diferenca_horizontal_entre_naves_e_aliens;
                if((jogo.iteracoes +1)% 2 !=0){
                    jogo.coordenada.X_tiro_inimigo_2 = X_de_cada_nave+1;
                }else{
                    jogo.coordenada.X_tiro_inimigo_2 = X_de_cada_nave;
                }
            }
        }
    }
    }
    return jogo;
}

int VerificaSeTiroVaiAtingirNave(tJogo jogo){
    if(jogo.coordenada.Y_tiro_inimigo_1+1 == Y_inicial_Nave && jogo.coordenada.X_tiro_inimigo_1 >= jogo.coordenada.X_nave && jogo.coordenada.X_tiro_inimigo_1 <= (jogo.coordenada.X_nave+2)){
        return 1;
    }else if(jogo.coordenada.Y_tiro_inimigo_2 == Y_inicial_Nave && jogo.coordenada.X_tiro_inimigo_2 >= jogo.coordenada.X_nave && jogo.coordenada.X_tiro_inimigo_2 <= jogo.coordenada.X_nave+2){
        return 1;
    }else{
        return 0;
    }
}
tJogo EliminaJogador(tJogo jogo){
    jogo.arena[Y_inicial_Nave][jogo.coordenada.X_nave] = ' ';
    jogo.arena[Y_inicial_Nave][jogo.coordenada.X_nave+1] = ' ';
    jogo.arena[Y_inicial_Nave][jogo.coordenada.X_nave+2] = ' ';
    jogo.arena[Y_inicial_Nave + 1][jogo.coordenada.X_nave] = ' ';
    jogo.arena[Y_inicial_Nave + 1][jogo.coordenada.X_nave+1] = ' ';
    jogo.arena[Y_inicial_Nave + 1][jogo.coordenada.X_nave+2] = ' ';
    return jogo;
}

int main(int argc, char *argv[]){
    tJogo jogo;
    FILE * pSaida;
    FILE * pSaida_Mapa;
    pSaida = fopen("saida/saida.txt", "w");
    pSaida_Mapa = fopen("saida/arquivo_saida.txt", "w");

    ProcuraDiretorio(argc, argv, pSaida);
    jogo = LeArquivosEGeraPrimeiroMapa(argc, argv, jogo, pSaida);

    while(1){
        jogo = RealizaJogo(jogo, pSaida);
    }
    return 0;
}
