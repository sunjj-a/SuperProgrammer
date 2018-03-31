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
 * Matthew W. Campbell
 * Erik Johnson
 */
#ifndef DELTA_LOG
#define DELTA_LOG

#include <string>
#include <cstdarg>
#include <vector>
#include <time.h>  
#include "export.h"

#define LOG_SHOW
namespace dtUtil
{
   // Fwd declaration
   class LogObserver;
   class LogFile;
   class Log;
   class DT_UTIL_EXPORT LogFile
   {
   public:
      ///Change the name of the log file (defaults to "delta3d_log.html")
      static void SetFileName(const std::string& name);

      ///Get the current filename of the log file.
      static const std::string GetFileName();

      /// change the title string used in HTML
      /// defaults to "Delta 3D Engine Log File" or "Delta 3D Engine Log File (Debug Libs)"
      static void SetTitle(const std::string& title);

      ///Get the current HTML title string.
      static const std::string& GetTitle();
   };

   
   /**
    * Helps making logging a little easier.  However, if printf style
    *   logging is desired, you cannot use this macro.
    */
#ifdef LOG_SHOW
    #define LOG_DEBUG(msg)\
        dtUtil::Log::GetInstance().LogMessage(__FUNCTION__, __LINE__, msg, dtUtil::Log::LOG_DEBUG);

    #define LOG_INFO(msg)\
        dtUtil::Log::GetInstance().LogMessage(__FUNCTION__, __LINE__, msg, dtUtil::Log::LOG_INFO);

    #define LOG_WARNING(msg)\
        dtUtil::Log::GetInstance().LogMessage(__FUNCTION__, __LINE__, msg, dtUtil::Log::LOG_WARNING);

    #define LOG_ERROR(msg)\
        dtUtil::Log::GetInstance().LogMessage(__FUNCTION__, __LINE__, msg, dtUtil::Log::LOG_ERROR);

    #define LOG_ALWAYS(msg)\
        dtUtil::Log::GetInstance().LogMessage(__FUNCTION__, __LINE__, msg, dtUtil::Log::LOG_ALWAYS);

    #define LOGN_DEBUG(name, msg)\
        dtUtil::Log::GetInstance(name).LogMessage(__FUNCTION__, __LINE__, msg, dtUtil::Log::LOG_DEBUG);

    #define LOGN_INFO(name, msg)\
        dtUtil::Log::GetInstance(name).LogMessage(__FUNCTION__, __LINE__, msg, dtUtil::Log::LOG_INFO);

    #define LOGN_WARNING(name, msg)\
        dtUtil::Log::GetInstance(name).LogMessage(__FUNCTION__, __LINE__, msg, dtUtil::Log::LOG_WARNING);

    #define LOGN_ERROR(name, msg)\
        dtUtil::Log::GetInstance(name).LogMessage(__FUNCTION__, __LINE__, msg, dtUtil::Log::LOG_ERROR);

    #define LOGN_ALWAYS(name, msg)\
        dtUtil::Log::GetInstance(name).LogMessage(__FUNCTION__, __LINE__, msg, dtUtil::Log::LOG_ALWAYS);
#else
    #define LOG_DEBUG(msg)

    #define LOG_INFO(msg)

    #define LOG_WARNING(msg)

    #define LOG_ERROR(msg)

    #define LOG_ALWAYS(msg)

    #define LOGN_DEBUG(name, msg)

    #define LOGN_INFO(name, msg)

    #define LOGN_WARNING(name, msg)

    #define LOGN_ERROR(name, msg)

    #define LOGN_ALWAYS(name, msg)
#endif
   

   struct LogImpl;

   /**
    * Log class which the engine uses for all of its logging
    * needs.  The log file is formatted using html tags,
    * therefore, any browser should display the log without
    *  any problems.
    */
   class DT_UTIL_EXPORT Log 
   {
   public:
      /**
       * The different types of log messages.
       */
      enum LogMessageType
      {
         LOG_DEBUG,
         LOG_INFO,
         LOG_WARNING,
         LOG_ERROR,
         LOG_ALWAYS
      };
      void LogMessage(const std::string& source, int line, double msg,
          LogMessageType msgType = LOG_INFO) const;

