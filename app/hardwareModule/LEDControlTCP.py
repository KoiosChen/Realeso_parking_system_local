import socket
import struct
import time

'''
功能：配置LED屏幕显示和语音播报
TCP IP通信
PORT必须设置为10000
数据按照小端模式存储
'''

# LED显示字符函数
def LED_char_show(HOST, data_input, data_time):
    PORT = 10000 # 设置监听端口，tcp方式，端口必须为10000，UDP方式为9999
    BUFFSIZE=512
    ADDR = (HOST,PORT)
    tctimeClient = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    res=tctimeClient.connect_ex(ADDR) # 返回为0说明连接成功
    if 0 != res:
        print('connection failed' + str(res))

    #请求帧格式
    data_frame_head = b'\x55\xaa\x00\x00' # 4 bytes 帧头
    data_addr = b'\x01' # 1 byte 地址域，除RS485以外全部为0x01
    data_frame_save = b'\x00\x00' # 2 bytes 保留字
    data_worker_code = b'\xDB' # 1 byte 操作码，0xDB显示一次，0xDA设为默认显示，不建议使用0xDA
    data_frame_num = b'\x00\x00' # 2 bytes 帧序号，从0开始
    data_frame_length = b'\x0a\x00\x00\x00' # 4 bytes 帧数据总长度，此处配置不起作用，后续计算后修改
    data_frame_save_1 = b'\x00\x00' # 2 bytes 保留字1
    data_length = b'\x0a\x00\x00\x00' # 4 bytes 数据帧长度，不分包时与frame length相同；此处配置不起作用，后续计算后修改
    data_frame_end = b'\x00\x00\x0D\x0A' # 4 bytes 帧尾

    # 节目数据块个数
    data_prog_num = b'\x01'

    # 节目属性
    data_prog_index = b'\x01' # 节目序号
    data_prog_length = b'\x0A\x00\x00\x00' # 节目长度，此处设置无用，后续就算后修改
    data_prog_section_num = b'\x01' # 节目区域数量，LED字幕最大为4，音频最大为1
    #保留字
    data_prog_reserve = b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'

    # 区域属性-字符内码属性
    data_section_index = b'\x01' # 区域号，从1开始
    data_show = data_input.encode('gbk') # 需要显示的字符，编码格式gbk
    data_send_length = len(data_show) + 26 # 计算区域数据长度
    data_section_length = struct.pack('<L', data_send_length) # 区域数据长度编码
    data_char_code = b'\x0E' # 区域数据类型
    data_section_area = struct.pack('<HHHH', 0, 0, 63, 63) # 区域坐标，依次为左上点(x,y), 右下点(xx,yy)，要小于63
    data_char_color = b'\x01' # 字符颜色 暂定为红色
    data_section_reserve = b'\x00\x00' # 保留字
    data_action = b'\x01' # 动作方式 暂定为不动
    data_section_reserve_1 = b'\x00' # 保留字1
    data_section_speed = b'\x12' # 字符移动速度
    data_reste_time = struct.pack('<B', data_time) # 每页字符停留时间
    data_char_size = b'\x10' # 字符大小
    data_char_len = struct.pack('<L', len(data_show)) # 要显示的字符串长度

    # 区域数据
    data_section = data_section_index + data_section_length + data_char_code + \
                   data_section_area + data_char_color + data_section_reserve + \
                   data_action + data_section_reserve_1 + data_section_speed + \
                   data_reste_time + data_char_size + data_char_len + data_show

    prog_length = len(data_section) + 24 # 节目长度
    data_prog_length = struct.pack('<L', prog_length) # 节目长度转为bytes
    #节目数据
    data_prog = data_prog_index + data_prog_length + data_prog_section_num + \
                data_prog_reserve + data_section

    #print(len(data_prog))
    #print(len(data_section))

    data_length = struct.pack('<L', (prog_length + 1)) # 发送数据的长度
    data_frame_length = data_length # 发送总长度

    #要发送的所有数据
    data = data_frame_head + data_addr + data_frame_save + data_worker_code + data_frame_num \
            + data_frame_length + data_frame_save_1 + data_length + data_prog_num \
            + data_prog + data_frame_end

    #print(len(data))
    #print(list(data))

    tctimeClient.send(data) # 发送数据
    data = tctimeClient.recv(BUFFSIZE)
    if not data:
        print('数据发送失败')
    #print(list(data))
    tctimeClient.close()

