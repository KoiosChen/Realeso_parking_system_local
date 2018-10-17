package test;

import javax.swing.JLabel;
import javax.swing.SwingConstants;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.DefaultComboBoxModel;
import javax.swing.JTextField;
import javax.swing.JSlider;
import javax.swing.JButton;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import javax.swing.event.ChangeListener;

import com.sun.jna.NativeLong;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;

import javax.swing.event.ChangeEvent;
import java.awt.event.ItemListener;
import java.awt.event.ItemEvent;

public class VideoCfg extends JDialog {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	static HCNetSDK hCNetSDK = HCNetSDK.INSTANCE;
	private NativeLong m_lUserID;
	
	private NET_DVR_MULTI_STREAM_COMPRESSIONCFG_COND_4 m_struMultiStreamCompressionCond = new NET_DVR_MULTI_STREAM_COMPRESSIONCFG_COND_4();
	private NET_DVR_MULTI_STREAM_COMPRESSIONCFG_4 m_struMultiStreamCompressionCfg = new NET_DVR_MULTI_STREAM_COMPRESSIONCFG_4();
	private STATUS_LIST_4 m_struStatusList = new STATUS_LIST_4();
	private COMPRESSION_INFO_LOCAL m_struCompressionInfoLocal = new COMPRESSION_INFO_LOCAL();
	
	private JComboBox<String> m_cmbStreamType;
	private String[] m_sStreamType = {"Main Stream (Timing)", "Sub Stream (Net)", "Event Stream", "Third Stream"};
	private JComboBox<String> m_cmbVideoType;
	private String[] m_sVideoType = {"Video Stream", "Compound Stream", "Auto"};
	private JComboBox<String> m_cmbResolution;
	private String[] m_sResolution = {"DCIF(528*384/528*320)", "CIF(352*288/352*240)", "QCIF(176*144/176*120)", "4CIF(704*576/704*480)", "2CIF(704*288/704*240)", "QVGA(320*240)", "QQVGA(160*120)", "384*288", "576*576", "VGA(640*480)", "UXGA(1600*1200)", "SVGA(800*600)", "HD720P(1280*720)", "XVGA(1280*960)", "HD900P(1600*900)", "1360*1024", "1536*1536", "1920*1920", "1920*1080", "2560*1920", "1600*304", "2048*1536", "2448*2048", "2448*1200", "2448*800", "XGA(1024*768)", "SXGA(1280*1024)", "WD1(960*576/960*480)", "1080i(1920*1080)", "WXGA(1440*900)", "HD_F(1920*1080/1280*720)", "HD_H(1920*540/1280*360)", "HD_Q(960*540/630*360)", "2336*1744", "1920*1456", "2592*2048", "3296*2472", "1376*768", "1366*768", "1360*768", "WSXGA+", "720*720", "1280*1280", "2048*768", "2048*2048", "2560*2048", "3072*2048", "2304*1296", "WXGA(1280*800)", "1600*600", "1600*900", "2752*2208", "384*288", "4000*3000", "4096*2160", "3840*2160", "4000*2250", "3072*1728", "2592*1944", "2464*1520", "1280*1920", "2560*1440", "1024*1024", "160*128", "324*240", "324*256", "336*256", "640*512", "2720*2048", "384*256", "384*216", "320*256", "320*180", "320*192", "512*384", "325*256", "256*192", "640*360", "Auto"};
	private JComboBox<String> m_cmbRateType;
	private String[] m_sRateType = {"Variable Rate", "Constant Rate"};
	private JComboBox<String> m_cmbImageQuality;
	private String[] m_sImageQuality = {"Highest", "Higher", "High", "Medium", "Low", "Lower", "Auto"};
	private JComboBox<String> m_cmbVideoBitrate;
	private String[] m_sVideoBitrate = {"32K", "48K", "64K", "80K", "96K", "128K", "160k", "192K", "224K", "256K", "320K", "384K", "448K", "512K", "640K", "768K", "896K", "1024K", "1280K", "1536K", "1792K", "2048K", "3072K", "4096K", "8192K", "16384K", "Auto"};
	private JComboBox<String> m_cmbVideoFrameRate;
	private String[] m_sVidemoFrameRate = {"All", "1/16", "1/8", "1/4", "1/2", "1", "2", "4", "6", "8", "10", "12", "16", "20", "15", "18", "22", "25", "30", "35", "40", "45", "50", "55", "60", "3", "5", "7", "9", "100", "120", "24", "48", "Auto"};
	private JComboBox<String> m_cmbIntervalBPFrame;
	private String[] m_sIntervalBPFrame = {"BBP Frame", "BP Frame", "P Frame"};
	private JComboBox<String> m_cmbVideoEncType;
	private String[] m_sVideoEncType = {"Private 264", "Standard h264", "Standard mpeg4", "M-JPEG", "MPEG2", "SVAC", "Standard h265", "Auto"};
	private JComboBox<String> m_cmbVideoEncComplexity;
	private String[] m_sVideoEncComplexity = {"Low", "Medium", "High", "Auto"};
	private JSlider m_slider;
	private JComboBox<String> m_cmbEnableSvc;
	private String [] m_sEnableSvc = {"Disable", "Enable", "Auto"};
	private JComboBox<String> m_cmbFormatType;
	private String [] m_sFormatType = {"Naked Stream", "RTP", "PS", "TS", "Private", "FLV", "ASF", "3GP", "RTP+PS"};
	private JTextField m_textFieldStreamSmoothing;
	private String m_sPreStreamType;
	private String m_sCurStreamType;
	
