#coding=utf-8
import CrawThread
import sqlite3
import json
import time
import logging

SPIDERCOUNT = 7000 #默认爬取7000+数据
RESULTCOUNT = 100

def timeToStr(timeStamp):
    timeArray = time.localtime(int(timeStamp / 1000))
    return time.strftime("%Y-%m-%d %H:%M:%S", timeArray)

def callback(spiderResult):
    #if spiderResult and len(spiderResult) >= RESULTCOUNT:
    if True:
        connect = sqlite3.connect('SpideBihu.db')
        cursor = connect.cursor()

        papeInfos = []
        curTimeStamp = int(round(time.time() * 1000))

        for result in spiderResult:
            jsonResult = json.loads(result[1])
            if jsonResult['resMsg'] == 'success':
                papeInfo = []
                papeInfo.append(result[0]['artId'])
                papeInfo.append(jsonResult['data']['title'])
                papeInfo.append(result[0]['artId'])
                papeInfo.append(jsonResult['data']['userName'])
                papeInfo.append(jsonResult['data']['userId'])
                papeInfo.append(jsonResult['data']['creatime'])
                papeInfo.append(timeToStr(jsonResult['data']['creatime']))
                papeInfo.append(jsonResult['data']['boardName'])
                papeInfo.append(jsonResult['data']['money'])
                papeInfo.append(jsonResult['data']['ups'])
                papeInfo.append(jsonResult['data']['downs'])
                papeInfo.append(jsonResult['data']['cmts'])
                papeInfo.append(timeToStr(curTimeStamp))
                papeInfos.append(tuple(papeInfo))

        sqlStr = '''INSERT INTO BiHuInfo (paperId, title, paperUrl, userName, userUrl, creatime, creatimeStr, boardName, money, ups, downs, cmts, spiderTime) \
              VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)'''
        cursor.executemany(sqlStr, papeInfos)

        connect.commit()
        connect.close()

        return True
    return False


def maxTitleId():
    connect = sqlite3.connect('SpideBihu.db')
    cursor = connect.cursor()
    sqlStr = '''SELECT max(paperId) FROM BiHuInfo'''
    cursor.execute(sqlStr)

    maxId = 0
    results = cursor.fetchone()
    if results[0]:
        maxId = results[0] + 1

    connect.commit()
    connect.close()
    return maxId


def spiderPaper():
    baseUrl = 'https://be02.bihu.com/bihube-pc/api/content/show/getArticle'
    maxId = maxTitleId()
    urlCount = maxId + SPIDERCOUNT
    urls = []
    datas = []
    for index in range(maxId, urlCount + 1, 1):
        data = {'artId':index}
        urls.append(baseUrl)
        datas.append(data)

    crawTread = CrawThread.CrawThread(threadNums = 4, delay = 6, proxy = None, retryNums = 12)
    #crawTread.multiCraw(urls=urls, datas=datas, callback=callback)
    crawTread.singleCraw(urls=urls, datas=datas, callback=callback)


if __name__ == '__main__':

    logger = logging.getLogger("mainModule")
    logger.setLevel(level=logging.INFO)
    handler = logging.FileHandler("log.txt")
    handler.setLevel(logging.INFO)
    formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
    handler.setFormatter(formatter)

    console = logging.StreamHandler()
    console.setLevel(logging.INFO)
    console.setFormatter(formatter)

    logger.addHandler(handler)
    logger.addHandler(console)


    logger.info("begin downloading...")
    startTime = time.time()
    spiderPaper()
    endTime = time.time()
    useTime = endTime - startTime
    logger.info("use time: " + str(useTime))
    logger.info("begin downloading...")
    raw_input((u'抓取数据完毕，请按任意键结束^_^').encode("gbk"))