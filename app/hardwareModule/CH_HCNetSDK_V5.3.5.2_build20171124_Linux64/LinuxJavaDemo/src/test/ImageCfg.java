package test;

import com.sun.jna.Native;
import com.sun.jna.NativeLong;
import com.sun.jna.Pointer;
import com.sun.jna.examples.win32.W32API.HWND;
import com.sun.jna.ptr.IntByReference;

import java.awt.Panel;
import java.awt.Color;
import javax.swing.JButton;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import javax.swing.JLabel;
import javax.swing.SwingConstants;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.JSlider;
import javax.swing.JTextField;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.DefaultComboBoxModel;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

public class ImageCfg extends JDialog {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	static HCNetSDK hCNetSDK = HCNetSDK.INSTANCE;
	private NativeLong m_lUserID;
	private int m_iChanNum;
	private HWND m_hWnd;
	private HCNetSDK.NET_DVR_CLIENTINFO m_sClientInfo = new HCNetSDK.NET_DVR_CLIENTINFO();	// play structure
	private NativeLong m_lRealPalyHandle = new NativeLong(-1);	// play handle
	private HCNetSDK.NET_DVR_ISP_CAMERAPARAMCFG m_struISPCameraparamCfg = new HCNetSDK.NET_DVR_ISP_CAMERAPARAMCFG();
	
	private JTextField m_textFieldBrightness;
	private JTextField m_textFieldContrast;
	private JTextField m_textFieldSharpness;
	private JTextField m_textFieldSaturation;
	private JSlider m_sliderBrightness;
	private JSlider m_sliderContrast;
	private JSlider m_sliderSharpness;
	private JSlider m_sliderSaturation;
	private JComboBox<String> m_cmbWhiteBalance;
	private String [] m_sWhiteBalance = {"MWB", "AWB1", "AWB2", "Locked WB", "Outdoor", "Indoor", "Daylight", "Sodiumlight", "Auto Track", "One Push", "Auto Outdoor", "Auto Sodiumlight", "Mercury Lamp", "Auto", "IncandescentLamp", "Warm Light Lamp", "Natural Light"};
	private JComboBox<String> m_cmbWDREnabled;
	private String [] m_sWDREnabled = {"Disable", "Enable", "Auto"};
	private JComboBox<String> m_cmbBacklightMode;
	private String [] m_sBacklightMode = {"Off", "UP", "DOWN", "LEFT", "RIGHT", "MIDDLE"};
	private JComboBox<String> m_cmbScenceMode;
	private String [] m_sScenceMode = {"Indoor", "Outdoor", "Default", "Low Light"};
	private JComboBox<String> m_cmbDefogMode;
	private String [] m_sDefogMode = {"Disable", "Auto", "Normally Open"};
	private JComboBox<String> m_cmbElectronicstAblization;
	private String [] m_sElectronicstAblization = {"Disable", "Enable"};
	private JComboBox<String> m_cmbCorridorMode;
	private String [] m_sCorridorMode = {"Disable", "Enable"};
	private JComboBox<String> m_cmbMirror;
	private String [] m_sMirror = {"Close", "Left and Right", "Up and Down", "Middle"};
	
