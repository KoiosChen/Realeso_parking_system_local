package test;

import java.awt.BorderLayout;
import java.awt.FlowLayout;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;

import com.sun.jna.NativeLong;


import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JComboBox;
import javax.swing.border.EtchedBorder;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

public class DialogCruiseRoute extends JDialog
{

	private final JPanel contentPanel = new JPanel();
	private JComboBox comboBoxCruiseRoute;
	private JComboBox comboBoxCruisePoint;
	private JComboBox comboBoxPresetPoint;
	private JComboBox comboBoxTime;
	private JComboBox comboBoxSpeed;
	
    public static final int MAX_CRUISE_ROUTE = 32;
    public static final int MAX_CRUISE_POINT = 32;
    public static final int MAX_CRUISE_PRESET = 255;
    public static final int MAX_CRUISE_TIME = 255;
    public static final int MAX_CRUISE_SPEED = 40;
    
    static HCNetSDK hCNetSDK = HCNetSDK.INSTANCE;
    private NativeLong m_lRealHandle;


	/**
	 * Create the dialog.
	 */
	public DialogCruiseRoute(NativeLong lRealHandle)
	{
		super();
		InitComponents();
		InitDialog();
		m_lRealHandle = lRealHandle;
		setModal(true);
	}
	
	public void InitComponents()
	{
		setTitle("Set CruiseRoute");
		setBounds(100, 100, 450, 236);
		getContentPane().setLayout(new BorderLayout());
		contentPanel.setBorder(new EmptyBorder(5, 5, 5, 5));
		getContentPane().add(contentPanel, BorderLayout.CENTER);
		contentPanel.setLayout(null);
		{
			JLabel lblNewLabel = new JLabel("CruiseRoute");
			lblNewLabel.setBounds(26, 24, 54, 15);
			contentPanel.add(lblNewLabel);
		}
		{
			comboBoxCruiseRoute = new JComboBox();
			comboBoxCruiseRoute.setBounds(90, 21, 92, 21);
			contentPanel.add(comboBoxCruiseRoute);
		}
		{
			JLabel lblNewLabel_1 = new JLabel("Cruise Point");
			lblNewLabel_1.setBounds(26, 71, 54, 15);
			contentPanel.add(lblNewLabel_1);
		}
		{
			comboBoxCruisePoint = new JComboBox();
			comboBoxCruisePoint.setBounds(75, 65, 41, 21);
			contentPanel.add(comboBoxCruisePoint);
		}
		{
			JLabel label = new JLabel("Preset Point");
			label.setBounds(122, 71, 54, 15);
			contentPanel.add(label);
		}
		{
			comboBoxPresetPoint = new JComboBox();
			comboBoxPresetPoint.setBounds(168, 65, 41, 21);
			contentPanel.add(comboBoxPresetPoint);
		}
		{
			JLabel label = new JLabel("Time");
			label.setBounds(216, 71, 54, 15);
			contentPanel.add(label);
		}
		{
			comboBoxTime = new JComboBox();
			comboBoxTime.setBounds(265, 65, 41, 21);
			contentPanel.add(comboBoxTime);
		}
		{
			JLabel label = new JLabel("Speed");
			label.setBounds(312, 71, 54, 15);
			contentPanel.add(label);
		}
		{
			comboBoxSpeed = new JComboBox();
			comboBoxSpeed.setBounds(358, 65, 41, 21);
			contentPanel.add(comboBoxSpeed);
		}
		{
			JButton btnNewButtonAdd = new JButton("Add CruisePoint");
			btnNewButtonAdd.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					AddPointActionPerformed(e);
				}
			});
			btnNewButtonAdd.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
			btnNewButtonAdd.setBounds(26, 130, 93, 23);
			contentPanel.add(btnNewButtonAdd);
		}
		{
			JButton buttonDel = new JButton("Delete CruisePoint");
			buttonDel.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					DelPointActionPerformed(e);
				}
			});
			buttonDel.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
			buttonDel.setBounds(159, 130, 93, 23);
			contentPanel.add(buttonDel);
		}
		{
			JButton buttonExit = new JButton("Exit");
			buttonExit.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					ExitActionPerformed(e);
				}
			});
			buttonExit.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
			buttonExit.setBounds(286, 130, 93, 23);
			contentPanel.add(buttonExit);
		}
		{
			JPanel buttonPane = new JPanel();
			buttonPane.setLayout(new FlowLayout(FlowLayout.RIGHT));
			getContentPane().add(buttonPane, BorderLayout.SOUTH);
		}
	}
	
	public void InitDialog()
	{
		int i=0;
		for(i = 0; i< MAX_CRUISE_ROUTE; i++)
		{
			comboBoxCruiseRoute.addItem("CruiseRoute"+(i+1));
		}		
		for(i = 0; i < MAX_CRUISE_POINT; i++)
		{
			comboBoxCruisePoint.addItem(i+1);
		}
		for(i = 0;i<MAX_CRUISE_PRESET;i++)
		{
			comboBoxPresetPoint.addItem(i+1);
		}
		for(i = 0;i<MAX_CRUISE_SPEED;i++)
		{
			comboBoxSpeed.addItem(i+1);
		}
		for(i=0;i<MAX_CRUISE_TIME;i++)
		{
			comboBoxTime.addItem(i+1);
		}
	}
	
	//add cruite point
	public void AddPointActionPerformed(ActionEvent event)
	{
		byte byCruiseRoute = (byte)(comboBoxCruiseRoute.getSelectedIndex()+1);
		byte byCruisePoint = (byte)(comboBoxPresetPoint.getSelectedIndex()+1);
		short sPresetPoint = (short)(comboBoxPresetPoint.getSelectedIndex()+1);
		short sTime = (short)(comboBoxTime.getSelectedIndex()+1);
		short sSpeed = (short)(comboBoxSpeed.getSelectedIndex()+1);
		boolean bRet = false;
		bRet = hCNetSDK.NET_DVR_PTZCruise(m_lRealHandle,HCNetSDK.FILL_PRE_SEQ,byCruiseRoute,byCruisePoint,sPresetPoint);
		if(bRet)
		{
			bRet = hCNetSDK.NET_DVR_PTZCruise(m_lRealHandle,HCNetSDK.SET_SEQ_DWELL,byCruiseRoute,byCruisePoint,sTime);
			if(bRet)
			{
				bRet = hCNetSDK.NET_DVR_PTZCruise(m_lRealHandle,HCNetSDK.SET_SEQ_SPEED,byCruiseRoute,byCruisePoint,sSpeed);
			}
		}
		if(!bRet)
		{
			DialogMessage dlg = new DialogMessage("add fail,error code:"+hCNetSDK.NET_DVR_GetLastError());
			dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
			dlg.setVisible(true);
			return;
		}
	}
	
	//delete cruise route point
	public void DelPointActionPerformed(ActionEvent event)
	{
		byte byCruiseRoute = (byte)(comboBoxCruiseRoute.getSelectedIndex()+1);
		byte byCruisePoint = (byte)(comboBoxPresetPoint.getSelectedIndex()+1);
		short sPresetPoint = (short)(comboBoxPresetPoint.getSelectedIndex()+1);
		if(!hCNetSDK.NET_DVR_PTZCruise(m_lRealHandle,HCNetSDK.CLE_PRE_SEQ,byCruiseRoute,byCruisePoint,sPresetPoint))
		{
			DialogMessage dlg = new DialogMessage("delete fail,error code:"+hCNetSDK.NET_DVR_GetLastError());
			dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
			dlg.setVisible(true);
			return;
		}
	}
	
	//exit
	public void ExitActionPerformed(ActionEvent e)
	{
		this.dispose();
	}

}
