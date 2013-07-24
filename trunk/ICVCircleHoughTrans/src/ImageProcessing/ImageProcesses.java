package ImageProcessing;

import java.awt.Color;

public class ImageProcesses {
	static int[][] edgeFilterX = { { -1, 0, 1 },
								   { -2, 0, 2 },
								   { -1, 0, 1 } };
	static int[][] edgeFilterY = { { 1, 2, 1 },
		                           { 0, 0, 0 }, 
		                           { -1, -2, -1 } };

	public static Picture createEdgeImage(Picture pic, String output) {
		int height = pic.height();
		int width = pic.width();
		Picture pic2 = new Picture(output, width, height);

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				// get colours
				int c1 = 0, c2 = 0;
				for (int i = 0; i < 3; ++i)
					for (int j = 0; j < 3; ++j) {
						int lum = (int) ImageMethods.lum(pic.get(x - 1 + i, y
								- 1 + j));
						c1 += lum * edgeFilterX[i][j];
						c2 += lum * edgeFilterY[i][j];
					}

				int mag = 255 - ImageMethods.truncate((int) Math.sqrt(c1 * c1
						+ c2 * c2));
				pic2.set(x, y, new Color(mag, mag, mag));
			}
		}

		return pic2;
	}
}
