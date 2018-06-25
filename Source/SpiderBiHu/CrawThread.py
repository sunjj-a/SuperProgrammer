#coding=GB18030

import Downloader
import threading
import time
import json
import logging
import requests
from RandomAgent import randomAgent
from datetime import datetime

SLEEP_TIME = 1
CHECK_COUNT = 100 #连续100个文章无效，认为非法
USER_TIME = 60 * 60 * 12 #12小时

module_logger = logging.getLogger("mainModule.sub")

def get_random_ip():
    proxyUrl = 'http://api.ip.data5u.com/dynamic/get.html?order=2e8bd3b557c089c65fbc15b6301d25c7&sep=3'
    headers = {'User-agent': randomAgent()}
    result = requests.get(url=proxyUrl, timeout=500.0)
    proxies = {'https': 'http://' + result.content.replace('\n', ''),
               'http': 'http://' + result.content.replace('\n', '')}
    return proxies

def isOverUserTime(currentTime, createTime):
    currentTime = currentTime
    createTime = datetime.fromtimestamp(createTime / 1000)

    diffTime = (currentTime - createTime).seconds
    if diffTime > USER_TIME:
        return True
    else:
        return False


class CrawThread:

    def __init__(self, threadNums = 1, delay = 3, proxy = None, retryNums = 3):
        self.threadNums = threadNums
        self.delay = delay
        self.proxy = proxy
        self.retryNums = retryNums
        self.curTime = datetime.now()

    #multi thread craw
    def multiCraw(self, urls, datas, callback = None):
        crawUrlQueue = urls
        crawDataQueue = datas

        def processCore(proxy):
            while True:
                try:
                    url = crawUrlQueue.pop()
                    data = crawDataQueue.pop()
                except IndexError:
                    print 'IndexError'
                    break
                else:
                    download = Downloader.Downloader(delay=self.delay, proxy=self.proxy, retryNums=self.retryNums)
                    result = download.downloadPost(url=url, data=data, proxy=proxy)
                    if result and callback:
                        spiderResult = [(data, result.content)]
                        callback(spiderResult)
                        time.sleep(SLEEP_TIME)
                    break

        threads = []
        while threads or (crawUrlQueue and crawDataQueue):
            for thread in threads:
                if not thread.isAlive():
                    #remove stopped thread
                    threads.remove(thread)

            proxy = get_random_ip()
            while len(threads) < self.threadNums and (crawUrlQueue and crawDataQueue):
                thread = threading.Thread(target=processCore, args=(proxy,))
                thread.setDaemon(True)
                thread.start()
                threads.append(thread)

            time.sleep(SLEEP_TIME)
            threads = []

    #single thread craw
    def singleCraw(self, urls, datas, callback = None):
        curDataTime = datetime.now()
        nInvalidPaperCount = 0
        for nIndex in range(len(urls)):
            url = urls[nIndex]
            data = datas[nIndex]
            download = Downloader.Downloader(delay=self.delay, retryNums=self.retryNums)
            result = download.downloadPost(url=url, data=data, proxy=None)
            if result and callback:
                jsonResult = json.loads(result.content)
                if jsonResult['res'] == 1:
                    createTime = jsonResult['data']['creatime']
                    if isOverUserTime(self.curTime, createTime) == False:
                        break
                elif jsonResult['res'] == 100004:
                    nInvalidPaperCount += 1
                    if nInvalidPaperCount >= CHECK_COUNT:
                        break
                else:
                    nInvalidPaperCount = 0
                spiderResult = [(data, result.content)]
                callback(spiderResult)
                time.sleep(SLEEP_TIME)
            time.sleep(SLEEP_TIME)