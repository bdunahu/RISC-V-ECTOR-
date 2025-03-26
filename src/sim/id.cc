#include "id.h"
#include "instrDTO.h"
#include "logger.h"
#include "response.h"

static Logger *global_log = Logger::getInstance();

Response ID::advance(InstrDTO &i)
{
	global_log->log(INFO, "hello from decode!");
	return OK;
}
