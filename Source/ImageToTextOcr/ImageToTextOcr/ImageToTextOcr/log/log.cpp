/*
 * Delta3D Open Source Game and Simulation Engine
 * Copyright (C) 2005-2010, BMH Associates, Inc.
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
 * Matthew W. Campbell, Curtiss Murphy, Erik Johnson
 */

#include "log.h"
#include "bits.h"
#include "logobserver.h"
#include "logobserverconsole.h"
#include "logobserverfile.h"

#include <algorithm>
#include <cstdarg>
//#include <cstdio>
#include <ctime>
#include <map>
#include <strstream>
#include <Windows.h>

namespace dtUtil
{
   static const char* sLogFileName = "delta3d_log.html";

#ifdef _DEBUG
   static std::string sTitle("GCL Log File (Debug Libs)");
#else
   static std::string sTitle("GCL Log File");
#endif

   //forward declaration
   class LogManager;

   static LogManager* LOG_MANAGER(NULL);
   static Log::LogMessageType DEFAULT_LOG_LEVEL(Log::LOG_WARNING);


   //////////////////////////////////////////////////////////////////////////
   class LogManager
   {
   public:
      LogObserverConsole* mLogObserverConsole; ///writes to console
      LogObserverFile* mLogObserverFile; ///writes to file

      ////////////////////////////////////////////////////////////////
      LogManager()
      : mLogObserverConsole(new LogObserverConsole())
      , mLogObserverFile(new LogObserverFile())
      {
      }

      ////////////////////////////////////////////////////////////////
      ~LogManager()
      {
         mInstances.clear();
         delete mLogObserverConsole;
         delete mLogObserverFile;
      }

      ////////////////////////////////////////////////////////////////
      bool AddInstance(const std::string& name, Log* log)
      {
         return mInstances.insert(std::make_pair<std::string, Log*>(name,log)).second;
      }

      ////////////////////////////////////////////////////////////////
      Log* GetInstance(const std::string& name)
      {
         std::map<std::string, Log* >::iterator i = mInstances.find(name);
         if (i == mInstances.end())
         {
            return NULL;
         }
         return i->second;
      }

      ////////////////////////////////////////////////////////////////
      void SetAllLogLevels(const Log::LogMessageType &newLevel)
      {
         std::map<std::string, Log* >::iterator i, iend;

         i = mInstances.begin();
         iend = mInstances.end();

         for (;i != iend; i++)
         {
            Log* log = i->second;
            log->SetLogLevel(newLevel);
         }
      }
   private:
      std::map<std::string, Log* > mInstances;
   };

   ////////////////////////////////////////////////////////////////////
   ////////////////////////////////////////////////////////////////////

   /** This will close the existing file (if opened) and create a new file with
    *  the supplied filename.
    * @param name : The name of the new file (will be written using HTML)
    */
   void LogFile::SetFileName(const std::string& name)
   {
      //std::cout << "LogFile try to change files to " << name << std::endl;

      sLogFileName = name.c_str();
      if (LOG_MANAGER == NULL)
      {
         LOG_MANAGER = new LogManager;
      }
      else
      {
         // reset open failed if the file name changes.
         LOG_MANAGER->mLogObserverFile->mOpenFailed = false;
         LOG_MANAGER->mLogObserverFile->OpenFile();
      }
   }

   const std::string LogFile::GetFileName()
   {
      return std::string(sLogFileName);
   }

   void LogFile::SetTitle(const std::string& title)
   {
      sTitle = title;
   }

   const std::string& LogFile::GetTitle()
   {
      return sTitle;
   }

   //////////////////////////////////////////////////////////////////////////
   //////////////////////////////////////////////////////////////////////////
   struct LogImpl
   {
      LogImpl(const std::string& name)
      : mOutputStreamBit(Log::STANDARD)
      , mName(name)
      , mLevel(DEFAULT_LOG_LEVEL)
      , mObservers()
      {
      }

      static const std::string mDefaultName;

      unsigned int mOutputStreamBit; ///<the current output stream option
      std::string mName;
      Log::LogMessageType mLevel;
      Log::LogObserverContainer mObservers;
   };

   const std::string LogImpl::mDefaultName("__+default+__");
   //////////////////////////////////////////////////////////////////////////
   //////////////////////////////////////////////////////////////////////////

   Log::Log(const std::string& name)
      : 
        mImpl(new LogImpl(name))      
   {
       m_nTickCount = GetTickCount();
   }

   //////////////////////////////////////////////////////////////////////////
   Log::~Log()
   {
      delete mImpl;
      mImpl = NULL;
   }


   //////////////////////////////////////////////////////////////////////////
   void Log::LogMessage(const std::string& source, int line, double msg,
       LogMessageType msgType) const
   {
        std::strstream ss;
        std::string s;
        ss << msg;
        ss >> s;
        LogMessage( source, line, s, msgType );
   }

