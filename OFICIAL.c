//PROJETO 3 - DISCIPLINA PROGRAMAÇÃO II
//EQUIPE:
//Raysa Carla;
//Kaelany Soares;
//Carlle Gabryelle;

#include <raylib.h>
#include <stdio.h>

#define MAX_INIMIGOS 6 //Número máximo de obstáculos na tela ao mesmo tempo
#define NOME "PontoMaximo.txt" //Nome do arquivo para salvar o estado do jogo (recorde)

///Struct com as informações do obstáculo (cupcake ou bruxa)
typedef struct Obstaculo{
    Rectangle bordas; //Dimensões do personagem
    int caminho; //Caminhos possíveis divindo a altura da tela pela altura do personagem
    int tipo; // 1-cupcake ; 2-bruxa;
    bool ativo; 
}Obstaculo; 

//Função para salvar os pontos em um arquivo txt
void Salvar(char *nome, int pontos)
{
 FILE *ponteiroArquivo = fopen(nome, "w");
 fprintf(ponteiroArquivo, "%d", pontos);
 fclose(ponteiroArquivo);
}

//Função para atualizar a variável com os pontos do arquivo         
int Carregar(char *nome)
{
  int pontos = 0;
  FILE *ponteiroArquivo = fopen(nome, "r");
  if(ponteiroArquivo == NULL) return 0;
  fscanf(ponteiroArquivo, "%d", &pontos);
  fclose(ponteiroArquivo);
  return pontos;
}


