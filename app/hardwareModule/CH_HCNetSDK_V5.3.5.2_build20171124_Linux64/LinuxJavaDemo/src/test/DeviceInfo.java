package test;

import com.sun.jna.NativeLong;

public class DeviceInfo
{
	private String m_sDVRIP;
	private short m_wDVRPort;
	private String m_sUserName;
	private String m_sPassword;
	private NativeLong m_nUserID;
	HCNetSDK.NET_DVR_DEVICEINFO_V30 m_lpDeviceInfo;
	
    NativeLong m_lAlarmHandle;//guading handle
    NativeLong m_lListenHandle;//listening handle
	
	DeviceInfo(String sDVRIP, short wDVRPort, String sUserName, String sPassword, HCNetSDK.NET_DVR_DEVICEINFO_V30 lpDeviceInfo,NativeLong nUserID)
	{
		m_sDVRIP=sDVRIP;
		m_wDVRPort=wDVRPort;
		m_sUserName=sUserName;
		m_sPassword=sPassword;
		m_nUserID=nUserID;
		m_lpDeviceInfo=lpDeviceInfo;
		m_lAlarmHandle=new NativeLong(-1);
		m_lListenHandle=new NativeLong(-1);
		
	}
	
	void SetNUserID(NativeLong nUserID)
	{
		m_nUserID=nUserID;
	}
	
	String GetIP()
	{
		return m_sDVRIP;
	}
	
	short GetPort()
	{
		return m_wDVRPort;
	}
	
	String GetUserName()
	{
		return m_sUserName;
	}
	
	String GetPassword()
	{
		return m_sPassword;
	}
		
	NativeLong GetNUserID()
	{
		return m_nUserID;
	}
	
	HCNetSDK.NET_DVR_DEVICEINFO_V30 GetlpDeviceInfo()
	{
		return m_lpDeviceInfo;
	}
	
	NativeLong GetAlarmHandle()
	{
		return m_lAlarmHandle;
	}
	
	void SetAlarmHandle(NativeLong lAlarm)
	{
		m_lAlarmHandle=lAlarm;
	}
	
	NativeLong GetListenHandle()
	{
		return m_lListenHandle;
	}
	
	void SetListenHandle(NativeLong lListen)
	{
		m_lListenHandle=lListen;
	}
	
	
	
	
	
}
