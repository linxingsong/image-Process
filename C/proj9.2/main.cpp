//
//  main.cpp
//  proj9.2
//
//  Created by Eric Lin on 4/15/17.
//  Copyright © 2017 XingSong  Lin. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
using namespace std;

class point{
    public:
        int Xcoordinate;
        int Ycoordinate;
        int clusterID;
        double distance;
    point(){
    
    }
    
    void printPoint(){
        cout<<Xcoordinate<<" for row, "<<Ycoordinate<<" for col, "<<clusterID<<" "<<distance<<endl;
    }
 
};

struct xyCoord{
    int Xcoordinate;
    int Ycoordinate;
};

class KIsoData{
public:
    int K, numPts, numRow, numCol;
    point *pointSet;
    struct xyCoord *centroids;
    double *XX2ndMM;
    double *YY2ndMM;
    double *XY2ndMM;
    double *radix;
    double *temp;
    int *countPoint;
    int *distance;
    int *sumForRow;
    int *sumForCol;
    int *sumRow;
    int *sumCol;
    int *sumRowCol;
    int **imageArray;
    
    KIsoData(int group, int numbPoints, int row, int col){
        K = group;
        numPts = numbPoints;
        numRow = row;
        numCol = col;
        pointSet = new point[numbPoints];
        centroids = new struct xyCoord[group+1];
        XX2ndMM = new double[group+1];
        XY2ndMM = new double[group+1];
        YY2ndMM = new double[group+1];
        radix = new double[group+1];
        temp = new double[group+1];
        distance = new int[group+1];
        countPoint = new int[group+1];
        sumForRow= new int[group+1];
        sumForCol= new int[group+1];
        sumRow=new int[group+1];
        sumCol=new int[group+1];
        sumRowCol=new int[group+1];
        
        imageArray = new int*[row];
        for(int i=0; i<row; i++){
            imageArray[i] = new int[col];
        }
        for(int i=0; i<row; i++){
            for(int j=0; j<col; j++){
                imageArray[i][j] =0;
            }
        }
    }
    
    void loadPointSet(ifstream &input){
        int row, col;
        for(int i=0; i<numPts; i++){
            input>>row>>col;
            pointSet[i].Xcoordinate = row;
            pointSet[i].Ycoordinate = col;
        }
    }
    
    void assignLable(){
        for(int i=0; i<numPts; i++){
            pointSet[i].clusterID = (i%K)+1;
        }
    }
    
    void mapPoint2Image(){
        for(int i=0; i<numPts; i++){
            imageArray[pointSet[i].Xcoordinate][pointSet[i].Ycoordinate] = pointSet[i].clusterID;
        }
    }
    
    void printPointSet(ofstream &output){
        for(int i=0; i<numPts; i++){
            output<<pointSet[i].Xcoordinate<<" "<<pointSet[i].Ycoordinate<<" "<<pointSet[i].clusterID<<endl;
        }
    }
    
    void PrettyPrint(ofstream &output){
        for(int i=0; i<numRow; i++){
            for(int j=0; j<numCol; j++){
                if(imageArray[i][j]!=0){
                    output<<imageArray[i][j]<<" ";
                }
                else{
                    output<<"  ";
                }
            }
            output<<endl;
        }
    }
    
    bool IsoClustering(){
        int group=1;
        bool flag;
        while(group<=K){
            computeInitialVariables(group);
            group++;
        }
        
        /*
        cout<<"Points:   "<<endl;
        debug(countPoint);
        cout<<"sumForRow: "<<endl;
        debug(sumForRow);
        cout<<"SumForCol: "<<endl;
        debug(sumForCol);
        cout<<"sumRow: "<<endl;
        debug(sumRow);
        cout<<"sumCol: "<<endl;
        debug(sumCol);
        cout<<"sumRowCol: "<<endl;
        debug(sumRowCol);
        */
        
        group=1;
        while(group<=K){
            computeAllVariables(group);
            group++;
        }
        
        for(int a=0; a<=K; a++){
            countPoint[a]=0;
        }
        
        /*
        cout<<"XX2ndMM: "<<endl;
        debug2(XX2ndMM);
        cout<<"XY2ndMM: "<<endl;
        debug2(XY2ndMM);
        cout<<"yy2ndMM: "<<endl;
        debug2(YY2ndMM);
        */
        
        computeBestFitLine();
        
        /*
        cout<<"bestfit line: "<<endl;
        debug2(radix);
        
        //cout<<"Distance: "<<endl;
        //for(int i=0; i<numPts; i++){
        //    cout<<i<<" "<<pointSet[i].clusterID<<" "<<pointSet[i].distance<<endl;
        //}
        */
        
        for(int i=0; i<numPts; i++){
           computeMinDistance(i);
        }
        
        flag = changeLable();
       
        return flag;
    }
    
