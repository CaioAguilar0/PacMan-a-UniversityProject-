#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib> // Para std::rand() e std::srand()
#include <ctime>   // Para std::time()
#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <set>


// Definições
const float SIZE = 50.0f; // Tamanho de cada bloco do mapa   
const int MAP_WIDTH = 21;  // Largura do mapa em células
const int MAP_HEIGHT = 11; // Altura do mapa em células


// Declarações globais ou no escopo apropriado
sf::Sprite ghostSpriteRed;
sf::Sprite ghostSpriteOrange;
sf::Sprite ghostSpritePink;
sf::Sprite ghostSpriteBlue;

// Texturas associadas aos sprites
sf::Texture ghostTextureRed;
sf::Texture ghostTextureOrange;
sf::Texture ghostTexturePink;
sf::Texture ghostTextureBlue;
sf::Vector2i ghostPositionRed; // Posição lógica do Blinky
std::vector<sf::Vector2i> ghostPositions = {
    {1, 9}, // Posição inicial de Blinky
    {1, 1}, // Posição inicial de Inky
    {18, 1}, // Posição inicial de Pinky
    {18, 9}  // Posição inicial de Clyde
};

// Mapa do jogo
char mapa[MAP_HEIGHT][MAP_WIDTH] = {
    "11111111111111111111",
    "10000100000000100001",
    "10110101111110101101",
    "10100000000000000101",
    "00101101100110110100",
    "10000001000010000001",
    "10101101111110110101",
    "10100000000000000101",
    "10110101111110101101",
    "10000100000000100001",
    "11111111111111111111"
};



// Função para verificar se a posição é uma parede
bool isWall(int x, int y) {
    // Verifica os limites do mapa
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) {
        return true;
    }
    return mapa[y][x] == '1';
}






// Direções possíveis para o movimento
enum Direction { NONE, LEFT, RIGHT, UP, DOWN };

struct Ghost {
    sf::Vector2i position;
    Direction direction;
    float speed;
};

Ghost blinky = { {1, 2}, NONE, 0.1f }; // Defina a posição inicial e a velocidade
// Variáveis globais
int posx = 9;  // Posição inicial do Pac-Man (em células do mapa)
int posy = 7;  // Posição inicial do Pac-Man (em células do mapa)
int score = 0; // Pontuação inicial
Direction currentDirection = NONE; // Direção atual do movimento do Pac-Man
Direction nextDirection = NONE;    // Próxima direção a ser tomada pelo Pac-Man
 float timeElapsed = 0.0f;
    float ghostTimeElapsed = 0.0f;
float pacmanSpeed = 0.2f; // Velocidade do Pac-Man (segundos por movimento)

// Posições iniciais dos fantasmas
// Declara e inicializa o vetor com posições dos fantasmas
// Inicialize as posições dos fantasmas




// Função para verificar colisão do Pac-Man com fantasmas
bool checkCollisionWithGhosts() {
    // Posições do Pac-Man em pixels
    float pacmanX = posx * SIZE;
    float pacmanY = posy * SIZE;

    // Itera sobre as posições dos fantasmas
    for (int i = 0; i < 4; ++i) {
        // Posições dos fantasmas em pixels
        float ghostX = ghostPositions[i].x * SIZE;
        float ghostY = ghostPositions[i].y * SIZE;

        // Verifica a colisão com o Pac-Man
        if (fabs(pacmanX - ghostX) < SIZE && fabs(pacmanY - ghostY) < SIZE) {
            std::cout << "Colisão detectada em (" << posx << ", " << posy << ") com fantasma em (" << ghostPositions[i].x << ", " << ghostPositions[i].y << ")" << std::endl;
            return true; // Colisão detectada
        }
    }
    return false; // Nenhuma colisão
}

// 






