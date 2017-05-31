//
//  main.cpp
//  proj12
//
//  Created by Eric Lin on 5/13/17.
//  Copyright © 2017 XingSong  Lin. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class morphology{
public:
    int numRowsIMG, numColsIMG, minValIMG, maxValIMG,
        numRowsStrctElem, numColsStrctElem, minValStrctElem, maxValStrctElem,
        rowOrigin, colOrigin, rowFrameSize, colFrameSize;
    int** imgAry;
    int** morphAry;
    int** structElem;
    
    morphology(ifstream &input1, ifstream &input2){
        input1>>numRowsIMG>>numColsIMG>>minValIMG>>maxValIMG;
        input2>>numRowsStrctElem>>numColsStrctElem>>minValStrctElem>>maxValStrctElem>>rowOrigin>>colOrigin;
    }

    void computeFrameSize(){
        rowFrameSize = numRowsStrctElem;
        colFrameSize = numColsStrctElem;
    }
    
    void setMophAry(){
        morphAry= new int*[numRowsIMG+rowFrameSize];
        for(int i = 0; i<numRowsIMG+rowFrameSize;i++){
            morphAry[i]= new int[numColsIMG+colFrameSize];
        }
        for(int i = 0;i<numRowsIMG+rowFrameSize;i++){
            for(int j =0;j<numColsIMG+colFrameSize;j++){
                morphAry[i][j] = 0; 
            }
        }
    }
    
    void loadImage(ifstream &input){
        imgAry = new int*[numRowsIMG+rowFrameSize];
        for(int i = 0; i<numRowsIMG+rowFrameSize;i++){
            imgAry[i]= new int[numColsIMG+colFrameSize];
        }
        
        for(int i = 0;i<numRowsIMG+rowFrameSize;i++){
            for(int j =0;j<numColsIMG+colFrameSize;j++){
                imgAry[i][j] = 0;
            }
        }
        int data;
        for(int i=rowFrameSize/2; i<numRowsIMG+rowFrameSize/2; i++){
            for(int j=colFrameSize/2; j<numColsIMG+colFrameSize/2;j++){
                input>>data;
                imgAry[i][j] = data;
            }
        }
    }

    void loadStruct(ifstream &input){
        structElem = new int*[numRowsStrctElem];
        for(int i = 0; i<numRowsStrctElem;i++){
            structElem[i]= new int[numColsStrctElem];
        }
        for(int i=0; i<numRowsStrctElem; i++){
            for(int j=0; j< numColsStrctElem;j++){
                structElem[i][j] = 0;
            }
        }
        int data;
        for(int i=0; i<numRowsStrctElem; i++){
            for(int j=0; j< numColsStrctElem;j++){
                input>>data;
                structElem[i][j] = data;
            }
        }
    }
    
    void PrettyPrint(int** ary, int starR, int starC,int endR,int endC){
        for(int i = starR; i<endR;i++){
            for(int j = starC;j<endC;j++){
                if(ary[i][j] > 0){
                    cout<<ary[i][j]<<" ";
                }
                else{
                    cout<< "  ";	
                }
            }
            cout<<endl; 
        }
    }
    
    int** dilation(int** imageAry,int** strAry){
        for(int i = rowFrameSize/2;i <numRowsIMG + rowFrameSize/2;i++){
            for(int j = colFrameSize/2;j < numColsIMG + colFrameSize/2;j++){
                if(imageAry[i][j]>0){
                    for(int x =  i - rowOrigin; x < i - rowOrigin + numRowsStrctElem;x++){
                        for(int y = j - colOrigin; y < j - colOrigin + numColsStrctElem;y++){
                            if(strAry[x+rowOrigin-i][y+colOrigin-j] == 1)
                                morphAry[x][y] = strAry[x+rowOrigin-i][y+colOrigin-j];
                        }
                    }
                }
            }
        }
        return morphAry;
    }
    
    int** erosion(int** imageAry,int** strAry)    {
        for(int i = rowFrameSize/2;i <numRowsIMG + rowFrameSize/2;i++){
            for(int j = colFrameSize/2;j < numColsIMG + colFrameSize/2;j++){
                if(imageAry[i][j]>0){
                    int diff = 0;
                    for(int x =  i - rowOrigin; x < i - rowOrigin + numRowsStrctElem; x++){
                        for(int y = j - colOrigin; y < j - colOrigin + numColsStrctElem; y++){
                            if(strAry[x+rowOrigin-i][y+rowOrigin-j] == 1 && imageAry[x][y] != 1){
                                diff++;
                            }
                        }
                    }
                    if(diff == 0){
                        morphAry[i][j] = 1;
                    }
                }
            }
        }
        return morphAry;
    }
    
    void closing(int** imageAry,int** strAry){
        int** temp = dilation(imageAry,strAry);
        setMophAry();
        morphAry = erosion(temp,strAry);
    }
    
    void opening(int** imageAry,int** strAry){
        int** temp = erosion(imageAry,strAry);
        setMophAry();
        morphAry = dilation(temp,strAry);
    }
    
    void writeToFile(ofstream &outfile){
        outfile << numRowsIMG << " " << numColsIMG << " " << minValIMG << " " << maxValIMG << "\n";
        for(int i = rowFrameSize/2; i<numRowsIMG+1;i++){
            for(int j = colFrameSize/2;j<numColsIMG+1;j++){
                outfile << morphAry[i][j] << " ";
            }
            outfile << "\n";
        }
    }

    void process( ifstream &input1, ifstream &input2, ofstream &output1, ofstream &output2, ofstream &output3, ofstream &output4){
        //step1:computeFrameSize
        computeFrameSize();
        //step2: - dynamically allocate imgAry with extra rows and extra columns
        loadImage(input1);
        cout<<"input Image after prettyPrint: "<<endl;
        PrettyPrint(imgAry, rowFrameSize/2, colFrameSize/2, numRowsIMG+1, numColsIMG+1);
        //step3: - dynamically allocate structElem array
        loadStruct(input2);
        cout<<"Input Structure element after pretty Print: "<<endl;
        PrettyPrint(structElem, 0, 0, numRowsStrctElem, numColsStrctElem);
        //step 4: call delation
        setMophAry();
        cout<<"By dilation Operation: "<<endl;
        morphAry = dilation(imgAry, structElem);
        PrettyPrint(morphAry, rowFrameSize/2, colFrameSize/2, numRowsIMG+1, numColsIMG+1);
        //output1<<"By dilation Operation: "<<endl;
        writeToFile(output1);
        //step 5: call erosion
        setMophAry();
        cout<<"By erosion Operation: "<<endl;
        morphAry = erosion(imgAry, structElem);
        PrettyPrint(morphAry, rowFrameSize/2, colFrameSize/2, numRowsIMG+1, numColsIMG+1);
        //output2<<"By erosion Operation: "<<endl;
        writeToFile(output2);
        //step 6: call closing
        setMophAry();
        cout<<"By closing Operation: "<<endl;
        closing(imgAry, structElem);
        PrettyPrint(morphAry, rowFrameSize/2, colFrameSize/2, numRowsIMG+1, numColsIMG+1);
        //output3<<"By closing Operation: "<<endl;
        writeToFile(output3);
        //step 7: call opening
        setMophAry();
        cout<<"By opening Operation: "<<endl;
        opening(imgAry, structElem);
        PrettyPrint(morphAry, rowFrameSize/2, colFrameSize/2, numRowsIMG+1, numColsIMG+1);
        //output4<<"By opening Operation"<<endl;
        writeToFile(output4);
    }
    
};

int main(int argc, const char * argv[]) {
    //step 0
    ifstream input1; input1. open(argv[1]);
    ifstream input2; input2. open(argv[2]);
    ofstream output1; output1.open(argv[3]);
    ofstream output2; output2.open(argv[4]);
    ofstream output3; output3.open(argv[5]);
    ofstream output4; output4.open(argv[6]);
    
    morphology object(input1, input2);
    object.process(input1, input2, output1, output2, output3, output4);
    
    //step 8
    input1.close(); input2.close(); output1.close(); output2.close(); output3.close(); output4.close();
    return 0;
}
