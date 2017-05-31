package computeHistrogram;

//proj1.2: Histrogram
//By XingSong Lin
//2/4/2017

import java.io.*;
import java.util.Scanner;

public class computeHistrogram{	
	
	public static void constructor(int[] histogram_Ary, int[][] p, int numRows, int numCols){
		for(int i=0; i<numRows; i++){
			for(int j=0; j<numCols; j++){
				histogram_Ary[p[i][j]]++;
			}
		}
	}
	
	public static void computerHistogram(int maxVals, int[] histogram_Ary, FileWriter outFile) throws IOException{
		String myChar = "+";
		String symbolLine = " ";
		String finalLine = "";
		for(int i=0; i<=maxVals; i++){
			int counts = histogram_Ary[i];
			while(counts >0){
				if(counts>60){
					symbolLine = " 60 +'s";
					counts = 0;
				}
				symbolLine = symbolLine+myChar;
				counts--;	
				}
				finalLine = "( "+ i +"): "+ histogram_Ary[i] + symbolLine+ "\n";
				outFile.write(finalLine);
				symbolLine = "";
				finalLine = "";
		}
	}
	
	public static void main(String[] args) throws IOException{
		File input = new File(args[0]);
		Scanner input_scan = new Scanner(input);
		//get all requirement value
		int numRows = input_scan.nextInt();
		int numCols = input_scan.nextInt();
		int minVal = input_scan.nextInt();
		int maxVal = input_scan.nextInt();
		int[][] p= new int[numRows][numCols];
		
		int newMax =0;
		int data = input_scan.nextInt();
		while(input_scan.hasNextInt()){
			for(int i=0; i<numRows; i++){
				for(int j=0; j<numCols; j++){
					p[i][j] = data;
					if(p[i][j]>newMax){
						newMax = p[i][j];
					}
					if(input_scan.hasNextInt()){
						data=input_scan.nextInt();
					}
				}	
			}
		}

		int[] histogram_Ary = new int[newMax+1];
		for(int m=0; m<=newMax; m++){
			histogram_Ary[m] = 0;
		}
		//System.out.println("finished here.");
		constructor(histogram_Ary, p, numRows, numCols);
		
		//System.out.println("constructor finished.");
		
		String outFileName = args[0].substring(0,args[0].length()-4)+"_hist.txt";
		File output = new File(outFileName);
		FileWriter outFile = new FileWriter(output);

		
		computerHistogram(newMax, histogram_Ary, outFile);
		//System.out.println("all done");
		
		input_scan.close();
		outFile.close();
		return;
	}
	
}