#coding=utf-8

from selenium import webdriver
import os

if __name__ == '__main__':
    #chromedriver = "C:/Program Files (x86)/Google/Chrome/Application/chromedriver.exe"
    #os.environ["webdriver.chrome.driver"] = chromedriver
    browser = webdriver.Chrome()
    browser.maximize_window()
    print (browser.get_window_size())
    #browser = webdriver.Firefox()


    browser.get("http://www.baidu.com")

    browser.implicitly_wait(0.5)

    browser.find_element_by_id("kw").send_keys(u"武汉")

    browser.find_element_by_id("su").click()

    eleList=browser.find_elements_by_xpath(".//*/h3/a")

    curHandle=browser.current_window_handle

    #searchList=dict()

    itemNum=len(eleList)

    #网页上边有固定菜单

    solidForm=browser.find_element_by_xpath(".//*[@id='head']/div[1]/div[1]")

    solidH=solidForm.size['height']

    for i in xrange(itemNum):

        #执行js代码自动滚动页面直到链接可见，否则报错
       # js="scroll(0,%s)"%(eleList[i].rect['y']-solidH)
        # browser.execute_script(js)

        try:

           browser.implicitly_wait(30)

           eleList[i].click()

        except:

           print "timeout"

           browser.close()

                         #selenium目前只能在新窗口打开网页，不能在新标签页中打开网页

           browser.switch_to_window(curHandle)


    all_handles=browser.window_handles

    #找到新开的窗口

    browser.switch_to_window(all_handles[-1])

    #searchList[browser.title]=browser.current_url

    print browser.title,":",browser.current_url

    browser.close()

    browser.switch_to_window(curHandle)

    browser.back()

    browser.quit()

    '''
    
    url = "http://www.baidu.com"
    browser.get(url)
    
    browser.find_element_by_id("kw").send_keys("python")
    
    browser.find_element_by_id("su").click()
    
    element_List = browser.find_elements_by_xpath(".//*/h3/a")
    itemNum = len(element_List)
    print 'length is :' + str(itemNum)
    
    for a in element_List:
        tmptitle = a.text
        if '菜鸟教程' in tmptitle:
            print  a.get_attribute('href')
            tmpurl = a.get_attribute('href')
            # a.click()
            browser.get(tmpurl)
            print 'I got it'
            break
    print browser.current_url
    print 'end'
    '''



    #browser.find_element_by_xpath("//*[@id=\"2\"]/h3[1]/a[1]").click()

    #browser.find_element_by_css_selector("#\32 > h3 > a").click()

    #browser.find_element_by_xpath("//*[@id=\"4\"]/div[2]/a[1]").click()

    browser.quit()

    '''
    http://www.baidu.com/link?url=GUtrEySd3PMBQ6tNMgECzhN5tfXIjBZjD93bsupFsdYYa2kvkS7Hvkxl-SUzrFtn8E5uZNqNTRCPDHwblOi9S_
    
    https://www.baidu.com/link?url=GUtrEySd3PMBQ6tNMgECzhN5tfXIjBZjD93bsupFsdYYa2kvkS7Hvkxl-SUzrFtn8E5uZNqNTRCPDHwblOi9S_&wd=&eqid=b2d35b3c000a8ba0000000035aef1896
    https://www.baidu.com/link?url=GUtrEySd3PMBQ6tNMgECzhN5tfXIjBZjD93bsupFsdYYa2kvkS7Hvkxl-SUzrFtn8E5uZNqNTRCPDHwblOi9S_&wd=&eqid=b2d35b3c000a8ba0000000035aef1896
    '''
