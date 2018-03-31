
# -*- coding: UTF-8 -*-

import urllib2
from lxml import etree

import sys
import codecs
reload(sys)
sys.setdefaultencoding('utf8')

#预警内容
warningContent = u'赔率监控：%sVS%s %s %s %s大于等于预警值%s \n网站监控：%s'

#监控赔率网站，超出预警值给出提示
def oddsMonintor(monitorUrl, warningValue):
    #打开对应网址
    headers = {
        'User-Agent': 'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.99 Safari/537.36'}
    request = urllib2.Request(monitorUrl, None, headers)
    result = urllib2.urlopen(request)

    #获取HTML源文件
    sourcePage = etree.HTML(result.read())
    gameOdds = sourcePage.xpath('//*[@id="odds"]/table/tr[1]/td[3]/text()')

    #赔率与预警值对比
    if float(gameOdds[0]) >= warningValue :
        hostTeam = sourcePage.xpath('//*[@id="odds2"]/table/tr[1]/td[1]/b/font/text()')
        guestTeam = sourcePage.xpath('//*[@id="odds2"]/table/tr[1]/td[3]/b/font/text()')
        dateTime = sourcePage.xpath('//*[@id="odds"]/table/tr[1]/td[1]/text()')
        pkResult = sourcePage.xpath('//*[@id="odds"]/table/tr[1]/td[2]/text()')
        return warningContent % (hostTeam[0], guestTeam[0], dateTime[0], pkResult[0], gameOdds[0], warningValue, monitorUrl)
    else:
        return ""

#oddsMonintor('http://vip.win007.com/changeDetail/handicap.aspx?id=1501960&companyID=12&l=0', 0.1)