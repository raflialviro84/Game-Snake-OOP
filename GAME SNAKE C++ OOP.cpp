#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <cstdlib>
using namespace std;

const int WIDTH = 20;
const int HEIGHT = 20;

class Position {
public:
    int x, y;
    Position(int x = 0, int y = 0) : x(x), y(y) {}
};

class Entity {
protected:
    Position position;

public:
    virtual void move() = 0; // Semua entitas harus dapat bergerak
    virtual ~Entity() {}
    Position getPosition() const {
        return position;
    }
    void setPosition(int x, int y) {
        position.x = x;
        position.y = y;
    }
};

class Snake : public Entity {
public:
    enum class Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

private:
    std::vector<Position> tail;
    int tailLength;
    Direction direction;

public:
    Snake() : tailLength(0), direction(Direction::STOP) {
        position.x = WIDTH / 2;
        position.y = HEIGHT / 2;
    }

    // Memindahkan kepala dan memperbarui posisi ekor
    void move() override {
        if (tailLength > 0) {
            tail.insert(tail.begin(), position);
            if (tail.size() > static_cast<size_t>(tailLength))
                tail.pop_back();
        }

        // Perbarui posisi kepala berdasarkan arah
        switch (direction) {
            case Direction::LEFT:  position.x--; break;
            case Direction::RIGHT: position.x++; break;
            case Direction::UP:    position.y--; break;
            case Direction::DOWN:  position.y++; break;
            case Direction::STOP:  break;
        }


        // Logika pembungkus (wrap around) jika mencapai batas layar
        if (position.x >= WIDTH) position.x = 0;
        else if (position.x < 0) position.x = WIDTH - 1;
        if (position.y >= HEIGHT) position.y = 0;
        else if (position.y < 0) position.y = HEIGHT - 1;
    }

    // Memeriksa apakah kepala ular bertabrakan dengan ekornya sendiri
    bool checkSelfCollision() const {
        for (const auto& segment : tail) {
            if (segment.x == position.x && segment.y == position.y)
                return true;
        }
        return false;
    }

    // Mengubah arah ular
    void setDirection(Direction dir) {
        direction = dir;
    }

    // Mendapatkan panjang ekor
    int getTailLength() const {
        return tailLength;
    }

    // Menambah panjang ekor
    void increaseTailLength() {
        tailLength++;
    }

    // Mendapatkan posisi kepala ular
    Position getHeadPosition() const {
        return position;
    }

    // Mendapatkan ekor ular
    const vector<Position>& getTail() const {
        return tail;
    }
};

class Fruit : public Entity {
public:
    Fruit() {
        setRandomPosition();
    }

    // Menetapkan posisi buah secara acak
    void setRandomPosition() {
        position.x = rand() % WIDTH;
        position.y = rand() % HEIGHT;
    }

    // Fungsi untuk memindahkan buah (tidak diperlukan di sini)
    void move() override {}
};

class Game {
private:
    bool gameOver;
    int score;
    Snake snake;
    Fruit fruit;

public:
    Game() : gameOver(false), score(0) { }

    // Menyiapkan kondisi awal permainan
    void setup() {
        snake.setPosition(WIDTH / 2, HEIGHT / 2);
        fruit.setRandomPosition();
    }

    // Menggambar papan permainan, dinding, ular, dan buah
    void draw() {
        system("cls");
        // Gambar dinding atas
        for (int i = 0; i < WIDTH + 2; i++)
            cout << "-";
        cout << endl;

        // Gambar area permainan
        for (int i = 0; i < HEIGHT; i++) {
            cout << "|";
            for (int j = 0; j < WIDTH; j++) {
                if (i == snake.getHeadPosition().y && j == snake.getHeadPosition().x)
                    cout << "O"; // Kepala ular
                 else if (i == fruit.getPosition().y && j == fruit.getPosition().x)
                    cout << "@"; // Buah
                else {
                    bool printed = false;
                    for (const auto& segment : snake.getTail()) {
                        if (segment.x == j && segment.y == i) {
                            cout << "o"; // Bagian ekor ular
                            printed = true;
                            break;
                        }
                    }
                    if (!printed)
                        cout << " ";
                }
            }
            cout << "|\n";
        }

        // Gambar dinding bawah
        for (int i = 0; i < WIDTH + 2; i++)
            cout << "-";
        cout << "\nScore: " << score << "\n";
    }

    // Menangani input dari pengguna
    void input() {
        if (_kbhit()) {
            switch (_getch()) {
                case 'a': snake.setDirection(Snake::Direction::LEFT);  break;
                case 'd': snake.setDirection(Snake::Direction::RIGHT); break;
                case 'w': snake.setDirection(Snake::Direction::UP);    break;
                case 's': snake.setDirection(Snake::Direction::DOWN);  break;
                case 'x': gameOver = true;                             break;
            }
        }
    }


    // Mengelola logika permainan, seperti pergerakan ular dan deteksi tabrakan
    void logic() {
        snake.move();

        if (snake.checkSelfCollision())
            gameOver = true;

        // Jika kepala ular bertemu dengan buah
        if (snake.getHeadPosition().x == fruit.getPosition().x && snake.getHeadPosition().y == fruit.getPosition().y) {
            score += 10;
            fruit.setRandomPosition();
            snake.increaseTailLength();
        }
    }

    // Metode utama untuk menjalankan permainan
    void run() {
        while (!gameOver) {
            draw();
            input();
            logic();
            Sleep(200);
        }
    }

    ~Game() {
        cout << "Permainan selesai. Skor akhir: " << score << endl;
    }
};

int main() {
    Game game;
    game.setup();
    game.run();
    return 0;
}
