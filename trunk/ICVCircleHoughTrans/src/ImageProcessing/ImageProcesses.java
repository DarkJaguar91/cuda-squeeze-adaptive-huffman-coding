package ImageProcessing;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.geom.Ellipse2D;

import javax.swing.JPanel;

public class ImageProcesses {
	public static int total= 1, current = 0;
	
	public static Picture createEdgeImage(Picture pic, String out) {
		int[][] edgeFilterX = { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } };
		int[][] edgeFilterY = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };

		Picture p = ImageMethods.runDualBandFilter(pic, out, edgeFilterX,
				edgeFilterY);

		return p;
	}

	public static Picture findCircles(Picture pic, int minCircles,
			int maxCircles, float percOfEdge, float percentOfCirc,
			boolean drawHough, JPanel contentPane) {
		Picture out = new Picture("Circles.jpg", pic.width(), pic.height());
		
		int numRadiusBlocks = Math.abs(maxCircles - minCircles) + 1;


		total = (pic.width() * pic.height() * numRadiusBlocks) * (2  + (drawHough ? 2 : 0)) + 1;
		current = 0;
		
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

		++current;
		contentPane.updateUI();
		
		// draw the circles
		for (int y = 0; y < pic.height(); ++y) {
			for (int x = 0; x < pic.width(); ++x) {
				for (int i = 0; i < numRadiusBlocks; ++i) {
					if (pic.get(x, y).getRed() < 255 * percOfEdge) {
						int rad = minCircles + i;
						DrawCircle(x, y, rad, images[i]);
					}
					++current;
				}
			}
			contentPane.updateUI();
		}

		// normalise
		if (drawHough) {
			for (int y = 0; y < pic.height(); ++y) {
				for (int x = 0; x < pic.width(); ++x) {
					for (int i = 0; i < numRadiusBlocks; ++i) {
						max[i] = Math.max(max[i], images[i][x][y]);
						++current;
					}
				}
				contentPane.updateUI();
			}
			for (int y = 0; y < pic.height(); ++y) {
				for (int x = 0; x < pic.width(); ++x) {
					for (int i = 0; i < numRadiusBlocks; ++i) {
						int col = images[i][x][y];
						col = (int) ((col / (float) max[i]) * 255f);

						col = Math.max(col,
								new Color(out.image.getRGB(x, y)).getRed());

						out.image.setRGB(x, y, new Color(col, 0, 0).getRGB());
						++current;
					}
				}
				contentPane.updateUI();
			}
		}
		// ///// normalized

		Graphics2D g = out.image.createGraphics();
		g.setColor(Color.white);
		g.setStroke(new BasicStroke(1));
		for (int y = 0; y < pic.height(); ++y) {
			for (int x = 0; x < pic.width(); ++x) {
				for (int i = 0; i < numRadiusBlocks; ++i) {
					int r = minCircles + i;
					int d = r * 2;
					int check = (int) ((Math.PI * 2 * r) * percentOfCirc);
					int col = images[i][x][y];
					if (col > check) {
						if (maxima(x, y, images[i])) {
							if (checkSurroundings(x, y, center)) {
								center[x][y] = true;
								Ellipse2D e = new Ellipse2D.Double(x - r,
										y - r, d, d);
								g.draw(e);
								g.drawString("" + r, x - 5, y + 5);
							}
						}
					}
					++current;
				}
			}
			contentPane.updateUI();
		}
		contentPane.updateUI();
		return out;
	}

	public static boolean checkSurroundings(int x, int y, boolean center[][]) {
		for (int i = -2; i <= 2; ++i)
			for (int j = -2; j <= 2; ++j) {
				if (getValue(x + i, y + j, center) == true)
					return false;
			}
		return true;
	}

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

	public static boolean getValue(int x, int y, boolean[][] image) {
		if (x < 0 || y < 0 || x >= image.length || y >= image[0].length)
			return false;
		return image[x][y];
	}

	public static int getValue(int x, int y, int[][] image) {
		if (x < 0 || y < 0 || x >= image.length || y >= image[0].length)
			return -1;
		return image[x][y];
	}

	public static void DrawPixel(int x, int y, int[][] image) {
		if (x < 0 || y < 0 || x >= image.length || y >= image[0].length)
			return;

		image[x][y] += 1;
	}

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
