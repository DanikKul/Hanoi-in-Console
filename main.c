#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define LENGTH 200
#define LLD long long unsigned

struct tower_disc{
    char* val;
    int num;
};

LLD power(LLD int number, int p){
    LLD int tmp = number;
    if (p == 0){
        return 1;
    }
    for (int i = 0; i < p - 1; ++i) {
        number *= tmp;
    }
    return number;
}

LLD int current = 0;
void classicRecursive(int nDisks, char fromTower, char toTower, char midTower, struct tower_disc* t1,
        struct tower_disc* t2, struct tower_disc* t3, int global, int clear, LLD int count, int delay){
    current++;
    if (nDisks == 0){
        return;
    }
    classicRecursive(nDisks - 1, fromTower, midTower, toTower, t1, t2, t3, global, clear, count, delay);
    char* tmp = (char*) malloc(100 * sizeof(char));
    int ind1 = 0, ind2 = 0, ind3 = 0, bf;
    char *del = (char*) malloc(200 * sizeof (char));
    for (int i = 0; i < 2 * 3 * global + 4 * 2; ++i) {
        strcat(del, "-");
    }
    printf("%s\n", del);
    char *empty = (char*) malloc(200 * sizeof (char));
    for (int i = 0; i < global; ++i) {
        strcat(empty, " ");
    }
    if (fromTower == 'a'){
        while (ind1 < global && t1[ind1].num == 0) ind1++;
        strcpy(tmp, t1[ind1].val);
        bf = t1[ind1].num;
        t1[ind1].num = 0;
        strcpy(t1[ind1].val, "");
        for (int j = 0; j < global; ++j) {
            strcat(t1[ind1].val, "  ");
        }
    } else if (fromTower == 'b'){
        while (ind2 < global && t2[ind2].num == 0) ind2++;
        strcpy(tmp, t2[ind2].val);
        bf = t2[ind2].num;
        t2[ind2].num = 0;
        strcpy(t2[ind2].val, "");
        for (int j = 0; j < global; ++j) {
            strcat(t2[ind2].val, "  ");
        }
    } else {
        while (ind3 < global && t3[ind3].num == 0) ind3++;
        strcpy(tmp, t3[ind3].val);
        bf = t3[ind3].num;
        t3[ind3].num = 0;
        strcpy(t3[ind3].val, "");
        for (int j = 0; j < global; ++j) {
            strcat(t3[ind3].val, "  ");
        }
    }
    ind1 = 0, ind2 = 0, ind3 = 0;
    if (toTower == 'a'){
        while (ind1 < global && t1[ind1].num == 0) ind1++;
        ind1--;
        t1[ind1].val = (char*) malloc(200 * sizeof (char));
        strcpy(t1[ind1].val, tmp);
        t1[ind1].num = bf;
    } else if (toTower == 'b'){
        while (ind2 < global && t2[ind2].num == 0) ind2++;
        ind2--;
        t2[ind2].val = (char*) malloc(200 * sizeof (char));
        strcpy(t2[ind2].val, tmp);
        t2[ind2].num = bf;
    } else {
        while (ind3 < global && t3[ind3].num == 0) ind3++;
        ind3--;
        t3[ind3].val = (char*) malloc(200 * sizeof (char));
        strcpy(t3[ind3].val, tmp);
        t3[ind3].num = bf;
    }
    if (clear == 1) {
        LLD int percentage = (LLD int)((double)((current / 2) * 1.0 / count) * 100);
        printf("loading: %llu%c \n", percentage, '%');
        usleep(1000 * delay);
        system("clear");
    }
    for (int i = 0; i < global; ++i) {
        printf("%s\t%s\t%s\n", t1[i].val, t2[i].val, t3[i].val);
    }
    classicRecursive(nDisks - 1, midTower, toTower, fromTower, t1, t2, t3, global, clear, count, delay);
}

LLD int countMinimalSteps(int nDisks, int towers){
    LLD int **pascalTriangle = (LLD int**) malloc(LENGTH * sizeof(LLD int*));
    if (!pascalTriangle){
        printf("Ошибка: не выделена память\n");
        return 0;
    }
    for (LLD int i = 0; i < LENGTH; ++i) {
        pascalTriangle[i] = (LLD int*) malloc(LENGTH * sizeof(LLD int));
        if (!pascalTriangle[i]){
            printf("Ошибка: не выделена память\n");
            return 0;
        }
    }
    for (LLD int i = 0; i < LENGTH; ++i) {
        pascalTriangle[0][i] = 1;
        pascalTriangle[i][0] = 1;
    }
    for (LLD int i = 1; i < LENGTH; ++i) {
        for (LLD int j = 1; j < LENGTH; ++j) {
            pascalTriangle[i][j] = pascalTriangle[i - 1][j] + pascalTriangle[i][j - 1];
        }
    }
    int ind1 = towers - 2, ind2 = 0;
    while (nDisks > pascalTriangle[ind1][ind2 + 1]) ind2++;
    LLD int diff = nDisks - pascalTriangle[ind1][ind2];
    LLD int min = 0;
    ind1 = towers - 3;
    for (int i = 0; i <= ind2; ++i) {
        min += power(2, i) * pascalTriangle[ind1][i];
    }
    min += power(2, ind2 + 1) * diff;
    return min;
}