      void LogMessage(const std::string& source, int line, int msg,
          LogMessageType msgType = LOG_INFO) const;

      void LogMessage(const std::string& source, int line, long long int msg,
          LogMessageType msgType = LOG_INFO) const;

      void LogMessage(const std::string& source, int line, float msg,
          LogMessageType msgType = LOG_INFO) const;

      void LogMessage(const std::string& source, int line, const wchar_t* msg,
          LogMessageType msgType = LOG_INFO) const;

      void LogMessage(const std::string& source, int line, const std::wstring& msg,
          LogMessageType msgType = LOG_INFO) const;

      /**
       * Logs a time-stamped message.
       * @param source - String identifier of the source of the message.
       *  (__FUNCTION__ is useful here.
       * @param line the line number.
       *  @param msg - Message to display.
       *  @param msgType - Type of message being displayed. (error,warning,info)
       */
      void LogMessage(const std::string& source, int line, const std::string& msg,
                      LogMessageType msgType = LOG_INFO) const;

      /**
       * Little more sophisticated method for logging messages.  Allows for
       *  an unlimited number of parameters in a C-style printf syntax.
       *  @param msgType - Type of message being displayed. (error,warning,info)
       *  @param source - String identifier of the source of the message.
       *  @param line - the line number.
       *  @param msg - Printf - style format string.
       *  @note
       *      Max length of the string to be printed is 2048 characters.
       */
      void LogMessage(LogMessageType msgType, const std::string& source, int line,
                      const char* msg, ...) const;

      /**
       * Little more sophisticated method for logging messages.  Allows for
       *  an unlimited number of parameters in a C-style printf syntax.
       *  @param msgType - Type of message being displayed. (error,warning,info)
       *  @param source - String identifier of the source of the message.
       *  @param line - the line number.
       *  @param msg - std::string that has been formatted.
       *  @note
       *      Max length of the string to be printed is 2048 characters.
       */
      void LogMessage(LogMessageType msgType, const std::string& source, int line,
                      const std::string& msg) const;

      /**
       * Logs a time-stamped message.  Takes a variable-argument list
       *  (va_list) that was created with va_start.
       *  @param msgType - Type of message being displayed. (error,warning,info)
       *  @param source - String identifier of the source of the message.
       *  @param line  - line number or negative for unknown.
       *  @param msg - Printf - style format string.
       *  @param list - va_list created with va_start.
       *  @note
       *      Max length of the string to be printed is 2048 characters.
       */
      void LogMessage(LogMessageType msgType, const std::string& source, int line,
                      const char* msg, va_list list) const;

      /**
       * Little more sophisticated method for logging messages.  Allows for
       *  an unlimited number of parameters in a C-style printf syntax.
       *  @param msgType - Type of message being displayed. (error,warning,info)
       *  @param source - String identifier of the source of the message.
       *  @param msg - Printf - style format string.
       *  @note
       *      Max length of the string to be printed is 2048 characters.
       */
      void LogMessage(LogMessageType msgType, const std::string &source,
                      const char *msg, ...) const;
      /**
       * Inserts a horizontal rule into the log file.
       */
      void LogHorizRule();

      /**
       * @return true if log messages of the given level will be sent to the log
       * output
       * @param msgType the type of message to query about.
       */
      bool IsLevelEnabled(LogMessageType msgType) const;

      /**
       * Sets the lowest level of logging that will be logged.
       * If the level in set to Debug, all messages will be sent. If the level is set error, only
       * errors will be sent.
       * @param msgType the new logging level
       */
      void SetLogLevel(LogMessageType msgType);

      /**
       * @return the lowest level of logging that will be logged.
       */
      LogMessageType GetLogLevel() const;

      ///@return a string version of a LogMessageType.
      static const std::string GetLogLevelString(LogMessageType msgType);

      ///@return the LogMessageType matching a string or WARNING if there is no match.
      static LogMessageType GetLogLevelForString(const std::string& levelString);

      /*
       * Retrieve the default singleton instance of the log class.
       */
      static Log& GetInstance();

