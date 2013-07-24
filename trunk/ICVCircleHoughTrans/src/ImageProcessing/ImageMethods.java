package ImageProcessing;

import java.awt.Color;

public class ImageMethods {
	public static int [][] xEdgeFilter = {{-1, 0, 1},
						 				  {-2, 0, 2},
						 				  {-1, 0, 1}};
	public static int [][] yEdgeFilter = {{1, 2, 1},
		 								  {0, 0, 0},
		 								  {-1, -2, -1}};		
			
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
}
