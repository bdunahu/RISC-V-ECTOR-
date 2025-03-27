#include "ex.h"
#include "instrDTO.h"
#include "logger.h"
#include "response.h"

static Logger *global_log = Logger::getInstance();

Response EX::advance(InstrDTO &i)
{
	global_log->log(INFO, "hello from execute!");
	return OK;
}
