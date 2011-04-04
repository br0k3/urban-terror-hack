#ifndef LOGGER_H_
#define LOGGER_H_

#ifdef URTH_DEBUG
	extern std::ofstream *logFile;

	#define LOG_DEBUG(text)								\
		{												\
			time_t now = time(NULL);					\
			struct tm *t = localtime(&now);				\
			*logFile << std::setfill('0')				\
				<< std::setw(2) << t->tm_hour << ":"	\
				<< std::setw(2) << t->tm_min << ":"		\
				<< std::setw(2) << t->tm_sec << " "		\
				<< std::dec								\
				<< __FILE__ << ":"						\
				<< __LINE__ << ": "						\
				<< text << std::endl;					\
		}

	#define LOG_FILE_DEF \
		std::ofstream *logFile = NULL;

	#define LOG_FILE_INIT							\
		char *filename = getenv("URTH_LOG_FILE");	\
		if (!filename)								\
			filename = URTH_DEFAULT_LOG_FILE;		\
		logFile = new std::ofstream(filename);

	#define LOG_FILE_STOP	\
		logFile->close();	\
		delete(logFile);
#else
	#define LOG_DEBUG(text)
	#define LOG_FILE_DEF
	#define LOG_FILE_INIT
	#define LOG_FILE_STOP
#endif

#define LOG_INFO(text)																				\
	{																								\
		std::cout << "urth: " << text << std::endl;													\
		std::ostringstream oss;																		\
		oss << text;																				\
		/*if (cgame)																				\
			et->etInfo->Cbuf_ExecuteText(EXEC_APPEND, va("echo ^nurth: %s\n", oss.str().c_str()));	\
		else																						\
			syscall_Print(va("^nurth: %s\n", oss.str().c_str()));	*/								\
		std::cout << "urth: " << text << std::endl;													\
		LOG_DEBUG("info: " << text);																\
	}

#endif /*LOGGER_H_*/
