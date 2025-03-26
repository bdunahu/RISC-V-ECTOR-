#include "wb.h"
#include "instrDTO.h"
#include "logger.h"
#include "response.h"

static Logger *global_log = Logger::getInstance();

Response WB::advance(InstrDTO &i)
{
	global_log->log(INFO, "hello from write back!");
	return OK;
}