	/**
	 * Create the panel.
	 */
	public ImageCfg(NativeLong lUserID, int iChanNum) {
		final Panel m_panelPlay = new Panel();
		m_panelPlay.setBackground(Color.DARK_GRAY);
		m_panelPlay.setBounds(10, 10, 373, 313);
		getContentPane().add(m_panelPlay);
		
		this.addWindowListener(new WindowAdapter()
        {
           @Override
            public void windowClosing(WindowEvent e)
            {
        	   long lRealPalyHandle = m_lRealPalyHandle.longValue();
        	   if (lRealPalyHandle > 0)
				{
					hCNetSDK.NET_DVR_StopRealPlay(m_lRealPalyHandle);
				}
            }
        });
		
		addComponentListener(new ComponentAdapter() {
			@Override
			public void componentShown(ComponentEvent arg0) {
				m_hWnd = new HWND(Native.getComponentPointer(m_panelPlay));
				m_sClientInfo.hPlayWnd = m_hWnd;
				m_sClientInfo.lChannel = new NativeLong(m_iChanNum);
				m_lRealPalyHandle = hCNetSDK.NET_DVR_RealPlay_V30(m_lUserID, m_sClientInfo, null, null, true);
				long lRealPalyHandle = m_lRealPalyHandle.longValue();
				if (-1 == lRealPalyHandle)
				{
					int iErr = hCNetSDK.NET_DVR_GetLastError();
					DialogMessage dlg = new DialogMessage("start preview failed,error code is:"+iErr);
					dlg.setBounds(dlg.getX()+dlg.getWidth()/2,dlg.getY()+dlg.getHeight() , 370, 200);
					dlg.setVisible(true);
					return;
				}
			}
		});
		setTitle("Image Configure");
		InitImageCfg();
		getContentPane().setLayout(null);
		m_lUserID = lUserID;
		m_iChanNum = iChanNum;
		
		JLabel lblBrightnessLevel = new JLabel("Brightness Level");
		lblBrightnessLevel.setHorizontalAlignment(SwingConstants.RIGHT);
		lblBrightnessLevel.setBounds(389, 24, 131, 15);
		getContentPane().add(lblBrightnessLevel);
		
		JLabel lblContrastLevel = new JLabel("Contrast Level");
		lblContrastLevel.setHorizontalAlignment(SwingConstants.RIGHT);
		lblContrastLevel.setBounds(389, 54, 131, 15);
		getContentPane().add(lblContrastLevel);
		
		JLabel lblSharpnessLevel = new JLabel("Sharpness Level");
		lblSharpnessLevel.setHorizontalAlignment(SwingConstants.RIGHT);
		lblSharpnessLevel.setBounds(389, 84, 131, 15);
		getContentPane().add(lblSharpnessLevel);
		
		JLabel lblSaturationLevel = new JLabel("Saturation Level");
		lblSaturationLevel.setHorizontalAlignment(SwingConstants.RIGHT);
		lblSaturationLevel.setBounds(389, 114, 131, 15);
		getContentPane().add(lblSaturationLevel);
		
		JLabel lblWhiteBalance = new JLabel("White Balance");
		lblWhiteBalance.setHorizontalAlignment(SwingConstants.RIGHT);
		lblWhiteBalance.setBounds(389, 144, 131, 15);
		getContentPane().add(lblWhiteBalance);
		
		JLabel lblWDREnabled = new JLabel("WDR Enabled");
		lblWDREnabled.setHorizontalAlignment(SwingConstants.RIGHT);
		lblWDREnabled.setBounds(389, 174, 131, 15);
		getContentPane().add(lblWDREnabled);
		
		JLabel lblBacklightMode = new JLabel("Backlight Mode");
		lblBacklightMode.setHorizontalAlignment(SwingConstants.RIGHT);
		lblBacklightMode.setBounds(389, 204, 131, 15);
		getContentPane().add(lblBacklightMode);
		
		JLabel lblScenceMode = new JLabel("Scence Mode");
		lblScenceMode.setHorizontalAlignment(SwingConstants.RIGHT);
		lblScenceMode.setBounds(389, 234, 131, 15);
		getContentPane().add(lblScenceMode);
		
		JLabel lblDefogMode = new JLabel("Defog Mode");
		lblDefogMode.setHorizontalAlignment(SwingConstants.RIGHT);
		lblDefogMode.setBounds(389, 264, 131, 15);
		getContentPane().add(lblDefogMode);
		
		JLabel lblElectronicstAblization = new JLabel("Electronicst Ablization");
		lblElectronicstAblization.setHorizontalAlignment(SwingConstants.RIGHT);
		lblElectronicstAblization.setBounds(389, 294, 131, 15);
		getContentPane().add(lblElectronicstAblization);
		
		JLabel lblCorridorMode = new JLabel("Corridor Mode");
		lblCorridorMode.setHorizontalAlignment(SwingConstants.RIGHT);
		lblCorridorMode.setBounds(389, 324, 131, 15);
		getContentPane().add(lblCorridorMode);
		
		JLabel lblMirror = new JLabel("Mirror");
		lblMirror.setHorizontalAlignment(SwingConstants.RIGHT);
		lblMirror.setBounds(389, 354, 131, 15);
		getContentPane().add(lblMirror);
		
		m_sliderBrightness = new JSlider();
		m_sliderBrightness.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				if ((JSlider)e.getSource() == m_sliderBrightness)
				{
					m_textFieldBrightness.setText(""+m_sliderBrightness.getValue());
				}
			}
		});
		m_sliderBrightness.setBounds(530, 24, 177, 23);
		getContentPane().add(m_sliderBrightness);
		
		m_sliderContrast = new JSlider();
		m_sliderContrast.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				if ((JSlider)e.getSource() == m_sliderContrast)
				{
					m_textFieldContrast.setText(""+m_sliderContrast.getValue());
				}
			}
		});
		m_sliderContrast.setBounds(530, 54, 177, 23);
		getContentPane().add(m_sliderContrast);
		
		m_sliderSharpness = new JSlider();
		m_sliderSharpness.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				if ((JSlider)e.getSource() == m_sliderSharpness)
				{
					m_textFieldSharpness.setText(""+m_sliderSharpness.getValue());
				}
			}
		});
		m_sliderSharpness.setBounds(530, 84, 177, 23);
		getContentPane().add(m_sliderSharpness);
		
		m_sliderSaturation = new JSlider();
		m_sliderSaturation.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				if ((JSlider)e.getSource() == m_sliderSaturation)
				{
					m_textFieldSaturation.setText(""+m_sliderSaturation.getValue());
				}
			}
		});
		m_sliderSaturation.setBounds(530, 114, 177, 23);
		getContentPane().add(m_sliderSaturation);
		
		m_textFieldBrightness = new JTextField();
		m_textFieldBrightness.setText("50");
		m_textFieldBrightness.setHorizontalAlignment(SwingConstants.CENTER);
		m_textFieldBrightness.setEditable(false);
		m_textFieldBrightness.setColumns(10);
		m_textFieldBrightness.setBounds(708, 24, 32, 21);
		getContentPane().add(m_textFieldBrightness);
		
		m_textFieldContrast = new JTextField();
		m_textFieldContrast.setText("50");
		m_textFieldContrast.setHorizontalAlignment(SwingConstants.CENTER);
		m_textFieldContrast.setEditable(false);
		m_textFieldContrast.setColumns(10);
		m_textFieldContrast.setBounds(708, 54, 32, 21);
		getContentPane().add(m_textFieldContrast);
		
		m_textFieldSharpness = new JTextField();
		m_textFieldSharpness.setText("50");
		m_textFieldSharpness.setHorizontalAlignment(SwingConstants.CENTER);
		m_textFieldSharpness.setEditable(false);
		m_textFieldSharpness.setColumns(10);
		m_textFieldSharpness.setBounds(708, 84, 32, 21);
		getContentPane().add(m_textFieldSharpness);
		
		m_textFieldSaturation = new JTextField();
		m_textFieldSaturation.setText("50");
		m_textFieldSaturation.setHorizontalAlignment(SwingConstants.CENTER);
		m_textFieldSaturation.setEditable(false);
		m_textFieldSaturation.setColumns(10);
		m_textFieldSaturation.setBounds(708, 114, 32, 21);
		getContentPane().add(m_textFieldSaturation);
		
		m_cmbWhiteBalance = new JComboBox<String>();
		m_cmbWhiteBalance.setModel(new DefaultComboBoxModel<String>(m_sWhiteBalance));
		m_cmbWhiteBalance.setBounds(530, 141, 177, 21);
		getContentPane().add(m_cmbWhiteBalance);
		
		m_cmbWDREnabled = new JComboBox<String>();
		m_cmbWDREnabled.setModel(new DefaultComboBoxModel<String>(m_sWDREnabled));
		m_cmbWDREnabled.setBounds(530, 171, 177, 21);
		getContentPane().add(m_cmbWDREnabled);
		
		m_cmbBacklightMode = new JComboBox<String>();
		m_cmbBacklightMode.setModel(new DefaultComboBoxModel<String>(m_sBacklightMode));
		m_cmbBacklightMode.setBounds(530, 201, 177, 21);
		getContentPane().add(m_cmbBacklightMode);
		
		m_cmbScenceMode = new JComboBox<String>();
		m_cmbScenceMode.setModel(new DefaultComboBoxModel<String>(m_sScenceMode));
		m_cmbScenceMode.setBounds(530, 231, 177, 21);
		getContentPane().add(m_cmbScenceMode);
		
		m_cmbDefogMode = new JComboBox<String>();
		m_cmbDefogMode.setModel(new DefaultComboBoxModel<String>(m_sDefogMode));
		m_cmbDefogMode.setBounds(530, 261, 177, 21);
		getContentPane().add(m_cmbDefogMode);
		
		m_cmbElectronicstAblization = new JComboBox<String>();
		m_cmbElectronicstAblization.setModel(new DefaultComboBoxModel<String>(m_sElectronicstAblization));
		m_cmbElectronicstAblization.setBounds(530, 291, 177, 21);
		getContentPane().add(m_cmbElectronicstAblization);
		
		m_cmbCorridorMode = new JComboBox<String>();
		m_cmbCorridorMode.setModel(new DefaultComboBoxModel<String>(m_sCorridorMode));
		m_cmbCorridorMode.setBounds(530, 321, 177, 21);
		getContentPane().add(m_cmbCorridorMode);
		
		m_cmbMirror = new JComboBox<String>();
		m_cmbMirror.setModel(new DefaultComboBoxModel<String>(m_sMirror));
		m_cmbMirror.setBounds(530, 351, 177, 21);
		getContentPane().add(m_cmbMirror);
		
		JButton btnGet = new JButton("Get");
		btnGet.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				Pointer lpISPCameraparamCfg = m_struISPCameraparamCfg.getPointer();
				IntByReference ibrBytesReturned = new IntByReference(0);
				
				m_struISPCameraparamCfg.write();
				if(false == hCNetSDK.NET_DVR_GetDVRConfig(m_lUserID, HCNetSDK.NET_DVR_GET_ISP_CAMERAPARAMCFG, new NativeLong(m_iChanNum), lpISPCameraparamCfg, m_struISPCameraparamCfg.size(), ibrBytesReturned))
				{
					int iErr = hCNetSDK.NET_DVR_GetLastError();
					DialogMessage dlg = new DialogMessage("get failed,error code is: "+iErr);
					dlg.setBounds(dlg.getX()+dlg.getWidth()/2,dlg.getY()+dlg.getHeight() , 370, 200);
					dlg.setVisible(true);
					return;
				}
				else
				{
					DialogMessage dlg = new DialogMessage("get succeed!");
					dlg.setBounds(dlg.getX()+dlg.getWidth()/2,dlg.getY()+dlg.getHeight() , 370, 200);
					dlg.setVisible(true);
				}
				m_struISPCameraparamCfg.read();
				
				GetISPCameraparamCfg(m_struISPCameraparamCfg.struSelfAdaptiveParam);
			}
		});
		btnGet.setBounds(438, 420, 82, 23);
		getContentPane().add(btnGet);
		
		JButton btnSet = new JButton("Set");
		btnSet.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				SetISPCameraparamCfg(m_struISPCameraparamCfg.struSelfAdaptiveParam);
				Pointer lpISPCameraparamCfg = m_struISPCameraparamCfg.getPointer();
				
				m_struISPCameraparamCfg.write();
				if(false == hCNetSDK.NET_DVR_SetDVRConfig(m_lUserID, HCNetSDK.NET_DVR_SET_ISP_CAMERAPARAMCFG, new NativeLong(m_iChanNum), lpISPCameraparamCfg, m_struISPCameraparamCfg.size()))
				{
					int iErr = hCNetSDK.NET_DVR_GetLastError();
					DialogMessage dlg = new DialogMessage("set failed,error code is: "+iErr);
					dlg.setBounds(dlg.getX()+dlg.getWidth()/2,dlg.getY()+dlg.getHeight() , 370, 200);
					dlg.setVisible(true);
					return;
				}
				else
				{
					DialogMessage dlg = new DialogMessage("set succeed!");
					dlg.setBounds(dlg.getX()+dlg.getWidth()/2,dlg.getY()+dlg.getHeight() , 370, 200);
					dlg.setVisible(true);
				}
				m_struISPCameraparamCfg.read();
			}
		});
		btnSet.setBounds(635, 420, 82, 23);
		getContentPane().add(btnSet);
	}
	
	private void InitImageCfg()
	{
		m_struISPCameraparamCfg.dwSize = m_struISPCameraparamCfg.size();
	}
	
	private void GetISPCameraparamCfg(HCNetSDK.NET_DVR_CAMERAPARAMCFG_EX struCameraparamCfg)
	{
		m_sliderBrightness.setValue(struCameraparamCfg.struVideoEffect.byBrightnessLevel);
		m_sliderContrast.setValue(struCameraparamCfg.struVideoEffect.byContrastLevel);
		m_sliderSharpness.setValue(struCameraparamCfg.struVideoEffect.bySharpnessLevel);
		m_sliderSaturation.setValue(struCameraparamCfg.struVideoEffect.bySaturationLevel);
		m_cmbWhiteBalance.setSelectedIndex(struCameraparamCfg.struWhiteBalance.byWhiteBalanceMode);
		m_cmbWDREnabled.setSelectedIndex(struCameraparamCfg.struWdr.byWDREnabled);
		m_cmbBacklightMode.setSelectedIndex(struCameraparamCfg.struBackLight.byBacklightMode);
		m_cmbScenceMode.setSelectedIndex(struCameraparamCfg.bySceneMode);
		m_cmbDefogMode.setSelectedIndex(struCameraparamCfg.struDefogCfg.byMode);
		m_cmbElectronicstAblization.setSelectedIndex(struCameraparamCfg.struElectronicStabilization.byEnable);
		m_cmbCorridorMode.setSelectedIndex(struCameraparamCfg.struCorridorMode.byEnableCorridorMode);
		m_cmbMirror.setSelectedIndex(struCameraparamCfg.byMirror);
	}
	
	private void SetISPCameraparamCfg(HCNetSDK.NET_DVR_CAMERAPARAMCFG_EX struCameraparamCfg)
	{
		struCameraparamCfg.struVideoEffect.byBrightnessLevel = (byte)m_sliderBrightness.getValue();
		struCameraparamCfg.struVideoEffect.byContrastLevel = (byte)m_sliderContrast.getValue();
		struCameraparamCfg.struVideoEffect.bySharpnessLevel = (byte)m_sliderSharpness.getValue();
		struCameraparamCfg.struVideoEffect.bySaturationLevel = (byte)m_sliderSaturation.getValue();
		struCameraparamCfg.struWhiteBalance.byWhiteBalanceMode = (byte)m_cmbWhiteBalance.getSelectedIndex();
		struCameraparamCfg.struWdr.byWDREnabled = (byte)m_cmbWDREnabled.getSelectedIndex();
		struCameraparamCfg.struBackLight.byBacklightMode = (byte)m_cmbBacklightMode.getSelectedIndex();
		struCameraparamCfg.bySceneMode = (byte)m_cmbScenceMode.getSelectedIndex();
		struCameraparamCfg.struDefogCfg.byMode = (byte)m_cmbDefogMode.getSelectedIndex();
		struCameraparamCfg.struElectronicStabilization.byEnable = (byte)m_cmbElectronicstAblization.getSelectedIndex();
		struCameraparamCfg.struCorridorMode.byEnableCorridorMode = (byte)m_cmbCorridorMode.getSelectedIndex();
		struCameraparamCfg.byMirror = (byte)m_cmbMirror.getSelectedIndex();
	}
}
