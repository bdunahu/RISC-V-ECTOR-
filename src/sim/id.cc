#include "id.h"
#include "logger.h"
#include "response.h"

static Logger *global_log = Logger::getInstance();

Response ID::advance()
{
	global_log->log(INFO, "hello from decode!");
	return OK;
}
