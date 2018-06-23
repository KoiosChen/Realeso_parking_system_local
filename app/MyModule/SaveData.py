from . import MyExcel


def save_virtual_data(title, **rows):
    """

    Args:
        title:
        rows:

    Returns: the end position of row and column

    """
    we = MyExcel.MyExcel(**rows)
    wb = we.open_workbook()
    if rows['c_l'] == 'c':
        ws = wb.active
    else:
        ws = wb.create_sheet(0)
    ws.title = title
    we.write_data(ws)
    out = we.save_virtual_excel(wb)
    print(title, 'saved')
    # return row, col
    return out


def save_virtual_data2(print_data, **rows):
    """

    Args:
        title:
        rows:

    Returns: the end position of row and column

    """
    we = MyExcel.MyExcel(**rows)
    wb = we.open_workbook()
    c_l = rows['c_l']
    for key, value in print_data.items():
        if c_l == 'c':
            ws = wb.active
        else:
            ws = wb.create_sheet(0)
        ws.title = key
        we.data = value
        we.write_data(ws)
        print(key, 'saved')
        we.col_start = 1
        we.row_start = 1
        c_l = 'l'
    out = we.save_virtual_excel(wb)

    # return row, col
    return out


def save_data(title, **rows):
    """

    Args:
        title: the sheet's name
        rows:

    Returns: the end position of row and column

    """
    we = MyExcel.MyExcel(**rows)
    wb = we.open_workbook()
    if rows['c_l'] == 'c':
        ws = wb.active
    else:
        ws = wb.create_sheet(0)
    ws.title = title
    row, col = we.write_data(ws)
    we.save_excel(wb)
    print(title, 'saved')
    return row, col