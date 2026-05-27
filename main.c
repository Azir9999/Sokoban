#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#define MAX_LEVEL 5
#define MAX_ROW 30
#define MAX_COL 31 // 널문자 포함
#define MAX_UNDO 5

void display_help(){
    printf("=======================================\n");
    printf("S O K O B A N H E L P\n");
    printf("=======================================\n");
    printf("왼쪽     h (left)\n");
    printf("아래     j (down)\n");
    printf("위       k (up)\n");
    printf("오른쪽   l (right)\n\n");
    printf("u (undo)\n");
    printf("a (again)\n");
    printf("n (new)\n");
    printf("r (record)\n");
    printf("e (record end)\n");
    printf("p (play recorded game)\n");
    printf("x (exit)\n");
    printf("s (save)\n");
    printf("f (file load)\n");
    printf("d (display help)\n");
    printf("t (top)\n");
    printf("enter (redraw map)\n\n");
    printf("=======================================\n");
}

void copy_map(char dest[MAX_ROW][MAX_COL], char src[MAX_ROW][MAX_COL])
{
    int i;

    for (i = 0; i < MAX_ROW; i++)
    {
        strcpy(dest[i], src[i]);
    }
}

void make_origin_map(char origin[MAX_ROW][MAX_COL],
                     char map[MAX_ROW][MAX_COL]) //가변요소(플레이어, 박스)를 제거한 원본 맵 생성
{
    int i, j;

    for (i = 0; i < MAX_ROW; i++)
    {
        for (j = 0; j < MAX_COL; j++)
        {
            if (map[i][j] == '@' || map[i][j] == '$')
            {
                origin[i][j] = '.';
            }
            else
            {
                origin[i][j] = map[i][j];
            }
        }
    }
}

void print_map(char state[MAX_ROW][MAX_COL])
{
    int i;

    for (i = 0; i < MAX_ROW; i++)
    {
        if (state[i][0] == '\0') //맵의 끝에 도달하면 더이상 출력 안함
            break;

        printf("%s\n", state[i]);
    }
}

int get_height(char state[MAX_ROW][MAX_COL])
{
    int i;

    for (i = 0; i < MAX_ROW; i++)
    {
        if (state[i][0] == '\0')
            return i;
    }

    return MAX_ROW;
}

int is_cleared(char state[MAX_ROW][MAX_COL],
               char origin[MAX_ROW][MAX_COL])
{
    int i, j;

    for (i = 0; i < MAX_ROW; i++)
    {
        for (j = 0; j < MAX_COL; j++)
        {
            if (origin[i][j] == 'O')
            {
                if (state[i][j] != '$')
                {
                    return 0;
                }
            }
        }
    }

    return 1;
}

int move_player(char state[MAX_ROW][MAX_COL],
                char origin[MAX_ROW][MAX_COL],
                char cmd)
{
    int y, x;
    int py = -1, px = -1;
    int dy = 0, dx = 0;

    for (y = 0; y < MAX_ROW; y++)
    {
        for (x = 0; x < MAX_COL; x++)
        {
            if (state[y][x] == '@')
            {
                py = y;
                px = x;
                break;
            }

            if (state[y][x] == '\0')
                break;
        }

        if (py != -1)
            break;

        if (state[y][0] == '\0')
            break;
    }

    if (py == -1)
        return 0;

    if (cmd == 'h' || cmd == 'H') dx = -1;
    else if (cmd == 'l' || cmd == 'L') dx = 1;
    else if (cmd == 'k' || cmd == 'K') dy = -1;
    else if (cmd == 'j' || cmd == 'J') dy = 1;
    else return 0;

    y = py + dy;
    x = px + dx;

    if (y < 0 || y >= MAX_ROW || x < 0 || x >= MAX_COL) //다음 이동할 위치가 맵을 넘어갔을 경우
        return 0;

    if (state[y][x] == '\0')
        return 0;

    if (state[y][x] == '#') //다음 위치가 벽의 위치일 경우
        return 0;

    if (state[y][x] == '$') //다음 위치가 박스의 위치일 경우
    {
        int by = y + dy, bx = x + dx; //박스가 이동할 위치

        if (by < 0 || by >= MAX_ROW || bx < 0 || bx >= MAX_COL) //박스가 이동할 위치가 맵을 넘어갔을 경우
            return 0;

        if (state[by][bx] == '\0')
            return 0;

        if (state[by][bx] == '.' || state[by][bx] == 'O') //박스가 이동할 위치가 빈 공간이나 목표 지점일 경우
        {
            state[by][bx] = '$'; //박스 이동
            state[y][x] = '@'; //플레이어 이동
            state[py][px] = origin[py][px]; //플레이어가 있던 위치를 . 으로 변경

            return 1; //이동 성공
        }

        return 0; //박스가 이동할 위치가 벽이나 다른 박스일 경우
    }

    if (state[y][x] == '.' || state[y][x] == 'O') //다음 위치가 빈 공간이나 목표 지점일 경우
    {
        state[y][x] = '@';
        state[py][px] = origin[py][px]; //현재 위치를 . 혹은 O으로 변경

        return 1; //이동 성공
    }

    return 0; //예외적인 경우
}

