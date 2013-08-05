package ImageProcessing;

import java.awt.Color;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * 
 * @author Brandon James Talbot
 * Static methods class required for image manipulation
 */
public class ImageMethods {
	
	/**
	 * Calculates the luminance value for a colour
	 * @param color The colour to change
	 * @return A double value (luminance)
	 */
	public static double lum(Color color) {
		int r = color.getRed();
		int g = color.getGreen();
		int b = color.getBlue();
		return .299 * r + .587 * g + .114 * b;
	}

	/**
	 * Truncates a value into a range of 0 - 255 (make sure the value can fit into the java color class)
	 * 	anything over 255 becomes 255 anything below 0 becomes 0
	 * @param a The value to truncate
	 * @return value between 0 - 255
	 */
	public static int truncate(int a) {
		if (a < 0)
			return 0;
		else if (a > 255)
			return 255;
		else
			return a;
	}

	// /////////////////////////////////////////////////////////////////////////////////////////////////////////
	// edge stuff

	/**
	 * 
	 * @author Brandon James Talbot
	 *	Runnable class for concurrent edge detection
	 *	Calculates the value for the current pixel using the Sobel operator
	 */
	private static class edgeRunenr implements Runnable {
		// globals
		Picture in, out;
		int x, y;
		int[][] edgex, edgey;

		/**
		 * Constructor for the runnable
		 * @param in The input image
		 * @param out The output image
		 * @param x the x position
		 * @param y the y position
		 * @param ex the x filter to run
		 * @param ey the y filter to run
		 */
		public edgeRunenr(Picture in, Picture out, int x, int y, int[][] ex,
				int[][] ey) {
			this.in = in;
			this.out = out;
			this.x = x;
			this.y = y;
			this.edgex = ex;
			this.edgey = ey;
		}

		@Override
		public void run() {
			int c1 = 0, c2 = 0; // first and second colour
			for (int i = 0; i < 3; ++i) // loop for x -1 to 1
				for (int j = 0; j < 3; ++j) { // loop for y -1 to 1
					int lum = (int) ImageMethods.lum(in.get(x - 1 + i, y - 1
							+ j)); // calc luminance for the values
					c1 += lum * edgex[i][j];  // calc filter of value for x
					c2 += lum * edgey[i][j]; // calc filter or value for y
				}

			int mag = 255 - ImageMethods.truncate((int) Math.sqrt(c1 * c1 + c2
					* c2)); // get the magnitude value (making the edges black lines)
			out.set(x, y, new Color(mag, mag, mag)); // set the output image colour
		}
	}

	/**
	 * Run a dual band filter (a filter that runs in x and y)
	 * @param input The imput image
	 * @param the name for output image
	 * @param xFilter the x filter
	 * @param yFilter the y filter
	 * @return the output image
	 */
	public static Picture runDualBandFilter(Picture input, String output,
			int[][] xFilter, int[][] yFilter) {
		int height = input.height();
		int width = input.width();

		Picture out = new Picture(output, width, height);

		ExecutorService serv = Executors.newCachedThreadPool();

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				edgeRunenr r = new edgeRunenr(input, out, x, y, xFilter,
						yFilter);
				serv.execute(r);
			}
		}
		serv.shutdown();
		while (!serv.isTerminated()) {
		}
		return out;
	}
}
