

# -*- coding: utf-8 -*-
from selenium import webdriver

driver = webdriver.Chrome()
driver.maximize_window()
driver.implicitly_wait(8) 

driver.get("https://www.baidu.com")
driver.quit()