void save_game(char name[4],
               int level,
               int moves,
               char state[MAX_ROW][MAX_COL])
{
    FILE* fp = fopen("soko", "w");
    int i;
    int height;

    if (fp == NULL)//예외처리
    {
        printf("soko 파일을 저장할 수 없습니다.\n");
        return;
    }

    height = get_height(state); //저장을 위한 높이 추출

    fprintf(fp, "%s\n", name); //이름 저장
    fprintf(fp, "%d\n", level); //레벨 저장
    fprintf(fp, "%d\n", moves); //이동 횟수 저장
    fprintf(fp, "%d\n", height); //맵의 높이 저장 => 다음 불러오기 때 맵의 높이를 알아야 하기 때문

    for (i = 0; i < height; i++) //현재 맵 상태 저장
    {
        fprintf(fp, "%s\n", state[i]);
    }

    fclose(fp);
    printf("Saved\n");
}

void rank(void)
{
    FILE* fp = fopen("ranking", "r");
    int level;
    char name[10];
    char move[10];

    int current_level = 0;
    int printed = 0;

    if (fp == NULL)
    {
        printf("ranking 파일이 없습니다.\n");
        return;
    }

    printf("================\n\n");
    printf(" R A N K I N G\n\n");
    printf("================\n\n");

    while (fscanf(fp, "%d %9s %9s", &level, name, move) == 3)
    {
        if (level != current_level)
        {
            if (current_level != 0 && printed == 0)
            {
                printf("NONE\n\n");
            }

            current_level = level;
            printed = 0;

            printf("*** LEVEL %d ***\n\n", current_level);
        }

        if (strcmp(name, "NONE") != 0 && strcmp(move, "NONE") != 0)
        {
            printf("%s %s\n", name, move);
            printed = 1;
        }
    }

    if (current_level != 0 && printed == 0)
    {
        printf("NONE\n");
    }

    fclose(fp);
}

