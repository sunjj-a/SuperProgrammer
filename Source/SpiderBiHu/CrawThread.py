#coding=GB18030

import Downloader
import threading
import time
import json

SLEEP_TIME = 1

import requests
from RandomAgent import randomAgent

import logging

module_logger = logging.getLogger("mainModule.sub")

def get_random_ip():
    #print("正在设置随机代理...")
    proxyUrl = 'http://api.ip.data5u.com/dynamic/get.html?order=2e8bd3b557c089c65fbc15b6301d25c7&sep=3'
    headers = {'User-agent': randomAgent()}
    result = requests.get(url=proxyUrl, timeout=500.0)
    proxies = {'https': 'http://' + result.content.replace('\n', ''),
               'http': 'http://' + result.content.replace('\n', '')}
    #print("代理设置成功.")
    return proxies

class CrawThread:

    def __init__(self, threadNums = 1, delay = 3, proxy = None, retryNums = 3):
        self.threadNums = threadNums
        self.delay = delay
        self.proxy = proxy
        self.retryNums = retryNums

    #multi thread craw
    def multiCraw(self, urls, datas):
        crawUrlQueue = urls
        crawDataQueue = datas


        spiderResult = []
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
                    if result:
                        #print result.text
                        module_logger.info(result.text)
                    spiderResult.append(result)
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

            #for thread in threads:
            #    thread.join(4.0)
            time.sleep(SLEEP_TIME)
            threads = []

        return spiderResult

    #single thread craw
    def singleCraw(self, urls, datas):
        crawUrlQueue = urls
        crawDataQueue = datas
        spiderResult = []
        while True:
            try:
                url = crawUrlQueue.pop()
                data = crawDataQueue.pop()
            except IndexError:
                print 'IndexError'
                break
            else:
                proxy = get_random_ip()
                download = Downloader.Downloader(delay=self.delay, proxy=self.proxy, retryNums=self.retryNums)
                result = download.downloadPost(url=url, data=data, proxy=proxy)
                if result:
                    print result.text
                spiderResult.append(result)
                time.sleep(SLEEP_TIME)
                print 'wait someTime...'

        return spiderResult