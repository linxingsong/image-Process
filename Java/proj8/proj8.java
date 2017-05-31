package proj8;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class proj8 {
	public static void main(String[] args) throws IOException{	
		//step 0: - open input file and output files
		//	dynamically allocate Hough2DAry and initiallize to zero
		File input = new File(args[0]);
		Scanner inFile = new Scanner(input);
		File output1 = new File(args[1]);
		FileWriter outFile1 = new FileWriter(output1);
		int row = inFile.nextInt();
		int col = inFile.nextInt();
		int min = inFile.nextInt();
		int max = inFile.nextInt();
		image  img = new image(row, col, min, max);
		img.loadImage(inFile);
		HoughTransform ht = new HoughTransform();
		ht.run(img, outFile1);
		//step 7: close all files
		inFile.close();
		outFile1.close();
	}
}