   void Log::LogMessage(const std::string& source, int line, int msg,
       LogMessageType msgType ) const
   {
       std::strstream ss;
       std::string s;
       ss << msg;
       ss >> s;
       LogMessage( source, line, s, msgType );
   }

   void Log::LogMessage(const std::string& source, int line, long long int msg,
       LogMessageType msgType ) const
   {
       std::strstream ss;
       std::string s;
       ss << msg;
       ss >> s;
       LogMessage( source, line, s, msgType );
   }

   void Log::LogMessage(const std::string& source, int line, float msg,
       LogMessageType msgType ) const
   {
       std::strstream ss;
       std::string s;
       ss << msg;
       ss >> s;
       LogMessage( source, line, s, msgType );
   }

   void Log::LogMessage(const std::string& source, int line, const wchar_t* msg,
       LogMessageType msgType) const
   {
        size_t len = wcslen(msg) + 1;
        size_t converted = 0;
        char *CStr;
        CStr=(char*)malloc(len*sizeof(char));
        if( CStr != NULL )
        {
            wcstombs_s(&converted, CStr, len, msg, _TRUNCATE);
            LogMessage( source, line, CStr, msgType );
            free(CStr);
        }
   }

   void Log::LogMessage(const std::string& source, int line, const std::wstring& msg,
       LogMessageType msgType ) const
   {
       LogMessage( source, line, msg.c_str(), msgType );        
   }

   void Log::LogMessage(const std::string& source, int line, const std::string& msg,
                LogMessageType msgType) const
   {
      if (mImpl->mOutputStreamBit == Log::NO_OUTPUT)
      {
         return;
      }

      if (msgType < mImpl->mLevel)
      {
         return;
      }




      if (dtUtil::Bits::Has(mImpl->mOutputStreamBit, Log::TO_FILE))
      {
         LOG_MANAGER->mLogObserverFile->LogMessage(msgType, source, line, msg);
      }

      if (dtUtil::Bits::Has(mImpl->mOutputStreamBit, Log::TO_CONSOLE))
      {
         LOG_MANAGER->mLogObserverConsole->LogMessage(msgType, source, line, msg);
      }

      if (dtUtil::Bits::Has(mImpl->mOutputStreamBit, Log::TO_OBSERVER) && !mImpl->mObservers.empty())
      {
         Log::LogObserverContainer::iterator itr = mImpl->mObservers.begin();
         while (itr != mImpl->mObservers.end())
         {
            (*itr)->LogMessage(msgType, source, line, msg);
            ++itr;
         }
      }
   }

   //////////////////////////////////////////////////////////////////////////
   void Log::LogMessage(LogMessageType msgType, const std::string& source, int line,
                            const char* msg, va_list list) const
   {
      char buffer[2049];

      vsnprintf(buffer, 2049, msg, list);

      LogMessage(source, line, buffer, msgType);
   }

   //////////////////////////////////////////////////////////////////////////
   void Log::LogMessage(LogMessageType msgType, const std::string& source,
                     const char* msg, ...) const
   {
      va_list list;

      va_start(list,msg);
      LogMessage(msgType, source, -1, msg, list);
      va_end(list);
   }

   //////////////////////////////////////////////////////////////////////////
   void Log::LogMessage(LogMessageType msgType, const std::string& source, int line,
                            const char* msg, ...) const
   {
      va_list list;

      va_start(list,msg);
      LogMessage(msgType, source, line, msg, list);
      va_end(list);
   }

   //////////////////////////////////////////////////////////////////////////
   void Log::LogMessage(LogMessageType msgType,
                        const std::string& source,
                        int line,
                        const std::string& msg) const
   {
      LogMessage(source, line, msg, msgType);
   }

   char* unicodeToAnsi( const wchar_t* pszW )
   {
	   char* pszA = nullptr;
	   if (pszW == nullptr)
	   {
		   return nullptr;
	   }

	   int cCharacters = int(wcslen(pszW) + 1);
	   int cbAnsi = cCharacters*2;

	  unsigned int iTextLen = static_cast<unsigned int>(WideCharToMultiByte( CP_ACP,
		   0,
		   pszW,
		   -1,
		   NULL,
		   0,
		   NULL,
		   NULL ));



	   pszA = new char[iTextLen + 1];
	   if( nullptr == pszA )
		   return nullptr;
	   memset( ( void* )pszA, 0, sizeof( char ) * ( iTextLen + 1 ) );


	   if (0 == WideCharToMultiByte(CP_ACP, 0, pszW, cCharacters, pszA,
		   cbAnsi, NULL, NULL))
	   {
		   DWORD dwError = GetLastError();
		   delete []pszA;
		   pszA = nullptr;
		   return nullptr;
	   }
	   return pszA;
   }

//    void Log::LogMessage(const std::string& source, int line, const std::wstring& wmsg,
// 	   LogMessageType msgType ) const
//    {
// 	   char* pszA = unicodeToAnsi( wmsg.c_str() );
// 	   if( nullptr == pszA )
// 		   return;
// 	   std::string msg = pszA;
// 	   delete []pszA;
// 	   LogMessage(source, line, msg, msgType);
//    }

 

