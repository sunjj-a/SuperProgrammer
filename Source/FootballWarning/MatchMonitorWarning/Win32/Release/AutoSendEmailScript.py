# -*- coding: UTF-8 -*-

import smtplib
from email.mime.text import MIMEText

import sys
import codecs
reload(sys)
sys.setdefaultencoding('utf8')

#发送Email 简略信息 详细信息
def sendEmail(headerInfo, emailContent, receiver):
    mail_host = "smtp.qq.com" #代理服务器
    mail_user = "898508832@qq.com" #用户名
    mail_pass = "tdzpomfsnsebbecf" #密码

    message = MIMEText(emailContent, 'plain', 'utf-8')
    message['From'] = mail_user
    message['To'] = receiver
    message['Subject'] = headerInfo

    try:
        userEmail = smtplib.SMTP_SSL(mail_host, 465)
        userEmail.login(mail_user, mail_pass)
        userEmail.sendmail(mail_user, receiver, message.as_string())
        return "success"
    except smtplib.SMTPException, e:
        return "Falied,%s" % e
    except:
        return "fail"

#sendEmail(u'hello', u'hello', '898508832@qq.com')