// Função para reiniciar o jogo
void resetGame() {
    posx = 9;
    posy = 7;
    score = 0;
    currentDirection = NONE;
    nextDirection = NONE;
    pacmanSpeed = 0.2f;

   
   ghostPositions[0] = sf::Vector2i(1, 9);
    ghostPositions[1] = sf::Vector2i(1, 1);
    ghostPositions[2] = sf::Vector2i(18,1 );
    ghostPositions[3] = sf::Vector2i(18,2);

    // Reinicie a posição dos sprites dos fantasmas
    ghostSpriteRed.setPosition(ghostPositions[0].x * SIZE, ghostPositions[0].y * SIZE);
    ghostSpriteOrange.setPosition(ghostPositions[1].x * SIZE, ghostPositions[1].y * SIZE);
    ghostSpritePink.setPosition(ghostPositions[2].x * SIZE, ghostPositions[2].y * SIZE);
    ghostSpriteBlue.setPosition(ghostPositions[3].x * SIZE, ghostPositions[3].y * SIZE);



    // Restaurar as pílulas no mapa
    char initialMap[MAP_HEIGHT][MAP_WIDTH] = {
        "11111111111111111111",
        "10000100000000100001",
        "10110101111110101101",
        "10100000000000000101",
        "00101101100110110100",
        "10000001000010000001",
        "10101101111110110101",
        "10100000000000000101",
        "10110101111110101101",
        "10000100000000100001",
        "11111111111111111111"
    };
    std::copy(&initialMap[0][0], &initialMap[0][0] + MAP_HEIGHT * MAP_WIDTH, &mapa[0][0]);
}





// Função para verificar se a posição contém uma pílula
bool hasPill(int x, int y) {
    return mapa[y][x] == '0';
}

// Função para comer a pílula
void eatPill(int x, int y) {
    if (hasPill(x, y)) {
        mapa[y][x] = ' '; // Remove a pílula do mapa
        score += 10; // Aumenta a pontuação
    }
}


sf::Vector2i getPacmanPosition() {
    return sf::Vector2i(posx, posy); // Assumindo que posx e posy são as coordenadas atuais do Pac-Man
}
// Função auxiliar para verificar se há um fantasma em uma posição
bool isGhost(int x, int y) {
    for (int i = 0; i < 4; ++i) {
        if (ghostPositions[i].x == x && ghostPositions[i].y == y) {
            return true;
        }
    }
    return false;
}
void refreshGhostPosition(int ghostIndex, const sf::Vector2i& pacmanPos) {
    static std::vector<sf::Clock> ghostClocks(4);  // Relógios individuais para cada fantasma
    static std::vector<sf::Vector2i> lastPositions(4, {-1, -1}); // Última posição de cada fantasma

    if (ghostClocks[ghostIndex].getElapsedTime() > sf::seconds(0.4)) {
        ghostClocks[ghostIndex].restart();  // Reinicia o relógio do fantasma atual

        sf::Vector2i& ghostPos = ghostPositions[ghostIndex];

        // Blinky (fantasma 0) persegue o Pac-Man diretamente
        if (ghostIndex == 0) {
            sf::Vector2i bestMove = ghostPos;  // Melhor movimento a ser feito
            int minDistance = std::numeric_limits<int>::max();

            // Tenta mover o fantasma em todas as quatro direções possíveis
            std::vector<std::pair<int, int>> directions = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
            for (const auto& dir : directions) {
                sf::Vector2i newPos = ghostPos + sf::Vector2i(dir.first, dir.second);

                if (!isWall(newPos.x, newPos.y)) {
                    int distance = abs(pacmanPos.x - newPos.x) + abs(pacmanPos.y - newPos.y);
                    
                    if (distance < minDistance) {
                        minDistance = distance;
                        bestMove = newPos;
                    }
                }
            }

            // Aplica o efeito de túnel se o fantasma estiver na linha do túnel
            if (ghostPos.y == 4) { // Verifica se está na linha do túnel
                if (bestMove.x < 0) {
                    bestMove.x = MAP_WIDTH - 2; // Saiu pela esquerda, reaparece à direita
                } else if (bestMove.x >= MAP_WIDTH - 1) {
                    bestMove.x = 0; // Saiu pela direita, reaparece à esquerda
                }
            }

            // Atualiza a posição do Blinky para a melhor direção encontrada
            if (bestMove != ghostPos) {
                lastPositions[ghostIndex] = ghostPos;  // Armazena a posição anterior
                ghostPos = bestMove;
            }

            // Atualiza a posição do sprite do Blinky
            ghostSpriteRed.setPosition(ghostPos.x * SIZE, ghostPos.y * SIZE);

        } else {
            // Movimento aleatório para outros fantasmas
            std::vector<std::pair<int, int>> directions = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
            std::vector<int> validDirections;

            for (int i = 0; i < directions.size(); ++i) {
                sf::Vector2i newPos = ghostPos + sf::Vector2i(directions[i].first, directions[i].second);

                // Verifica se não é uma parede e não é outro fantasma
                if (!isWall(newPos.x, newPos.y)) {
                    bool isOtherGhost = false;
                    for (int j = 0; j < 4; ++j) {
                        if (j != ghostIndex && ghostPositions[j] == newPos) {
                            isOtherGhost = true;
                            break;
                        }
                    }

                    if (!isOtherGhost && newPos != lastPositions[ghostIndex]) {
                        validDirections.push_back(i);
                    }
                }
            }

            // Se nenhuma direção válida foi encontrada, permite voltar à última posição
            if (validDirections.empty()) {
                for (int i = 0; i < directions.size(); ++i) {
                    sf::Vector2i newPos = ghostPos + sf::Vector2i(directions[i].first, directions[i].second);
                    if (newPos == lastPositions[ghostIndex]) {
                        validDirections.push_back(i);
                    }
                }
            }

            // Escolhe uma direção aleatória válida
            if (!validDirections.empty()) {
                int chosenDirection = validDirections[std::rand() % validDirections.size()];

                // Atualiza a posição do fantasma
                lastPositions[ghostIndex] = ghostPos;  // Armazena a posição anterior
                ghostPos += sf::Vector2i(directions[chosenDirection].first, directions[chosenDirection].second);

                // Aplica o efeito de túnel se o fantasma estiver na linha do túnel
                if (ghostPos.y == 4) { // Verifica se está na linha do túnel
                    if (ghostPos.x < 0) {
                        ghostPos.x = MAP_WIDTH - 2; // Saiu pela esquerda, reaparece à direita
                    } else if (ghostPos.x >= MAP_WIDTH - 1) {
                        ghostPos.x = 0; // Saiu pela direita, reaparece à esquerda
                    }
                }

                // Atualiza a posição do sprite do fantasma
                switch (ghostIndex) {
                    case 1: ghostSpriteOrange.setPosition(ghostPos.x * SIZE, ghostPos.y * SIZE); break;
                    case 2: ghostSpritePink.setPosition(ghostPos.x * SIZE, ghostPos.y * SIZE); break;
                    case 3: ghostSpriteBlue.setPosition(ghostPos.x * SIZE, ghostPos.y * SIZE); break;
                }
            }
        }
    }
}

