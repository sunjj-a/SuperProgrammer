#coding=GB18030
import CrawThread
from time import time

def spiderPaper():
    baseUrl = 'https://be02.bihu.com/bihube-pc/api/content/show/getArticle'
    urlCount = 160000
    urls = []
    datas = []
    for index in range(urlCount):
        data = {'artId':index}
        urls.append(baseUrl)
        datas.append(data)

    crawTread = CrawThread.CrawThread(threadNums = 4, delay = 2, proxy = None, retryNums = 10)
    crawTread.multiCraw(urls=urls, datas=datas)
    #crawTread.singleCraw(urls=urls, datas=datas)

import logging
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


    #print 'begin downloading...'
    logger.info("begin downloading...")
    startTime = time()
    spiderPaper()
    endTime = time()
    useTime = endTime - startTime
    #print 'useTime', useTime
    #print 'end downloading...'
    logger.info("begin downloading...")
    raw_input(u'抓取数据完毕，请按任意键结束^_^')