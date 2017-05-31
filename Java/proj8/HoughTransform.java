package proj8;

import java.io.FileWriter;
import java.io.IOException;

public class HoughTransform{
private
	int numRows;
	int numCols;
	int minVal;
	int maxVal;
	int[][] HoughSpace;	
	int angleInDegree;
	double angleInRadians;
	xyCoord point;
	public HoughTransform(){
		
	}
	
	void run(image img, FileWriter outFile1) throws IOException{
		numRows = 180;
		numCols = (int) (Math.sqrt(img.numRows*img.numRows + 
						img.numCols*img.numCols));

		HoughSpace = new int[numRows][numCols];
		for(int i = 0;i<numRows;i++){
			for(int j =0;j<numCols;j++){
				HoughSpace[i][j] = 0; //set all hough Array to 0
			}
		}	
		//step 2:  angleInDegree <-- 0
        //angleInRadians <-- angleInDegree / 180.00 * pi 
		for (int i = 0; i < img.numRows; i++){
			for (int j = 0; j < img.numCols; j++){
				//step 5: repeat step 1 to step 4 until all pixels are processed
				if(img.imgAry[i][j] > 0){
					//step 4: repeat step 3 until angleInDegree > 179
					for(int angleInDegree = 0; angleInDegree<180; angleInDegree++){
						//step 3: dist <-- computeDistance (point, angleInRadians)
						double angleInRadians = (angleInDegree *3.14159)/180.0;
						int distance = computeDistance(j,i,angleInRadians);
						HoughSpace[angleInDegree][distance]++;
					}
				}
			}
		}
		//step 6: determineHeader (HoughAry)
		determineHeader();
		printAry(outFile1);
	}
	
	int computeDistance(int col, int row, double angRadions){
		double t;
		t = angRadions- Math.atan2(row,col)-(3.14159/2);
		int dis;
		dis = (int) (Math.sqrt(col*col + 
				row*row)*Math.cos(t));
		return Math.abs(dis);
	}
	
	void determineHeader(){
		int newMin = 0;
		int newMax = 0;
		for(int i = 0;i<numRows;i++){
			for(int j =0;j<numCols;j++){
					if(HoughSpace[i][j]<newMin)
					newMin = HoughSpace[i][j];
					if(HoughSpace[i][j]>newMax)
					newMax = HoughSpace[i][j];
				}
		}
		minVal = newMin;
		maxVal = newMax;
	}
	
	void printAry(FileWriter outFile1) throws IOException{		
		outFile1.write(this.numRows + " " + this.numCols + " " + this.minVal + " " + this.maxVal);
		outFile1.write("\n");
		for(int i = 0;i<numRows;i++){
			for(int j =0;j<numCols;j++){
				outFile1.write(HoughSpace[i][j]+" ");
			}
			outFile1.write("\n");
		}
	}

}
	
