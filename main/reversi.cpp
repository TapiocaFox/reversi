/*
all map format (Y,X) function format (x,y)
copyright(c) 2017 MAGNET inc.
YOU O:2   COM @:2
   X 0 1 2 3 4 5 6 7
 Y ------------------
 0|                  |
 1|                  |
 2|                  |
 3|        O @       |
 4|        @ O       |
 5|                  |
 6|                  |
 7|                  |
   ------------------
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;
char version [128] = "beta 1.2.0";
int com();
class reversi {
  private:
    int map[8][8];
    int benefit_map[8][8];
    int benefit(int, int, int, int, int);
    bool check(int, int, int, int, int, int); //recursion
  public:
    reversi();
    int clr(); int print();
    int value(int, int);
    int set(int, int, int);
    int mypoint()    ; int compoint();
    int getbenefit(int, int, int);
    int generatekey(char *&key);
    int setbykey(char *key);
    int printkey();
    int can_set(int);
};
reversi::reversi() {}
int reversi::printkey () {
  char *A=new char[65];
  (*this).generatekey(A);
  cout<<"0123456701234567012345670123456701234567012345670123456701234567"<<endl;
  cout<<A<<endl;
}
int reversi::can_set (int sti) {
  int i=0, j=0;
  for (i=0; i<8; i++) {
    for(j=0; j<8; j++) {
      if((*this).getbenefit(i, j, sti) != -1 ) {
        return 1;
      }
    }
  }
  return 0;
}
int reversi::generatekey (char *&key) {
  int i,j;
  for (i=0; i<8; i++) {
    for (j=0; j<8; j++) {
      switch ((*this).map[j][i]) {
        case 1:
          key[i*8+j]='A';
        break;
        case 0:
          key[i*8+j]='N';
        break;
        case -1:
          key[i*8+j]='B';
        break;
      }
    }
  }
  key[64]='\0';
}
int reversi::setbykey (char *key) {
  int i,j;
  for (i=0; i<8; i++) {
    for (j=0; j<8; j++) {
      switch (key[i*8+j]) {
        case 'A':
          (*this).map[j][i]=1;
        break;
        case 'N':
          (*this).map[j][i]=0;
        break;
        case 'B':
          (*this).map[j][i]=-1;
        break;
      }
    }
  }
}
int reversi::mypoint() {
    int temp=0, i, j;
    for (i=0; i<8; i++) {for (j=0; j<8; j++) {if(map[i][j] == 1)temp++; }}
    return temp;
}
int reversi::compoint() {
    int temp=0, i, j;
    for (i=0; i<8; i++) {for (j=0; j<8; j++) {if(map[i][j] == -1)temp++; }}
    return temp;
}
int reversi::clr() {
    int i, j;
    for (i=0; i<8; i++) { for (j=0; j<8; j++)  { map[i][j]=0; } }
    map[3][3]=1 ;  map[3][4]=-1 ;  map[4][3]=-1 ;  map[4][4]=1;//default
    return 0;
}
int reversi::value(int x, int y) {return map[y][x]; }
int reversi::print() {
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
int reversi::set(int X,  int Y,  int sti) {
    bool temp=0;
    if (map[Y][X] == 0) {
        if (map[Y-1][X-1] == sti*-1 && (*this).check(X-1,  Y-1, -1, -1, sti, 1)) {temp=1; }
        if (map[Y-1][X] == sti*-1 && (*this).check(X, Y-1, 0, -1, sti, 1)) {temp=1; }
        if (map[Y-1][X+1] == sti*-1 && (*this).check(X+1, Y-1, 1, -1, sti, 1)) {temp=1; }
        if (map[Y][X-1] == sti*-1 && (*this).check(X-1, Y, -1, 0, sti, 1)) {temp=1; }
        if (map[Y][X+1] == sti*-1 && (*this).check(X+1, Y, 1, 0, sti, 1)) {temp=1; }
        if (map[Y+1][X-1] == sti*-1 && (*this).check(X-1, Y+1, -1, 1, sti, 1)) {temp=1; }
        if (map[Y+1][X] == sti*-1 && (*this).check(X, Y+1, 0, 1, sti, 1)) {temp=1; }
        if (map[Y+1][X+1] == sti*-1 && (*this).check(X+1, Y+1, 1, 1, sti, 1)) {temp=1; }
    }
    if(temp == 1) {map[Y][X]=sti; }else {return -1;}
    if(temp == 1)return 0;
    return 1;
}
bool reversi::check(int x, int y, int nx, int ny, int sti, int set) {//(start X, Y) (Next X, Y) (mychess) (set or not)
    if(x<0||x>7||y<0||y>7) {return 0; }
    if((*this).value(x, y) == 0)  {return 0; }
    if((*this).value(x, y) == sti)  {return 1; }
    if((*this).value(x, y) == -sti) {
        bool a=(*this).check(x+nx, y+ny, nx, ny, sti, set);
        if(set && a) {map[y][x]=sti; }
        return a;
    }
    return 0;
}
int reversi::getbenefit (int x, int y, int sti) {
  int i, j;
  for (i=0; i<8; i++) {for(j=0; j<8; j++) {(*this).benefit_map[i][j]=-1; }}
  for (i=0; i<8; i++) {
      for (j=0; j<8; j++) {
          if((*this).value(j, i) == sti) {
              (*this).benefit(j, i, -1, -1, sti); (*this).benefit(j, i,  0, -1, sti);  (*this).benefit(j, i,  1, -1, sti);
              (*this).benefit(j, i, -1,  0, sti);                              (*this).benefit(j, i,  1,  0, sti);
              (*this).benefit(j, i, -1,  1, sti); (*this).benefit(j, i,  0,  1, sti);  (*this).benefit(j, i,  1,  1, sti);
          }
      }
  }
  return (*this).benefit_map[y][x];
}
int reversi::benefit(int x, int y, int nx, int ny,int sti) {
    int value=0;
    bool temp=1;
    x+=nx; y+=ny;
    while(temp) {
        if(x<8 && x>=0 && y<8 && y>=0) {
            if((*this).value(x, y) == sti) {(*this).benefit_map[y][x]=-1; temp=0; }
            if((*this).value(x, y) == 0) {
              if((*this).benefit_map[y][x]==-1 && value > 0) {
                (*this).benefit_map[y][x]= 0;
              }
              (*this).benefit_map[y][x]+=value; temp=0;
            }
            if((*this).value(x, y) == -sti) {(*this).benefit_map[y][x]=-1; value++; x+=nx; y+=ny; }
        }else {temp=0; }
    }
    return 0;
}
reversi ctrl;
//-------------------------------com----------------------------------
int com() {
  int i=0, j=0, temp[3]= {-1, 0, 0};
  double temp_value=-1;
  for (i=0; i<8; i++) {
    for(j=0; j<8; j++) {
      if(ctrl.getbenefit(i, j, -1) != -1 ) {
        double value=0;
        value=double(ctrl.getbenefit(i, j, -1));
        //corner
        if ((i==0 && j==0)||(i==7 && j==0)||(i==0 && j==7)||(i==7 && j==7)) {
          if (value<20) {
            value=25;
          }
        }
        //subcorder
        if (  (i==1 && j==0)||(i==0 && j==1)||(i==1 && j==1)) {
          if (ctrl.value(0, 0)!=-1) {
            value = 0;
          }
          else {
            value*=1.5;
          }
        }
        if ((i==6 && j==0)||(i==6 && j==1)||(i==7 && j==1)) {
          if (ctrl.value(7, 0)!=-1) {
            value = 0;
          }
          else {
            value*=1.5;
          }
        }
        if ((i==0 && j==6)||(i==1 && j==6)||(i==1 && j==7)) {
          if (ctrl.value(0, 7)!=-1) {
            value = 0;
          }
          else {
            value*=1.5;
          }
        }
        if ((i==7 && j==6)||(i==6 && j==6)||(i==6 && j==7)) {
          if (ctrl.value(7, 7)!=-1) {
            value = 0;
          }
          else {
            value*=1.5;
          }
        }
        //inner corner
        if ((i==2 && j==2)||(i==5 && j==2)||(i==2 && j==5)||(i==5 && j==5)) {
          value = value*2.5;
        }
        //inner border
        if (
          (i==2 && j==3)||(i==2 && j==4)||
          (i==3 && j==5)||(i==4 && j==5)||
          (i==5 && j==3)||(i==5 && j==4)||
          (i==3 && j==2)||(i==4 && j==2)
        ) {
          value = value + 2;
        }
        //border
        if (i==0) {
            if (ctrl.value(0, 0) == 1 || ctrl.value(0, 7) == 1) {
              value=0;
            }
        }
        if (i==7) {
            if (ctrl.value(7, 0) == 1 || ctrl.value(7, 7) == 1) {
              value=0;
            }
        }
        if (j==0) {
            if (ctrl.value(0, 0) == 1 || ctrl.value(7, 0) == 1) {
              value=0;
            }
        }
        if (j==7) {
            if (ctrl.value(0, 7) == 1 || ctrl.value(7, 7) == 1) {
              value=0;
            }
        }

        if (value >= temp_value) {
          temp_value = value; temp[1]=i; temp[2]=j;
        }
      }
    }
  }
  if(temp_value>=0) {
    int p1,p2;
    p1=ctrl.mypoint();
    p2=ctrl.compoint();
    char *A=new char[65];
    ctrl.generatekey(A);
    ctrl.set(temp[1], temp[2], -1);
    char *B=new char[65];
    ctrl.generatekey(B);
    cout
    <<"{\n"
    <<"    \"key1\": \""<<A<<"\",\n"
    <<"    \"key2\": \""<<B<<"\",\n"
    <<"    \"p1_1\": \""<<p1<<"\",\n"
    <<"    \"p2_1\": \""<<p2<<"\",\n"
    <<"    \"p1_2\": \""<<ctrl.mypoint()<<"\",\n"
    <<"    \"p2_2\": \""<<ctrl.compoint()<<"\",\n"
    <<"    \"x\": \""<<temp[1]<<"\",\n"
    <<"    \"y\": \""<<temp[2]<<"\",\n"
    <<"    \"version\": \""<<version<<"\",\n"
    <<"    \"message\": \""<<"OK"<<"\",\n"
    <<"    \"copyright\": \""<<"copyright(c) 2017 MAGNET inc."<<"\"\n"
    <<"}";
  }else {
    int p1,p2;
    p1=ctrl.mypoint();
    p2=ctrl.compoint();
    char *A=new char[65];
    ctrl.generatekey(A);
    char *B=new char[65];
    ctrl.generatekey(B);
    cout
    <<"{\n"
    <<"    \"key1\": \""<<A<<"\",\n"
    <<"    \"key2\": \""<<B<<"\",\n"
    <<"    \"p1_1\": \""<<p1<<"\",\n"
    <<"    \"p2_1\": \""<<p2<<"\",\n"
    <<"    \"p1_2\": \""<<ctrl.mypoint()<<"\",\n"
    <<"    \"p2_2\": \""<<ctrl.compoint()<<"\",\n"
    <<"    \"x\": \""<<-1<<"\",\n"
    <<"    \"y\": \""<<-1<<"\",\n"
    <<"    \"version\": \""<<version<<"\",\n"
    <<"    \"message\": \""<<"OK"<<"\",\n"
    <<"    \"copyright\": \""<<"copyright(c) 2017 MAGNET inc."<<"\"\n"
    <<"}";
  }
  return 0;
}
//--------------------------------------------------------------------
int main() {
    ctrl.clr();
    int X, Y, temp=1;
    char *A=new char[65];
    string B="NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN";
    cin>>B;
    cin>>X>>Y;
    strcpy(A,B.c_str());
    ctrl.setbykey(A);
    if (ctrl.can_set(1) == 0) {
      com();
      return 0;
    }
    if (ctrl.set(X, Y, 1) == -1) {
      cout
      <<"{\n"
      <<"    \"version\": \""<<version<<"\",\n"
      <<"    \"message\": \""<<"err"<<"\",\n"
      <<"    \"copyright\": \""<<"copyright(c) 2017 MAGNET inc."<<"\"\n"
      <<"}";
      return 0;
    }
    com();
}
