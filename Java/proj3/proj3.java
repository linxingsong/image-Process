package proj3;

import java.io.*;
import java.util.Scanner;

public class proj3 {
	public static void main(String[] args) throws IOException{
		//step 0: read the image header

		File input = new File(args[0]);
		Scanner input_scan = new Scanner(input);
		File output1 = new File(args[1]);
		FileWriter outFile1 = new FileWriter(output1);
		File output2 = new File(args[2]);
		FileWriter outFile2 = new FileWriter(output2);
		File output3 = new File(args[3]);
		FileWriter outFile3 = new FileWriter(output3);
		
		int numRows = input_scan.nextInt();
		int numCols = input_scan.nextInt();
		int minVal = input_scan.nextInt();
		int maxVal = input_scan.nextInt();
		EightConnectedComponent ecc = new EightConnectedComponent(numRows, numCols, minVal, maxVal);
		
		//step 1: load image onto zeroFramedARY
		int data = input_scan.nextInt();
		while(input_scan.hasNextInt()){
			for(int i=0; i<numRows; i++){
				for(int j=0; j<numCols; j++){
					ecc.zeroFramedARY[i+1][j+1] = data;
					if(input_scan.hasNext()){
						data = input_scan.nextInt();
					}
				}
			}	
		}
		ecc.mirrorFramed();
		//step 2: 8ConnectCC_Pass 1
		ecc.E_ConnectCC_pass1(ecc);
		outFile1.write("Pretty print for Pass 1: \n");
		ecc.prettyPrint(outFile1, ecc);
		outFile1.write("EQAry for Pass 1: \n");
		ecc.printEQAry(outFile1, ecc);
		outFile1.write("\n \n");
		
		//step 3: 8ConnectCC_Pass 2
		ecc.E_ConnectCC_pass2(ecc);
		outFile1.write("Pretty print for Pass 2: \n");
		ecc.prettyPrint(outFile1, ecc);
		
		outFile1.write("EQAry for Pass 2: \n");
		ecc.printEQAry(outFile1, ecc);
		//step 4: manageEQAry
		ecc.manageEQAry(ecc);
		
		outFile1.write("After Manage EQAry: \n");
		ecc.printEQAry(outFile1, ecc);		
		outFile1.write("\n \n");
		
		//step 5: 8ConnectCC_Pass3
		ecc.E_ConnectCC_pass3(ecc);
		outFile1.write("Pretty print for Pass 3: \n");
		ecc.prettyPrint(outFile1, ecc);
		outFile1.write("EQAry for Pass 3: \n");
		ecc.printEQAry(outFile1, ecc);
		outFile1.write("\n \n");
		ecc.construct_ECCProperty(ecc);
		
		ecc.PrintImage(outFile2, ecc);
		outFile2.write("\n \n");
		
		outFile3.write("Connected Component Property: \n");
		ecc.printECCProperty(outFile3, ecc);
		outFile3.write("\n \n");
	
		input_scan.close();
		outFile1.close();
		outFile2.close();
		outFile3.close();
	}
}
