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
	"NO PROGRAM PROVIDED", "INSTRUCTIONS NOT INCLUDED",
	"NOTHING TO DO, GIVING UP"};
const std::vector<std::string> bad_cache = {
	"INVALID NUMBER OF WAYS", "WAYS CANNOT BE BELOW 0 OR ABOVE 5"};
const std::vector<std::string> no_pipeline = {
	"PIPELINE--HUMANS PROBABLY WORKED HARD ON THAT",
	"I WOULD PREFER YOU LEAVE THE PIPE ON", "SLOW MODE ENABLED",
	"SIMULATION READY"};
const std::vector<std::string> no_cache = {
	"NO CACHE HAS NO WAY TO GO", "SLOW MODE ENABLED", "SIMULATION READY"};
const std::vector<std::string> initialize = {"SIMULATION READY"};

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
std::string get_bad_cache() { return RANDOM_MESSAGE(bad_cache); }

/**
 * @return unsolicited complaints for successful initialization
 */
std::string get_no_pipeline() { return RANDOM_MESSAGE(no_pipeline); }
std::string get_no_cache() { return RANDOM_MESSAGE(no_cache); }
std::string get_initialize() { return RANDOM_MESSAGE(initialize); }

#endif // MESSAGES_H
