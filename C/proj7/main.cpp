//
//  main.cpp
//  proj7
//
//  Created by Eric Lin on 3/25/17.
//  Copyright © 2017 XingSong Lin. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;

class image{
public:
    int numRows, numCols, minVal, maxVal;
    int** mirrorFramedAry;
    
    image(int r, int c, int min, int max){
        numRows = r;
        numCols = c;
        minVal = min;
        maxVal = max;
        mirrorFramedAry = new int*[numRows+2];
        for(int i=0; i<numRows+2; i++){
            mirrorFramedAry[i]= new int[numCols+2];
        }
        for(int i=0; i<numRows+2; i++){
            for(int j=0; j<numCols+2; j++){
                mirrorFramedAry[i][j] =0;
            }
        }
    }
    
    void mirrorFramed(int **array, int numRows, int numCols){
        for(int i=0; i<numRows+2; i++){
            array[i][0]=array[i][1];
            array[i][numCols+1] = array[i][numCols];
        }
        for(int j=0; j<numCols+2; j++){
            array[0][j] = array[1][j];
            array[numRows+1][j] = array[numRows][j];
        }
    }
    
    void loadImage(ifstream &input){
        int data;
        for(int i=0; i<numRows; i++){
            for(int j=0; j< numCols;j++){
                input>>data;
                mirrorFramedAry[i+1][j+1] = data;
            }
        }
    }
    
    string prettyPrint(int** array){
        string line="";
        for(int i=1; i<numRows+1; i++){
            for(int j=1; j<numCols+1; j++){
                if(array[i][j] > 0){
                    line = line+to_string(array[i][j]) +" ";
                }
                else{
                    line =line+"  ";
                }
            }
            line= line + "\n";
        }
        return line;
    }
};

class edge{
public:
    int** maskVertical;
    int** maskHorizonta;
    int** maskRightDiag;
    int** maskLeftDiag;
    int** sobelVertical;
    int** sobelHorizontal;
    int** sobelLeftDiag;
    int** sobelRightDiag;
    int** GradiantEdge;
    int neightborAry[3][3];
    
    edge(image img){
        maskVertical = new int*[3];
        maskHorizonta = new int*[3];
        maskLeftDiag = new int*[3];
        maskRightDiag = new int*[3];
        
        for(int i=0; i<3; i++){
            maskVertical[i] = new int[3];
            maskHorizonta[i] = new int[3];
            maskLeftDiag[i] = new int[3];
            maskRightDiag[i] = new int[3];
        }

        //maskVertical[3][3] hard code
        maskVertical[0][0] = 1;
        maskVertical[0][1] = 0;
        maskVertical[0][2] = -1;
        maskVertical[1][0] = 2;
        maskVertical[1][1] = 0;
        maskVertical[1][2] = -2;
        maskVertical[2][0] = 1;
        maskVertical[2][1] = 0;
        maskVertical[2][2] = -1;
        
        //maskHorizonta[3][3]
        maskHorizonta[0][0] = 1;
        maskHorizonta[0][1] = 2;
        maskHorizonta[0][2] = 1;
        maskHorizonta[1][0] = 0;
        maskHorizonta[1][1] = 0;
        maskHorizonta[1][2] = 0;
        maskHorizonta[2][0] = -1;
        maskHorizonta[2][1] = -2;
        maskHorizonta[2][2] = -1;
        
        //maskLeftDiag[3][3]
        maskLeftDiag[0][0] = 2;
        maskLeftDiag[0][1] = 1;
        maskLeftDiag[0][2] = 0;
        maskLeftDiag[1][0] = 1;
        maskLeftDiag[1][1] = 0;
        maskLeftDiag[1][2] = -1;
        maskLeftDiag[2][0] = -2;
        maskLeftDiag[2][1] = -1;
        maskLeftDiag[2][2] = 0;
        
        //maskRightDiag[3][3]
        maskRightDiag[0][0]= 0;
        maskRightDiag[0][1]= 1;
        maskRightDiag[0][2]= 2;
        maskRightDiag[1][0]= -1;
        maskRightDiag[1][1]= 0;
        maskRightDiag[1][2]= 1;
        maskRightDiag[2][0]= 0;
        maskRightDiag[2][1]= -1;
        maskRightDiag[2][2]= -2;
        
        sobelVertical = new int*[img.numRows+2];
        sobelHorizontal = new int*[img.numRows+2];
        sobelLeftDiag = new int*[img.numRows+2];
        sobelRightDiag = new int*[img.numRows+2];
        GradiantEdge = new int*[img.numRows+2];
        
        for(int i=0; i<img.numRows+2; i++){
            sobelVertical[i] = new int[img.numCols+2];
            sobelHorizontal[i] = new int[img.numCols+2];
            sobelLeftDiag[i]  = new int[img.numCols+2];
            sobelRightDiag[i] = new int[img.numCols+2];
            GradiantEdge[i] = new int[img.numCols+2];
        }
        for(int i=0; i<img.numRows+2; i++){
            for(int j=0; j<img.numCols+2; j++){
                sobelVertical[i][j] =0;
                sobelHorizontal[i][j] =0;
                sobelLeftDiag[i][j] =0;
                sobelRightDiag[i][j] =0;
                GradiantEdge[i][j] =0;
            }
        }
        
        for(int i=0; i<3; i++){
            for(int j=0; j<3;j++){
                neightborAry[i][j]=0;
            }
        }
        
    }
    
