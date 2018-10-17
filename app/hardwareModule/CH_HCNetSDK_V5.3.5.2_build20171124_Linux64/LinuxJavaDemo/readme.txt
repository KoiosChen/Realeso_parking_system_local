Demo开发编译环境：Eclipse，JDK1.8 64位

方法一 
在系统的/usr/lib文件中加入你Java工程所需要使用的so文件,然后将HCNetSDKCom文件夹下的组件库也copy到/usr/lib目录

方法二
修改etc目录下ld.so.conf文件中的配置。加上你自己的Java工程所需要的so文件的路径。然后调用ldconfig命令使配置生效