#coding=GB18030
import requests
import random
import time
from RandomAgent import randomAgent
import logging

module_logger = logging.getLogger("mainModule.sub")

def get_random_ip():
    #print("正在设置随机代理...")
    #http://api.ip.data5u.com/dynamic/get.html?order=2e8bd3b557c089c65fbc15b6301d25c7&sep=3
    proxyUrl = 'http://api.ip.data5u.com/dynamic/get.html?order=2e8bd3b557c089c65fbc15b6301d25c7&sep=3'
    headers = {'User-agent': randomAgent()}
    result = requests.get(url=proxyUrl, headers=headers)
    proxies = {'https': 'https://' + result.content.replace('\n', ''),
               'http': 'http://' + result.content.replace('\n', '')}
    print("代理设置成功.")
    return proxies


class Downloader:

    def __init__(self, delay = 3, proxy = None, retryNums = 3):
        self.delay = delay
        self.proxy = proxy
        self.retryNums = retryNums


    def downloadGet(self, url, data):
        print 'downloading:', url
        headers = {'User-agent': randomAgent()}
        proxy = self.proxy

        try:
            time.sleep(random.choice(range(2, self.delay)))
            result = requests.get(url=url, params=data, headers=headers)
        except requests.exceptions.ConnectionError as exception:
            print 'download error:', exception
            result = None
            if self.retryNums > 0:
                if hasattr(exception, 'code') and 500 <= exception.code < 600:
                    self.retryNums -= 1
                    result = self.downloadGet(url, data)
        return result


    def downloadPost(self, url, data, proxy):
        #print 'downloading url:', url
        #print 'downloading data:', data
        module_logger.info(data)
        #module_logger.info(data)

        headers = {'User-agent': randomAgent()}

        try:
            time.sleep(random.choice(range(1, self.delay)))
            #proxy = get_random_ip()
            #print 'proxy:', proxy
            #module_logger.info(proxy)
            result = requests.post(url=url, params=data, headers=headers, proxies = proxy, verify=False, timeout=500.0)
        except requests.exceptions.ConnectionError as exception:
            result = None
            #print 'self.retryNums', self.retryNums
            if self.retryNums > 0:
                #if hasattr(exception, 'code') and 500 <= exception.code < 600:
                #print 'retry downloading...'
                time.sleep(2.0)
                self.retryNums -= 1
                result = self.downloadPost(url, data, proxy)
            else:
                #print 'download error:', exception
                module_logger.info(exception)
        return result