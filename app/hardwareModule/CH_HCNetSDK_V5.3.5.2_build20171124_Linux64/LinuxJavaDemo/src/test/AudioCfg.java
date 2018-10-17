package test;

import javax.swing.JLabel;
import javax.swing.SwingConstants;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JSlider;
import javax.swing.JTextField;
import javax.swing.event.ChangeListener;

import com.sun.jna.NativeLong;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;

import javax.swing.event.ChangeEvent;
import javax.swing.JButton;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import javax.swing.DefaultComboBoxModel;

public class AudioCfg extends JDialog {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	static HCNetSDK hCNetSDK = HCNetSDK.INSTANCE;
	private NativeLong m_lUserID;
	private int m_iChanNum;
	
	private HCNetSDK.NET_DVR_AUDIO_INPUT_PARAM m_struAduioInputParam = new HCNetSDK.NET_DVR_AUDIO_INPUT_PARAM();
	
	private JComboBox<String> m_cmbAudioEncode;
	private String[] m_sAudioEncode = {"G.722.1", "G.711ulaw", "G.711alaw", "MP2L2", "G.726", "AAC", "PCM"};
	private JComboBox<String> m_cmbAudioInput;
	private String[] m_sAudioInput = {"MicIn", "LineIn"};
	private JSlider m_sliderVolume;
	private JTextField m_textFieldInputVolume;
	private JComboBox<String> m_cmbNoiseFilter;
	private String[] m_sNoiseFilter = {"Close", "Open"};
	
	/**
	 * Create the panel.
	 */
	public AudioCfg(NativeLong lUserID, int iChanNum) {
		setTitle("Audio Configure");
		getContentPane().setLayout(null);
		m_lUserID = lUserID;
		m_iChanNum = iChanNum;
		
		JLabel lblAudioEncode = new JLabel("Audio Encode");
		lblAudioEncode.setHorizontalAlignment(SwingConstants.RIGHT);
		lblAudioEncode.setBounds(62, 24, 131, 15);
		getContentPane().add(lblAudioEncode);
		
		JLabel lblAudioInput = new JLabel("Audio Input");
		lblAudioInput.setHorizontalAlignment(SwingConstants.RIGHT);
		lblAudioInput.setBounds(62, 54, 131, 15);
		getContentPane().add(lblAudioInput);
		
		JLabel lblInputVolume = new JLabel("Input Volume");
		lblInputVolume.setHorizontalAlignment(SwingConstants.RIGHT);
		lblInputVolume.setBounds(62, 84, 131, 15);
		getContentPane().add(lblInputVolume);
		
		JLabel lblNoiseFilter = new JLabel("Noise Filter");
		lblNoiseFilter.setHorizontalAlignment(SwingConstants.RIGHT);
		lblNoiseFilter.setBounds(62, 114, 131, 15);
		getContentPane().add(lblNoiseFilter);
		
		m_cmbAudioEncode = new JComboBox<String>();
		m_cmbAudioEncode.setModel(new DefaultComboBoxModel<String>(m_sAudioEncode));
		m_cmbAudioEncode.setBounds(240, 21, 177, 21);
		getContentPane().add(m_cmbAudioEncode);
		
		m_cmbAudioInput = new JComboBox<String>();
		m_cmbAudioInput.setModel(new DefaultComboBoxModel<String>(m_sAudioInput));
		m_cmbAudioInput.setBounds(240, 51, 177, 21);
		getContentPane().add(m_cmbAudioInput);
		
		m_sliderVolume = new JSlider();
		m_sliderVolume.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				if ((JSlider)e.getSource() == m_sliderVolume)
				{
					m_textFieldInputVolume.setText(""+m_sliderVolume.getValue());
				}
			}
		});
		m_sliderVolume.setBounds(240, 81, 177, 21);
		getContentPane().add(m_sliderVolume);
		
		m_cmbNoiseFilter = new JComboBox<String>();
		m_cmbNoiseFilter.setModel(new DefaultComboBoxModel<String>(m_sNoiseFilter));
		m_cmbNoiseFilter.setBounds(240, 111, 177, 21);
		getContentPane().add(m_cmbNoiseFilter);
		
		m_textFieldInputVolume = new JTextField();
		m_textFieldInputVolume.setText("50");
		m_textFieldInputVolume.setHorizontalAlignment(SwingConstants.CENTER);
		m_textFieldInputVolume.setEditable(false);
		m_textFieldInputVolume.setColumns(10);
		m_textFieldInputVolume.setBounds(427, 81, 32, 21);
		getContentPane().add(m_textFieldInputVolume);
		
		JButton btnSet = new JButton("Set");
		btnSet.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				SetAudioInputStru(m_struAduioInputParam);
				Pointer lpNetDvrAduioInputParam = m_struAduioInputParam.getPointer();
				
				m_struAduioInputParam.write();
				if (false == hCNetSDK.NET_DVR_SetDVRConfig(m_lUserID, HCNetSDK.NET_DVR_SET_AUDIO_INPUT, new NativeLong(m_iChanNum), lpNetDvrAduioInputParam, m_struAduioInputParam.size()))
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
				m_struAduioInputParam.read();
			}
		});
		btnSet.setBounds(335, 161, 82, 23);
		getContentPane().add(btnSet);
		
		JButton btnGet = new JButton("Get");
		btnGet.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				IntByReference ibrBytesReturned = new IntByReference(0);
				m_struAduioInputParam.write();
				Pointer lpNetDvrAduioInputParam = m_struAduioInputParam.getPointer();
				if (false == hCNetSDK.NET_DVR_GetDVRConfig(m_lUserID, HCNetSDK.NET_DVR_GET_AUDIO_INPUT, new NativeLong(m_iChanNum), lpNetDvrAduioInputParam, m_struAduioInputParam.size(), ibrBytesReturned))
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
				m_struAduioInputParam.read();
				
				GetAudioInputStru(m_struAduioInputParam);
			}
		});
		btnGet.setBounds(111, 161, 82, 23);
		getContentPane().add(btnGet);
	}
	
	private void GetAudioInputStru(HCNetSDK.NET_DVR_AUDIO_INPUT_PARAM struAduioInputParam)
	{
		m_cmbAudioInput.setSelectedIndex(struAduioInputParam.byAudioInputType);
		m_sliderVolume.setValue(struAduioInputParam.byVolume);
		m_cmbNoiseFilter.setSelectedIndex(struAduioInputParam.byEnableNoiseFilter);
	}
	
	private void SetAudioInputStru(HCNetSDK.NET_DVR_AUDIO_INPUT_PARAM struAduioInputParam)
	{
		struAduioInputParam.byAudioInputType = (byte)m_cmbAudioInput.getSelectedIndex();
		struAduioInputParam.byVolume = (byte)m_sliderVolume.getValue();
		struAduioInputParam.byEnableNoiseFilter = (byte)m_cmbNoiseFilter.getSelectedIndex();
	}
}
