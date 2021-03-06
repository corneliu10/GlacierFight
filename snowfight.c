// Copyright 2019 <Copyright Owner>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define inFile "snowfight.in"
#define outFile "snowfight.out"
#define NMAX 500

typedef struct CelulaGhetar {
    int dmg, h;
    int idSpiridus; // -1 if empty
}CelulaGhetar;

typedef struct {
    int hp, stamina, dmg, eliminated;
    int id, x, y;
    char nume[50];
}Spiridus;

union {
    unsigned long storm;
    unsigned char bytes[4];
} Storm;

int R, P, EPx, EPy, players;
Spiridus sp[NMAX];
CelulaGhetar gt[NMAX][NMAX];

int comparator(const void *a, const void *b) {
    Spiridus *A = (Spiridus *)a;
    Spiridus *B = (Spiridus *)b;

    if (A->hp > 0 && B->hp <= 0)
        return -1;
    else if (A->hp <= 0 && B->hp > 0)
        return 1;
    else if (A->eliminated != B->eliminated)
        return B->eliminated - A->eliminated;
    else
        return strcmp(A->nume, B->nume);
}

int isInside(int circle_x, int circle_y, int rad, int x, int y) {
    if ((x - circle_x) * (x - circle_x) +
        (y - circle_y) * (y - circle_y) <= rad * rad)
        return 1;
    else
        return 0;
}

int fellOffGlacier(int spIndex) {
    sp[spIndex].hp = 0; players--;
    printf("%s fell off the glacier.\n", sp[spIndex].nume);
    return spIndex;
}

void printSpiridus(Spiridus sp) {
    printf("%s x:%d y:%d hp:%d stamina:%d dmg:%d\n",
            sp.nume, sp.x, sp.y, sp.hp, sp.stamina, sp.dmg);
}

void swapInt(int *a, int *b) {
    int c = *a;
    *a = *b;
    *b = c;
}

int fight(int spIndex1, int spIndex2) {
    int dmg1 = sp[spIndex1].dmg, dmg2 = sp[spIndex2].dmg;

    players--;
    if (sp[spIndex1].stamina < sp[spIndex2].stamina) {
        while (sp[spIndex1].hp > 0 && sp[spIndex2].hp > 0) {
            sp[spIndex1].hp -= dmg2;
            if (sp[spIndex1].hp > 0) sp[spIndex2].hp -= dmg1;
        }
    } else {
        while (sp[spIndex1].hp > 0 && sp[spIndex2].hp > 0) {
            sp[spIndex2].hp -= dmg1;
            if (sp[spIndex2].hp > 0) sp[spIndex1].hp -= dmg2;
        }
    }

    if (sp[spIndex1].hp <= 0) {
        printf("%s sent %s back home.\n", sp[spIndex2].nume, sp[spIndex1].nume);
        sp[spIndex2].stamina += sp[spIndex1].stamina;
        sp[spIndex2].eliminated++;

        return spIndex2;
    } else {
        printf("%s sent %s back home.\n", sp[spIndex1].nume, sp[spIndex2].nume);
        sp[spIndex1].stamina += sp[spIndex2].stamina;
        sp[spIndex1].eliminated++;

        return spIndex1;
    }
}

