package ImageProcessing;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Composite;
import java.awt.CompositeContext;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.geom.Ellipse2D;
import java.awt.image.ColorModel;
import java.awt.image.DataBuffer;
import java.awt.image.Raster;
import java.awt.image.WritableRaster;

public class ImageProcesses {
	public static Picture createEdgeImage(Picture pic, String out) {
		int[][] edgeFilterX = { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } };
		int[][] edgeFilterY = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };

		Picture p = ImageMethods.runDualBandFilter(pic, out, edgeFilterX,
				edgeFilterY);

		return p;
	}

	public static Picture findCircles(Picture pic) {
		Picture out = new Picture("Circles.jpg", pic.width(), pic.height());

		int numRadiusBlocks = 50;
		
		// create the radius images
		int[][][] images = new int[numRadiusBlocks][pic.width()][pic.height()];
		int[][] image = new int[pic.width()][pic.height()];
		int max = Integer.MIN_VALUE;
		for (int y = 0; y < pic.height(); ++y)
			for (int x = 0; x < pic.width(); ++x) {
				image[x][y] = 0;
				for (int i = 0; i < numRadiusBlocks; ++i) {
					images[i][x][y] = 0;
				}
			}

		// draw the circles
		for (int y = 0; y < pic.height(); ++y) {
			for (int x = 0; x < pic.width(); ++x) {
				for (int i = 0; i < numRadiusBlocks; ++i) {
					if (pic.get(x, y).getRed() < 255 * 0.9f) {
						int rad = 10 + i;
						DrawCircle(x, y, rad, image);
						DrawCircle(x, y, rad, images[i]);
					}
				}
			}
		}

		// normalise
		for (int y = 0; y < pic.height(); ++y) {
			for (int x = 0; x < pic.width(); ++x) {
				max = Math.max(max, image[x][y]);
			}
		}
		for (int y = 0; y < pic.height(); ++y) {
			for (int x = 0; x < pic.width(); ++x) {
				int col = image[x][y];
				col = (int) ((col / (float) max) * 220f);
				out.image.setRGB(x, y, new Color(col, 0, 0).getRGB());
			}
		}
		// ///// normalized

		Graphics2D g = out.image.createGraphics();
		g.setColor(Color.white);
		g.setStroke(new BasicStroke(1));
		for (int y = 0; y < pic.height(); ++y) {
			for (int x = 0; x < pic.width(); ++x) {
				for (int i = 0; i < numRadiusBlocks; ++i) {
					int r = 10 + i;
					int d = r * 2;
					int check = (int) ((Math.PI * 2 * r) * 0.9f);
					int col = images[i][x][y];
					if (col > check) {
						if (maxima(x, y, images[i])) {
							Ellipse2D e = new Ellipse2D.Double(x - r, y - r, d,
									d);
							g.draw(e);
						}
					}
				}
			}
		}

		return out;
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