void moveAllGhosts() {
    sf::Vector2i pacmanPos = getPacmanPosition(); // Função que retorna a posição do Pac-Man
    for (int i = 0; i < 4; ++i) {
        refreshGhostPosition(i, pacmanPos);
    }
}


// Esta função pode ser chamada dentro do loop principal do jogo para cada fantasma:

#include <iostream>
// Função para verificar se a posição é um túnel e aplicar o efeito


int main() {
    // Inicializa a semente aleatória

    std::srand(static_cast<unsigned>(std::time(nullptr)));
  sf::Clock clocki; // Para medir o tempo decorrido
    // Cria a janela
     sf::RenderWindow window(sf::VideoMode(MAP_WIDTH * SIZE, MAP_HEIGHT * SIZE + 50), "Pac-Man");

float ghostSpeed;
    // Carregar texturas
    sf::Texture wallTexture, backgroundTexture, pillTexture, ghostRedTexture, ghostOrangeTexture, ghostPinkTexture, ghostBlueTexture, pacmanleftTexture,pacmanrightTexture, pacmandownTexture, pacmanupTexture;
    if (!wallTexture.loadFromFile("wall.png") ||
        !pacmanrightTexture.loadFromFile("pacman_right.png") ||
        !backgroundTexture.loadFromFile("background.png") ||
        !pillTexture.loadFromFile("pill.png") ||
        !ghostRedTexture.loadFromFile("ghost_red.png") ||
        !ghostOrangeTexture.loadFromFile("ghost_orange.png") ||
        !ghostPinkTexture.loadFromFile("ghost_pink.png") ||
        !ghostBlueTexture.loadFromFile("ghost_blue.png") ||
        !pacmanleftTexture.loadFromFile("pacman_left.png") ||
        !pacmanupTexture.loadFromFile("pacman_up.png") ||
        !pacmandownTexture.loadFromFile("pacman_down.png")) {
        std::cerr << "Erro ao carregar texturas" << std::endl;
        return -1;
    }

    // Cria sprites
    sf::Sprite wallSprite(wallTexture);
    sf::Sprite pacmanrightsprite(pacmanrightTexture);
    sf::Sprite backgroundSprite(backgroundTexture);
    sf::Sprite pillSprite(pillTexture);
    sf::Sprite ghostSpriteRed(ghostRedTexture);
    sf::Sprite ghostSpriteOrange(ghostOrangeTexture);
    sf::Sprite ghostSpritePink(ghostPinkTexture);
    sf::Sprite ghostSpriteBlue(ghostBlueTexture);
    sf::Sprite pacmanleftsprite(     pacmanleftTexture);
    sf::Sprite pacmanupsprite(pacmanupTexture);
    sf::Sprite pacmandownsprite(pacmandownTexture);
    sf::Sprite pacmanSprite(pacmanrightTexture);


    
    // Ajustar tamanhos dos sprites
   // Ajustando o tamanho do sprite da parede para corresponder ao tamanho especificado (SIZE)
wallSprite.setScale(static_cast<float>(SIZE) / wallTexture.getSize().x, static_cast<float>(SIZE) / wallTexture.getSize().y);

// Ajustando o tamanho dos sprites do Pac-Man para diferentes direções
pacmanrightsprite.setScale(static_cast<float>(SIZE) / pacmanrightTexture.getSize().x, static_cast<float>(SIZE) / pacmanrightTexture.getSize().y);
pacmanleftsprite.setScale(static_cast<float>(SIZE) / pacmanleftTexture.getSize().x, static_cast<float>(SIZE) / pacmanleftTexture.getSize().y);
pacmanupsprite.setScale(static_cast<float>(SIZE) / pacmanupTexture.getSize().x, static_cast<float>(SIZE) / pacmanupTexture.getSize().y);
pacmandownsprite.setScale(static_cast<float>(SIZE) / pacmandownTexture.getSize().x, static_cast<float>(SIZE) / pacmandownTexture.getSize().y);

// Ajustando o tamanho do sprite de fundo para cobrir toda a janela do jogo
backgroundSprite.setScale(static_cast<float>(MAP_WIDTH * SIZE) / backgroundTexture.getSize().x, static_cast<float>(MAP_HEIGHT * SIZE + 50) / backgroundTexture.getSize().y);

// Ajustando o tamanho dos sprites das pílulas
pillSprite.setScale(static_cast<float>(SIZE / 2) / pillTexture.getSize().x, static_cast<float>(SIZE / 2) / pillTexture.getSize().y);

// Ajustando o tamanho dos sprites dos fantasmas para diferentes cores
ghostSpriteRed.setScale(static_cast<float>(SIZE) / ghostRedTexture.getSize().x, static_cast<float>(SIZE) / ghostRedTexture.getSize().y);
ghostSpriteOrange.setScale(static_cast<float>(SIZE) / ghostOrangeTexture.getSize().x, static_cast<float>(SIZE) / ghostOrangeTexture.getSize().y);
ghostSpritePink.setScale(static_cast<float>(SIZE) / ghostPinkTexture.getSize().x, static_cast<float>(SIZE) / ghostPinkTexture.getSize().y);
ghostSpriteBlue.setScale(static_cast<float>(SIZE) / ghostBlueTexture.getSize().x, static_cast<float>(SIZE) / ghostBlueTexture.getSize().y);



    // Inicializar outras variáveis
    ghostSpeed = 2.0f;


    sf::Clock clock;
    float timeElapsed = 0.0f;

ghostSpriteRed.setPosition(ghostPositions[0].x * SIZE, ghostPositions[0].y * SIZE);
    ghostSpriteOrange.setPosition(ghostPositions[1].x * SIZE, ghostPositions[1].y * SIZE);
    ghostSpritePink.setPosition(ghostPositions[2].x * SIZE, ghostPositions[2].y * SIZE);
    ghostSpriteBlue.setPosition(ghostPositions[3].x * SIZE, ghostPositions[3].y * SIZE);

   // Inicializar as fontes
sf::Font font;
sf::Font fontnum;
if (!font.loadFromFile("PAC-FONT.ttf")) {
    std::cerr << "Erro ao carregar fonte" << std::endl;
    return -1;
}
if (!fontnum.loadFromFile("Arial.ttf")) {
    std::cerr << "Erro ao carregar fonte" << std::endl;
    return -1;
}
// Ajuste a posição inicial dos sprites dos fantasmas após definir ghostPositions

// Inicializar os textos
sf::Text pacmove;
sf::Text scoreText;
sf::Text scoreTextnum;
pacmove.setFont(font);
scoreText.setFont(font);
scoreTextnum.setFont(fontnum);
pacmove.setCharacterSize(30);
scoreText.setCharacterSize(24);
scoreTextnum.setCharacterSize(24);
pacmove.setFillColor(sf::Color::Yellow);
scoreText.setFillColor(sf::Color::White);
scoreTextnum.setFillColor(sf::Color::Yellow);
pacmove.setPosition(275.0f, MAP_HEIGHT * SIZE + 8.0f);
scoreText.setPosition(10.0f, MAP_HEIGHT * SIZE + 10.0f);
scoreTextnum.setPosition(200.0f, MAP_HEIGHT * SIZE + 10.0f);

int temp = 0;
sf::Clock pacmoveClock;  // Temporizador para controlar a atualização do texto
float pacmoveUpdateInterval = 0.2f;  // Atualizar a cada 0.5 segundos 
float pacmoveElapsedTime = 0.0f;

while (window.isOpen()) {
          // Verifica colisão com fantasmas e mostra a tela de derrota se necessário
     
        
    sf::Time deltaTime = clock.restart();
    timeElapsed += deltaTime.asSeconds();
    pacmoveElapsedTime += deltaTime.asSeconds();




    // Processar eventos
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }



        // Atualiza a próxima direção com base nas teclas pressionadas
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Left) {
                nextDirection = LEFT;
            } else if (event.key.code == sf::Keyboard::Right) {
                nextDirection = RIGHT;
            } else if (event.key.code == sf::Keyboard::Up) {
                nextDirection = UP;
            } else if (event.key.code == sf::Keyboard::Down) {
                nextDirection = DOWN;
            }
            else if (event.key.code == sf::Keyboard::Escape) { // Adiciona a opção de encerrar o jogo
                window.close();
            }
        }
    }
     // Variável para armazenar o tempo decorrido para controle de velocidade
    
    // Atualiza a direção
