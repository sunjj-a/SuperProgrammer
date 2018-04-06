#coding=utf-8

from selenium import webdriver
from selenium import common
import time
import random
import ConfigParser

MAX_COUNT = 70
SLEEP_TIME = 60 * 15

def initDriver():
    print "initialise web driver..."
    driver = webdriver.PhantomJS()
    return driver

def unInitDriver(driver):
    print "uninitialise web driver..."
    driver.quit()


nCount = 1

def voteBlog(driver, url, userName, password):
    try:
        driver.get(url)

        #针对异常页面（文章删除/账号异常）强制校验
        driver.find_element_by_partial_link_text("转载")

        time.sleep(random.choice(range(45, 55)))
        driver.find_element_by_link_text("收藏").click()

        print "loging sina blog..."
        time.sleep(random.choice(range(5, 10)))
        driver.find_element_by_id("login_name_d").send_keys(userName)
        driver.find_element_by_id("login_pass_d").send_keys(password)
        driver.find_element_by_xpath("//*[@id=\"login_button\"]/cite").click()
    except common.exceptions.NoSuchElementException:
        print "islogined..."
        #return

    time.sleep(random.choice(range(5, 10)))


    driver.save_screenshot('E:\\' + str(nCount) + '.png')
    global nCount
    nCount += 1



def loadUserData():
    config = ConfigParser.ConfigParser()
    config.read("UserData\\userData.ini")
    userName = config.get("section", "userName")
    passWord = config.get("section", "passWord")
    return (userName, passWord)

def loadVisitedData():
    visitedUrls = set()
    visitedFile = open("UserData\\visitedUrls.txt", "r")
    urlLines = visitedFile.readlines()
    for urlLine in urlLines:
        urlLine = urlLine.strip()
        visitedUrls.add(urlLine)
    return visitedUrls

def loadSpiderData():
    spiderUrls = []
    spiderFile = open("UserData\\spiderUrls.txt", "r")
    urlLines = spiderFile.readlines()
    for urlLine in urlLines:
        urlLine = urlLine.strip()
        spiderUrls.append(urlLine)
    return spiderUrls

def saveVisitedData(url):
    visitedFile = open("UserData\\visitedUrls.txt", "a+")
    visitedFile.write(url)
    visitedFile.write("\n")

def spiderSinaBlogCore():
    visitedUrls = loadVisitedData()
    spiderUrls = loadSpiderData()
    userData = loadUserData()

    votedBlogCount = 1
    driver = initDriver()
    for url in spiderUrls:
        if url not in visitedUrls:
            print 'vote bolg url:', url
            if votedBlogCount <= MAX_COUNT:
                votedBlogCount += 1
                saveVisitedData(url)
                voteBlog(driver=driver, url=url, userName=userData[0], password=userData[1])
            else:
                time.sleep(SLEEP_TIME)
                votedBlogCount = 1

    unInitDriver(driver=driver)


if __name__ == '__main__':
    spiderSinaBlogCore()
    raw_input ((u'收藏结束^_^...\n按任意键退出...').encode("gbk"))