	/**
	 * Create the panel.
	 */
	public VideoCfg(NativeLong lUserID) {
		setTitle("Video Configure");
		InitVideoCfg();
		getContentPane().setLayout(null);
		m_lUserID = lUserID;
		
		JLabel lblStreamType = new JLabel("Stream Type");
		lblStreamType.setHorizontalAlignment(SwingConstants.RIGHT);
		lblStreamType.setBounds(62, 24, 131, 15);
		getContentPane().add(lblStreamType);
		
		JLabel lblVideoType = new JLabel("Video Type");
		lblVideoType.setHorizontalAlignment(SwingConstants.RIGHT);
		lblVideoType.setBounds(62, 54, 131, 15);
		getContentPane().add(lblVideoType);
		
		JLabel lblResolution = new JLabel("Resolution");
		lblResolution.setHorizontalAlignment(SwingConstants.RIGHT);
		lblResolution.setBounds(62, 84, 131, 15);
		getContentPane().add(lblResolution);
		
		JLabel lblRateType = new JLabel("Bitrate Type");
		lblRateType.setHorizontalAlignment(SwingConstants.RIGHT);
		lblRateType.setBounds(62, 114, 131, 15);
		getContentPane().add(lblRateType);
		
		JLabel lblPicQuality = new JLabel("Pic Quality");
		lblPicQuality.setHorizontalAlignment(SwingConstants.RIGHT);
		lblPicQuality.setBounds(62, 144, 131, 15);
		getContentPane().add(lblPicQuality);
		
		JLabel lblVideoBitrate = new JLabel("Video Bitrate");
		lblVideoBitrate.setHorizontalAlignment(SwingConstants.RIGHT);
		lblVideoBitrate.setBounds(62, 174, 131, 15);
		getContentPane().add(lblVideoBitrate);
		
		JLabel lblVideoFrameRate = new JLabel("Video Frame Rate");
		lblVideoFrameRate.setHorizontalAlignment(SwingConstants.RIGHT);
		lblVideoFrameRate.setBounds(62, 205, 131, 15);
		getContentPane().add(lblVideoFrameRate);
		
		JLabel lblIntervalBPFrame = new JLabel("Interval BP Frame");
		lblIntervalBPFrame.setHorizontalAlignment(SwingConstants.RIGHT);
		lblIntervalBPFrame.setBounds(62, 236, 131, 15);
		getContentPane().add(lblIntervalBPFrame);
		
		JLabel lblVideoEncType = new JLabel("Video EncType");
		lblVideoEncType.setHorizontalAlignment(SwingConstants.RIGHT);
		lblVideoEncType.setBounds(62, 268, 131, 15);
		getContentPane().add(lblVideoEncType);
		
		JLabel lblVideoEncComplexity = new JLabel("Video Enc Complexity");
		lblVideoEncComplexity.setHorizontalAlignment(SwingConstants.RIGHT);
		lblVideoEncComplexity.setBounds(62, 296, 131, 15);
		getContentPane().add(lblVideoEncComplexity);
		
		JLabel lblEnableSvc = new JLabel("Enable Svc");
		lblEnableSvc.setHorizontalAlignment(SwingConstants.RIGHT);
		lblEnableSvc.setBounds(62, 325, 131, 15);
		getContentPane().add(lblEnableSvc);
		
		JLabel lblFormatType = new JLabel("Format Type");
		lblFormatType.setHorizontalAlignment(SwingConstants.RIGHT);
		lblFormatType.setBounds(62, 356, 131, 15);
		getContentPane().add(lblFormatType);
		
		JLabel lblStreamSmooth = new JLabel("Stream Smooth");
		lblStreamSmooth.setHorizontalAlignment(SwingConstants.RIGHT);
		lblStreamSmooth.setBounds(62, 388, 131, 15);
		getContentPane().add(lblStreamSmooth);
		
		m_cmbStreamType = new JComboBox<String>();
		m_cmbStreamType.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent e) {
				if (ItemEvent.SELECTED == e.getStateChange())
				{
					m_sCurStreamType = (String)m_cmbStreamType.getSelectedItem();
					SetStreamType(m_sPreStreamType);
					StreamTypeChanged(m_sCurStreamType);
					m_sPreStreamType = m_sCurStreamType;	
				}
			}
		});
		m_cmbStreamType.setModel(new DefaultComboBoxModel<String>(m_sStreamType));
		m_cmbStreamType.setBounds(240, 21, 177, 21);
		getContentPane().add(m_cmbStreamType);
		
		m_cmbVideoType = new JComboBox<String>();
		m_cmbVideoType.setModel(new DefaultComboBoxModel<String>(m_sVideoType));
		m_cmbVideoType.setBounds(240, 51, 177, 21);
		getContentPane().add(m_cmbVideoType);
		
		m_cmbResolution = new JComboBox<String>();
		m_cmbResolution.setModel(new DefaultComboBoxModel<String>(m_sResolution));
		m_cmbResolution.setBounds(240, 81, 177, 21);
		getContentPane().add(m_cmbResolution);
		
		m_cmbRateType = new JComboBox<String>();
		m_cmbRateType.setModel(new DefaultComboBoxModel<String>(m_sRateType));
		m_cmbRateType.setBounds(240, 111, 177, 21);
		getContentPane().add(m_cmbRateType);
		
		m_cmbImageQuality = new JComboBox<String>();
		m_cmbImageQuality.setModel(new DefaultComboBoxModel<String>(m_sImageQuality));
		m_cmbImageQuality.setBounds(240, 141, 177, 21);
		getContentPane().add(m_cmbImageQuality);
		
		m_cmbVideoBitrate = new JComboBox<String>();
		m_cmbVideoBitrate.setModel(new DefaultComboBoxModel<String>(m_sVideoBitrate));
		m_cmbVideoBitrate.setBounds(240, 171, 177, 21);
		getContentPane().add(m_cmbVideoBitrate);
		
		m_cmbVideoFrameRate = new JComboBox<String>();
		m_cmbVideoFrameRate.setModel(new DefaultComboBoxModel<String>(m_sVidemoFrameRate));
		m_cmbVideoFrameRate.setBounds(240, 202, 177, 21);
		getContentPane().add(m_cmbVideoFrameRate);
		
		m_cmbIntervalBPFrame = new JComboBox<String>();
		m_cmbIntervalBPFrame.setModel(new DefaultComboBoxModel<String>(m_sIntervalBPFrame));
		m_cmbIntervalBPFrame.setBounds(240, 233, 177, 21);
		getContentPane().add(m_cmbIntervalBPFrame);
		
		m_cmbVideoEncType = new JComboBox<String>();
		m_cmbVideoEncType.setModel(new DefaultComboBoxModel<String>(m_sVideoEncType));
		m_cmbVideoEncType.setBounds(240, 265, 177, 21);
		getContentPane().add(m_cmbVideoEncType);
		
		m_cmbVideoEncComplexity = new JComboBox<String>();
		m_cmbVideoEncComplexity.setModel(new DefaultComboBoxModel<String>(m_sVideoEncComplexity));
		m_cmbVideoEncComplexity.setBounds(240, 293, 177, 21);
		getContentPane().add(m_cmbVideoEncComplexity);
		
		m_slider = new JSlider();
		m_slider.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				if ((JSlider)e.getSource() == m_slider)
				{
					m_textFieldStreamSmoothing.setText(""+m_slider.getValue());
				}
			}
		});
		m_slider.setBounds(240, 388, 177, 23);
		getContentPane().add(m_slider);
		
		m_cmbEnableSvc = new JComboBox<String>();
		m_cmbEnableSvc.setModel(new DefaultComboBoxModel<String>(m_sEnableSvc));
		m_cmbEnableSvc.setBounds(240, 322, 177, 21);
		getContentPane().add(m_cmbEnableSvc);
		
		m_cmbFormatType = new JComboBox<String>();
		m_cmbFormatType.setModel(new DefaultComboBoxModel<String>(m_sFormatType));
		m_cmbFormatType.setBounds(240, 353, 177, 21);
		getContentPane().add(m_cmbFormatType);
		
		m_textFieldStreamSmoothing = new JTextField();
		m_textFieldStreamSmoothing.setHorizontalAlignment(SwingConstants.CENTER);
		m_textFieldStreamSmoothing.setText("50");
		m_textFieldStreamSmoothing.setColumns(10);
		m_textFieldStreamSmoothing.setBounds(427, 385, 32, 21);
		m_textFieldStreamSmoothing.setEditable(false);
		getContentPane().add(m_textFieldStreamSmoothing);
		
		JLabel lblClearToSmooth = new JLabel("Clear<—>Smooth");
		lblClearToSmooth.setHorizontalAlignment(SwingConstants.LEFT);
		lblClearToSmooth.setBounds(469, 388, 115, 15);
		getContentPane().add(lblClearToSmooth);
		
		JButton btnGet = new JButton("Get");
		btnGet.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				Pointer lpMultiStreamCompressionCond = m_struMultiStreamCompressionCond.getPointer();
				Pointer lpMultiStreamCompressionCfg = m_struMultiStreamCompressionCfg.getPointer();
				Pointer lpStatusList = m_struStatusList.getPointer();
				
				m_struMultiStreamCompressionCond.write();
				m_struMultiStreamCompressionCfg.write();
				m_struStatusList.write();
				if (false == hCNetSDK.NET_DVR_GetDeviceConfig(m_lUserID, HCNetSDK.NET_DVR_GET_MULTI_STREAM_COMPRESSIONCFG, 4, lpMultiStreamCompressionCond, m_struMultiStreamCompressionCond.size(), lpStatusList, lpMultiStreamCompressionCfg, m_struMultiStreamCompressionCfg.size()))
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
				m_struMultiStreamCompressionCond.read();
				m_struMultiStreamCompressionCfg.read();
				m_struStatusList.read();
				
				m_sCurStreamType = m_sPreStreamType = (String)m_cmbStreamType.getSelectedItem();
				StreamTypeChanged(m_sCurStreamType);
			}
		});
		btnGet.setBounds(111, 441, 82, 23);
		getContentPane().add(btnGet);
		
		JButton btnSet = new JButton("Set");
		btnSet.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				SetStreamType(m_sCurStreamType);
				
				Pointer lpMultiStreamCompressionCond = m_struMultiStreamCompressionCond.getPointer();
				Pointer lpMultiStreamCompressionCfg = m_struMultiStreamCompressionCfg.getPointer();
				Pointer lpStatusList = m_struStatusList.getPointer();
				
				m_struMultiStreamCompressionCond.write();
				m_struMultiStreamCompressionCfg.write();
				m_struStatusList.write();
				if (false == hCNetSDK.NET_DVR_SetDeviceConfig(m_lUserID, HCNetSDK.NET_DVR_SET_MULTI_STREAM_COMPRESSIONCFG, 4, lpMultiStreamCompressionCond, m_struMultiStreamCompressionCond.size(), lpStatusList, lpMultiStreamCompressionCfg, m_struMultiStreamCompressionCfg.size()))
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
				m_struMultiStreamCompressionCond.read();
				m_struMultiStreamCompressionCfg.read();
				m_struStatusList.read();
				
			}
		});
		btnSet.setBounds(335, 441, 82, 23);
		getContentPane().add(btnSet);

	}
	
	private void InitVideoCfg()
	{
		m_struMultiStreamCompressionCond.struMultiStreamCompressionCond1.dwSize = m_struMultiStreamCompressionCond.struMultiStreamCompressionCond1.size();
		m_struMultiStreamCompressionCond.struMultiStreamCompressionCond2.dwSize = m_struMultiStreamCompressionCond.struMultiStreamCompressionCond2.size();
		m_struMultiStreamCompressionCond.struMultiStreamCompressionCond3.dwSize = m_struMultiStreamCompressionCond.struMultiStreamCompressionCond3.size();
		m_struMultiStreamCompressionCond.struMultiStreamCompressionCond4.dwSize = m_struMultiStreamCompressionCond.struMultiStreamCompressionCond4.size();
		m_struMultiStreamCompressionCond.struMultiStreamCompressionCond1.struStreamInfo.dwSize = m_struMultiStreamCompressionCond.struMultiStreamCompressionCond1.struStreamInfo.size();
		m_struMultiStreamCompressionCond.struMultiStreamCompressionCond2.struStreamInfo.dwSize = m_struMultiStreamCompressionCond.struMultiStreamCompressionCond1.struStreamInfo.size();
		m_struMultiStreamCompressionCond.struMultiStreamCompressionCond3.struStreamInfo.dwSize = m_struMultiStreamCompressionCond.struMultiStreamCompressionCond1.struStreamInfo.size();
		m_struMultiStreamCompressionCond.struMultiStreamCompressionCond4.struStreamInfo.dwSize = m_struMultiStreamCompressionCond.struMultiStreamCompressionCond1.struStreamInfo.size();
		m_struMultiStreamCompressionCond.struMultiStreamCompressionCond1.struStreamInfo.dwChannel = 1;
		m_struMultiStreamCompressionCond.struMultiStreamCompressionCond2.struStreamInfo.dwChannel = 1;
		m_struMultiStreamCompressionCond.struMultiStreamCompressionCond3.struStreamInfo.dwChannel = 1;
		m_struMultiStreamCompressionCond.struMultiStreamCompressionCond4.struStreamInfo.dwChannel = 1;
		m_struMultiStreamCompressionCond.struMultiStreamCompressionCond1.dwStreamType = 0;
		m_struMultiStreamCompressionCond.struMultiStreamCompressionCond2.dwStreamType = 1;
		m_struMultiStreamCompressionCond.struMultiStreamCompressionCond3.dwStreamType = 2;
		m_struMultiStreamCompressionCond.struMultiStreamCompressionCond4.dwStreamType = 3;
			
		m_struMultiStreamCompressionCfg.struMultiStreamCompressionCfg1.dwSize = m_struMultiStreamCompressionCfg.struMultiStreamCompressionCfg1.size();
		m_struMultiStreamCompressionCfg.struMultiStreamCompressionCfg2.dwSize = m_struMultiStreamCompressionCfg.struMultiStreamCompressionCfg2.size();
		m_struMultiStreamCompressionCfg.struMultiStreamCompressionCfg3.dwSize = m_struMultiStreamCompressionCfg.struMultiStreamCompressionCfg3.size();
		m_struMultiStreamCompressionCfg.struMultiStreamCompressionCfg4.dwSize = m_struMultiStreamCompressionCfg.struMultiStreamCompressionCfg4.size();
	}
	
	public static class NET_DVR_MULTI_STREAM_COMPRESSIONCFG_COND_4 extends Structure{
		public HCNetSDK.NET_DVR_MULTI_STREAM_COMPRESSIONCFG_COND struMultiStreamCompressionCond1 = new HCNetSDK.NET_DVR_MULTI_STREAM_COMPRESSIONCFG_COND();
		public HCNetSDK.NET_DVR_MULTI_STREAM_COMPRESSIONCFG_COND struMultiStreamCompressionCond2 = new HCNetSDK.NET_DVR_MULTI_STREAM_COMPRESSIONCFG_COND();
		public HCNetSDK.NET_DVR_MULTI_STREAM_COMPRESSIONCFG_COND struMultiStreamCompressionCond3 = new HCNetSDK.NET_DVR_MULTI_STREAM_COMPRESSIONCFG_COND();
		public HCNetSDK.NET_DVR_MULTI_STREAM_COMPRESSIONCFG_COND struMultiStreamCompressionCond4 = new HCNetSDK.NET_DVR_MULTI_STREAM_COMPRESSIONCFG_COND();
	}
	public static class NET_DVR_MULTI_STREAM_COMPRESSIONCFG_4 extends Structure{
		public HCNetSDK.NET_DVR_MULTI_STREAM_COMPRESSIONCFG struMultiStreamCompressionCfg1 = new HCNetSDK.NET_DVR_MULTI_STREAM_COMPRESSIONCFG();
		public HCNetSDK.NET_DVR_MULTI_STREAM_COMPRESSIONCFG struMultiStreamCompressionCfg2 = new HCNetSDK.NET_DVR_MULTI_STREAM_COMPRESSIONCFG();
		public HCNetSDK.NET_DVR_MULTI_STREAM_COMPRESSIONCFG struMultiStreamCompressionCfg3 = new HCNetSDK.NET_DVR_MULTI_STREAM_COMPRESSIONCFG();
		public HCNetSDK.NET_DVR_MULTI_STREAM_COMPRESSIONCFG struMultiStreamCompressionCfg4 = new HCNetSDK.NET_DVR_MULTI_STREAM_COMPRESSIONCFG();
	}
	public static class STATUS_LIST_4 extends Structure{
		public int iStatusList1 = 0;
		public int iStatusList2 = 0;
		public int iStatusList3 = 0;
		public int iStatusList4 = 0;
	}
	public static class COMPRESSION_INFO_LOCAL extends Structure{
		public int iStreamType = 0;
		public int iResolution = 0;
		public int iBitrateType = 0;
		public int iPicQuality = 0;
		public int iVideoBitrate = 0;
		public int iVideoFrameRate = 0;
		public int iIntervalBPFrame = 0;
		public int iVideoEncType = 0;
		public int iVideoEncComplexity = 0;
		public int iEnableSvc = 0;
		public int iFormatType = 0;
		public int iSteamSmooth = 0;
	}
	
	private void FindLocalCur(HCNetSDK.NET_DVR_COMPRESSION_INFO_V30 struCompressionInfo, COMPRESSION_INFO_LOCAL struCompressionInfoLocal)
	{
		// 1.StreamType
		if (0xfe == struCompressionInfo.byStreamType)
		{
			struCompressionInfoLocal.iStreamType = 2;
		}
		else
		{
			struCompressionInfoLocal.iStreamType = struCompressionInfo.byStreamType;
		}
		
		// 2.Resolution
		if (struCompressionInfo.byResolution >= 0 && struCompressionInfo.byResolution <= 4)
		{
			struCompressionInfoLocal.iResolution = struCompressionInfo.byResolution;
		}
		else if (struCompressionInfo.byResolution >= 6 && struCompressionInfo.byResolution <= 7)
		{
			struCompressionInfoLocal.iResolution = struCompressionInfo.byResolution-1;
		}
		else if (struCompressionInfo.byResolution >= 12 && struCompressionInfo.byResolution <= 13)
		{
			struCompressionInfoLocal.iResolution = struCompressionInfo.byResolution-5;
		}
		else if (struCompressionInfo.byResolution >= 16 && struCompressionInfo.byResolution <= 24)
		{
			struCompressionInfoLocal.iResolution = struCompressionInfo.byResolution-7;
		}
		else if (struCompressionInfo.byResolution >= 27 && struCompressionInfo.byResolution <= 86)
		{
			struCompressionInfoLocal.iResolution = struCompressionInfo.byResolution-9;
		}
		else
		{
			struCompressionInfoLocal.iResolution = 78; 
		}
		
		// 3.BitrateType
		struCompressionInfoLocal.iBitrateType = struCompressionInfo.byBitrateType;
		
		// 4.PicQuality
		if (0xfe == struCompressionInfo.byPicQuality)
		{
			struCompressionInfoLocal.iPicQuality = 6;
		}
		else
		{
			struCompressionInfoLocal.iPicQuality = struCompressionInfo.byPicQuality;
		}
		
		// 5.VideoBitrate
		if (0xfffffffe == struCompressionInfo.dwVideoBitrate)
		{
			struCompressionInfoLocal.iVideoBitrate = 26;
		}
		else if (0 == struCompressionInfo.dwVideoBitrate)
		{
			struCompressionInfoLocal.iVideoBitrate = 26;
		}
		else
		{
			struCompressionInfoLocal.iVideoBitrate = struCompressionInfo.dwVideoBitrate-2;
		}
		
		// 6.VideoFrameRate
		if (0xfffffffe == struCompressionInfo.dwVideoFrameRate)
		{
			struCompressionInfoLocal.iVideoFrameRate = 33;
		}
		else
		{
			struCompressionInfoLocal.iVideoFrameRate = struCompressionInfo.dwVideoFrameRate;
		}
		
		// 7.IntervalBPFrame
		struCompressionInfoLocal.iIntervalBPFrame = struCompressionInfo.byIntervalBPFrame;
		
		// 8.VideoEncType
		if (struCompressionInfo.byVideoEncType >= 0 && struCompressionInfo.byVideoEncType <= 2)
		{
			struCompressionInfoLocal.iVideoEncType = struCompressionInfo.byVideoEncType;
		}
		else if (struCompressionInfo.byVideoEncType >= 7 && struCompressionInfo.byVideoEncType <= 10)
		{
			struCompressionInfoLocal.iVideoEncType = struCompressionInfo.byVideoEncType-4;
		}
		else 
		{
			struCompressionInfoLocal.iVideoEncType = 7;
		}
		
		// 9.VideoEncComplexity
		if (0xfe == struCompressionInfo.byVideoEncComplexity)
		{
			struCompressionInfoLocal.iVideoEncComplexity = 3;
		}
		else
		{
			struCompressionInfoLocal.iVideoEncComplexity = struCompressionInfo.byVideoEncComplexity;
		}
		
		// 10.EnableSvc
		struCompressionInfoLocal.iEnableSvc = struCompressionInfo.byEnableSvc;
		
		// 11.FormatType
		if (0 == struCompressionInfo.byFormatType)
		{
			struCompressionInfoLocal.iFormatType = 0;
		}
		else
		{
			struCompressionInfoLocal.iFormatType = struCompressionInfo.byFormatType-1;
		}
		
		// 12.SteamSmooth
		struCompressionInfoLocal.iSteamSmooth = struCompressionInfo.bySteamSmooth;
	}
	
	private void SetLocalCur(COMPRESSION_INFO_LOCAL struCompressionInfoLocal)
	{
		// 1.StreamType
		m_cmbVideoType.setSelectedItem(m_sVideoType[struCompressionInfoLocal.iStreamType]);
		
		// 2.Resolution
		m_cmbResolution.setSelectedItem(m_sResolution[struCompressionInfoLocal.iResolution]);
		
		// 3.BitrateType
		m_cmbRateType.setSelectedItem(m_sRateType[struCompressionInfoLocal.iBitrateType]);
		
		// 4.PicQuality
		m_cmbImageQuality.setSelectedItem(m_sImageQuality[struCompressionInfoLocal.iPicQuality]);
		
		// 5.VideoBitrate
		m_cmbVideoBitrate.setSelectedItem(m_sVideoBitrate[struCompressionInfoLocal.iVideoBitrate]);
		
		// 6.VideoFrameRate
		m_cmbVideoFrameRate.setSelectedItem(m_sVidemoFrameRate[struCompressionInfoLocal.iVideoFrameRate]);
		
		// 7.IntervalBPFrame
		m_cmbIntervalBPFrame.setSelectedItem(m_sIntervalBPFrame[struCompressionInfoLocal.iIntervalBPFrame]);
		
		// 8.VideoEncType
		m_cmbVideoEncType.setSelectedItem(m_sVideoEncType[struCompressionInfoLocal.iVideoEncType]);
		
		// 9.VideoEncComplexity
		m_cmbVideoEncComplexity.setSelectedItem(m_sVideoEncComplexity[struCompressionInfoLocal.iVideoEncComplexity]);
		
		// 10.EnableSvc
		m_cmbEnableSvc.setSelectedItem(m_sEnableSvc[struCompressionInfoLocal.iEnableSvc]);
		
		// 11.FormatType
		m_cmbFormatType.setSelectedItem(m_sFormatType[struCompressionInfoLocal.iFormatType]);
		
		// 12.SteamSmooth
		m_slider.setValue(struCompressionInfoLocal.iSteamSmooth);
		m_textFieldStreamSmoothing.setText(""+m_slider.getValue());
	}
	
	private void StreamTypeChanged(String sStreamType)
	{
		switch (sStreamType)
		{
		case "Main Stream (Timing)":
			FindLocalCur(m_struMultiStreamCompressionCfg.struMultiStreamCompressionCfg1.struStreamPara, m_struCompressionInfoLocal);
			SetLocalCur(m_struCompressionInfoLocal);
			break;
		case "Sub Stream (Net)":
			FindLocalCur(m_struMultiStreamCompressionCfg.struMultiStreamCompressionCfg2.struStreamPara, m_struCompressionInfoLocal);
			SetLocalCur(m_struCompressionInfoLocal);
			break;
		case "Event Stream":
			FindLocalCur(m_struMultiStreamCompressionCfg.struMultiStreamCompressionCfg3.struStreamPara, m_struCompressionInfoLocal);
			SetLocalCur(m_struCompressionInfoLocal);
			break;
		case "Third Stream":
			FindLocalCur(m_struMultiStreamCompressionCfg.struMultiStreamCompressionCfg4.struStreamPara, m_struCompressionInfoLocal);
			SetLocalCur(m_struCompressionInfoLocal);
			break;
		default:
			break;
		}
	}
	
	private void SetCompressionInfo(HCNetSDK.NET_DVR_COMPRESSION_INFO_V30 struCompressionInfo)
	{
		// 1.StreamType
		if (2 == m_cmbVideoType.getSelectedIndex())
		{
			struCompressionInfo.byStreamType = (byte)0xfe;
		}
		else
		{
			struCompressionInfo.byStreamType = (byte)m_cmbVideoType.getSelectedIndex();
		}
		
		// 2.Resolution
		if (m_cmbResolution.getSelectedIndex() >= 0 && m_cmbResolution.getSelectedIndex() <=4)
		{
			struCompressionInfo.byResolution = (byte)m_cmbResolution.getSelectedIndex();
		}
		else if (m_cmbResolution.getSelectedIndex() >= 5 && m_cmbResolution.getSelectedIndex() <=6)
		{
			struCompressionInfo.byResolution = (byte)(m_cmbResolution.getSelectedIndex()+1);
		}
		else if (m_cmbResolution.getSelectedIndex() >= 7 && m_cmbResolution.getSelectedIndex() <=8)
		{
			struCompressionInfo.byResolution = (byte)(m_cmbResolution.getSelectedIndex()+5);
		}
		else if (m_cmbResolution.getSelectedIndex() >= 9 && m_cmbResolution.getSelectedIndex() <=17)
		{
			struCompressionInfo.byResolution = (byte)(m_cmbResolution.getSelectedIndex()+7);
		}
		else if (m_cmbResolution.getSelectedIndex() >= 18 && m_cmbResolution.getSelectedIndex() <=77)
		{
			struCompressionInfo.byResolution = (byte)(m_cmbResolution.getSelectedIndex()+9);
		}
		else 
		{
			struCompressionInfo.byResolution = (byte)0xff;
		}
		
		// 3.BitrateType
		struCompressionInfo.byBitrateType = (byte)m_cmbRateType.getSelectedIndex();
		
		// 4.PicQuality
		if (m_cmbImageQuality.getSelectedIndex() >= 0 && m_cmbImageQuality.getSelectedIndex() <= 5)
		{
			struCompressionInfo.byPicQuality = (byte)m_cmbImageQuality.getSelectedIndex();
		}
		else
		{
			struCompressionInfo.byPicQuality = (byte)0xfe;
		}
		
		// 5.VideoBitrate
		if (m_cmbVideoBitrate.getSelectedIndex() >= 0 && m_cmbVideoBitrate.getSelectedIndex() <= 25)
		{
			struCompressionInfo.dwVideoBitrate = m_cmbVideoBitrate.getSelectedIndex()+2;
		}
		else
		{
			struCompressionInfo.dwVideoBitrate = 0xfffffffe;
		}
		
		// 6.VideoFrameRate
		if (33 == m_cmbVideoFrameRate.getSelectedIndex())
		{
			struCompressionInfo.dwVideoFrameRate = 0xfffffffe;
		}
		else
		{
			struCompressionInfo.dwVideoFrameRate = m_cmbVideoFrameRate.getSelectedIndex();
		}
		
		// 7.IntervalBPFrame
		struCompressionInfo.byIntervalBPFrame = (byte)m_cmbIntervalBPFrame.getSelectedIndex();
		
		// 8.VideoEncType
		if (m_cmbVideoEncType.getSelectedIndex() >= 0 && m_cmbVideoEncType.getSelectedIndex() <= 2)
		{
			struCompressionInfo.byVideoEncType = (byte)m_cmbVideoEncType.getSelectedIndex();
		}
		else if (m_cmbVideoEncType.getSelectedIndex() >= 3 && m_cmbVideoEncType.getSelectedIndex() <= 6)
		{
			struCompressionInfo.byVideoEncType = (byte)(m_cmbVideoEncType.getSelectedIndex()+4);
		}
		else 
		{
			struCompressionInfo.byVideoEncType = (byte)0xfe;
		}
		
		// 9.VideoEncComplexity
		if (3 == m_cmbVideoEncComplexity.getSelectedIndex())
		{
			struCompressionInfo.byVideoEncComplexity = (byte)0xfe;
		}
		else
		{
			struCompressionInfo.byVideoEncComplexity = (byte)m_cmbVideoEncComplexity.getSelectedIndex();
		}
		
		// 10.EnableSvc
		struCompressionInfo.byEnableSvc = (byte)m_cmbEnableSvc.getSelectedIndex();
		
		// 11.FormatType
		struCompressionInfo.byFormatType = (byte)(m_cmbFormatType.getSelectedIndex()+1);
		
		// 12.SteamSmooth
		struCompressionInfo.bySteamSmooth = (byte)m_slider.getValue();
	}
	
	private void SetStreamType(String sStreamType)
	{
		//String sStreamType = (String)m_cmbStreamType.getSelectedItem();
		switch (sStreamType)
		{
		case "Main Stream (Timing)":
			SetCompressionInfo(m_struMultiStreamCompressionCfg.struMultiStreamCompressionCfg1.struStreamPara);
			break;
		case "Sub Stream (Net)":
			SetCompressionInfo(m_struMultiStreamCompressionCfg.struMultiStreamCompressionCfg2.struStreamPara);
			break;
		case "Event Stream":
			SetCompressionInfo(m_struMultiStreamCompressionCfg.struMultiStreamCompressionCfg3.struStreamPara);
			break;
		case "Third Stream":
			SetCompressionInfo(m_struMultiStreamCompressionCfg.struMultiStreamCompressionCfg4.struStreamPara);
			break;
		default:
			break;
		}
	}
}
