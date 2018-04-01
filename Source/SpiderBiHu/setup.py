from distutils.core import setup
import py2exe
import certifi

setup(
    name='SpideBihu',
    version='',
    packages=[''],
    url='',
    license='',
    author='sunjj',
    author_email='',
    description='',
    data_files=[certifi.where()],
    console = ["SpideBihu.py", "QueryData.py"],
    options = {"py2exe": {"dll_excludes": ["MSVCP90.dll"], "bundle_files": 3, "includes":['lxml.etree', 'lxml._elementpath', "sip"]}},
)