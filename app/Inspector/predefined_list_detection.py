import re

def white_list_check(number_plate):
    # 如果是警车或者无中文字符的军车，直接放行
    # 要确认摄像头传输的车牌里是否含车牌颜色的中文
    if re.search('WJ', number_plate) or re.search('警', number_plate) or not re.findall("[\u4e00-\u9fa5]+", number_plate):
        return True
    else:
        return False