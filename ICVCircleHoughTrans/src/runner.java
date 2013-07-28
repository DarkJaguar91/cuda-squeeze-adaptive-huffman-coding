import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.InputEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import javax.swing.BorderFactory;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;

import ImageProcessing.ImageProcesses;
import ImageProcessing.Picture;

public class runner {
	static JPanel panel;
	static Picture pic1, pic2, pic3;
	static JPopupMenu menu;
	static Picture currentPic = null;
	static JFileChooser chooser = null;
	
	public static void main(String [] args){
		JFrame f = new JFrame("Circle Finder");
		f.setSize(1250, 750);
		f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		f.setLayout(new BorderLayout());
		
		JMenuBar bar = new JMenuBar();
		JMenuItem itm = new JMenuItem("Load Image File");
		JMenuItem itm2 = new JMenuItem("Load Image from URL");
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
//				loadImage();
			}
		});
		itm2.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				loadImageURL();
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
		
		panel = new JPanel();
		panel.setLayout(new GridLayout(1, 3));
		
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
				if (e.getModifiers() == InputEvent.BUTTON3_MASK){
					Picture p = (Picture) e.getSource();
					currentPic = p;
					menu.show(e.getComponent(), e.getX(), e.getY());
				}
			}
		};
		
		JPanel p1 = new JPanel();
		JPanel p2 = new JPanel();
		JPanel p3 = new JPanel();
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
		
		pic1.addMouseListener(listen);
		pic2.addMouseListener(listen);
		pic3.addMouseListener(listen);
		
		f.setJMenuBar(bar);
		f.getContentPane().add(panel);
		
		f.setVisible(true);
		chooser = new JFileChooser();
	}
	
	public static void saveImage(Picture p){
		int ans = chooser.showSaveDialog(null);
		
		if (ans == JFileChooser.APPROVE_OPTION){		
			p.save(chooser.getSelectedFile().getAbsolutePath());			
		}
	}
	
	public static void loadImage(){
		int ans = chooser.showOpenDialog(null);
		
		if (ans == JFileChooser.APPROVE_OPTION){
			pic1.setImage(chooser.getSelectedFile().getAbsolutePath());
			panel.updateUI();
			pic2.setImage(ImageProcesses.createEdgeImage(pic1, "out.jpg"));
			panel.updateUI();
			pic3.setImage(ImageProcesses.findCircles(pic2));
			panel.updateUI();
			System.out.println("Done");
		}
	}

	public static void loadImageURL(){
		String ans = JOptionPane.showInputDialog("URL to image"); 
		
		if (ans != null){
			pic1.setImage(ans);
			panel.updateUI();
			pic2.setImage(ImageProcesses.createEdgeImage(pic1, "out.jpg"));
			panel.updateUI();
		}
	}	
}