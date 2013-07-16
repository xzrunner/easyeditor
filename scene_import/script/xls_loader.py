# -*- coding: utf-8 -*-

import xlrd

def load_xls(filename):
    bk = xlrd.open_workbook(filename)
    sheets = []

    for idx in range(bk.nsheets):
        line_tokens = []
        sheet = bk.sheet_by_index(idx)
        for row in xrange(sheet.nrows):
            values = sheet.row_values(row)
            line_tokens.append(values)
        sheets.append((sheet.name, line_tokens))

    return sheets