    void loadNeighbor(int r, int c, int** array){
            neightborAry[0][0] = array[r-1][c-1];
            neightborAry[0][1] = array[r-1][c];
            neightborAry[0][2] = array[r-1][c+1];
            neightborAry[1][0] = array[r][c-1];
            neightborAry[1][1] = array[r][c];
            neightborAry[1][2] = array[r][c+1];
            neightborAry[2][0] = array[r+1][c-1];
            neightborAry[2][1] = array[r+1][c];
            neightborAry[2][2] = array[r+1][c+1];
    }
    
    int convolute(int r, int c, int** array){
        int result = abs(neightborAry[0][0]*array[0][0]+
                          neightborAry[0][1]*array[0][1]+
                          neightborAry[0][2]*array[0][2]+
                          neightborAry[1][0]*array[1][0]+
                          neightborAry[1][1]*array[1][1]+
                          neightborAry[1][2]*array[1][2]+
                          neightborAry[2][0]*array[2][0]+
                          neightborAry[2][1]*array[2][1]+
                          neightborAry[2][2]*array[2][2]);
        return result;
    }
    
    int computeGradiant(int r, int c, int** array){
        int result = sqrt((array[r][c]-array[r+1][c])*(array[r][c]-array[r+1][c])+
                          (array[r][c]-array[r][c+1])*(array[r][c]-array[r][c+1])
                          );
        return result;
    }
};

int main(int argc, const char * argv[]) {
    ifstream infile(argv[1]);
    ofstream outfile1(argv[2]);
    ofstream outfile2(argv[3]);
    ofstream outfile3(argv[4]);
    ofstream outfile4(argv[5]);
    ofstream outfile5(argv[6]);
    ofstream outfile6(argv[7]);
    
    
    //stpe 1: load image
    int numRows, numCols, minVal, maxVal;
    infile>>numRows>>numCols>>minVal>>maxVal;
    
    image img =*new image(numRows, numCols, minVal, maxVal);
    
    img.loadImage(infile);

    img.mirrorFramed(img.mirrorFramedAry, numRows, numCols);

    edge edg = *new edge(img);
    
    for(int i=1; i<img.numRows+1; i++){
        for(int j=1; j<img.numCols+1; j++){
            edg.loadNeighbor(i, j, img.mirrorFramedAry);
            edg.sobelVertical[i][j] = edg.convolute(i, j, edg.maskVertical);
            edg.sobelHorizontal[i][j] = edg.convolute(i, j, edg.maskHorizonta);
            edg.sobelLeftDiag[i][j] = edg.convolute(i, j, edg.maskLeftDiag);
            edg.sobelRightDiag[i][j] = edg.convolute(i, j, edg.maskRightDiag);
            edg.GradiantEdge[i][j] = edg.computeGradiant(i, j, img.mirrorFramedAry);
        }
    }
    
    outfile1<<img.numRows<<" "<<img.numCols<<" "<<img.minVal<<" "<<img.maxVal<<endl;
    outfile2<<img.numRows<<" "<<img.numCols<<" "<<img.minVal<<" "<<img.maxVal<<endl;
    outfile3<<img.numRows<<" "<<img.numCols<<" "<<img.minVal<<" "<<img.maxVal<<endl;
    outfile4<<img.numRows<<" "<<img.numCols<<" "<<img.minVal<<" "<<img.maxVal<<endl;
    outfile5<<img.numRows<<" "<<img.numCols<<" "<<img.minVal<<" "<<img.maxVal<<endl;
    for(int m=1; m<img.numRows+1; m++){
        for(int n=1; n<img.numCols+1; n++){
            outfile1<< edg.sobelVertical[m][n]<<" ";
            outfile2<< edg.sobelHorizontal[m][n]<<" ";
            outfile3<< edg.sobelLeftDiag[m][n]<<" ";
            outfile4<< edg.sobelRightDiag[m][n]<<" ";
            outfile5<< edg.GradiantEdge[m][n]<<" ";
        }
        outfile1<<endl; outfile2<<endl; outfile3<<endl; outfile4<<endl; outfile5<<endl;
    }
    
    outfile6<<"SobelVertical: \n";
    outfile6<<img.prettyPrint(edg.sobelVertical);
    outfile6<<"\nSobelHorizontal: \n";
    outfile6<<img.prettyPrint(edg.sobelHorizontal);
    outfile6<<"\nSobelLeftDiag: \n";
    outfile6<<img.prettyPrint(edg.sobelLeftDiag);
    outfile6<<"\nSobelRightDiag: \n";
    outfile6<<img.prettyPrint(edg.sobelRightDiag);
    outfile6<<"\nGradiantEdge: \n";
    outfile6<<img.prettyPrint(edg.GradiantEdge);
    
    
    infile.close();
    outfile1.close();
    outfile2.close();
    outfile3.close();
    outfile4.close();
    outfile5.close();
    outfile6.close();
    
    return 0;
}
