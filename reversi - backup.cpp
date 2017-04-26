#include<stdio.h>
bool check(int, int, int, int, int, int);
int benefit(int, int, int, int);
int com();
class control {
private:
    int map[8][8];
public:
    control();
    int clr(); int print();
    int value(int, int);
    int setpoint(int, int, int) ; int set(int, int, int);
    int mypoint()    ; int compoint();
};
control::control() {}
int control::mypoint() {
    int temp=0, i, j;
    for (i=1; i<8; i++) {for (j=1; j<8; j++) {if(map[i][j] == 1)temp++; }}
    return temp;
}
int control::compoint() {
    int temp=0, i, j;
    for (i=1; i<8; i++) {for (j=1; j<8; j++) {if(map[i][j] == -1)temp++; }}
    return temp;
}
int control::clr() {
    int i, j;
    for (i=1; i<8; i++) { for (j=1; j<8; j++)  { map[i][j]=0; } }
    /*set default*/map[3][3]=1 ;  map[3][4]=-1 ;  map[4][3]=-1 ;  map[4][4]=1;
    return 0;
}
int control::value(int x, int y) {return map[y][x]; }
int control::setpoint(int x, int y, int sti) {map[y][x]=sti; return 0; }
int control::print() {
    int i, j;
    printf("copyright(c) 2017 MAGNET inc.\nHelp: first input X , press \"Enter\",  and then input Y.\nYOU O:%d   COM @:%d \n", mypoint(), compoint());
    printf("\n");
    printf("   X 0 1 2 3 4 5 6 7 \n");
    printf(" Y ------------------ \n");
    for (i=0; i<8; i++) {
        printf("%2d| ", i);
        for (j=0; j<8; j++) {
            switch(map[i][j]) {
                case 1:
                    printf(" O");
                    break;
                case 0:
                    printf("  ");
                    break;
                case -1:
                    printf(" @");
                    break;
            }
        }
        printf(" |\n");
    }
    printf("   ------------------\n");
    return 0;
}
int control::set(int X,  int Y,  int sti) {
    bool temp=0;
    if (map[Y][X] == 0) {
        if (map[Y-1][X-1] == sti*-1 && check(X-1,  Y-1, -1, -1, sti, 1)) {temp=1; }
        if (map[Y-1][X] == sti*-1 && check(X, Y-1, 0, -1, sti, 1)) {temp=1; }
        if (map[Y-1][X+1] == sti*-1 && check(X+1, Y-1, 1, -1, sti, 1)) {temp=1; }
        if (map[Y][X-1] == sti*-1 && check(X-1, Y, -1, 0, sti, 1)) {temp=1; }
        if (map[Y][X+1] == sti*-1 && check(X+1, Y, 1, 0, sti, 1)) {temp=1; }
        if (map[Y+1][X-1] == sti*-1 && check(X-1, Y+1, -1, 1, sti, 1)) {temp=1; }
        if (map[Y+1][X] == sti*-1 && check(X, Y+1, 0, 1, sti, 1)) {temp=1; }
        if (map[Y+1][X+1] == sti*-1 && check(X+1, Y+1, 1, 1, sti, 1)) {temp=1; }
    }
    if(temp == 1) {map[Y][X]=sti; }else {printf("cannot set this block\n"); }
    if(temp == 1)return 0;
    return 1;
}
control ctrl;
//-------------------------------com----------------------------------
int commap[8][8];
int com() {
    int i=0, j=0, temp[3]= {-1, 0, 0};
    for (i=0; i<8; i++) {for(j=0; j<8; j++) {commap[i][j]=-1; }}
    for (i=0; i<8; i++) {
        for (j=0; j<8; j++) {
            if(ctrl.value(i, j) == -1) {
                benefit(i, j, -1, -1); benefit(i, j,  0, -1);  benefit(i, j,  1, -1);
                benefit(i, j, -1,  0);                     benefit(i, j,  1,  0);
                benefit(i, j, -1,  1); benefit(i, j,  0,  1);  benefit(i, j,  1,  1);
            }
        }
    }
    for (i=0; i<8; i++) {for(j=0; j<8; j++) {if(commap[i][j]!=-1 && commap[i][j]>temp[0]) {temp[0]=commap[i][j]; temp[1]=j; temp[2]=i; }}}
    if(temp[0]!=-1) {ctrl.set(temp[1], temp[2], -1); }
    return 0;
}
int benefit(int x, int y, int nx, int ny) {
    int value=0;
    bool temp=1;
    x+=nx; y+=ny;
    while(temp) {
        if(x<8 && x>=0 && y<8 && y>=0) {
            if(ctrl.value(x, y) == -1) {commap[y][x]=-1; temp=0; }
            if(ctrl.value(x, y) == 0) {commap[y][x]+=value; temp=0; }
            if(ctrl.value(x, y) == 1) {commap[y][x]=-1; value++; x+=nx; y+=ny; }
        }else {temp=0; }
    }
    return 0;
}
//--------------------------------------------------------------------
bool check(int x, int y, int nx, int ny, int sti, int set) {//(start X, Y) (Next X, Y) (mychess) (set or not)
    if(x<0||x>7||y<0||y>7) {return 0; }
    if(ctrl.value(x, y) == 0)  {return 0; }
    if(ctrl.value(x, y) == sti)  {return 1; }
    if(ctrl.value(x, y) == -sti) {
        bool a=check(x+nx, y+ny, nx, ny, sti, set);
        if(set && a) {ctrl.setpoint(x, y, sti); }
        return a;
    }
    return 0;
}
int main() {
    ctrl.clr();
    while(1) {
        int X, Y, temp=1;
        ctrl.print();
        while(temp) {
            scanf("%d%d", &Y, &X);
            temp=ctrl.set(Y, X, 1);
        }
        ctrl.print();
        com();
    }
}