void update_ranking(char name[4], int level, int moves)
{
    FILE* fp;
    int r_level[25];
    char r_name[25][10];
    int r_move[25];
    char move_text[25][10];

    int i;
    int start;
    int insert_pos = -1;
    int old_pos = -1;

    fp = fopen("ranking", "r");

    if (fp == NULL)
    {
        printf("ranking 파일이 없습니다.\n");
        return;
    }

    for (i = 0; i < 25; i++)
    {
        if (fscanf(fp, "%d %9s %9s", &r_level[i], r_name[i], move_text[i]) != 3)
        {
            fclose(fp);
            printf("ranking 파일 형식이 잘못되었습니다.\n");
            return;
        }

        if (strcmp(move_text[i], "NONE") == 0)
            r_move[i] = -1;
        else
            sscanf(move_text[i], "%d", &r_move[i]);
    }

    fclose(fp);

    start = level * 5;

    for (i = start; i < start + 5; i++)
    {
        if (strcmp(r_name[i], name) == 0)
        {
            old_pos = i;
            break;
        }
    }

    if (old_pos != -1)
    {
        if (r_move[old_pos] <= moves)
            return;

        for (i = old_pos; i < start + 4; i++)
        {
            strcpy(r_name[i], r_name[i + 1]);
            r_move[i] = r_move[i + 1];
        }

        strcpy(r_name[start + 4], "NONE");
        r_move[start + 4] = -1;
    }

    for (i = start; i < start + 5; i++)
    {
        if (r_move[i] == -1 || moves < r_move[i])
        {
            insert_pos = i;
            break;
        }
    }

    if (insert_pos == -1)
        return;

    for (i = start + 4; i > insert_pos; i--)
    {
        strcpy(r_name[i], r_name[i - 1]);
        r_move[i] = r_move[i - 1];
    }

    strcpy(r_name[insert_pos], name);
    r_move[insert_pos] = moves;

    fp = fopen("ranking", "w");

    if (fp == NULL)
    {
        printf("ranking 파일을 저장할 수 없습니다.\n");
        return;
    }

    for (i = 0; i < 25; i++)
    {
        if (r_move[i] == -1)
            fprintf(fp, "%d NONE NONE\n", r_level[i]);
        else
            fprintf(fp, "%d %s %d\n", r_level[i], r_name[i], r_move[i]);
    }

    fclose(fp);
}

void play_game(char name[4],
               int level,
               int start_moves,
               char map[MAX_LEVEL][MAX_ROW][MAX_COL],
               char origin_map[MAX_ROW][MAX_COL],
               char start_state[MAX_ROW][MAX_COL],
               char action[100])
{
    char origin[MAX_ROW][MAX_COL] = {'\0'};
    char state[MAX_ROW][MAX_COL] = {'\0'};
    char undoStack[MAX_UNDO][MAX_ROW][MAX_COL] = {'\0'};
    char cmd;
    int undoTop = -1;
    int moves = start_moves;
    int i, j;
    int height;

    make_origin_map(origin, origin_map);
    copy_map(state, start_state);

    while (1)
    {
        printf("================\n");
        printf("%s in Level %d\n", name, level + 1);
        printf("================\n");

        print_map(state);
        printf("\n%s\n", action);
        printf("(Moves) %04d\n", moves);
        printf("(Command) ");
        scanf(" %c", &cmd);

        if (cmd == 'x' || cmd == 'X')
        {
            printf("Good bye\n");
            return;
        }
        else if (cmd == 's' || cmd == 'S')
        {
            save_game(name, level, moves, state);
            strcpy(action, "Saved");
        }
        else if (cmd == 't' || cmd == 'T')
        {
            rank();
        }
        else if (cmd == 'd' || cmd == 'D')
        {
            display_help();
        }
        else if (cmd == 'u' || cmd == 'U')
        {
            if (undoTop >= 0)
            {
                height = get_height(state);

                for (i = 0; i < height; i++)
                {
                    strcpy(state[i], undoStack[undoTop][i]);
                }

                undoTop--;
                moves++;
                strcpy(action, "Undid");
            }
        }
        else if (cmd == 'h' || cmd == 'H' ||
                 cmd == 'j' || cmd == 'J' ||
                 cmd == 'k' || cmd == 'K' ||
                 cmd == 'l' || cmd == 'L')
        {
            height = get_height(state);

            if (undoTop == MAX_UNDO - 1)
            {
                for (i = 0; i < MAX_UNDO - 1; i++)
                {
                    for (j = 0; j < height; j++)
                    {
                        strcpy(undoStack[i][j], undoStack[i + 1][j]);
                    }
                }
            }
            else
            {
                undoTop++;
            }

            for (i = 0; i < height; i++)
            {
                strcpy(undoStack[undoTop][i], state[i]);
            }

            if (move_player(state, origin, cmd))
            {
                if (cmd == 'h' || cmd == 'H') strcpy(action, "Left");
                else if (cmd == 'j' || cmd == 'J') strcpy(action, "Down");
                else if (cmd == 'k' || cmd == 'K') strcpy(action, "Up");
                else if (cmd == 'l' || cmd == 'L') strcpy(action, "Right");

                moves++;
            }
            else
            {
                undoTop--;
            }
        }
        else if(cmd == 'n' || cmd == 'N'){
            strcpy(action, "Replay from level 1");
            play_game(name, 1, 0, map, map[0], map[0], action);
        }

        else if (cmd == 'a' || cmd == 'A'){
            strcpy(action, "Again");
            play_game(name, level, 0, map, map[level], map[level], action);
        }

        
        if (is_cleared(state, origin))
        {
            update_ranking(name, level, moves);

            printf("================\n");
            printf("%s in Level %d\n", name, level + 1);
            printf("================\n");

            print_map(state);
            printf("\n%s\n", action);
            printf("(Moves) %04d\n", moves);

            break;
        }
    }

    printf("Good job! Continue (N/Y) ");
    scanf(" %c", &cmd);

    if (cmd == 'y' || cmd == 'Y')
    {
        if (level + 1 >= MAX_LEVEL || map[level + 1][0][0] == '\0')
        {
            printf("Congratulations!!\n");
            return;
        }

        strcpy(action, "");
        play_game(name, level + 1, 0, map, map[level + 1], map[level + 1], action);
        return;
    }
    else if (cmd == 'n' || cmd == 'N')
    {
        printf("Good bye!!\n");
        return;
    }
}

