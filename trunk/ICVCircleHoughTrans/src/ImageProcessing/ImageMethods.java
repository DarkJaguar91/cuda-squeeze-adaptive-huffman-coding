package ImageProcessing;

import java.awt.Color;

public class ImageMethods {			
    public static double lum(Color color) {
        int r = color.getRed();
        int g = color.getGreen();
        int b = color.getBlue();
        return .299*r + .587*g + .114*b;
    }

    public static int truncate(int a) {
        if      (a <   0) return 0;
        else if (a > 255) return 255;
        else              return a;
    }
    
    public static Color toGray(Color color) {
        int y = (int) (Math.round(lum(color)));   // round to nearest int
        Color gray = new Color(y, y, y);
        return gray;
    }

    public static boolean compatible(Color a, Color b) {
        return Math.abs(lum(a) - lum(b)) >= 128.0;
    }
    
    public static Picture runFilter(Picture input, String output, int [][] filter){
    	int height = input.height();
		int width = input.width();

		Picture out = new Picture(output, width, height);
		
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				// get colours
				int c1 = 0;
				for (int i = 0; i < 3; ++i)
					for (int j = 0; j < 3; ++j) {
						int lum = (int) ImageMethods.lum(input.get(x - 1 + i, y
								- 1 + j));
						c1 += lum * filter[i][j];
					}

				int mag = 255 - truncate(c1);
				out.set(x, y, new Color(mag, mag, mag));
			}
		}
		return out;
    }
    
    public static Picture runDualBandFilter(Picture input, String output, int [][] xFilter, int [][] yFilter){
    	int height = input.height();
		int width = input.width();

		Picture out = new Picture(output, width, height);
		
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				// get colours
				int c1 = 0, c2 = 0;
				for (int i = 0; i < 3; ++i)
					for (int j = 0; j < 3; ++j) {
						int lum = (int) ImageMethods.lum(input.get(x - 1 + i, y
								- 1 + j));
						c1 += lum * xFilter[i][j];
						c2 += lum * yFilter[i][j];
					}

				int mag = 255 - ImageMethods.truncate((int) Math.sqrt(c1 * c1
						+ c2 * c2));
				out.set(x, y, new Color(mag, mag, mag));
			}
		}
		return out;
    }
}
