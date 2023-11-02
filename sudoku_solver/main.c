#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int mappa[10];
} QUADRANTE;

typedef struct {
    int row[10];
} RIGA;

typedef struct {
    int col[10];
} COLONNA;

int flag=0;

int** get_input_matrix(char*);
void sudoku_solver(int**);
QUADRANTE** load_quadrante(int**);
void load_righe_colonne(RIGA*,COLONNA*,int**);
void load_colonne(COLONNA*,int**);
void solver(QUADRANTE**,RIGA*,COLONNA*,int**,int,int);
int check_value(QUADRANTE**,RIGA*,COLONNA*,int,int,int);
void unset_value(QUADRANTE**,RIGA*,COLONNA*,int,int,int);

int main()
{
    int** mat = get_input_matrix("sudoku_diabolico_2.txt");
    sudoku_solver(mat);
    printf("\nFINITO!!!\n");
    return 0;
}

int** get_input_matrix(char* filename){
    FILE *fp;
    int** mat;

    if((fp=fopen(filename,"r"))==NULL){
        fprintf(stdout,"Error occurred");
        exit(1);
    }

    mat=(int**)malloc(9*sizeof(int*));
    if(mat == NULL){
        printf("Memory not allocated.\n");
        exit(1);
    }
    for(int i=0;i<9;i++){
        mat[i]=(int*)malloc(9*sizeof(int));
        if(mat[i]==NULL){
            printf("Memory not allocated.\n");
            exit(1);
        }
    }

    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            fscanf(fp,"%d",&mat[i][j]);
        }
    }

    return mat;
}

void sudoku_solver(int** mat){
    QUADRANTE** quad;
    RIGA riga[9];
    COLONNA colonna[9];

    load_righe_colonne(riga,colonna,mat);
    quad=load_quadrante(mat);
    solver(quad,riga,colonna,mat,0,0);
}

QUADRANTE** load_quadrante(int** mat){
    int count_i=0;
    int count_j=0;
    int i,j,k,t,index;
    QUADRANTE** quad;

    quad=(QUADRANTE**)malloc(3*sizeof(QUADRANTE*));
    for(i=0;i<3;i++){
        quad[i]=(QUADRANTE*)malloc(3*sizeof(QUADRANTE));
    }

    for( i=0; i<3; i++){
        count_j=0;
        for( j=0; j<3; j++){
            (quad)[i][j].mappa[0]=-1;
            for( index=1;index<10;index++){
                (quad)[i][j].mappa[index]=0;
            }
            for( k=count_i;k<count_i+3;k++){
                for( t=count_j;t<count_j+3;t++){
                    if(mat[k][t]!=0){
                        (quad)[i][j].mappa[mat[k][t]]=1;
                    }
                }
            }
            count_j+=3;
        }
        count_i+=3;
    }
    return quad;
}

void load_righe_colonne(RIGA* riga,COLONNA* colonna,int** mat){
    for(int i=0; i<9; i++){
        riga[i].row[0]=-1;
        colonna[i].col[0]=-1;
        for(int k=1;k<10;k++){
            riga[i].row[k]=0;
            colonna[i].col[k]=0;
        }
        for(int j=0; j<9; j++){
            if(mat[i][j]!=0) riga[i].row[mat[i][j]]=1;
            if(mat[j][i]!=0) colonna[i].col[mat[j][i]]=1;
        }
    }
}

void load_colonne(COLONNA* colonna,int** mat){
    for(int i=0; i<9; i++){
        colonna[i].col[0]=-1;
        for(int k=1;k<10;k++){
            colonna[i].col[k]=0;
        }
        for(int j=0; j<9; j++){
            if(mat[j][i]==0) continue;
            colonna[i].col[mat[j][i]]=1;
        }
    }
}

void solver(QUADRANTE** quad,RIGA* riga,COLONNA* colonna,int** mat,int i,int j){
    int value;
    if(i==9){
        //End of algorithm
        //Print result
        for(int i=0;i<9;i++){
            for(int j=0;j<9;j++){
                fprintf(stdout,"%d ",mat[i][j]);
            }
            fprintf(stdout,"\n");
        }
        flag=1;
        return;
    }
    if(mat[i][j]==0){
        //Prova a mettere un valore
        for(value=1;value<10 && !flag;value++){
            if(check_value(quad,riga,colonna,value,i,j)){
                mat[i][j]=value;
                if(j>=8){
                    solver(quad,riga,colonna,mat,i+1,0);
                    unset_value(quad,riga,colonna,value,i,j);
                    mat[i][j]=0;
                }else{
                    solver(quad,riga,colonna,mat,i,j+1);
                    unset_value(quad,riga,colonna,value,i,j);
                    mat[i][j]=0;
                }
            }
        }
        if(value==10) return;
    }else{
        if(j>=8){
            solver(quad,riga,colonna,mat,i+1,0);
        }else{
            solver(quad,riga,colonna,mat,i,j+1);
        }
    }
}

int check_value(QUADRANTE** quad,RIGA* riga,COLONNA* colonna,int value,int i,int j){
    int quad_i,quad_j;

    if(riga[i].row[value]==1) return 0;
    if(colonna[j].col[value]==1) return 0;

    if(i>=0 && i<=2){
        quad_i=0;
        if(j>=0 && j<=2){
            quad_j=0;
        }else if(j>=3 && j<=5){
            quad_j=1;
        }else if(j>=6 && j<=8){
            quad_j=2;
        }
    }else if(i>=3 && i<=5){
        quad_i=1;
        if(j>=0 && j<=2){
            quad_j=0;
        }else if(j>=3 && j<=5){
            quad_j=1;
        }else if(j>=6 && j<=8){
            quad_j=2;
        }
    }else if(i>=6 && i<=8){
        quad_i=2;
        if(j>=0 && j<=2){
            quad_j=0;
        }else if(j>=3 && j<=5){
            quad_j=1;
        }else if(j>=6 && j<=8){
            quad_j=2;
        }
    }
    if(quad[quad_i][quad_j].mappa[value]==1) return 0;

    riga[i].row[value]=1;
    colonna[j].col[value]=1;
    quad[quad_i][quad_j].mappa[value]=1;

    return 1;
}

void unset_value(QUADRANTE** quad,RIGA* riga,COLONNA* colonna,int value,int i,int j){
    int quad_i,quad_j;
    riga[i].row[value]=0;
    colonna[j].col[value]=0;

    if(i>=0 && i<=2){
        quad_i=0;
        if(j>=0 && j<=2){
            quad_j=0;
        }else if(j>=3 && j<=5){
            quad_j=1;
        }else if(j>=6 && j<=8){
            quad_j=2;
        }
    }else if(i>=3 && i<=5){
        quad_i=1;
        if(j>=0 && j<=2){
            quad_j=0;
        }else if(j>=3 && j<=5){
            quad_j=1;
        }else if(j>=6 && j<=8){
            quad_j=2;
        }
    }else if(i>=6 && i<=8){
        quad_i=2;
        if(j>=0 && j<=2){
            quad_j=0;
        }else if(j>=3 && j<=5){
            quad_j=1;
        }else if(j>=6 && j<=8){
            quad_j=2;
        }
    }
    quad[quad_i][quad_j].mappa[value]=0;
}
