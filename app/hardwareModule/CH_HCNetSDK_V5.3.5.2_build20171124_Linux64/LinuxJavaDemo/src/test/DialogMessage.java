package test;

import java.awt.BorderLayout;
import java.awt.FlowLayout;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;
import javax.swing.JLabel;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import javax.swing.ImageIcon;
import java.awt.Color;

public class DialogMessage extends JDialog
{

	private final JPanel contentPanel = new JPanel();
	private JLabel InfoText;

	

	/**
	 * Create the dialog.
	 */
	public DialogMessage(String str)
	{
		setTitle("Infomation");
		InitComponent();
		InfoText.setText(str);
		setModal(true);
	}
	
	public void InitComponent()
	{
		setBounds(100, 100, 413, 207);
		getContentPane().setLayout(new BorderLayout());
		contentPanel.setBorder(new EmptyBorder(5, 5, 5, 5));
		getContentPane().add(contentPanel, BorderLayout.CENTER);
		contentPanel.setLayout(null);
		
		JPanel panel = new JPanel();
		panel.setBounds(10, 10, 377, 149);
		contentPanel.add(panel);
		panel.setLayout(null);
		
		InfoText = new JLabel("");
		InfoText.setBounds(62, 30, 305, 47);
		panel.add(InfoText);
		
		JButton btnNewButton = new JButton("Sure");
		btnNewButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent paramActionEvent) {
				actionPerformedOK(paramActionEvent);
				
			}
		});
		btnNewButton.setBounds(118, 87, 112, 39);
		panel.add(btnNewButton);
		
		JLabel lblNewLabel = new JLabel("INFO:");
		lblNewLabel.setBackground(Color.RED);
		lblNewLabel.setForeground(Color.RED);
		lblNewLabel.setBounds(27, 41, 49, 24);
		panel.add(lblNewLabel);
		{
			JPanel buttonPane = new JPanel();
			buttonPane.setLayout(new FlowLayout(FlowLayout.RIGHT));
			getContentPane().add(buttonPane, BorderLayout.SOUTH);
		}
	}
	
	public void actionPerformedOK(ActionEvent event)
	{
		this.dispose();
	}
}
