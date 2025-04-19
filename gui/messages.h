#ifndef MESSAGES_H
#define MESSAGES_H
#include <string>
#include <vector>

/**
 * Humorous computer speak.
 */
#define RANDOM_MESSAGE(v) (v[std::rand() % v.size()])

const std::vector<std::string> waiting = {
	"WAITING FOR USER", "FRIENDS MISSING", "BORED", "SLEEPING"};
const std::vector<std::string> load_file = {
	"FILE LOADED", "FINISHED READING DATA. EAGERLY WAITING"};
const std::vector<std::string> no_instructions = {
	"NO PROGRAM PROVIDED", "RISC V[ECTOR]: INSTRUCTIONS NOT INCLUDED",
	"NOTHING TO DO, GIVING UP"};

/**
 * @return an unsolicited waiting message
 */
std::string get_waiting() { return RANDOM_MESSAGE(waiting); }

/**
 * @return confirmation of file upload
 */
std::string get_load_file() { return RANDOM_MESSAGE(load_file); }

/**
 * @return a friendly reminder that the simulation is not configured yet
 */
std::string get_no_instructions() { return RANDOM_MESSAGE(no_instructions); }

#endif // MESSAGES_H