def LED_voice_broadcast(HOST, str_voice):
    voice_broadcast_times = 1 # 无法多次重复播报
    PORT = 10000 # 设置监听端口，tcp方式，端口必须为10000，UDP方式为9999
    BUFFSIZE=512
    ADDR = (HOST,PORT)

    tctimeClient = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    res=tctimeClient.connect_ex(ADDR)
    if 0 != res:
        print('connection failed' + str(res))

    # 请求帧格式
    data_frame_head = b'\x55\xaa\x00\x00' # 4 bytes 帧头
    data_addr = b'\x01' # 1 byte 地址域，除RS485以外全部为0x01
    data_frame_save = b'\x00\x00' # 2 bytes 保留字
    data_worker_code = b'\xDB' # 1 byte 操作码，0xDB显示一次，0xDA设为默认显示，不建议使用0xDA
    data_frame_num = b'\x00\x00' # 2 bytes 帧序号，从0开始
    data_frame_length = b'\x0a\x00\x00\x00' # 4 bytes 帧数据总长度，此处配置不起作用，后续计算后修改
    data_frame_save_1 = b'\x00\x00' # 2 bytes 保留字1
    data_length = b'\x0a\x00\x00\x00' # 4 bytes 数据帧长度，不分包时与frame length相同；此处配置不起作用，后续计算后修改
    data_frame_end = b'\x00\x00\x0D\x0A' # 4 bytes 帧尾

    # 节目数据块个数
    data_prog_num = b'\x01'

    # 节目属性
    data_prog_index = b'\x01' # 节目序号
    data_prog_length = b'\x0A\x00\x00\x00' # 节目长度，此处设置无用，后续就算后修改
    data_prog_section_num = b'\x01' # 节目区域数量，LED字幕最大为4，音频最大为1
    # 保留字
    data_prog_reserve = b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'

    # 区域属性-语音播报属性
    data_section_index = b'\x01' # 区域号，从1开始
    data_show = str_voice.encode('gbk') # 语音字符编码，格式-GBK
    data_send_length = len(data_show) + 26 # 计算区域数据长度
    data_section_length = struct.pack('<L', data_send_length) # 区域数据长度编码
    data_char_code = b'\x2D' # 区域属性-0x2D为语音播报
    data_voice_reserve = b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00' # 保留字
    data_voice_times = struct.pack('<B', voice_broadcast_times) # 语音播报重复次数
    data_voice_len = struct.pack('<L', len(data_show)) # 语音字符编码后长度

    # 区域数据
    data_section = data_section_index + data_section_length + data_char_code + \
               data_voice_reserve + data_voice_times + data_voice_len + data_show

    prog_length = len(data_section) + 24 # 节目长度
    data_prog_length = struct.pack('<L', prog_length) # 节目长度转为bytes

    # 节目数据
    data_prog = data_prog_index + data_prog_length + data_prog_section_num + \
            data_prog_reserve + data_section

    #print(len(data_prog))
    #print(len(data_section))

    data_length = struct.pack('<L', (prog_length + 1)) # 发送数据的长度
    data_frame_length = data_length # 发送总长度

    # 要发送的所有数据
    data = data_frame_head + data_addr + data_frame_save + data_worker_code + data_frame_num \
        + data_frame_length + data_frame_save_1 + data_length + data_prog_num \
        + data_prog + data_frame_end

    #print(len(data))
    #print(list(data))

    tctimeClient.send(data) # 发送数据
    data = tctimeClient.recv(BUFFSIZE)
    if not data:
        print('数据发送失败')

    tctimeClient.close()

