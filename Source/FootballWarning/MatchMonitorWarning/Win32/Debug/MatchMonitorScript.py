
# -*- coding: UTF-8 -*-

import urllib2
from lxml import etree

import sys
import codecs
reload(sys)
sys.setdefaultencoding('utf8')

#预警内容
monitorGameTeam = u'交战双方：%s\n'
warningContent = u'初盘监控：%s-%s-%s %s大于等于预警值%s'
monitorWebUrl = u'\n网站监控：%s'

#监控初盘网站，超出预警值给出提示
def matchMonintor(monitorUrl, hostWarningValue, guestWarningValue):
    #打开对应网址
    headers = {
        'User-Agent': 'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.99 Safari/537.36'}
    request = urllib2.Request(monitorUrl, None, headers)
    result = urllib2.urlopen(request)

    #获取HTML源文件
    sourcePage = etree.HTML(result.read())
    crownHostTeam = sourcePage.xpath('//*[@id="odds"]/tr[8]/td[3]/text()')
    ysbGuestTeam = sourcePage.xpath('//*[@id="odds"]/tr[15]/td[5]/text()')

    #初盘与预警值对比
    warningResult = ""
    if float(crownHostTeam[0]) >= hostWarningValue :
        crownKey = sourcePage.xpath('//*[@id="odds"]/tr[8]/td[1]/text()')
        beginKey = sourcePage.xpath('//*[@id="odds"]/tr[1]/td[3]/text()')
        teamKey = sourcePage.xpath('//*[@id="odds"]/tr[2]/td[1]/text()')
        warningResult += warningContent % (crownKey[0].strip(), beginKey[0], teamKey[0], crownHostTeam[0], hostWarningValue)

    # 初盘与预警值对比
    if float(ysbGuestTeam[0]) >= guestWarningValue:
        ysbKey = sourcePage.xpath('//*[@id="odds"]/tr[15]/td[1]/text()')
        beginKey = sourcePage.xpath('//*[@id="odds"]/tr[1]/td[3]/text()')
        teamKey = sourcePage.xpath('//*[@id="odds"]/tr[2]/td[3]/text()')
        warningResult += "\n" + warningContent % (ysbKey[0].strip(), beginKey[0], teamKey[0], ysbGuestTeam[0], guestWarningValue)

    if warningResult <> "":
        gameTeam = sourcePage.xpath('//*[@id="webmain"]/table[1]/tbody/tr[2]/td[1]/text()')
        print (monitorGameTeam % gameTeam[0]) + warningResult + (monitorWebUrl % monitorUrl)
    return warningResult

#matchMonintor('http://vip.win007.com/AsianOdds_n.aspx?id=1501960', 1.0, 0.91)