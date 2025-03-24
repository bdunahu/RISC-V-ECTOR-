#include "ex.h"
#include "logger.h"
#include "response.h"

static Logger *global_log = Logger::getInstance();

Response EX::advance()
{
	global_log->log(INFO, "hello from execute!");
	return OK;
}
