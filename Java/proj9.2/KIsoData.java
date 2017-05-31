package proj9_2;

import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class KIsoData {
	public
	    int K, numPts, numRow, numCol;
	    point pointSet[];
	    xyCoord centroids[];
	    double XX2ndMM[];
	    double YY2ndMM[];
	    double XY2ndMM[];
	    double radix[];
	    double temp[];
	    int countPoint[];
	    int distance[];
	    int sumForRow[];
	    int sumForCol[];
	    int sumRow[];
	    int sumCol[];
	    int sumRowCol[];
	    int imageArray[][];
	    
	    KIsoData(int group, int numbPoints, int row, int col){
	        K = group;
	        numPts = numbPoints;
	        numRow = row;
	        numCol = col;
	        pointSet = new point[numbPoints];
	        for(int m=0; m<numbPoints; m++){
	        	pointSet[m]= new point();
	        }
	        centroids = new xyCoord[group+1];
	        for(int n=0; n<=group; n++){
	        	centroids[n] = new xyCoord();
	        }
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
	        
	        imageArray = new int[row][col];
	        for(int i=0; i<row; i++){
	            for(int j=0; j<col; j++){
	                imageArray[i][j] =0;
	            }
	        }
	    }
	    
	    void loadPointSet(Scanner input){
	        for(int i=0; i<numPts; i++){
	            pointSet[i].Xcoordinate = input.nextInt();
	            pointSet[i].Ycoordinate = input.nextInt();
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
	    
	    void printPointSet(FileWriter output) throws IOException{
	        for(int i=0; i<numPts; i++){
	            output.write(pointSet[i].Xcoordinate+" "+pointSet[i].Ycoordinate+" "+pointSet[i].clusterID+"\n");
	        }
	    }
	    
	    void PrettyPrint(FileWriter output) throws IOException{
	        for(int i=0; i<numRow; i++){
	            for(int j=0; j<numCol; j++){
	                if(imageArray[i][j]>0){
	                    output.write(imageArray[i][j]+" ");
	                }
	                else{
	                    output.write("  ");
	                }
	            }
	            output.write("\n");;
	        }
	    }
	    
	    boolean IsoClustering(){
	        int group=1;
	        boolean flag;
	        while(group<=K){
	            computeInitialVariables(group);
	            group++;
	        }
	        
	        group=1;
	        while(group<=K){
	            computeAllVariables(group);
	            group++;
	        }
	        
	        for(int a=0; a<=K; a++){
	            countPoint[a]=0;
	        }
	        
	        computeBestFitLine();
	        
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
	            result = (b*(-1)+Math.sqrt(delta))/(2*a);
	        }
	        double result2 = (double)3.1415+Math.atan(result);
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
	            t = radix[j]-Math.atan2(r,c)-(3.14159/2);
	            distance[j] = (int) (Math.sqrt(c*c+r*r)* Math.cos(t));
	        }
	        minDistance = distance[1];
	        for(int i=1; i<K; i++){
	            if(minDistance<distance[i+1]){
	                minDistance = distance[i+1];
	            }
	        }
	        //cout<<abs(minDistance)<<endl;
	        pointSet[index].distance = Math.abs(minDistance);
	    }
	    
	    boolean changeLable(){
	        boolean flag = false;
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

}