int main() {
    char *userAnswer = (char*) malloc(100 * sizeof(char));
    if (!userAnswer){
        printf("Ошибка: не выделена память\n");
        return 1;
    }
    while (1){
        printf("'classic' - Рекурсивное решение с 3 башнями и n дисками\n"
               "'minimal' - Нахождение минимального кол-ва ходов для n башен и k дисков\n"
               "'exit' - Завершение программы\n");
        fflush(stdin);
        fgets(userAnswer, 100, stdin);
        userAnswer[strlen(userAnswer) - 1] = '\0';
        if (strcmp(userAnswer, "exit") == 0){
            free(userAnswer);
            return 0;
        }
        else if (strcmp(userAnswer, "classic") == 0){
            int nDisks = 0;
            printf("Введите число дисков\n");
            while (nDisks <= 0){
                while (!scanf("%d", &nDisks)){
                    fflush(stdin);
                    printf("Ошибка: введено не число!\n");
                }
                if (nDisks <= 0){
                    printf("Число дисков не может быть меньше 1!\n");
                }
            }
            struct tower_disc *t1 = (struct tower_disc*) malloc(100 * sizeof(struct tower_disc));
            struct tower_disc *t2 = (struct tower_disc*) malloc(100 * sizeof(struct tower_disc));
            struct tower_disc *t3 = (struct tower_disc*) malloc(100 * sizeof(struct tower_disc));
            for (int i = 0; i < nDisks; ++i) {
                t1[i].val = (char*) malloc (200 * sizeof(char));
                t2[i].val = (char*) malloc (200 * sizeof(char));
                t3[i].val = (char*) malloc (200 * sizeof(char));
                for (int j = 0; j < nDisks; ++j) {
                    if (nDisks - 1 - i <= j) {
                        strcat(t1[i].val, "--");
                    } else {
                        strcat(t1[i].val, " ");
                    }
                }
                int temp = strlen(t1[i].val);
                while (temp < nDisks * 2){
                    strcat(t1[i].val, " ");
                    temp++;
                }
                for (int j = 0; j < nDisks; ++j) {
                    strcat(t2[i].val, "  ");
                    strcat(t3[i].val, "  ");
                }
                t1[i].num = i + 1;
                t2[i].num = 0;
                t3[i].num = 0;
            }
            printf("Если хотите пошаговый вывод, а не анимацию введите 'steps'\n");
            char* check = (char*) malloc (100 * sizeof (char));
            fflush(stdin);
            fgets(check, 100, stdin);
            check[strlen(check) - 1] = '\0';
            printf("Введите задержку между ходами (ms)\n");
            int delay = -1;
            fflush(stdin);
            while (delay <= -1){
                while (!scanf("%d", &delay)){
                    fflush(stdin);
                    printf("Введено неверное значение\n");
                }
            }
            system("clear");
            for (int i = 0; i < nDisks; ++i) {
                printf("%s\t%s\t%s\n", t1[i].val, t2[i].val,t3[i].val);
            }
            int global = nDisks;
            int ch = 1;
            if (strcmp(check, "steps") == 0){
                ch = 0;
            }
            current = 0;
            classicRecursive(nDisks, 'a', 'c', 'b', t1, t2, t3, global, ch, countMinimalSteps(nDisks, 3), delay);
            char *del = (char*) malloc(200 * sizeof (char));
            for (int i = 0; i < 2 * 3 * global + 4 * 2; ++i) {
                strcat(del, "-");
            }
            printf("%s\n", del);
            usleep(1000);
            printf("Количество ходов: %lld\n", countMinimalSteps(nDisks, 3));
        }
        else if (strcmp(userAnswer, "minimal") == 0){
            int nDisks = 0, towers = 0;
            printf("Введите число башен\n");
            while (towers <= 0){
                while (!scanf("%d", &towers)){
                    fflush(stdin);
                    printf("Ошибка: введено не число!\n");
                }
            }
            printf("Введите число дисков\n");
            while (nDisks <= 0){
                while (!scanf("%d", &nDisks)){
                    fflush(stdin);
                    printf("Ошибка: введено не число!\n");
                }
            }
            printf("Минимальное количество ходов для %d башен и %d дисков: %llu\n",
                   towers, nDisks, countMinimalSteps(nDisks, towers));
        }
        else {
            printf("Что-то пошло не так... Попробуйте еще раз!\n");
        }
    }
}