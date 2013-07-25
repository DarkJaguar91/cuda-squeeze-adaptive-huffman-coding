package ImageProcessing;

import java.awt.Color;
import java.awt.Graphics2D;

public class ImageProcesses {
	public static Picture createEdgeImage(Picture pic, String out) {
		int[][] edgeFilterX = { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } };
		int[][] edgeFilterY = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };

		Picture p = ImageMethods.runDualBandFilter(pic, out, edgeFilterX, edgeFilterY);

		return p;
	}

	public static void findCircles(Picture pic) {
		int[][][] images = new int[50][pic.width()][pic.height()];

		for (int y = 0; y < pic.height(); ++y) {
			for (int x = 0; x < pic.width(); ++x) {
				boolean picVal = pic.get(x, y).getRed() > 255 * 0.5f;

				if (picVal)
					for (int radius = 1; radius < 51; ++radius) {
						int xc = radius, yc = 0;
						int radiusError = 1 - x;
						int currentBox = radius - 1;

						while (xc >= yc) {
							if (xc + x < pic.width() && yc + y < pic.height())
								images[currentBox][xc + x][yc + y] += 1;

							if (yc + x < pic.width() && xc + y < pic.height())
								images[currentBox][yc + x][xc + y] += 1;

							if (-xc + x < pic.width() && yc + y < pic.height()
									&& -xc + x >= 0 && yc + y >= 0)
								images[currentBox][-xc + x][yc + y] += 1;

							if (-yc + x < pic.width() && xc + y < pic.height()
									&& -yc + x >= 0 && xc + y >= 0)
								images[currentBox][-yc + x][xc + y] += 1;

							if (-xc + x < pic.width() && -yc + y < pic.height()
									&& -xc + x >= 0 && -yc + y >= 0)
								images[currentBox][-xc + x][-yc + y] += 1;

							if (-yc + x < pic.width() && -xc + y < pic.height()
									&& -yc + x >= 0 && -xc + y >= 0)
								images[currentBox][-yc + x][-xc + y] += 1;

							if (xc + x < pic.width() && -yc + y < pic.height()
									&& xc + x >= 0 && -yc + y >= 0)
								images[currentBox][xc + x][-yc + y] += 1;

							if (yc + x < pic.width() && -xc + y < pic.height()
									&& yc + x >= 0 && -xc + y >= 0)
								images[currentBox][yc + x][-xc + y] += 1;

							yc++;
							if (radiusError < 0)
								radiusError += 2 * yc + 1;
							else {
								xc--;
								radiusError += 2 * (yc - xc + 1);
							}
						}
					}
			}
		}// end of circle creation

		Graphics2D g = pic.image.createGraphics();
		g.setColor(Color.RED);
		for (int i = 0; i < 50; ++i) {
			for (int y = 0; y < pic.height(); ++y) {
				for (int x = 0; x < pic.width(); ++x) {
					int current = images[i][x][y];

					boolean maximum = true;
					for (int xc = x == 0 ? 0 : -1; xc < (x == pic.width() - 1 ? 0
							: 1); ++xc) {
						for (int yc = y == 0 ? 0 : -1; yc < (y == pic.height() - 1 ? 0
								: 1); ++yc) {
							if (images[i][x + xc][y + yc] >= current) {
								maximum = false;
								break;
							}
						}
					}

					if (maximum) {
						if (current > (i + 1) * 4)
							g.fillOval(x, y, i + 1, i + 1);
					}
				}
			}
		}

	}
}