void file_load(char map[MAX_LEVEL][MAX_ROW][MAX_COL])
{
    FILE* fp = fopen("soko", "r");
    char name[4];
    int level;
    int moves;
    int height;
    char state[MAX_ROW][MAX_COL] = {'\0'};
    char action[100] = {'\0'};
    int i;
    int len;

    if (fp == NULL) //예외처리
    {
        printf("soko 파일이 없습니다.\n");
        return;
    }

    fscanf(fp, "%3s", name);
    fscanf(fp, "%d", &level);
    fscanf(fp, "%d", &moves);
    fscanf(fp, "%d", &height);

    fgetc(fp);

    if (level < 0 || level >= MAX_LEVEL)
    {
        printf("soko 파일의 레벨 정보가 잘못되었습니다.\n");
        fclose(fp);
        return;
    }

    if (height < 0 || height > MAX_ROW)
    {
        printf("soko 파일의 맵 높이가 잘못되었습니다.\n");
        fclose(fp);
        return;
    }

    for (i = 0; i < height; i++)
    {
        if (fgets(state[i], MAX_COL, fp) == NULL)
            break;

        len = strlen(state[i]);

        if (len > 0 && state[i][len - 1] == '\n')
            state[i][len - 1] = '\0';
    }

    fclose(fp);

    strcpy(action, "Loaded");
    play_game(name, level, moves, map, map[level], state, action);
}

void start_new_game(int level,
                    char name[4],
                    char map[MAX_LEVEL][MAX_ROW][MAX_COL])
{
    char action[100] = {'\0'};

    if (level == 0)
    {
        strcpy(action, "Welcome ");
        strcat(action, name);
    }

    play_game(name, level, 0, map, map[level], map[level], action);
}