if (nextDirection != NONE) {
    int newX = posx;
    int newY = posy;

    // Atualiza a nova posição com base na próxima direção
    if (nextDirection == LEFT) newX--;
    else if (nextDirection == RIGHT) newX++;
    else if (nextDirection == UP) newY--;
    else if (nextDirection == DOWN) newY++;

    // Aplica o efeito de túnel na nova posição
    if (newY == 4) { // Verifica se está na linha dos túneis
        if (newX < 0) {
            newX = MAP_WIDTH - 2; // Saiu pela esquerda, reaparece à direita
        } else if (newX >= MAP_WIDTH -1) {
            newX = 0; // Saiu pela direita, reaparece à esquerda
        }
    }

    // Verifica se a nova posição não é uma parede
    if (!isWall(newX, newY)) {
        currentDirection = nextDirection;
        nextDirection = NONE;
    }
}

// Move o Pac-Man a cada pacmanSpeed segundos

if (timeElapsed >= pacmanSpeed) {
    int newX = posx;
    int newY = posy;

    // Atualiza a nova posição com base na direção atual
    if (currentDirection == LEFT) newX--;
    else if (currentDirection == RIGHT) newX++;
    else if (currentDirection == UP) newY--;
    else if (currentDirection == DOWN) newY++;

    // Aplica o efeito de túnel na nova posição
    if (newY == 4) { // Verifica se está na linha dos túneis
        if (newX < 0) {
            newX = MAP_WIDTH -2; // Saiu pela esquerda, reaparece à direita
        } else if (newX >= MAP_WIDTH-1) {
            newX = 0; // Saiu pela direita, reaparece à esquerda
        }
    }

    eatPill(posx, posy); // Come pílulas na posição antiga

    // Verifica se a nova posição não é uma parede
    if (!isWall(newX, newY)) {
        posx = newX;
        posy = newY;
    }

    timeElapsed = 0; // Reinicia o temporizador
}




    // Atualiza o texto pacmove com base no temporizador
    if (pacmoveElapsedTime >= pacmoveUpdateInterval) {
        pacmoveElapsedTime = 0;  // Reinicia o temporizador

  int g = 36;

if (score >= (g*30))
    pacmove.setString("9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9");
else if ( score >= (g*28))
    pacmove.setString("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 9 ");
else if ( score >= (g*27))
    pacmove.setString("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 9 ");
else if ( score >= (g*26))
    pacmove.setString("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 9 ");
else if ( score >= (g*25))
    pacmove.setString("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 9 ");
else if ( score >= (g*24))
    pacmove.setString("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 9 ");
else if ( score >= (g*23))
    pacmove.setString("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 9 ");
else if ( score >= (g*22))
    pacmove.setString("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 9 ");
else if ( score >= (g*21))
    pacmove.setString("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 9 ");
else if ( score >= (g*20))
    pacmove.setString("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 9 ");
else if ( score >= (g*19))
    pacmove.setString("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 9 ");
else if ( score >= (g*18))
    pacmove.setString("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 9 ");
else if ( score >= (g*17))
    pacmove.setString("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 9 ");
else if ( score >= (g*16))
    pacmove.setString("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 9 ");
else if ( score >= (g*15))
    pacmove.setString("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 9 ");
else if ( score >= (g*14))
    pacmove.setString("0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 9 ");
else if ( score >= (g*13))
    pacmove.setString("0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 9 ");
else if ( score >= (g*12))
    pacmove.setString("0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 9 ");
else if ( score >= (g*11))
    pacmove.setString("0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 9 ");
else if ( score >= (g*10))
    pacmove.setString("0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 9 ");
else if ( score >= (g*9))
    pacmove.setString("0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 9 ");
else if (score >= (g*8))
    pacmove.setString("0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 9 ");
else if (score >= (g*7))
    pacmove.setString("0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 9 ");
else if (score >= (g*6))
    pacmove.setString("0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 9 ");
else if (score >= (g*5))
    pacmove.setString("0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 9 ");
else if (score >= (g*4))
    pacmove.setString("0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 9 ");
else if (score >= (g*3))
    pacmove.setString("0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 9 ");
else if (score >= (g*2))
    pacmove.setString("0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 9 ");
else if (score >= (g*1))
    pacmove.setString("1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 9 ");

// Incrementa temp
temp = (temp + 1) % 31; // Ajuste o módulo conforme a necessidade


    }
    
    
    moveAllGhosts();



 if (checkCollisionWithGhosts()) {
    std::cout << "Pac-Man colidiu com um fantasma!" << std::endl;

    // Lógica para mostrar a tela de fim de jogo
    sf::Texture gameoverTexture;
    if (!gameoverTexture.loadFromFile("gameover.png")) {
        std::cerr << "Erro ao carregar a imagem da tela de fim de jogo." << std::endl;
        continue;
    }

    sf::Sprite gameoverSprite(gameoverTexture);
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = gameoverTexture.getSize();
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    float scale = std::max(scaleX, scaleY); // Usa o maior valor para preencher a tela
    gameoverSprite.setScale(scale, scale);
    gameoverSprite.setPosition(
        (windowSize.x - textureSize.x * scale) / 2.0f,
        (windowSize.y - textureSize.y * scale) / 2.0f
    );

    sf::Font font;
    if (!font.loadFromFile("PAC-FONT.ttf")) {
        std::cerr << "Erro ao carregar a fonte." << std::endl;
        continue;
    }

    sf::Text endText;
    endText.setFont(font);
    endText.setString("Press Enter to Restart");
    endText.setCharacterSize(30);
    endText.setFillColor(sf::Color::White);
    endText.setStyle(sf::Text::Bold);
    sf::FloatRect textRect = endText.getLocalBounds();
    endText.setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);
    endText.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f + 40.0f); // Move o texto 40 unidades para baixo

    sf::Clock clock; // Usado para controlar o efeito de pulsar

    bool keyPressed = false;
    while (!keyPressed) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                keyPressed = true;
            }
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            }
        }

        window.clear();
        window.draw(gameoverSprite);

        // Atualiza o efeito de pulsar
        float elapsedTime = clock.getElapsedTime().asSeconds();
        float scale = 1.0f + 0.1f * std::sin(elapsedTime * 3.0f); // Efeito de pulsar
        endText.setScale(scale, scale);
        window.draw(endText);

        window.display();
    }

    resetGame();
    continue;
}

