package test;
import javax.swing.tree.DefaultMutableTreeNode;


@SuppressWarnings("serial")
public class MyTreeNode extends DefaultMutableTreeNode
{
	public boolean m_isOnLine;

	public MyTreeNode()
	{
		// TODO Auto-generated constructor stub
		super();
		m_isOnLine=true;
	}
	
	public MyTreeNode(Object o)
	{
		super(o);
		m_isOnLine=true;
	}

}
