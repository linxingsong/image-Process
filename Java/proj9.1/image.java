package proj9_1;

import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class image{
	public
	int numRows, numCols, minVal, maxVal,count;
	int imgAry[][];
	
	public image(int r, int c, int min, int max){
	    numRows = r;
	    numCols = c;
	    minVal = min;
	    maxVal = max;
	    count=0;
	    imgAry = new int[r][c];
	    for(int i=0; i<numRows; i++){
			for(int j=0; j<numCols; j++){
				imgAry[i][j] =0;
			}
		}
	}
		
	void loadImage(Scanner input){
	    int data = input.nextInt();
	    for(int i=0; i<numRows; i++){
	        for(int j=0; j< numCols;j++){
	            if(data>0){
	            	count++;
	            }
	        	imgAry[i][j] = data;
	            if(input.hasNext()){
					data= input.nextInt();
				}
	        }
	    }
	}
	
    void findPoint(FileWriter output) throws IOException{
        output.write(count+"\n"+numRows+" "+numCols+"\n");
        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                if(imgAry[i][j]>0){
                    output.write(i+" "+j+"\n");
                }
            }
        }
    }
}