int moveSpiridus(int spIndex, char move) {
    int oldX = sp[spIndex].x, oldY = sp[spIndex].y;
    switch (move) {
        case 'U':
            if (oldX - 1 < 0) return fellOffGlacier(spIndex);

            if (abs(gt[oldX-1][oldY].h - gt[oldX][oldY].h)
                <= sp[spIndex].stamina) {
                gt[oldX][oldY].idSpiridus = -1; sp[spIndex].x--;
                sp[spIndex].stamina -= abs(gt[oldX-1][oldY].h
                                         - gt[oldX][oldY].h);

                if (!isInside(EPx, EPy, R, sp[spIndex].x, sp[spIndex].y)) {
                    return fellOffGlacier(spIndex);
                }
                if (gt[oldX-1][oldY].dmg > sp[spIndex].dmg)
                    swapInt(&sp[spIndex].dmg, &gt[oldX-1][oldY].dmg);
                if (gt[oldX-1][oldY].idSpiridus >= 0) {
                    int winner = fight(spIndex, gt[oldX-1][oldY].idSpiridus);
                    gt[oldX-1][oldY].idSpiridus = winner;
                    return winner;
                }
                gt[oldX-1][oldY].idSpiridus = spIndex;
            }
            break;
        case 'D':
            if (abs(gt[oldX+1][oldY].h - gt[oldX][oldY].h)
                    <= sp[spIndex].stamina) {
                sp[spIndex].x++; gt[oldX][oldY].idSpiridus = -1;
                sp[spIndex].stamina -= abs(gt[oldX+1][oldY].h
                                         - gt[oldX][oldY].h);

                if (!isInside(EPx, EPy, R, sp[spIndex].x, sp[spIndex].y)) {
                    return fellOffGlacier(spIndex);
                }
                if (gt[oldX+1][oldY].dmg > sp[spIndex].dmg)
                    swapInt(&sp[spIndex].dmg, &gt[oldX+1][oldY].dmg);
                if (gt[oldX+1][oldY].idSpiridus >= 0) {
                    int winner = fight(spIndex, gt[oldX+1][oldY].idSpiridus);
                    gt[oldX+1][oldY].idSpiridus = winner;
                    return winner;
                }

                gt[oldX+1][oldY].idSpiridus = spIndex;
            }
            break;
        case 'L':
            if (oldY - 1 < 0) return fellOffGlacier(spIndex);

            if (abs(gt[oldX][oldY-1].h - gt[oldX][oldY].h)
                    <= sp[spIndex].stamina) {
                sp[spIndex].y--; gt[oldX][oldY].idSpiridus = -1;
                sp[spIndex].stamina -= abs(gt[oldX][oldY-1].h
                                         - gt[oldX][oldY].h);

                if (!isInside(EPx, EPy, R, sp[spIndex].x, sp[spIndex].y))
                    return fellOffGlacier(spIndex);

                if (gt[oldX][oldY-1].dmg > sp[spIndex].dmg)
                    swapInt(&sp[spIndex].dmg, &gt[oldX][oldY-1].dmg);
                if (gt[oldX][oldY-1].idSpiridus >= 0) {
                    int winner = fight(spIndex, gt[oldX][oldY-1].idSpiridus);
                    gt[oldX][oldY-1].idSpiridus = winner;
                    return winner;
                }

                gt[oldX][oldY-1].idSpiridus = spIndex;
            }
            break;
        case 'R':
            if (abs(gt[oldX][oldY+1].h - gt[oldX][oldY].h)
                 <= sp[spIndex].stamina) {
                sp[spIndex].y++; gt[oldX][oldY].idSpiridus = -1;
                sp[spIndex].stamina -= abs(gt[oldX][oldY+1].h
                                         - gt[oldX][oldY].h);

                if (!isInside(EPx, EPy, R, sp[spIndex].x, sp[spIndex].y))
                    return fellOffGlacier(spIndex);

                if (gt[oldX][oldY+1].dmg > sp[spIndex].dmg)
                    swapInt(&sp[spIndex].dmg, &gt[oldX][oldY+1].dmg);
                if (gt[oldX][oldY+1].idSpiridus >= 0) {
                    int winner = fight(spIndex, gt[oldX][oldY+1].idSpiridus);
                    gt[oldX][oldY+1].idSpiridus = winner;
                    return winner;
                }

                gt[oldX][oldY+1].idSpiridus = spIndex;
            }
            break;
    }
    return -1;
}

