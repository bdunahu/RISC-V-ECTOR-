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
const std::vector<std::string> bad_file = {
	"BAD FILE", "TRY AGAIN", "SEEKING NEW READING MATERIAL"};
const std::vector<std::string> load_file = {
	"FILE LOADED", "FINISHED READING DATA. EAGERLY WAITING"};

/**
 * @return an unsolicited waiting message
 */
std::string get_waiting() { return RANDOM_MESSAGE(waiting); }

/**
 * @return a complaint about a bad file name
 */
std::string get_bad_file() { return RANDOM_MESSAGE(bad_file); }

/**
 * @return confirmation of file upload
 */
std::string get_load_file() { return RANDOM_MESSAGE(load_file); }

#endif // MESSAGES_H