   //////////////////////////////////////////////////////////////////////////
   void Log::LogHorizRule()
   {
      if (mImpl->mOutputStreamBit == Log::NO_OUTPUT)
      {
         return;
      }

      if (dtUtil::Bits::Has(mImpl->mOutputStreamBit, Log::TO_FILE))
      {
         LOG_MANAGER->mLogObserverFile->LogHorizRule();
      }
   }

   //////////////////////////////////////////////////////////////////////////
   Log& Log::GetInstance()
   {
      return GetInstance(LogImpl::mDefaultName);
   }

   //////////////////////////////////////////////////////////////////////////
   Log& Log::GetInstance(const std::string& name)
   {
      if (LOG_MANAGER == NULL)
      {
         LOG_MANAGER = new LogManager;
      }

      Log* l = LOG_MANAGER->GetInstance(name);
      if (l == NULL)
      {
         l = new Log(name);
         LOG_MANAGER->AddInstance(name, l);
      }

      return *l;
   }

   //////////////////////////////////////////////////////////////////////////
   void Log::SetDefaultLogLevel(LogMessageType newLevel)
   {
      DEFAULT_LOG_LEVEL = newLevel;
   }

   //////////////////////////////////////////////////////////////////////////
   const std::string Log::GetLogLevelString(Log::LogMessageType msgType) //static
   {
      std::string lev;

      switch (msgType)
      {
      case Log::LOG_ALWAYS:  lev = "Always";  break;
      case Log::LOG_ERROR:   lev = "Error";   break;
      case Log::LOG_WARNING: lev = "Warn";    break;
      case Log::LOG_INFO:    lev = "Info";    break;
      case Log::LOG_DEBUG:   lev = "Debug";   break;
      default:
         break;
      }

      return lev;
   }

   //////////////////////////////////////////////////////////////////////////
   Log::LogMessageType Log::GetLogLevelForString(const std::string& levelString)  //static
   {
      if (levelString == "Always" || levelString == "ALWAYS")
      {
         return LOG_ALWAYS;
      }
      else if (levelString == "Error" || levelString == "ERROR")
      {
         return LOG_ERROR;
      }
      else if (levelString == "Warn" || levelString == "WARN" || levelString == "Warning" || levelString == "WARNING")
      {
         return LOG_WARNING;
      }
      else if (levelString == "Info" || levelString == "INFO")
      {
         return LOG_INFO;
      }
      else if (levelString == "Debug" || levelString == "DEBUG")
      {
         return LOG_DEBUG;
      }
      else
      {
         return LOG_WARNING;
      }
   }

   ///////////////////////////////////////////////////////////////////////////
   void Log::SetOutputStreamBit(unsigned int option)
   {
      mImpl->mOutputStreamBit = option;
   }

   ///////////////////////////////////////////////////////////////////////////
   unsigned int Log::GetOutputStreamBit() const
   {
      return mImpl->mOutputStreamBit;
   }

   ///////////////////////////////////////////////////////////////////////////
   const std::string& Log::GetName() const
   {
      return mImpl->mName;
   }

   ////////////////////////////////////////////////////////////////////////////////
   bool Log::IsLevelEnabled(LogMessageType msgType) const
   {
      return msgType >= mImpl->mLevel;
   }

   ////////////////////////////////////////////////////////////////////////////////
   void Log::SetLogLevel(LogMessageType msgType)
   {
      mImpl->mLevel = msgType;
   }

   ////////////////////////////////////////////////////////////////////////////////
   dtUtil::Log::LogMessageType Log::GetLogLevel() const
   {
      return mImpl->mLevel;
   }

   ////////////////////////////////////////////////////////////////////////////////
   void Log::SetAllLogLevels(LogMessageType newLevel) //static
   {
      if (LOG_MANAGER != NULL )
      {
         LOG_MANAGER->SetAllLogLevels(newLevel);
      }
   }

   ////////////////////////////////////////////////////////////////////////////////
   void Log::AddObserver(LogObserver& observer)
   {
      mImpl->mObservers.push_back(&observer);
   }

   ////////////////////////////////////////////////////////////////////////////////
   const Log::LogObserverContainer& Log::GetObservers() const
   {
      return mImpl->mObservers;
   }

   //////////////////////////////////////////////////////////////////////////
   Log::LogObserverContainer& Log::GetObservers()
   {
      return mImpl->mObservers;
   }

   ////////////////////////////////////////////////////////////////////////////////
   void Log::RemoveObserver(LogObserver& observer)
   {
      LogObserverContainer::iterator found = std::find(mImpl->mObservers.begin(),
                                                       mImpl->mObservers.end(), &observer);
      if (found != mImpl->mObservers.end())
      {
         mImpl->mObservers.erase(found);
      }
   }

} // namespace dtUtil
