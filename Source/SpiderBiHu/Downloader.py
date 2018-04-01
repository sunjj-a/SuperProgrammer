#coding=GB18030
import requests
import random
import time
from RandomAgent import randomAgent
import logging

module_logger = logging.getLogger("mainModule.sub")

class Downloader:

    def __init__(self, delay = 3, proxy = None, retryNums = 3):
        self.delay = delay
        self.proxy = proxy
        self.retryNums = retryNums


    def downloadGet(self, url, data, proxy):
        module_logger.info(data)
        headers = {'User-agent': randomAgent()}

        try:
            time.sleep(random.choice(range(self.delay, self.delay + 1)))
            result = requests.get(url=url, params=data, headers=headers, proxies = proxy, timeout=500.0)
        except requests.exceptions.ConnectionError as exception:
            result = None
            if self.retryNums > 0:
                time.sleep(self.delay)
                self.retryNums -= 1
                result = self.downloadGet(url, data, proxy)
            else:
                module_logger.info(exception)
        return result


    def downloadPost(self, url, data, proxy):
        module_logger.info(data)
        headers = {'User-agent': randomAgent()}

        try:
            time.sleep(random.choice(range(self.delay, self.delay + 1)))
            result = requests.post(url=url, params=data, headers=headers, proxies = proxy, verify=False, timeout=500.0)
        except requests.exceptions.ConnectionError as exception:
            result = None
            if self.retryNums > 0:
                time.sleep(self.delay)
                self.retryNums -= 1
                result = self.downloadPost(url, data, proxy)
            else:
                module_logger.info(exception)
        return result