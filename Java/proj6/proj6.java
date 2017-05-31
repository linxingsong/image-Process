package proj6;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;
//
//Created by Eric Lin on 3/11/17.
//Copyright © 2017 XingSong  Lin. All rights reserved.
//
public class proj6 {
	@SuppressWarnings("unused")
	public static void main(String[] args) throws IOException{
		//step 1: open files
		File input = new File(args[0]);
		Scanner inFile = new Scanner(input);
		File output1 = new File(args[1]);
		FileWriter outFile1 = new FileWriter(output1);
		File output2 = new File(args[2]);
		FileWriter outFile2 = new FileWriter(output2);	
		//step 2: load image
		int row = inFile.nextInt();
		int col = inFile.nextInt();
		int min = inFile.nextInt();
		int max = inFile.nextInt();
		outFile1.write("#row: "+row+" #col: "+col+" minVal: "+min+" maxVal: "+max+" \n");
		outFile2.write(row+" "+col+" "+min+" "+max+"\n");
		image  img = new image(row, col, min, max);
		img.zeroFramed(img.zeroFramedAry);
		img.loadImage(inFile);
		//step 3:if image(iRow, jCol)>0 ... 
		chainCode code = new chainCode(img, outFile1, outFile2);
		//step 9: close file.
		inFile.close();
		outFile1.close();
		outFile2.close();
	}
}