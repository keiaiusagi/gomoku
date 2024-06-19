#include <curses.h>
#include <string.h>

#define BORD_WIDTH (10)
#define BORD_HEIGHT (10)

#define WIDTH (38)
#define HEIGHT (20)

char bord[HEIGHT][WIDTH] = {
"+---+---+---+---+---+---+---+---+---+",
"|   |   |   |   |   |   |   |   |   |",
"+---+---+---+---+---+---+---+---+---+",
"|   |   |   |   |   |   |   |   |   |",
"+---+---+---+---+---+---+---+---+---+",
"|   |   |   |   |   |   |   |   |   |",
"+---+---+---+---+---+---+---+---+---+",
"|   |   |   |   |   |   |   |   |   |",
"+---+---+---+---+---+---+---+---+---+",
"|   |   |   |   |   |   |   |   |   |",
"+---+---+---+---+---+---+---+---+---+",
"|   |   |   |   |   |   |   |   |   |",
"+---+---+---+---+---+---+---+---+---+",
"|   |   |   |   |   |   |   |   |   |",
"+---+---+---+---+---+---+---+---+---+",
"|   |   |   |   |   |   |   |   |   |",
"+---+---+---+---+---+---+---+---+---+",
"|   |   |   |   |   |   |   |   |   |",
"+---+---+---+---+---+---+---+---+---+"
};

// 配置した石を格納する配列
char BORD_ARRAY[BORD_HEIGHT][BORD_WIDTH] = { 0 };

typedef struct {
    int x;
    int y;
} INDEX;

INDEX index[BORD_HEIGHT][BORD_WIDTH] = {
    { {0, 0} ,{ 4, 0},{8, 0},{12, 0},{16, 0},{20, 0},{24, 0},{28, 0},{32, 0},{36, 0}},
    { {0, 2} ,{ 4, 2},{8, 2},{12, 2},{16, 2},{20, 2},{24, 2},{28, 2},{32, 2},{36, 2}},
    { {0, 4} ,{ 4, 4},{8, 4},{12, 4},{16, 4},{20, 4},{24, 4},{28, 4},{32, 4},{36, 4}},
    { {0, 6} ,{ 4, 6},{8, 6},{12, 6},{16, 6},{20, 6},{24, 6},{28, 6},{32, 6},{36, 6}},
    { {0, 8} ,{ 4, 8},{8, 8},{12, 8},{16, 8},{20, 8},{24, 8},{28, 8},{32, 8},{36, 8}},
    { {0,10} ,{ 4,10},{8,10},{12,10},{16,10},{20,10},{24,10},{28,10},{32,10},{36,10}},
    { {0,12} ,{ 4,12},{8,12},{12,12},{16,12},{20,12},{24,12},{28,12},{32,12},{36,12}},
    { {0,14} ,{ 4,14},{8,14},{12,14},{16,14},{20,14},{24,14},{28,14},{32,14},{36,14}},
    { {0,16} ,{ 4,16},{8,16},{12,16},{16,16},{20,16},{24,16},{28,16},{32,16},{36,16}},
    { {0,18} ,{ 4,18},{8,18},{12,18},{16,18},{20,18},{24,18},{28,18},{32,18},{36,18}}
};

// 石を格納するボード
int BORD_DATA[BORD_HEIGHT][BORD_WIDTH] = { 0 };

//プレーヤーのアイコン
#define PL_BR "@" // 黒石アイコン
#define PL_WH "O" // 白石アイコン
#define CUR_I "_" // カーソルアイコン

typedef struct {
    int x;
    int y;
} CUR;



CUR cur;
int currentPlayer = 1; // 1: 黒石, -1: 白石

void PictScreen() {
    clear(); // 画面をクリア
    
    for (int y = 0; y < HEIGHT - 1; y++) {
        for (int x = 0; x < WIDTH - 1; x++) {
            mvprintw(y, x, &bord[y][x]); // １文字づつ表示しないと画面がずれる
        }
    }

    // ボード上の石を表示
    for (int y = 0; y < BORD_HEIGHT; y++) {
        for (int x = 0; x < BORD_WIDTH; x++) {
            if (BORD_DATA[y][x] == 1) {
                mvprintw(index[y][x].y, index[y][x].x, PL_BR); // 黒石
            }
            else if (BORD_DATA[y][x] == -1) {
                mvprintw(index[y][x].y, index[y][x].x, PL_WH); // 白石
            }
        }
    }

    // 画面表示
    //mvprintw(19, 1, "y=%d x=%d index_y =%d  index_x = %d ", cur.y, cur.x, index[cur.y][cur.x].y, index[cur.y][cur.x].x);

    // カーソルの表示
    mvprintw(index[cur.y][cur.x].y, index[cur.y][cur.x].x, currentPlayer == 1 ? PL_BR : PL_WH);
}

void init() {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();


    cur.x = 0, cur.y = 0; // カーソルの初期位置を左上に
    PictScreen();
}

bool isWin(int player) {
    // 方向のベクトル
    int dx[] = { 1, 0, 1, 1 };
    int dy[] = { 0, 1, 1, -1 };

    for (int y = 0; y < BORD_HEIGHT; y++) {
        for (int x = 0; x < BORD_WIDTH; x++) {
            if (BORD_DATA[y][x] == player) {
                for (int dir = 0; dir < 4; dir++) { //dir 0 右方向検索　1 下方向検索　2　右下方向検索　3 左下方向検索
                    int count = 1;
                    for (int step = 1; step < 5; step++) {
                        int nx = x + step * dx[dir];
                        int ny = y + step * dy[dir];
                        if (nx >= 0 && nx < BORD_WIDTH && ny >= 0 && ny < BORD_HEIGHT && BORD_DATA[ny][nx] == player) {
                            count++;
                        }
                        else {
                            break;
                        }
                    }
                    if (count == 5) {
                        //5つ並んだ方の勝利
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

int main() {
    int ch;
    init();
    refresh();

    while (1) {
        ch = getch();

        if (ch == KEY_UP) {
            cur.y = (cur.y - 1 + BORD_HEIGHT) % BORD_HEIGHT; // y座標を1減らす
        }
        else if (ch == KEY_DOWN) {
            cur.y = (cur.y + 1) % BORD_HEIGHT; // y座標を1増やす
        }
        else if (ch == KEY_LEFT) {
            cur.x = (cur.x - 1 + BORD_WIDTH) % BORD_WIDTH; // x座標を1減らす
        }
        else if (ch == KEY_RIGHT) {
            cur.x = (cur.x + 1) % BORD_WIDTH; // x座標を1増やす
        }
        else if (ch == ' ') {
            if (BORD_DATA[cur.y][cur.x] == 0) { // 空いている場所に石を置ける
                BORD_DATA[cur.y][cur.x] = currentPlayer; // 現在のプレイヤーの石を置く
                
                if (isWin(currentPlayer)) {
                    mvprintw(20, 1, "%s プレーヤーが勝利しました！", currentPlayer == 1 ? "黒" : "白");
                    refresh();
                    getch();
                    break;
                }
                currentPlayer = -currentPlayer; // プレイヤーを交代
            }
        }
        else if (ch == 'q') {
            break;
        }

        PictScreen();
        refresh();
    }

    endwin();
    return 0;
}