void snowstorm(int K) {
    Storm.storm = K;

    int DMG = Storm.bytes[3];
    int R = Storm.bytes[2];
    int Y = Storm.bytes[1];
    int X = Storm.bytes[0];

    for (int i=0; i < P; ++i)
        if (sp[i].hp > 0) {
            if (R == 0) {
                if (sp[i].x == X && sp[i].y == Y)
                    sp[i].hp -= DMG;
            } else {
                if (isInside(X, Y, R, sp[i].x, sp[i].y))
                    sp[i].hp -= DMG;
            }

            if (sp[i].hp <= 0) {
                gt[sp[i].x][sp[i].y].idSpiridus = -1;
                printf("%s was hit by snowstorm.\n", sp[i].nume);
                players--;
            }
        }
}

void meltdown(int stamina) {
    R--;

    for (int i=0; i < P; ++i)
        if (sp[i].hp > 0) {
            sp[i].stamina += stamina;
            if (!isInside(EPx, EPy, R, sp[i].x, sp[i].y)) {
                sp[i].hp = 0;
                printf("%s got wet because of global warming.\n", sp[i].nume);
                players--;
            }
        }
}

void printScoreboard() {
    Spiridus cp[NMAX];
    for (int i=0; i < P; ++i)
        cp[i] = sp[i];

    qsort(cp, P, sizeof(cp[0]), comparator);

    printf("SCOREBOARD:\n");
    for (int i=0; i < P; ++i)
        if (cp[i].hp > 0)
            printf("%s\tDRY\t%d\n", cp[i].nume, cp[i].eliminated);
        else
            printf("%s\tWET\t%d\n", cp[i].nume, cp[i].eliminated);
}

int main() {
    char cmd[100];

    freopen(inFile, "r", stdin);
    freopen(outFile, "w", stdout);

    scanf("%d%d", &R, &P);
    EPx = EPy = R;
    players = P;
    for (int i=0; i < 2*R+1; ++i)
        for (int j=0; j < 2*R+1; ++j) {
            scanf("%d%d", &gt[i][j].h, &gt[i][j].dmg);
            gt[i][j].idSpiridus = -1;
        }

    for (int i=0; i < P; ++i) {
        scanf("%s", sp[i].nume);
        scanf("%d%d%d%d", &sp[i].x, &sp[i].y, &sp[i].hp, &sp[i].stamina);
        if (!isInside(EPx, EPy, R, sp[i].x, sp[i].y)) {
            printf("%s has missed the glacier.\n", sp[i].nume);
            sp[i].hp = 0;
            players--;
            continue;
        }

        sp[i].dmg = gt[sp[i].x][sp[i].y].dmg;
        gt[sp[i].x][sp[i].y].dmg = -1;
        gt[sp[i].x][sp[i].y].idSpiridus = i;
        sp[i].eliminated = 0;

        // printSpiridus(sp[i]);
    }


    while (scanf("%s", cmd) > 0 && players > 1) {
        if (!strcmp(cmd, "MOVE")) {
            int id;
            char moves[100];

            scanf("%d %s", &id, moves);
            // printSpiridus(sp[id]);

            for (int i=0; moves[i] && sp[id].hp > 0 && players > 1; ++i) {
                int winner = moveSpiridus(id, moves[i]);
                if (winner == -1) {
                    // printSpiridus(sp[id]);
                } else if (winner != id) {
                    break;
                }
            }
        } else if (!strcmp(cmd, "MELTDOWN")) {
            int stamina;
            scanf("%d", &stamina);

            meltdown(stamina);
        } else if (!strcmp(cmd, "SNOWSTORM")) {
            int storm;
            scanf("%d", &storm);

            snowstorm(storm);
        } else {
            printScoreboard();
        }

        // printf("%s\n", cmd);
    }

    for (int i=0; players == 1 && i < P; ++i)
        if (sp[i].hp > 0) {
            printf("%s has won.\n", sp[i].nume);
            break;
        }

    return 0;
}
