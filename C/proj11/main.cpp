//
//  main.cpp
//  proj11
//
//  Created by Eric Lin on 5/7/17.
//  Copyright © 2017 XingSong  Lin. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
using namespace std;

class image{
    public :
    int numRows, numCols, minVal, maxVal;
    int** imageAry;
    
    image(int r, int c, int min, int max){
        numRows = r;
        numCols = c;
        minVal = min;
        maxVal = max;
        // dynamiclly allocate object, first and second array
        imageAry = new int*[numRows];
        for(int i=0; i<numRows; i++)
            imageAry[i]= new int[numCols];
    }
    
    void loadPtImage(int a, int b, int c){
        imageAry[a][b] = c;
    }
    
    void prettyPrint(ofstream &output){
        for (int i = 0; i < numRows; i++){
            for (int j = 0; j < numCols; j++){
                if(imageAry[i][j] > 0){
                    output<< imageAry[i][j] << " ";
                }
                else{
                    output << "  ";
                }
            }
            output << "\n";
        }
    }
};

class boundaryPt{
public:
    int x, y, maxVotes, corner;
    double maxDistance;
    boundaryPt(){
        x=0; y=0; maxVotes =0; maxDistance =0.0; corner =1;
    }
};

class arcChrodDistance{
    friend class boundaryPt;
public:
    int KChordLength, numPts,P1,P2,currPt;
    boundaryPt* boundPtAry;
    double* chordAry;
    
    arcChrodDistance(int K, int numberPoints){
        KChordLength = K;
        numPts = numberPoints;
        boundPtAry = new boundaryPt[numPts];
        chordAry = new double[KChordLength];
    }
    
    void loadData(ifstream &input){
        int a, b;
        //step 2, 3
        for(int i=0; i<numPts; i++){
            input>>a>>b;
            boundPtAry[i].x = a;
            boundPtAry[i].y = b;
        }
    }
    
    void computeDistance(int p1, int p2, int p3, int index){// from 1 to KChordLength, in chordAry.(P1,P2, currPt)
        double A = boundPtAry[p2].y - boundPtAry[p1].y;
        double B = boundPtAry[p2].x - boundPtAry[p1].x;
        double C = boundPtAry[p2].x * boundPtAry[p1].y - boundPtAry[p2].y * boundPtAry[p1].x;
        double D = sqrt(A*A+B*B);
        chordAry[index] = abs(A * boundPtAry[p3].x - B * boundPtAry[p3].y + C) / D;
    }
    
    int findMaxDist(){
        double max=0.0;
        int result = 0;
        for(int i=0; i<KChordLength; i++){
            if(chordAry[i]>max){
                max = chordAry[i];
            }
        }
        for(int j=0; j<KChordLength; j++){
            if(chordAry[j] == max){
                result =j;
            }
        }
        return result;
    }
    
    bool computeLocalMaxima(int pt){
        bool flag = false;
        for(int i = pt-3; i< pt+3; i++){
            if(boundPtAry[pt%numPts].maxVotes > boundPtAry[i%numPts].maxVotes){
                flag = true;
            }
            else{
                flag = false;
            }
        }
        return flag;
    }
    
    int isCorner(int pt){
        if(computeLocalMaxima(pt%numPts) &&(computeLocalMaxima((pt-1)%numPts) && computeLocalMaxima((pt+1)%numPts))){
            return boundPtAry[pt%numPts].corner = 9;
        }
        else {
            return boundPtAry[pt%numPts].corner = 1;
        }
    }
    
    void writeToFile(ofstream &output){
        for(int i=0; i<KChordLength; i++){
            output<<i<<" "<<chordAry[i]<<endl;
        }
    }
};


int main(int argc, const char * argv[]) {
    //step 0
    ifstream infile;
    infile.open(argv[1]);
    ofstream outfile1;
    outfile1.open(argv[2]);
    ofstream outfile2;
    outfile2.open(argv[3]);
    ofstream outfile3;
    outfile3.open(argv[4]);
    ofstream outfile4;
    outfile4.open(argv[5]);
    
    int row, col, min, max, label, numPoints, valueK, maxIndex, whichIndex, index;
    infile>>row>>col>>min>>max>>label>>numPoints;
    cout<<"Enter the KchordLength value: "<<endl;
    cin>>valueK;
    arcChrodDistance object(valueK, numPoints);
    //step 1 - 3
    object.loadData(infile);
    //step 4
    object.P1 =0;
    object.P2 =object.KChordLength-1;
    //step 12
    while(object.P2 != object.KChordLength/2){
        //step 5
        index=0;
        //step 7
        while(index<object.KChordLength){
            //step 6
            object.computeDistance(object.P1, object.P2, object.currPt,index);
            object.currPt++;
            object.currPt = object.currPt%object.numPts;//edit
            index++;
        }
        //step 8:
        object.writeToFile(outfile4);
        //step 9:
        maxIndex = object.findMaxDist();
        whichIndex = (object.P1 + maxIndex)%object.numPts;
        object.boundPtAry[whichIndex].maxVotes++;
        if(object.boundPtAry[whichIndex].maxDistance<object.chordAry[maxIndex]){
            object.boundPtAry[whichIndex].maxDistance = object.chordAry[maxIndex];
        }
        //step 10
        for(int i=object.P1; i<=object.P2; i++){
            outfile4<<object.boundPtAry[i].x<<" "<<object.boundPtAry[i].y<<" "
                <<object.boundPtAry[i].maxVotes<<" "<<object.boundPtAry[i].maxDistance<<endl;
        }
        //step 11
        object.P1++;
        object.P2++;
        object.P1 = object.P1 % object.numPts;
        object.P2 = object.P2 % object.numPts;
        object.currPt = (object.P1+1)%object.numPts;
    }
    
    //step 13
    int temp=0;
    for(int j=0; j<object.numPts; j++){
        temp++;
        outfile4<<"("<<object.boundPtAry[j].x<<", "<<object.boundPtAry[j].y<<", "
        <<object.boundPtAry[j].maxVotes<<", "<<object.boundPtAry[j].maxDistance<<") ";
        if(temp%5==0){
            outfile4<<endl;
        }
    }
    
    //step 14 ,15
    for(int i=0; i<(object.numPts+object.KChordLength/2); i++){
        object.isCorner(i);
    }
    
    //step 16  argv[2] finsih//
    outfile1<<row<<" "<<col<<" "<<min<<" "<<max<<"\n"<<label<<"\n"<<numPoints<<endl;
    for(int j=0; j<object.numPts; j++){
       outfile1<<object.boundPtAry[j].x<<" "<<object.boundPtAry[j].y<<" "
        <<object.boundPtAry[j].corner<<" "<<endl;
    }
    //argv[3]
    for(int j=0; j<object.numPts; j++){
        outfile2<<"("<<object.boundPtAry[j].x<<", "<<object.boundPtAry[j].y<<", "
        <<object.boundPtAry[j].corner<<")  "<<endl;;
    }
    
    //step 17
    image img(row, col, min, max);
    for(int j=0; j<object.numPts; j++){
        img.loadPtImage(object.boundPtAry[j].x, object.boundPtAry[j].y, object.boundPtAry[j].corner);
    }
    //step 18 argv[4]  finish
    img.prettyPrint(outfile3);
    
    //step 19
    infile.close();
    outfile1.close();
    outfile2.close();
    outfile3.close();
    return 0;
}
