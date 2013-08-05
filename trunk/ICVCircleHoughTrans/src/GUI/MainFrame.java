package GUI;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GridLayout;
import java.awt.Toolkit;
import java.awt.datatransfer.DataFlavor;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.InputEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JSlider;
import javax.swing.JSpinner;
import javax.swing.SpinnerNumberModel;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import ImageProcessing.ImageProcesses;
import ImageProcessing.Picture;

/**
 * 
 * @author Brandon James Talbot
 *
 * Main frame that contains all widgets and processing methods
 */
public class MainFrame extends JFrame {
	/**
	 * Serial ID
	 */
	private static final long serialVersionUID = 6987177916802281720L;
	
	// Global vars
	JPanel panel;
	Picture pic1, pic2, pic3;
	JPopupMenu menu;
	Picture currentPic = null;
	JFileChooser chooser = null;
	JSpinner minRadius;
	JSpinner maxRadius;
	JSlider circSlider;
	JSlider edgeSlider;
	JCheckBox drawHuphSpace;
	JButton runAgain;
	JMenuItem itm;
	JMenuItem itm2;
	public int total, current; // used for progress bar
	boolean drawProgress = false;
	
	/**
	 * Constructor
	 */
	public MainFrame(){
		// set frame details
		setIconImage(new ImageIcon("Images/icon.gif").getImage());
		setSize(1250, 750);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setLayout(new BorderLayout());

		// create menu bar
		JMenuBar bar = new JMenuBar();
		itm = new JMenuItem("Load Image File");
		itm2 = new JMenuItem("Load Image from URL");
		itm.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				Runnable tr = new Runnable() {

					@Override
					public void run() {
						loadImage();
					}
				};
				Thread t = new Thread(tr);
				t.start();
			}
		});
		itm2.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				Runnable tr = new Runnable() {

					@Override
					public void run() {
						loadImageURL();
					}
				};
				Thread t = new Thread(tr);
				t.start();
			}
		});
		bar.add(itm);
		bar.add(itm2);

		menu = new JPopupMenu();
		JMenuItem ritm = new JMenuItem("Save");
		ritm.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				saveImage(currentPic);
			}
		});
		menu.add(ritm);

		// picture panel
		panel = new JPanel();
		panel.setLayout(new GridLayout(1, 3));

		// picture jpanels
		pic1 = new Picture("", 10, 10);
		pic2 = new Picture("", 10, 10);
		pic3 = new Picture("", 10, 10);

		MouseListener listen = new MouseListener() {

			@Override
			public void mouseReleased(MouseEvent e) {
			}

			@Override
			public void mousePressed(MouseEvent e) {
			}

			@Override
			public void mouseExited(MouseEvent e) {
			}

			@Override
			public void mouseEntered(MouseEvent e) {
			}

			@Override
			public void mouseClicked(MouseEvent e) {
				// right click mask to save images
				if (e.getModifiers() == InputEvent.BUTTON3_MASK) {
					Picture p = (Picture) e.getSource();
					currentPic = p;
					menu.show(e.getComponent(), e.getX(), e.getY());
				}
			}
		};

		// Panels to hold jlabel images
		JPanel p1 = new JPanel();
		JPanel p2 = new JPanel();
		JPanel p3 = new JPanel() {
			/**
			 * serial ID
			 */
			private static final long serialVersionUID = 4050056765468843229L;

			public void paint(Graphics g) {
				if (drawProgress) { // if should draw the progress bar
					Graphics2D g2 = (Graphics2D) g;
					g2.setColor(Color.BLACK); // draw black background
					g2.fillRect(0, 0, this.getWidth(), this.getHeight());
					g2.setColor(Color.WHITE); // white outline and text
					int x = this.getWidth() / 4;
					int y = this.getHeight() / 2 - 5;
					int height = 10;
					int width = this.getWidth() / 2;
					g2.drawRect(x - 1, y - 1, width + 1, height + 1); // outline box

					double percen = current / (double) total;
					g2.drawString("Progress:", (int)(x + width * 0.25f), y - 10); // text
					g2.setColor(new Color((int)(255 - (220 * percen)), (int)(200 * percen), 0)); // interp color
					g2.fillRect( // draw rectangle
							x,
							y,
							(int) ((percen) * (double) width),
							height);
				} else {
					super.paint(g); // paint jlabel if not progressing
				}
			}
		};
		// add panel details (border etc)
		p1.setBorder(BorderFactory.createEtchedBorder());
		p2.setBorder(BorderFactory.createEtchedBorder());
		p3.setBorder(BorderFactory.createEtchedBorder());
		p1.setLayout(new BorderLayout());
		p2.setLayout(new BorderLayout());
		p3.setLayout(new BorderLayout());
		p1.add(pic1, BorderLayout.CENTER);
		p2.add(pic2, BorderLayout.CENTER);
		p3.add(pic3, BorderLayout.CENTER);

		panel.add(p1);
		panel.add(p2);
		panel.add(p3);

		// add the mouse listener
		pic1.addMouseListener(listen);
		pic2.addMouseListener(listen);
		pic3.addMouseListener(listen);

		// setting up the sliders
		minRadius = new JSpinner(new SpinnerNumberModel(5, 5, 60, 1));
		maxRadius = new JSpinner(new SpinnerNumberModel(60, 5, 500, 1));
		minRadius.addChangeListener(new ChangeListener() {
			@Override
			public void stateChanged(ChangeEvent e) {
				SpinnerNumberModel model = (SpinnerNumberModel) maxRadius
						.getModel();
				model.setMinimum((int) ((SpinnerNumberModel) minRadius
						.getModel()).getValue());
			}
		});
		maxRadius.addChangeListener(new ChangeListener() {
			@Override
			public void stateChanged(ChangeEvent e) {
				SpinnerNumberModel model = (SpinnerNumberModel) minRadius
						.getModel();
				model.setMaximum(((int) ((SpinnerNumberModel) maxRadius
						.getModel()).getValue()));
			}
		});

		circSlider = new JSlider(0, 100, 90);
		edgeSlider = new JSlider(0, 100, 20);
		setJSliderLabels(circSlider);
		setJSliderLabels(edgeSlider);

		// checkbox
		drawHuphSpace = new JCheckBox("Draw Hough Space?", false);

		JPanel Panel2 = new JPanel();
		Panel2.setLayout(new BorderLayout());

		JPanel holder = new JPanel();
		holder.setLayout(new GridLayout(1, 3));
		JPanel setter = new JPanel();
		setter.setLayout(new FlowLayout(FlowLayout.CENTER));
		setter.setBorder(BorderFactory.createEtchedBorder());
		setter.add(new JLabel("Min circle Radius:"));
		setter.add(minRadius);
		holder.add(setter);

		setter = new JPanel();
		setter.setLayout(new FlowLayout(FlowLayout.CENTER));
		setter.setBorder(BorderFactory.createEtchedBorder());
		setter.add(new JLabel("Max circle Radius:"));
		setter.add(maxRadius);
		holder.add(setter);

		setter = new JPanel();
		setter.setLayout(new FlowLayout(FlowLayout.CENTER));
		setter.setBorder(BorderFactory.createEtchedBorder());
		setter.add(drawHuphSpace);
		holder.add(setter);
		Panel2.add(holder, BorderLayout.NORTH);

		holder = new JPanel();
		holder.setLayout(new GridLayout(1, 2));
		setter = new JPanel();
		setter.setLayout(new GridLayout(2, 1));
		setter.setBorder(BorderFactory.createEtchedBorder());
		setter.add(new JLabel("Percentage of pixel intensity to look for:"));
		setter.add(edgeSlider);
		holder.add(setter);

		setter = new JPanel();
		setter.setLayout(new GridLayout(2, 1));
		setter.setBorder(BorderFactory.createEtchedBorder());
		setter.add(new JLabel("Percentage of circumferance to look for:"));
		setter.add(circSlider);
		Panel2.add(setter);
		holder.add(setter);
		Panel2.add(holder, BorderLayout.CENTER);

		holder = new JPanel();
		runAgain = new JButton("Run algorithm again");
		holder.add(runAgain);
		runAgain.setEnabled(false);
		runAgain.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				Runnable r = new Runnable() {

					@Override
					public void run() {
						runCircleAlgorithm();
					}
				};
				Thread t = new Thread(r);
				t.start();
			}
		});
		Panel2.add(holder, BorderLayout.SOUTH);

		setLayout(new BorderLayout(5, 10));
		setJMenuBar(bar);
		getContentPane().add(Panel2, BorderLayout.NORTH);
		getContentPane().add(panel, BorderLayout.CENTER);

		setVisible(true);
		// start file chooser
		chooser = new JFileChooser();
	}
	
	/**
	 * Setter for jslider labels (method to remove redundancy)
	 * @param slider Slider to add labels to
	 */
	public void setJSliderLabels(JSlider slider) {
		slider.setMinorTickSpacing(1);
		slider.setMajorTickSpacing(5);
		slider.setPaintTicks(true);
		slider.setPaintLabels(true);
	}

	/**
	 * Method to save an image to file
	 * @param p Picture object to save
	 */
	public void saveImage(Picture p) {
		int ans = chooser.showSaveDialog(null);

		if (ans == JFileChooser.APPROVE_OPTION) {
			p.save(chooser.getSelectedFile().getAbsolutePath());
		}
	}

	/**
	 * Method to load an image
	 * Uses jfilechooser
	 */
	public void loadImage() {
		int ans = chooser.showOpenDialog(null);

		if (ans == JFileChooser.APPROVE_OPTION) {
			pic1.setImage(chooser.getSelectedFile().getAbsolutePath());
			runTotalAlgorithm();
		}
	}

	/**
	 * Method to load a image from url
	 */
	public void loadImageURL() {
		String data = "";
		
		try {
			data = (String) Toolkit.getDefaultToolkit()
	                .getSystemClipboard().getData(DataFlavor.stringFlavor);
		}
		catch (Exception e){};
		 
		
		String ans = JOptionPane.showInputDialog("URL to image", data);

		if (ans != null) {
			pic1.setImage(ans);
			runTotalAlgorithm();
		}
	}

	/**
	 * Runs the entire algorithm, from edge detection to drawing the circles
	 */
	public void runTotalAlgorithm() {
		disable();
		panel.updateUI();
		pic2.setImage(ImageProcesses.createEdgeImage(pic1, "out.jpg"));
		panel.updateUI();
		pic3.setImage(ImageProcesses.findCircles(pic2,
				(int) (minRadius.getValue()), (int) (maxRadius.getValue()),
				edgeSlider.getValue() / 100f, circSlider.getValue() / 100f,
				drawHuphSpace.isSelected(), panel, this));
		enable();
		panel.updateUI();
	}

	/**
	 * Disables all buttons since the program uses threads 
	 */
	public void disable() {
		itm.setEnabled(false);
		itm2.setEnabled(false);
		runAgain.setEnabled(false);
		drawProgress = true;
	}

	/**
	 * re-enables the buttons for a re-run
	 */
	public void enable() {
		itm.setEnabled(true);
		itm2.setEnabled(true);
		runAgain.setEnabled(true);
		drawProgress = false;
	}

	/**
	 * Just runs the circle finding part of the algorithm (for a re-run)
	 */
	public void runCircleAlgorithm() {
		disable();
		panel.updateUI();
		pic3.setImage(ImageProcesses.findCircles(pic2,
				(int) (minRadius.getValue()), (int) (maxRadius.getValue()),
				edgeSlider.getValue() / 100f, circSlider.getValue() / 100f,
				drawHuphSpace.isSelected(), panel, this));
		enable();
		panel.updateUI();
	}
}
