#ifndef __LOGMANAGER_H__
#define __LOGMANAGER_H__

class LogManager
{
	// Member Methods:
public:
	static LogManager& GetInstance();
	~LogManager();

	void Log(const char* pcMessage);

private:
	LogManager();
	LogManager(const LogManager& logManager);
	LogManager& operator=(const LogManager& logManager);

	// Member Data:
protected:
	static LogManager* sm_pInstance;

};

#endif //__LOGMANAGER_H__