// Verifica a condição de vitória
if (score >= 1000) {
    std::cout << "Você venceu!" << std::endl;

    // Lógica para mostrar a tela de vitória
    sf::Texture victoryTexture;
    if (!victoryTexture.loadFromFile("victory.png")) {
        std::cerr << "Erro ao carregar a imagem da tela de vitória." << std::endl;
        continue;
    }

    sf::Sprite victorySprite(victoryTexture);
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = victoryTexture.getSize();
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    float scale = std::max(scaleX, scaleY); // Usa o maior valor para preencher a tela
    victorySprite.setScale(scale, scale);
    victorySprite.setPosition(
        (windowSize.x - textureSize.x * scale) / 2.0f,
        (windowSize.y - textureSize.y * scale) / 2.0f
    );

    sf::Font font;
    if (!font.loadFromFile("PAC-FONT.ttf")) {
        std::cerr << "Erro ao carregar a fonte." << std::endl;
        continue;
    }

    sf::Text victoryText;
    victoryText.setFont(font);
    victoryText.setString("Press Enter to Restart");
    victoryText.setCharacterSize(30);
    victoryText.setFillColor(sf::Color::White);
    victoryText.setStyle(sf::Text::Bold);
    sf::FloatRect textRect = victoryText.getLocalBounds();
    victoryText.setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);
    victoryText.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f + 40.0f); // Move o texto 40 unidades para baixo

    sf::Clock clock; // Usado para controlar o efeito de pulsar

    bool keyPressed = false;
    while (!keyPressed) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                keyPressed = true;
            }
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            }
        }

        window.clear();
        window.draw(victorySprite);

        // Atualiza o efeito de pulsar
        float elapsedTime = clock.getElapsedTime().asSeconds();
        float scale = 1.0f + 0.1f * std::sin(elapsedTime * 3.0f); // Efeito de pulsar
        victoryText.setScale(scale, scale);
        window.draw(victoryText);

        window.display();
    }

    resetGame();
    continue;
}
   
   // Encontrar a direção mais curta para Blinky
  



    // Limpa a tela
    window.clear();

   
    // Desenha o fundo
    window.draw(backgroundSprite);

    // Desenha o mapa
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            if (mapa[y][x] == '1') {
                wallSprite.setPosition(x * SIZE, y * SIZE);
                window.draw(wallSprite);
            }
            else if (mapa[y][x] == '0') {
                pillSprite.setPosition(x * SIZE + SIZE / 4, y * SIZE + SIZE / 4);
                window.draw(pillSprite);
            }
        }
    }
 

    // Desenha o Pac-Man
    if(currentDirection == NONE)
    {pacmanleftsprite.setPosition(posx * SIZE, posy * SIZE);
    window.draw(pacmanleftsprite);}
     else if (currentDirection == LEFT) {
        pacmanleftsprite.setPosition(posx * SIZE, posy * SIZE);
        window.draw(pacmanleftsprite);
    }
    else if (currentDirection == RIGHT) {
        pacmanrightsprite.setPosition(posx * SIZE, posy * SIZE);
        window.draw(pacmanrightsprite);
    }
    else if (currentDirection == UP) {
        pacmanupsprite.setPosition(posx * SIZE, posy * SIZE);
        window.draw(pacmanupsprite);
    }
    else if (currentDirection == DOWN) {
        pacmandownsprite.setPosition(posx * SIZE, posy * SIZE);
        window.draw(pacmandownsprite);
    }
;               
        
    
    
    
 
ghostSpriteRed.setPosition(ghostPositions[0].x * SIZE, ghostPositions[0].y * SIZE);
ghostSpriteOrange.setPosition(ghostPositions[1].x * SIZE, ghostPositions[1].y * SIZE);
ghostSpritePink.setPosition(ghostPositions[2].x * SIZE, ghostPositions[2].y * SIZE);
ghostSpriteBlue.setPosition(ghostPositions[3].x * SIZE, ghostPositions[3].y * SIZE);

      window.draw(ghostSpriteRed);
      window.draw(ghostSpriteOrange);
      window.draw(ghostSpritePink);
      window.draw(ghostSpriteBlue);
    // Exibe o placar
    scoreText.setString("S C O R E : ");
    scoreTextnum.setString(std::to_string(score));
    scoreText.setPosition(10.0f, MAP_HEIGHT * SIZE + 10.0f);
    scoreTextnum.setPosition(200.0f, MAP_HEIGHT * SIZE + 10.0f);

    window.draw(scoreText);
    window.draw(scoreTextnum);
    window.draw(pacmove);

    std::cout << "Teste\n";
    // Exibe o conteúdo da janela
    window.display();
}


    return 0;
}