int main(void)
{
    //INICIALIZAÇÃO DAS VARIÁVEIS
    float velocidade = 10;
    const int screenWidth = 800;
    const int screenHeight = 550; //500 para jogar e 50 para as informações
    int frames = 0;
    float scrollingSky = 0.0f; //Variável para o scrolling do background
    int pontos = 0; //Variável para acumular os pontos ganhos ao colidir com o cup cake
    int recorde;
    //Variáveis para o unicornio
    int alturaUni = 100;
    int larguraUni = 96;
    int caminhoUni = 2 ; //O unicornio tem 100 de altura, dividido pela altura da tela, tem 5 caminhos possíveis
                         //Ele começa no caminho 2 que é mais ou menos no meio da tela
    Rectangle bordasUni = {25.0f, caminhoUni*alturaUni, larguraUni, alturaUni }; //O '+30' é para ajustar a posição
    //Vaviáveis para o obstáculo
    Obstaculo obstaculos [MAX_INIMIGOS];
    int alturaObs = 100;
    int larguraObs = 114;
    
    //Variavel para o recorde    
    recorde = Carregar(NOME);
    
    //INICIALIZAÇÃO DA TELA   
    InitWindow(screenWidth, screenHeight, "Projeto 3");
   
    //INICIALIZAÇÃO DAS IMAGENS
    //As variáveis são do tipo 'Texture2D'
    //Estou declarando e atribuindo a imagem
    Texture2D personagem = LoadTexture("personagemOficial100.png"); //Unicornio
    Texture2D comida = LoadTexture("comida35.png"); //Cupcake
    Texture2D sky = LoadTexture("skyOficial.png"); //Plano de fundo
    Texture2D inimigo = LoadTexture("inimigoOficial100.png"); //Bruxa
    
    //MÚSICA
    InitAudioDevice(); 
    Music ambiente = LoadMusicStream("musica_de_fundo.wav");
    PlayMusicStream(ambiente);
    Sound ponto = LoadSound("pontos.wav");

    SetTargetFPS(60);           

    //INICIALIZAÇÃO DOS PARÂMETROS DO OBSTACULO (CUPCAKE OU BRUXA)
    for (int i = 0; i < MAX_INIMIGOS; i++) //Máximo de 6 inimigos na tela
    {
        obstaculos[i].tipo = GetRandomValue(1, 2); //Gera o tipo de obstáculo aleatoriamente
                                                   // 1- cupcake  e 2- bruxa

        obstaculos[i].caminho = GetRandomValue(0,4 ); //O obstáculo tem no máximo 100 de altura   
                                                      //A tela tem 500, portanto, existem 5 caminhos possíveis        
        
        //Retângulo do obstaculo, os parâmetros são:
        //X do canto esquerdo, Y do canto esquerdo, largura da imagem, altura da imagem
        obstaculos[i].bordas = (Rectangle){screenWidth, alturaObs*obstaculos[i].caminho, larguraObs, alturaObs};
        obstaculos[i].ativo = false;
    }
  
    //Loop principal do jogo
    while (!WindowShouldClose())           
    {
        frames++;
        UpdateMusicStream(ambiente);
        
        //Movimentação do personagem
        if (IsKeyPressed(KEY_UP) && caminhoUni > 0) caminhoUni--;
        if (IsKeyPressed(KEY_DOWN) && caminhoUni < 4) caminhoUni++;
        
        bordasUni = (Rectangle){25.0f, caminhoUni*alturaUni, larguraUni, alturaUni}; //Atualizando o retângulo com base na movimentação
                                                                    //Colocamos esse '+30' pra ajustar a posição dele
        
       //Velocidade do jogo de acordo com os pontos
        
        if(pontos <= 1000){
            scrollingSky -= 3.0f;
            if (scrollingSky <= -sky.width) scrollingSky = 0.0f;
            velocidade = 10;
        }
        
        if(1000 < pontos && pontos <= 2000){
            scrollingSky -= 5.0f;
            if (scrollingSky <= -sky.width) scrollingSky = 0.0f;
            velocidade = 20;
        }
        
        if(pontos > 2000){
            scrollingSky -= 7.0f;
            if (scrollingSky <= -sky.width) scrollingSky = 0.0f;
            velocidade = 30;
        }
      
      //O invervalo de frames determina se teremos mais obstaculos ativos 
      //A cada frame ativa um novo inimigo
        if (frames > 20)
        {
            for (int i = 0; i < MAX_INIMIGOS; i++)
            {
                if (obstaculos[i].ativo == false)
                {
                    obstaculos[i].ativo = true;
                    break;
                }
            }

            frames = 0;
        }

        //Movimentação dos obstáculos na tela
        for (int i = 0; i < MAX_INIMIGOS; i++)
        {
            if (obstaculos[i].ativo == true)
            {
               
                obstaculos[i].bordas.x -= velocidade; //Determinar a movimentação dos obstaculos
            }

          
            //Se o obstaculo saiu da tela, tem que gerar outro e desativar
            if (obstaculos[i].bordas.x <= 0 - 128) //O '-128' para dar a impressão que continua andando infinitamente
            {
                obstaculos[i].tipo = GetRandomValue(1, 2);
                obstaculos[i].caminho = GetRandomValue(0,4 );

                obstaculos[i].bordas = (Rectangle){ screenWidth, alturaObs*obstaculos[i].caminho, larguraObs, alturaObs };
                obstaculos[i].ativo = false;
            }
           
        }


        for (int i = 0; i < MAX_INIMIGOS; i++)
        {
            if (obstaculos[i].ativo == true)
            {
                if (CheckCollisionRecs(bordasUni, obstaculos[i].bordas))
                {
                    if (obstaculos[i].tipo == 2)   //Colidiu com a bruxa irá morrer 
                    {     
                          if(pontos > recorde) Salvar(NOME, pontos);
                          return 0;
                       
                    }
                    else    //Se colidir com o cupcake irá ganhar pontos
                    {
                        obstaculos[i].ativo = false;
                        obstaculos[i].tipo = GetRandomValue(1, 2);
                        obstaculos[i].caminho = GetRandomValue(0,4 );

                        obstaculos[i].bordas = (Rectangle){ screenWidth + 14, alturaObs*obstaculos[i].caminho, larguraObs, alturaObs };
                        pontos = pontos+50; //Cada cupcake vale 50 pontos
        
                        PlaySound(ponto);
                    }
                }
            }
        }


        
        //DESENHAR NA TELA
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            //Rolamento infinito do background
            DrawTextureEx(sky, (Vector2) {scrollingSky, 0},0.0f, 1.0f, WHITE); 
            DrawTextureEx(sky, (Vector2) {sky.width + scrollingSky, 0},0.0f, 1.0f, WHITE);
            
            //Painel de pontos
            DrawText(TextFormat("Pontos: %08i", pontos), 25, 510, 30, RED);
            DrawText(TextFormat("Pontuação Máxima: %08i", recorde), 350, 510, 30, PURPLE);                 
            
            //Personagens
            //DrawRectangleRec(bordasUni, RED);
            DrawTexture(personagem, bordasUni.x, bordasUni.y, WHITE);
           
            //Lógica para desenhar os obstáculos
            for (int i = 0; i < MAX_INIMIGOS; i++)
                    {
                        if (obstaculos[i].ativo == true)
                        {
                            
                            //DrawRectangleRec(obstaculos[i].bordas, BLACK);
                            switch(obstaculos[i].tipo)
                            {
                                case 1: DrawTexture(comida, obstaculos[i].bordas.x - 14, obstaculos[i].bordas.y + 10, WHITE); break; //Esse '+10' é para ajudar o cupcake no meio do retângulo
                                case 2: DrawTexture(inimigo, obstaculos[i].bordas.x - 14, obstaculos[i].bordas.y, WHITE); break;
                             
                                default: break;
                            }
                 
                        }
                    }

                         
        EndDrawing();
        
    }

    
    UnloadTexture(comida); 
    UnloadTexture(inimigo);
    
    UnloadMusicStream(ambiente);        

    CloseAudioDevice();  
    
    CloseWindow();        

    return 0;
}