package ImageProcessing;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Toolkit;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.net.URL;

import javax.imageio.ImageIO;
import javax.swing.JLabel;
import javax.swing.JOptionPane;

public class Picture extends JLabel {
	/**
	 * Serial ID
	 */
	private static final long serialVersionUID = -2298002615548555251L;

	private static final String regexImage = ".*[jpg|png|gif]$";

	public BufferedImage image;
	private String filename;
	private int width, height;

	public Picture(String path, int w, int h) {
		super();
		width = w;
		height = h;
		image = new BufferedImage(w, h, BufferedImage.TYPE_INT_RGB);
		if (!path.matches(regexImage))
			path += ".jpg";
		filename = path;
	}

	public Picture(Picture pic) {
		super();
		width = pic.width();
		height = pic.height();
		image = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
		filename = pic.filename;
		for (int i = 0; i < width(); i++)
			for (int j = 0; j < height(); j++)
				image.setRGB(i, j, pic.get(i, j).getRGB());
	}

	public Picture(String filename) {
		this.filename = filename;
		try {
			// try to read from file in working directory
			File file = new File(filename);
			if (file.isFile()) {
				image = ImageIO.read(file);
			}

			// now try to read from file in same directory as this .class file
			else {
				URL url = getClass().getResource(filename);
				if (url == null) {
					url = new URL(filename);
				}
				image = ImageIO.read(url);
			}
			width = image.getWidth(null);
			height = image.getHeight(null);
		} catch (IOException e) {
			JOptionPane
			.showMessageDialog(
					this,
					"Could not load image.\nEither the file selected was not an image file.\nor\nThe image file failed to download",
					"Error", JOptionPane.ERROR_MESSAGE);
		}
	}

	public void setImage(Picture pic) {
		width = pic.width();
		height = pic.height();
		image = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
		filename = pic.filename;
		for (int i = 0; i < width(); i++)
			for (int j = 0; j < height(); j++)
				image.setRGB(i, j, pic.get(i, j).getRGB());
	}

	public void setImage(String filename) {
		this.filename = filename;
		try {
			// try to read from file in working directory
			File file = new File(filename);
			if (file.isFile()) {
				image = ImageIO.read(file);
			}

			// now try to read from file in same directory as this .class file
			else {
				URL url = getClass().getResource(filename);
				if (url == null) {
					url = new URL(filename);
				}
				image = ImageIO.read(url);
			}
			width = image.getWidth(null);
			height = image.getHeight(null);
		} catch (IOException e) {
			JOptionPane
			.showMessageDialog(
					this,
					"Could not load image.\nEither the file selected was not an image file.\nor\nThe image file failed to download",
					"Error", JOptionPane.ERROR_MESSAGE);
		}
	}

	public String getFileName(){
		return filename;
	}
	
	public void save() {
		if (!filename.matches(regexImage)) {
			filename += ".jpg";
		}
		String suffix = filename.substring(filename.lastIndexOf('.') + 1);
		suffix = suffix.toLowerCase();
		File file = new File(filename);
		try {
			ImageIO.write(image, suffix, file);
		} catch (IOException e) {
			JOptionPane
			.showMessageDialog(
					this,
					"Failed to save image.\nplease make sure you have rights to the destination folder.",
					"Error", JOptionPane.ERROR_MESSAGE);
		}
	}

	public void save(String filePath) {
		this.filename = filePath;
		if (!filename.matches("[.jpg|.gif|.png|.tif]$")) {
			filename += ".jpg";
		}
		String suffix = filename.substring(filename.lastIndexOf('.') + 1);
		suffix = suffix.toLowerCase();
		File file = new File(filename);
		try {
			ImageIO.write(image, suffix, file);
		} catch (IOException e) {
			JOptionPane
			.showMessageDialog(
					this,
					"Failed to save image.\nplease make sure you have rights to the destination folder.",
					"Error", JOptionPane.ERROR_MESSAGE);
		}
	}

	public void paint(Graphics g) {
		super.paint(g);
		if (image != null){
			Dimension size = getSizeToFit(new Dimension(image.getWidth(), image.getHeight()), this.getSize());
			int x = (this.getWidth() - size.width) / 2;
			int y = (this.getHeight() - size.height) / 2;
			g.drawImage(
					Toolkit.getDefaultToolkit().createImage(image.getSource()),
					x, y, size.width, size.height, this);
		}
	}

	public Color get(int x, int y) {
		if (x < 0)
			x = this.width + x;
		if (y < 0)
			y = this.height + y;
		if (x >= this.width)
			x = this.width - x;
		if (y >= this.height)
			y = this.height - y;
		return new Color(image.getRGB(x, y));
	}

	public void set(int x, int y, Color c) {
		image.setRGB(x, y, c.getRGB());
	}

	public int height() {
		return height;
	}

	public int width() {
		return width;
	}

	Dimension getSizeToFit(Dimension original, Dimension toFit) {
        double factor = getScaleFactorToFit(original, toFit);
        Dimension size = new Dimension(original);
        size.width *= factor;
        size.height *= factor;
        return size;
    }
	
	double getScaleFactorToFit(Dimension original, Dimension toFit) {
        double dScale = 1d;
        if (original != null && toFit != null) {
            double dScaleWidth = getScaleFactor(original.width, toFit.width);
            double dScaleHeight = getScaleFactor(original.height, toFit.height);
            dScale = Math.min(dScaleHeight, dScaleWidth);
        }
        return dScale;
    }
	
	 double getScaleFactor(int iMasterSize, int iTargetSize) {
         return (double) iTargetSize / (double) iMasterSize;
     }
	
}
