#include "mm.h"
#include "logger.h"
#include "response.h"
#include "instrDTO.h"

static Logger *global_log = Logger::getInstance();

Response MM::advance(InstrDTO &i)
{
	global_log->log(INFO, "hello from memory!");
	return OK;
}
