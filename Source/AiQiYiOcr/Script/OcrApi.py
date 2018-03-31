
import cookielib
import urllib2
import urllib

def ocr():
    headers = {
        'Accept':'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8',
        'Accept-Encoding': 'gzip, deflate',
        'Accept-Language': 'zh-CN,zh;q=0.8',
        'Cache-Control': 'max-age=0',
        'Connection': 'keep-alive',
        'Host': 'i.vip.iqiyi.com',
        'Cookie': 'P00001=2ex7Flm3iqxFqAWOuPzQqWkYANF9m3gaYTm2Ej5m2m2K61VhFTpv0m2ZVNTyhNjgcdzy4ojcd9',
        'Upgrade-Insecure-Requests' : '1',
        'User-Agent': 'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/61.0.3163.100 Safari/537.36'
    }

    filename = 'cookie.txt'
    cookieJar = cookielib.MozillaCookieJar()

    cookieJar.load(filename, ignore_discard=True, ignore_expires=True)
    cookie_support = urllib2.HTTPCookieProcessor(cookieJar)
    opener = urllib2.build_opener(cookie_support, urllib2.HTTPHandler)
    response = opener.open(r'http://d.weibo.com/')
    print response.read()
    #opener.addheaders.append(('Cookie', 'P00001=2ex7Flm3iqxFqAWOuPzQqWkYANF9m3gaYTm2Ej5m2m2K61VhFTpv0m2ZVNTyhNjgcdzy4ojcd9'))
    #
    # urllib2.install_opener(opener)

    #cookie = cookielib.CookieJar()
    #handler = urllib2.HTTPCookieProcessor(cookie)
    #opener = urllib2.build_opener(handler)

    #values = {}
    #values['t'] = "0.434870029613861"
    #data = urllib.urlencode(values)

    #response = opener.open(r'http://d.weibo.com/')
    #for item in cookie:
    #    print 'Name = ' + item.name
    #    print 'Value = ' + item.value

    #loginParam = {'t': 0.85478346449933}
    #postData = urllib.urlencode(loginParam)
    #request = urllib2.Request("http://i.vip.iqiyi.com/order/gvc.action", postData, headers=headers)
    #cookieJar.add_cookie_header(request)
    #result = urllib2.urlopen(request)

    #urllib2.urlopen("http://i.vip.iqiyi.com/order/gvc.action?t=0.854783464499331")
    #cookieJar.save(ignore_discard=True, ignore_expires=True)
    #print opener.

ocr()
