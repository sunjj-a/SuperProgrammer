from distutils.core import setup
import py2exe
import certifi

setup(
    name='SpiderSinaBlog',
    version='',
    packages=[''],
    url='',
    license='',
    author='sunjj',
    author_email='',
    description='',
    console = ["SpiderSinaBlog.py"],
    options = {"py2exe": {"dll_excludes": ["MSVCP90.dll"], "bundle_files": 3, "includes":["sip"]}},
)