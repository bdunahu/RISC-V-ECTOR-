#include "if.h"
#include "logger.h"
#include "response.h"

static Logger *global_log = Logger::getInstance();

Response IF::advance()
{
	global_log->log(INFO, "hello from fetch!");
	return OK;
}




