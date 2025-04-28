// Simulator for the RISC-V[ECTOR] mini-ISA
// Copyright (C) 2025 Siddarth Suresh
// Copyright (C) 2025 bdunahu

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef MESSAGES_H
#define MESSAGES_H
#include <string>
#include <vector>

/**
 * Humorous computer speak.
 */
#define RANDOM_MESSAGE(v) (v[std::rand() % v.size()])

const std::vector<std::string> waiting = {
	"WAITING FOR USER", "IDLE",	  "BORED",		 "SLEEPING",
	"TAKING A BREAK",	"IDLING", "DAYDREAMING", "WAITING FOR INPUT"};
const std::vector<std::string> running = {
	"COMPUTING", "WORKING", "BUSY", "RUNNING"};
const std::vector<std::string> load_file = {
	"FILE LOADED", "FILE PARSED", "FILE READ", "READING... DONE"};
const std::vector<std::string> no_instructions = {
	"NO PROGRAM PROVIDED", "NOTHING TO DO, GIVING UP", "INSTRUCTIONS MISSING",
	"404 INSTRUCTIONS NOT FOUND"};
const std::vector<std::string> no_pipeline = {
	"SIMULATION READY: NO PIPE", "PIPE OFF, SIMULATION READY"};
const std::vector<std::string> no_cache = {
	"SIMULATION READY: NO CACHE", "CACHE OFF, SIMULATION READY"};
const std::vector<std::string> initialize = {
	"SIMULATION READY", "ALL MODULES LOADED, SIMULATION READY"};

/**
 * @return an unsolicited status messages
 */
std::string get_waiting() { return RANDOM_MESSAGE(waiting); }
std::string get_running() { return RANDOM_MESSAGE(running); }

/**
 * @return confirmation of file upload
 */
std::string get_load_file() { return RANDOM_MESSAGE(load_file); }

/**
 * @return a friendly reminder that the simulation is not configured yet
 */
std::string get_no_instructions() { return RANDOM_MESSAGE(no_instructions); }

/**
 * @return unsolicited complaints for successful initialization
 */
std::string get_no_pipeline() { return RANDOM_MESSAGE(no_pipeline); }
std::string get_no_cache() { return RANDOM_MESSAGE(no_cache); }
std::string get_initialize() { return RANDOM_MESSAGE(initialize); }

#endif // MESSAGES_H