void init(char map[MAX_LEVEL][MAX_ROW][MAX_COL])
{
    char option;
    char name[4];

    printf("===================================\n");
    printf("       S O K O B A N\n");
    printf("===================================\n\n");

    printf("n : New Game\n");
    printf("f : File Load\n");
    printf("1~5 : Level Number\n\n");

    while (1)
    {
        printf("Input option : ");
        scanf(" %c", &option);

        if (option == 'n' || option == 'N')
        {
            char temp[100];
            int valid;
            int i;

            while (1)
            {
                valid = 1;

                printf("Input your name : ");
                scanf("%99s", temp);

                if (strlen(temp) > 3)
                {
                    printf("이름은 최대 3글자까지 가능합니다.\n");
                    continue;
                }

                for (i = 0; temp[i] != '\0'; i++)
                {
                    if (!((temp[i] >= 'A' && temp[i] <= 'Z') ||
                          (temp[i] >= 'a' && temp[i] <= 'z')))
                    {
                        valid = 0;
                        break;
                    }
                }

                if (!valid)
                {
                    printf("영어 알파벳만 입력 가능합니다.\n");
                    continue;
                }

                strcpy(name, temp);
                start_new_game(0, name, map);
                return;
            }
        }
        else if (option == 'f' || option == 'F')
        {
            file_load(map);
            return;
        }
        else if (option >= '1' && option <= '5')
        {
            int level = option - '1';

            char temp[100];
            int valid;
            int i;

            while (1)
            {
                valid = 1;

                printf("Input your name : ");
                scanf("%99s", temp);

                if (strlen(temp) > 3)
                {
                    printf("이름은 최대 3글자까지 가능합니다.\n");
                    continue;
                }

                for (i = 0; temp[i] != '\0'; i++)
                {
                    if (!((temp[i] >= 'A' && temp[i] <= 'Z') ||
                          (temp[i] >= 'a' && temp[i] <= 'z')))
                    {
                        valid = 0;
                        break;
                    }
                }

                if (!valid)
                {
                    printf("영어 알파벳만 입력 가능합니다.\n");
                    continue;
                }

                strcpy(name, temp);
                start_new_game(level, name, map);
                return;
            }
        }
        else
        {
            printf("잘못된 입력입니다.\n");
        }
    }
}

int load_map(char map[MAX_LEVEL][MAX_ROW][MAX_COL])
{
    FILE* fp = fopen("map", "r");

    int level = -1;
    int row = 0;
    int ch;
    int col;
    int l;

    int box_count[MAX_LEVEL] = {0};
    int target_count[MAX_LEVEL] = {0};

    if (fp == NULL)
    {
        printf("map 파일을 열 수 없습니다.\n");
        return 0;
    }

    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch == 'e')
            break;

        if (ch == 's')
        {
            level++;

            if (level >= MAX_LEVEL)
            {
                printf("레벨 수가 최대 %d개를 초과했습니다.\n", MAX_LEVEL);
                fclose(fp);
                return 0;
            }

            row = 0;

            ch = fgetc(fp);
            if (ch != '\n' && ch != '\r' && ch != EOF)
            {
                printf("s 뒤에는 줄바꿈이 와야 합니다.\n");
                fclose(fp);
                return 0;
            }

            continue;
        }

        if (level < 0)
        {
            printf("map 파일은 s로 시작해야 합니다.\n");
            fclose(fp);
            return 0;
        }

        if (ch == '\n' || ch == '\r')
            continue;

        if (row >= MAX_ROW)
        {
            printf("Wrong level %d map\n", level + 1);
            fclose(fp);
            return 0;
        }

        col = 0;

        while (ch != '\n' && ch != '\r' && ch != EOF)
        {
            if (col >= MAX_COL - 1)
            {
                printf("Wrong level %d map\n", level + 1);
                fclose(fp);
                return 0;
            }

            if (ch != '@' && ch != '#' &&
                ch != '$' && ch != '.' &&
                ch != 'O')
            {
                printf("Wrong level %d map\n", level + 1);
                fclose(fp);
                return 0;
            }

            if (ch == '$')
                box_count[level]++;
            else if (ch == 'O')
                target_count[level]++;

            map[level][row][col] = (char)ch;
            col++;

            ch = fgetc(fp);
        }

        map[level][row][col] = '\0';
        row++;
    }

    fclose(fp);

    if (level < 0)
    {
        printf("읽어온 레벨이 없습니다.\n");
        return 0;
    }

    for (l = 0; l <= level; l++)
    {
        if (box_count[l] != target_count[l])
        {
            printf("Wrong level %d map\n", l + 1);
            return 0;
        }
    }

    return 1;
}

int main(void)
{
    char map[MAX_LEVEL][MAX_ROW][MAX_COL] = {'\0'};

    if (!load_map(map))
    {
        return 0;
    }

    init(map);

    printf("Thank you for playing!!\n");

    return 0;
}