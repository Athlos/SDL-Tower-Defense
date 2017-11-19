#pragma once

class LogManager
{
	// Member Methods:
public:
	static LogManager& GetInstance();
	~LogManager();

	void Log(const char* pcMessage); // Log a message

private:
	LogManager();
	LogManager(const LogManager& logManager);
	LogManager& operator=(const LogManager& logManager);

	// Member Data:
protected:
	static LogManager* sm_pInstance;

};