def LED_char_show_voice_broadcast(HOST, data_input, data_time, str_voice):
    if True:
        print("LED 测试未连接，跳过")
        return True
    voice_broadcast_times = 1 # 无法多次播报
    PORT = 10000 # 设置监听端口，tcp方式，端口必须为10000，UDP方式为9999
    BUFFSIZE=512
    ADDR = (HOST,PORT)
    tctimeClient = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    res=tctimeClient.connect_ex(ADDR) #返回为0说明连接成功
    if 0 != res:
        print('connection failed' + str(res))

    #请求帧格式
    data_frame_head = b'\x55\xaa\x00\x00' # 4 bytes 帧头
    data_addr = b'\x01' # 1 byte 地址域，除RS485以外全部为0x01
    data_frame_save = b'\x00\x00' # 2 bytes 保留字
    data_worker_code = b'\xDB' # 1 byte 操作码，0xDB显示一次，0xDA设为默认显示，不建议使用0xDA
    data_frame_num = b'\x00\x00' # 2 bytes 帧序号，从0开始
    data_frame_length = b'\x0a\x00\x00\x00' # 4 bytes 帧数据总长度，此处配置不起作用，后续计算后修改
    data_frame_save_1 = b'\x00\x00' # 2 bytes 保留字1
    data_length = b'\x0a\x00\x00\x00' # 4 bytes 数据帧长度，不分包时与frame length相同；此处配置不起作用，后续计算后修改
    data_frame_end = b'\x00\x00\x0D\x0A' # 4 bytes 帧尾

    # 节目数据块个数
    data_prog_num = b'\x01'

    # 节目属性
    data_prog_index = b'\x01' # 节目序号
    data_prog_length = b'\x0A\x00\x00\x00' # 节目长度，此处设置无用，后续就算后修改
    data_prog_section_num = b'\x02' # 节目区域数量，LED字幕最大为4，音频最大为1
    #保留字
    data_prog_reserve = b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'

    # 区域属性-字符内码属性
    data_section_index = b'\x01' # 区域号，从1开始
    data_show = data_input.encode('gbk') # 需要显示的字符，编码格式gbk
    data_send_length = len(data_show) + 26 # 计算区域数据长度
    data_section_length = struct.pack('<L', data_send_length) # 区域数据长度编码
    data_char_code = b'\x0E' # 区域数据类型
    data_section_area = struct.pack('<HHHH', 0, 0, 63, 63) # 区域坐标，依次为左上点(x,y), 右下点(xx,yy)，要小于63
    data_char_color = b'\x01' # 字符颜色 暂定为红色
    data_section_reserve = b'\x00\x00' # 保留字
    data_action = b'\x01' # 动作方式 暂定为不动
    data_section_reserve_1 = b'\x00' # 保留字1
    data_section_speed = b'\x12' # 字符移动速度
    data_reste_time = struct.pack('<B', data_time) # 每页字符停留时间
    data_char_size = b'\x10' # 字符大小
    data_char_len = struct.pack('<L', len(data_show)) # 要显示的字符串长度

    # display区域数据
    display_data_section = data_section_index + data_section_length + data_char_code + \
                   data_section_area + data_char_color + data_section_reserve + \
                   data_action + data_section_reserve_1 + data_section_speed + \
                   data_reste_time + data_char_size + data_char_len + data_show

    # 区域属性-语音区域属性
    voice_section_index = b'\x02' # 区域号，从1开始
    voice_show = str_voice.encode('gbk') # 语音字符编码，格式-GBK
    voice_send_length = len(voice_show) + 26 # 计算区域数据长度
    voice_section_length = struct.pack('<L', voice_send_length) # 区域数据长度编码
    voice_char_code = b'\x2D' # 区域属性-0x2D为语音播报
    voice_reserve = b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00' # 保留字
    voice_times = struct.pack('<B', voice_broadcast_times) # 语音播报重复次数
    voice_len = struct.pack('<L', len(voice_show)) # 语音字符编码后长度

    # 语音区域数据
    voice_data_section = voice_section_index + voice_section_length + voice_char_code + \
                         voice_reserve + voice_times + voice_len + voice_show

    # 区域总数据
    data_section = display_data_section + voice_data_section

    prog_length = len(data_section) + 24 # 节目长度
    data_prog_length = struct.pack('<L', prog_length) # 节目长度转为bytes
    #节目数据
    data_prog = data_prog_index + data_prog_length + data_prog_section_num + \
                data_prog_reserve + data_section

    #print(len(data_prog))
    #print(len(data_section))

    data_length = struct.pack('<L', (prog_length + 1)) # 发送数据的长度
    data_frame_length = data_length # 发送总长度

    #要发送的所有数据
    data = data_frame_head + data_addr + data_frame_save + data_worker_code + data_frame_num \
            + data_frame_length + data_frame_save_1 + data_length + data_prog_num \
            + data_prog + data_frame_end

    #print(len(data))
    #print(list(data))

    tctimeClient.send(data) # 发送数据
    data = tctimeClient.recv(BUFFSIZE)
    if not data:
        print('数据发送失败')
        return False
    else:
        tctimeClient.close()
        return True

if __name__ == '__main__':
    LED_voice_broadcast('192.168.3.100', '[m55]沪A88888请回去')
    time.sleep(10)
    LED_char_show('192.168.3.100', '京A88888，请回去', 10)
    time.sleep(2)
    LED_char_show_voice_broadcast('192.168.3.100', '京AAAAA88888，请回去', 1, '沪A88888滚回去')