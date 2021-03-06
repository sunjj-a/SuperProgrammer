/*
 * Delta3D Open Source Game and Simulation Engine
 * Copyright (C) 2010, MOVES Institute
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * Erik Johnson
 */
#include "logobserverfile.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include "logmemory.h"
#include <iomanip>

////////////////////////////////////////////////////////////////////////////////
dtUtil::LogObserverFile::LogObserverFile() 
: mOpenFailed(false)
{
    m_preTime = GetTickCount();
    m_preMemory = getCurrentMemory();
}

////////////////////////////////////////////////////////////////////////////////
dtUtil::LogObserverFile::~LogObserverFile()
{
   if (logFile.is_open())
   {
      EndFile();
   }
}

////////////////////////////////////////////////////////////////////////////////
void dtUtil::LogObserverFile::EndFile()
{
   logFile << "</body></html>" << std::endl;
   logFile.flush();
}

////////////////////////////////////////////////////////////////////////////////
void dtUtil::LogObserverFile::OpenFile()
{
   if (mOpenFailed)
   {
      return;
   }

   if (logFile.is_open())
   {
      logFile << "<p>Change to log file: "<< LogFile::GetFileName()<< std::endl;
      TimeTag("At ");
      EndFile();
      logFile.close();
   }

   //First attempt to create the log file.
   logFile.open(LogFile::GetFileName().c_str());
   if (!logFile.is_open())
   {
      std::cout << "could not open file \""<<LogFile::GetFileName()<<"\"" << std::endl;
      mOpenFailed = true;
      return;
   }
   else
   {
      //std::cout << "Using file \"delta3d_log.html\" for logging" << std::endl;
   }
   //Write a decent header to the html file.
   logFile << "<html><meta http-equiv=\"Content-Type\" content=\"text/html;charset=GB2312\">";
   logFile << "<title>" << LogFile::GetTitle() <<"</title><body>" << std::endl;
   logFile << "<h1 align=\"center\">" << LogFile::GetTitle() << "</h1><hr>" << std::endl;
   logFile << "<pre><h3 align=\"center\""
      "<font color=#808080><b>  Debug     </b></font>"
      "<font color=#008080><b>  Information     </b></font>"
      "<font color=#808000><b>  Warning  </b></font>"
      "<font color=#FF0000><b>  Error   </b></font></h3></pre><hr>"
      << std::endl;

   TimeTag("Started at ");

   logFile.flush();
}

////////////////////////////////////////////////////////////////////////////////
void dtUtil::LogObserverFile::TimeTag(std::string prefix)
{
   struct tm *t;
   time_t cTime;

   time(&cTime);
   t = localtime(&cTime);
   logFile << prefix
      << std::setw(2) << std::setfill('0') << (1900+t->tm_year) << "/"
      << std::setw(2) << std::setfill('0') << t->tm_mon + 1<< "/"
      << std::setw(2) << std::setfill('0') << t->tm_mday << " "
      << std::setw(2) << std::setfill('0') << t->tm_hour << ":"
      << std::setw(2) << std::setfill('0') << t->tm_min << ":"
      << std::setw(2) << std::setfill('0') << t->tm_sec << "<br>"
      << std::endl;
   logFile.flush();
}

std::string getBlank(int num)
{
    std::string s = "";
    int p = 10000000;
    int t = num;
    if (t<=0)
    {
        p = p/10;
        t = -t;
    }
    while (t < p)
    {
        s += "&nbsp";
        p = p / 10;
    }
    return s;
}

////////////////////////////////////////////////////////////////////////////////
void dtUtil::LogObserverFile::LogMessage(Log::LogMessageType type, const std::string& source, int line, const std::string& msg)
{
   if (!logFile.is_open())
   {
      OpenFile();

      if (!logFile.is_open())
      {
         return;
      }
   }
   mutex.lock();
   std::string color;
   switch (type)
   {
   case Log::LOG_DEBUG:
      color = "<b><font color=#808080>";
      break;

   case Log::LOG_INFO:
      color = "<b><font color=#008080>";
      break;

   case Log::LOG_ERROR:
      color = "<b><font color=#FF0000>";
      break;

   case Log::LOG_WARNING:
      color = "<b><font color=#808000>";
      break;

   case Log::LOG_ALWAYS:
      color = "<b><font color=#000000>";
      break;
   }
   struct tm *t;
   time_t cTime;
   time(&cTime);
   t = localtime(&cTime);
//    unsigned long nDiffTickCount =  GetTickCount() - m_preTime;
//    m_preTime = GetTickCount();

 //  __int64 nCurMemory = getCurrentMemory();
 //  __int64 nDiffMemory = nCurMemory - m_preMemory;
 //  m_preMemory = nCurMemory;

   static const std::string htmlNewline ("<br>\n");
   std::string htmlMsg (msg);
   for (size_t lineEnd = htmlMsg.find('\n');
      lineEnd != std::string::npos;
      lineEnd = htmlMsg.find('\n', lineEnd))
   {
      htmlMsg.replace (lineEnd, 1, htmlNewline);
      lineEnd += htmlNewline.size() + 1;
   }
   std::string strLev = Log::GetLogLevelString(type);
   int nlength = strLev.length();
   for (int i = 0; i < 6 - nlength; ++i)
   {
	   strLev += "&nbsp";
   }

   logFile << color << strLev << ": "   /// chenb-b 将时间和函数信息注释掉
      << std::setw(2) << std::setfill('0') << t->tm_hour << ":"
      << std::setw(2) << std::setfill('0') << t->tm_min << ":"
      << std::setw(2) << std::setfill('0') << t->tm_sec << ":";
 //     << std::setw(3) << std::setfill('0') << m_preTime % 1000 << ":"
//       << "[timediff:"   << getBlank(nDiffTickCount) << nDiffTickCount << "ms]:"
//      << "[currmemory:" << getBlank(nCurMemory) << nCurMemory << "KB]:"
//       << "[memorydiff:" << getBlank(nDiffMemory)  << nDiffMemory << "KB]:&lt;"
//      << source;
//    if (line > 0)
//    {
//       logFile <<( ":") << getBlank(line) << line;
//    }

   logFile << "&gt; " << "</font></b>" ;
   logFile << "<b><font color=#000000> " << htmlMsg << "</font></b><br>" << std::endl;

   logFile.flush(); //Make sure everything is written, in case of a crash.
   mutex.unlock();
}

////////////////////////////////////////////////////////////////////////////////
void dtUtil::LogObserverFile::LogHorizRule()
{
   if (!logFile.is_open())
   {
      return;
   }

   logFile << "<hr>" << std::endl;
}
