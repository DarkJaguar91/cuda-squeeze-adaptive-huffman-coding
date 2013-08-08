package ImageProcessing;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.geom.Ellipse2D;

import javax.swing.JPanel;

import GUI.MainFrame;

/**
 * 
 * @author Brandon James Talbot Image processing methods (call to the filter
 *         bands) Hough transforms Edge detection Sobel
 */
public class ImageProcesses {

	/**
	 * Runs the correct filters to run Sobel edge detection on the image methods
	 * dual band filter
	 * 
	 * @param pic
	 *            The input image
	 * @param out
	 *            the name for output image
	 * @return the output image
	 */
	public static Picture createEdgeImage(Picture pic, String out) {
		int[][] edgeFilterX = { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } };
		int[][] edgeFilterY = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };

		Picture p = ImageMethods.runDualBandFilter(pic, out, edgeFilterX,
				edgeFilterY);

		return p;
	}

	/**
	 * Hough Transform to find circles
	 * 
	 * @param pic
	 *            The input picture (Should be edge detected image)
	 * @param minCircles
	 *            The minimum circle radius
	 * @param maxCircles
	 *            The maximum circle radius
	 * @param percOfEdge
	 *            The percentage of the edge to search (how black the edge must
	 *            be to use that as reference point)
	 * @param percentOfCirc
	 *            Percentage of the circumferance of the circle
	 * @param drawHough
	 *            If the hough transform must be drawn
	 * @param contentPane
	 *            The content pane drawing the image (done to refresh the
	 *            progress bar)
	 * @param frame
	 *            The Main frame class in order to change th int vars for
	 *            progress bar
	 * @return The image of were circles were found
	 */
	public static Picture findCircles(Picture pic, int minCircles,
			int maxCircles, float percOfEdge, float percentOfCirc,
			JPanel contentPane, MainFrame frame, Picture accumulator, Picture outimage) {
		Picture out = new Picture("Circles.jpg", pic.width(), pic.height());
		accumulator.setImage(new Picture(accumulator.getFileName(),
				pic.width(), pic.height()));

		int numRadiusBlocks = Math.abs(maxCircles - minCircles) + 1;

		frame.total = (pic.width() * pic.height() * numRadiusBlocks)
				* (4) + 1;
		frame.current = 0;

		// create the radius images
		int[][][] images = new int[numRadiusBlocks][pic.width()][pic.height()];
		boolean center[][] = new boolean[pic.width()][pic.height()];
		int max[] = new int[numRadiusBlocks];
		for (int y = 0; y < pic.height(); ++y)
			for (int x = 0; x < pic.width(); ++x) {
				center[x][y] = false;

				for (int i = 0; i < numRadiusBlocks; ++i) {
					if (x == 0 && y == 0)
						max[i] = Integer.MIN_VALUE;
					images[i][x][y] = 0;
				}
			}

		++frame.current;
		contentPane.updateUI();

		// draw the circles
		for (int y = 0; y < pic.height(); ++y) {
			for (int x = 0; x < pic.width(); ++x) {
				for (int i = 0; i < numRadiusBlocks; ++i) {
					if (pic.get(x, y).getRed() < 255 * percOfEdge) {
						int rad = minCircles + i;
						DrawCircle(x, y, rad, images[i]);
					}
					++frame.current;
				}
			}
			contentPane.updateUI();
		}

		// normalise
		if (true) { // Normalize the hough space
			for (int y = 0; y < pic.height(); ++y) {
				for (int x = 0; x < pic.width(); ++x) {
					for (int i = 0; i < numRadiusBlocks; ++i) {
						max[i] = Math.max(max[i], images[i][x][y]);
						++frame.current;
					}
				}
				contentPane.updateUI();
			}
			for (int y = 0; y < pic.height(); ++y) { // draw the hough space
				for (int x = 0; x < pic.width(); ++x) {
					for (int i = 0; i < numRadiusBlocks; ++i) {
						int col = images[i][x][y];
						col = (int) ((col / (float) max[i]) * 255f);

						col = Math.max(col,
								new Color(accumulator.image.getRGB(x, y)).getRed());

						accumulator.image.setRGB(x, y,
								new Color(col, 0, 0).getRGB());
						++frame.current;
					}
				}
				contentPane.updateUI();
			}
		}
		// ///// normalized

		// draw the circles
		Graphics2D g = out.image.createGraphics();
		g.setColor(Color.white);
		g.setStroke(new BasicStroke(2));
		Graphics2D g2 = outimage.image.createGraphics();
		g2.setColor(Color.red);
		g2.setStroke(new BasicStroke(2));
		for (int y = 0; y < pic.height(); ++y) {
			for (int x = 0; x < pic.width(); ++x) {
				for (int i = 0; i < numRadiusBlocks; ++i) {
					int r = minCircles + i;
					int d = r * 2;
					int check = (int) ((Math.PI * 2 * r) * percentOfCirc);
					if (x < r || x > pic.width() - r) {
						double distDiff = (Math.min(x, pic.width() - x) / (double) r) / 2.5f;
						check = (int) (check * (0.5d + distDiff));
					}

					int col = images[i][x][y];
					if (col > check) {
						if (maxima(x, y, images[i])) {
							if (checkSurroundings(x, y, center)) {
								center[x][y] = true;
								Ellipse2D e = new Ellipse2D.Double(x - r,
										y - r, d, d);
								g.draw(e);
								g.drawString("" + r, x - 5, y + 5);
								g2.draw(e);
							}
						}
					}
					++frame.current;
				}
			}
			contentPane.updateUI();
		}
		return out;
	}

	/**
	 * Checks the surounding parts of the boolean image to see if a circle has
	 * already been found
	 * 
	 * @param x
	 *            The x co-ordinate
	 * @param y
	 *            the y co-ordinate
	 * @param center
	 *            The boolean picture array
	 * @return False a circle has already been drawn/ true a circle should be
	 *         drawn
	 */
	public static boolean checkSurroundings(int x, int y, boolean center[][]) {
		for (int i = -2; i <= 2; ++i)
			for (int j = -2; j <= 2; ++j) {
				if (getValue(x + i, y + j, center) == true)
					return false;
			}
		return true;
	}

	/**
	 * Checks if the current position is a maxima or not
	 * 
	 * @param x
	 *            The x co-ord
	 * @param y
	 *            the y co-ord
	 * @param image
	 *            the image to check
	 * @return if the point is a maxima
	 */
	public static boolean maxima(int x, int y, int[][] image) {
		int val = image[x][y];
		for (int i = -1; i < 2; ++i)
			for (int j = -1; j < 2; ++j) {
				if (i == 0 && j == 0) {
				} else {
					if (getValue(x + i, y + j, image) >= val)
						return false;
				}
			}
		return true;
	}

	/**
	 * Gets a value from a boolean array (checking boundries)
	 * 
	 * @param x
	 *            The x co-ord
	 * @param y
	 *            the y co-ord
	 * @param image
	 *            the boolean image array
	 * @return the boolean value at the point or false
	 */
	public static boolean getValue(int x, int y, boolean[][] image) {
		if (x < 0 || y < 0 || x >= image.length || y >= image[0].length)
			return false;
		return image[x][y];
	}

	/**
	 * Gets the value from the image int array (checking boundries)
	 * 
	 * @param x
	 *            The x co-ord
	 * @param y
	 *            the y co-ord
	 * @param image
	 *            the int array image
	 * @return the value or -1 (if not within boundries)
	 */
	public static int getValue(int x, int y, int[][] image) {
		if (x < 0 || y < 0 || x >= image.length || y >= image[0].length)
			return -1;
		return image[x][y];
	}

	/**
	 * Draws a pixel at the given point (if possible) (increases value)
	 * 
	 * @param x
	 *            The x co-ord
	 * @param y
	 *            the y co-ord
	 * @param image
	 *            the image to draw on
	 */
	public static void DrawPixel(int x, int y, int[][] image) {
		if (x < 0 || y < 0 || x >= image.length || y >= image[0].length)
			return;

		image[x][y] += 1;
	}

	/**
	 * Draws a circle at the given point with given radius
	 * 
	 * @param x0
	 *            The x co-ord
	 * @param y0
	 *            the y co-ord
	 * @param radius
	 *            the radius
	 * @param out
	 *            the image to write to (int array) Using the mid point
	 *            algorithm off Wikipedia
	 */
	public static void DrawCircle(int x0, int y0, int radius, int[][] out) {
		int x = radius, y = 0;
		int radiusError = 1 - x;

		while (x >= y) {
			DrawPixel(x + x0, y + y0, out);
			DrawPixel(y + x0, x + y0, out);
			DrawPixel(-x + x0, y + y0, out);
			DrawPixel(-y + x0, x + y0, out);
			DrawPixel(-x + x0, -y + y0, out);
			DrawPixel(-y + x0, -x + y0, out);
			DrawPixel(x + x0, -y + y0, out);
			DrawPixel(y + x0, -x + y0, out);

			y++;
			if (radiusError < 0)
				radiusError += 2 * y + 1;
			else {
				x--;
				radiusError += 2 * (y - x + 1);
			}
		}
	}
}