      /*
       * Retrieve singleton instance of the log class for a give string name.
       * @param name logger name
       */
      static Log& GetInstance(const std::string& name);

      ///Sets the default LogMessageType for new logs
      static void SetDefaultLogLevel(LogMessageType newLevel);

      /** 
        *  Set the LogMessageType for all existing Log instances.  
        *  @see SetLogLevel()
        */
      static void SetAllLogLevels(LogMessageType newLevel);

      /** 
        *  Add an observer that receives all log messages via callback.  The
        *  TO_OBSERVER OutputStreamOptions bit must be set in order for LogObservers
        *  to get triggered.
        *  @see SetOutputStreamBit()
        *  @see RemoveObserver()
        *  @param observer The LogObserver to register
        */
      void AddObserver(LogObserver& observer);

      /** 
        * Remove an existing LogObserver from the container.
        * @see AddObserver()
        * @param observer The LogObserver to remove
        */
      void RemoveObserver(LogObserver& observer);

      typedef std::vector<LogObserver* > LogObserverContainer;

      /** 
        *  Get all registered LogObservers that are registered to receive log messages.
        *  @return The container of LogObservers (could be empty)
        */
      const LogObserverContainer& GetObservers() const;

      /** 
        *  Get all registered LogObservers that are registered to receive log messages.
        *  @return The container of LogObservers (could be empty)
        */
      LogObserverContainer& GetObservers();

      enum OutputStreamOptions
      {
         NO_OUTPUT =    0x00000000, ///<Log messages don't get written to any device
         TO_FILE =      0x00000001,   ///<Log messages get sent to the output file
         TO_CONSOLE =   0x00000002,///<Log messages get sent to the console
         TO_OBSERVER =  0x00000004,///<Log messages get sent to all registered observers
         STANDARD = TO_FILE | TO_CONSOLE | TO_OBSERVER ///<The default setting
      };

      /** Tell the Log where to send output messages.  The supplied parameter is a
       *  bitwise combination of OutputStreamOptions.  The default is STANDARD, which
       *  directs messages to both the console and the output file.
       *  For example, to tell the Log to output to the file and console:
       *  \code
       *   dtUtil::Log::GetInstance().SetOutputStreamBit(dtUtil::Log::TO_FILE | dtUtil::Log::TO_CONSOLE);
       *  \endcode
       *  \param option A bitwise combination of options.
       */
      void SetOutputStreamBit(unsigned int option);

      ///Get the currently defined output stream options
      unsigned int GetOutputStreamBit() const;

      ///Returns the name of this logger.
      const std::string& GetName() const;

   //Constructor and destructor are both protected since this is a singleton.
   protected:
      /**
       * Opens the log file and writes the html header information.
       * All new logs are created with the global default log level. 
       */
      Log(const std::string& name);

      /**
       * Writes any closing html tags and closes the log file.
       */
      ~Log();

   private:
      LogImpl* mImpl;
      int m_nTickCount;
   };
  
} // namespace dtUtil

/// 时间统计类 #include <time.h>  
class DiffTime
{
public:
    clock_t m_dStart;    ///< 开始时间，单位毫秒
    clock_t m_dEnd;      ///< 结束时间，单位毫秒
    long m_dDiffTime;    ///< 时间差 

    DiffTime() : m_dStart( 0 ), m_dEnd( 0 ), m_dDiffTime(0)
    {
        m_dStart = clock();
    }

    ~DiffTime()
    {
        m_dEnd = clock();
        m_dDiffTime = m_dEnd - m_dStart;
    }

    long getDiff( void )
    {
        m_dEnd = clock();
        m_dDiffTime = m_dEnd - m_dStart;
        return m_dDiffTime;
    }
};

static  void  SetLogInformaiton( bool bOpenLog, const std::string& strFileName, int nLevel = 0 )
{
    if (!bOpenLog)
        return;
    if (!strFileName.empty())
    {
        dtUtil::LogFile::SetFileName( strFileName ); 
    }

    dtUtil::Log::GetInstance().SetLogLevel( static_cast< dtUtil::Log::LogMessageType >( nLevel ) );

    LOG_ALWAYS( "************************start log*******************" );
}

#endif // DELTA_LOG
