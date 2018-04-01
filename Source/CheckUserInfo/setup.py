from distutils.core import setup
import py2exe

setup(
    name='CheckUserInfo',
    version='',
    packages=[''],
    url='',
    license='',
    author='sunjj',
    author_email='',
    description='',
    console = [{"script": "CheckUserInfo.py"}],
    options = {"py2exe": {"dll_excludes": ["MSVCP90.dll"], "bundle_files": 3, "includes":["sip"]}},
    data_files = ["G:\JiraBatchTool\PersonalStudy\CheckUserInfo\UserInfo.ini"]
)
