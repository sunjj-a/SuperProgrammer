#coding=utf-8

import sys
import os
import time
import sqlite3
from openpyxl import Workbook
from openpyxl import utils

def translateTime(userTime):
    timeArray = time.strptime(userTime, "%Y-%m-%d %H:%M:%S")
    return int(time.mktime(timeArray) * 1000)

def queryUserData(startTime, endTime):
    selStartTime = translateTime(startTime)
    selEndTime = translateTime(endTime)

    newExcel = Workbook()
    workSheet = newExcel.active
    workSheet["A1"] = u"文章名"
    workSheet["B1"] = u"文章链接"
    workSheet["C1"] = u"作者名"
    workSheet["D1"] = u"作者链接"
    workSheet["E1"] = u"发布时间"
    workSheet["F1"] = u"所在板块"
    workSheet["G1"] = u"金钱数"
    workSheet["H1"] = u"赞数"
    workSheet["I1"] = u"踩数"
    workSheet["J1"] = u"评论数"


    sqlStr = '''SELECT * FROM BiHuInfo WHERE creatime >=''' + str(selStartTime) +  ''' and creatime <= ''' + str(selEndTime)
    connect = sqlite3.connect('SpideBihu.db')
    cursor = connect.cursor()
    cursor = connect.execute(sqlStr)
    userDatas = cursor.fetchall()

    for nIndex in range(len(userDatas)):
        try:
            workSheet["A%d" % (nIndex + 2)] = userDatas[nIndex][1]
        except utils.exceptions.IllegalCharacterError:
            workSheet["A%d" % (nIndex + 2)] = ''
        else:
            workSheet["B%d" % (nIndex + 2)] = "https://www.bihu.com/article/" + str(userDatas[nIndex][2])
            workSheet["C%d" % (nIndex + 2)] = userDatas[nIndex][3]
            workSheet["D%d" % (nIndex + 2)] = "https://www.bihu.com/people/" + str(userDatas[nIndex][4])
            workSheet["E%d" % (nIndex + 2)] = userDatas[nIndex][6]
            workSheet["F%d" % (nIndex + 2)] = userDatas[nIndex][7]
            workSheet["G%d" % (nIndex + 2)] = userDatas[nIndex][8]
            workSheet["H%d" % (nIndex + 2)] = userDatas[nIndex][9]
            workSheet["I%d" % (nIndex + 2)] = userDatas[nIndex][10]
            workSheet["J%d" % (nIndex + 2)] = userDatas[nIndex][11]


    newExcel.save(u"查询结果.xlsx")
    connect.close()


if __name__ == '__main__':
    print u'请输入类似时间格式：', u'2018-04-01 13:34:23'
    startTime = raw_input((u'起止时间：').encode("gbk"))
    endTime = raw_input((u'终止时间：').encode("gbk"))
    queryUserData(startTime, endTime)
    raw_input ((u'数据查询结束^_^\n按任意键退出...').encode("gbk"))
