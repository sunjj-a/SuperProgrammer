from distutils.core import setup
import py2exe

setup(
    name='SpiderBaidu',
    version='',
    packages=[''],
    url='',
    license='',
    author='sunjj',
    author_email='',
    description='',
    data_files=[],
    console = ["SpiderBaidu.py"],
    options = {"py2exe": {"dll_excludes": ["MSVCP90.dll"], "bundle_files": 3, "includes":["sip"]}},
)