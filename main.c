#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#define MAX_LEVEL 5
#define MAX_ROW 30
#define MAX_COL 31   /* 가로 최대 30 + 문자열 끝 '\0' */

/*

    아직 미구현
    1. 레벨별 박스 / 보관장소 수 다를 때 오류
    2. 파일 불러오기
    3. 이동 기능
    4. undo 기능

*/

void new_game(int level, char map[MAX_ROW][MAX_COL])
{
    printf("새 게임 시작!\n");
    printf("현재 레벨은 %d입니다.\n", level + 1);

    for (int i = 0; i < MAX_ROW; i++)
    {
        if (map[i][0] == '\0')
        {
            break;
        }

        for (int j = 0; j < MAX_COL; j++)
        {
            if (map[i][j] == '\0')
            {
                break;
            }

            printf("%c", map[i][j]);
        }

        printf("\n");
    }
}

int file_load(void)
{
    printf("\n파일 불러오기!\n");
    return 0;
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
            while (1)
            {
                char temp[100];
                int valid = 1;

                printf("Input your name (max 3 letters): ");
                scanf("%99s", temp);

                if (strlen(temp) > 3)
                {
                    printf("이름은 최대 3글자까지 가능합니다.\n");
                    continue;
                }

                for (int i = 0; temp[i] != '\0'; i++)
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

                printf("Player Name : %s\n", name);

                new_game(0, map[0]);
                return;
            }
        }
        else if (option == 'f' || option == 'F')
        {
            file_load();
            return;
        }
        else if (option >= '1' && option <= '5')
        {
            int level = option - '1';

            printf("Level %d 선택!\n", level + 1);
            new_game(level, map[level]);
            return;
        }
        else
        {
            printf("잘못된 입력입니다.\n");
        }
    }
}

void load_map(char map[MAX_LEVEL][MAX_ROW][MAX_COL])
{
    FILE* fp = fopen("map", "r");

    if (fp == NULL)
    {
        printf("map 파일을 열 수 없습니다.\n");
        return;
    }

    int level = -1;
    int row = 0;
    int ch;

    int box_count[MAX_LEVEL] = {0};
    int target_count[MAX_LEVEL] = {0};

    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch == 'e')
        {
            break;
        }

        if (ch == 's')
        {
            level++;

            if (level >= MAX_LEVEL)
            {
                printf("레벨 수가 최대 %d개를 초과했습니다.\n", MAX_LEVEL);
                fclose(fp);
                return;
            }

            row = 0;

            ch = fgetc(fp);
            if (ch != '\n' && ch != '\r' && ch != EOF)
            {
                /* s 뒤에 이상한 문자가 있으면 무시하지 않고 오류 처리 */
                printf("s 뒤에는 줄바꿈이 와야 합니다.\n");
                fclose(fp);
                return;
            }

            continue;
        }

        if (level < 0)
        {
            printf("map 파일은 s로 시작해야 합니다.\n");
            fclose(fp);
            return;
        }

        if (ch == '\n' || ch == '\r')
        {
            continue;
        }

        if (row >= MAX_ROW)
        {
            printf("레벨 %d의 세로 길이가 최대 %d를 초과했습니다.\n", level + 1, MAX_ROW);
            fclose(fp);
            return;
        }

        int col = 0;

        while (ch != '\n' && ch != '\r' && ch != EOF)
        {
            if (col >= MAX_COL - 1)
            {
                printf("레벨 %d의 가로 길이가 최대 30을 초과했습니다.\n", level + 1);
                fclose(fp);
                return;
            }

            if (ch != '@' && ch != '#' &&
                ch != '$' && ch != '.' &&
                ch != 'O')
            {
                printf("잘못된 문자 발견: %c\n", ch);
                fclose(fp);
                return;
            }

            if (ch == '$')
            {
                box_count[level]++;
            }
            else if (ch == 'O')
            {
                target_count[level]++;
            }

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
        return;
    }

    for (int l = 0; l <= level; l++)
    {
        if (box_count[l] != target_count[l])
        {
            printf("Wrong level %d map\n", l + 1);
            printf("박스: %d, 보관장소: %d\n", box_count[l], target_count[l]);
            return;
        }
    }

    for (int l = 0; l <= level; l++)
    {
        printf("\n===== LEVEL %d =====\n", l + 1);

        for (int r = 0; r < MAX_ROW; r++)
        {
            if (map[l][r][0] == '\0')
            {
                break;
            }

            printf("%s\n", map[l][r]);
        }
    }

    init(map);
}

int main(void)
{
    char map[MAX_LEVEL][MAX_ROW][MAX_COL] = {'\0'};

    load_map(map);

    return 0;
}