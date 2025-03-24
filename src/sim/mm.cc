#include "mm.h"
#include "logger.h"
#include "response.h"

static Logger *global_log = Logger::getInstance();

Response MM::advance()
{
	global_log->log(INFO, "hello from memory!");
	return OK;
}