    void computeInitialVariables(int index){
        for(int i=0; i<numPts; i++){
            if(pointSet[i].clusterID == index){
                sumForRow[index] = sumForRow[index]+ pointSet[i].Ycoordinate;
                sumForCol[index] = sumForCol[index]+ pointSet[i].Xcoordinate;
                countPoint[index]++;
                sumRow[index] = sumRow[index] + pointSet[i].Ycoordinate * pointSet[i].Ycoordinate;
                sumCol[index] = sumCol[index] + pointSet[i].Xcoordinate * pointSet[i].Xcoordinate;
                sumRowCol[index] = sumRowCol[index] + pointSet[i].Xcoordinate * pointSet[i].Ycoordinate;
            }
        }
        
    }
    
    void computeAllVariables(int index){
        if(countPoint[index]!=0){
            centroids[index].Xcoordinate = (int)sumForCol[index]/countPoint[index];
            centroids[index].Ycoordinate = (int)sumForRow[index]/countPoint[index];
            //cout<<centroids[index].Xcoordinate<<" "<<centroids[index].Ycoordinate<<endl;
            XX2ndMM[index] =(double) sumCol[index]/countPoint[index];
            XY2ndMM[index] = (double)sumRowCol[index]/countPoint[index];
            YY2ndMM[index] = (double)sumRow[index]/countPoint[index];
        }
        else{
            centroids[index].Xcoordinate = 0;
            centroids[index].Ycoordinate = 0;
            //cout<<centroids[index].Xcoordinate<<" "<<centroids[index].Ycoordinate<<endl;
            XX2ndMM[index] = 0;
            XY2ndMM[index] = 0;
            YY2ndMM[index] = 0;
        }
    }
    
   
    void debug(int* array){
        for(int i=1; i<=K; i++){
            cout<<array[i]<<" ";
        }
        cout<<endl;
    }
    void debug2(double* array){
        for(int i=1; i<=K; i++){
            cout<<array[i]<<" ";
        }
        cout<<endl;
    }
    
    
    void computeBestFitLine(){
        for(int i=1; i<=K; i++){
            temp[i] = (double)(XX2ndMM[i]-YY2ndMM[i])/XY2ndMM[i];
        }
        for(int j=1; j<=K; j++){
            radix[j] =(double) solveEquation(1, temp[j], -1);
        }
    }
    
    double solveEquation(double a, double b, double c){
        double result = 0.0;
        double delta = b*b -4*a*c;
        if(delta>0){
            result = (b*(-1)+sqrt(delta))/(2*a);
        }
        double result2 = (double)3.1415+atan(result);
        return result2;
    }
    
    void computeMinDistance(int index){
        double t;
        int r;
        int c;
        double minDistance;
        c = pointSet[index].Ycoordinate;
        r = pointSet[index].Xcoordinate;
        for(int j=1; j<=K; j++){
            t = radix[j]-atan2(r,c)-(3.14159/2);
            distance[j] = (int) (sqrt(c*c+r*r)* cos(t));
        }
        minDistance = distance[1];
        for(int i=1; i<K; i++){
            if(minDistance<distance[i+1]){
                minDistance = distance[i+1];
            }
        }
        //cout<<abs(minDistance)<<endl;
        pointSet[index].distance = abs(minDistance);
    }
    
    bool changeLable(){
        bool flag = false;
        for(int i=0; i<numPts-1; i++){
            for(int j=1; j<numPts; j++){
                if(pointSet[j].distance == pointSet[i].distance){
                    pointSet[j].clusterID = pointSet[i].clusterID;
                    flag = true;
                }
                else{
                    flag = false;
                }
            }
        }
        return flag;
    }
    
};

int main(int argc, const char * argv[]) {
    ifstream input(argv[1]);
    ofstream output(argv[2]);
    ofstream output2(argv[3]);
    int group, numPoints, row, col;
    bool flag = true;
    input>>numPoints>>row>>col;
    cout<<"How many group you want? (For K) "<<endl;
    cin>>group;
    KIsoData Kcluster  = *new KIsoData(group, numPoints, row, col);
    Kcluster.loadPointSet(input);
    Kcluster.assignLable();
    output<<group<<"\n"<<numPoints<<"\n"<<row<<" "<<col<<"\n";
    
    while(flag == true){
        Kcluster.mapPoint2Image();
        Kcluster.PrettyPrint(output2);
        flag = Kcluster.IsoClustering();
    
    }
    
    //Kcluster.mapPoint2Image();
    //Kcluster.PrettyPrint(output2);
    Kcluster.printPointSet(output);
    
    input.close();
    output.close();
    output2.close();
    return 0;
}
