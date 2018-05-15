#pragma once
#include <string>
#include <fstream>
#include <vector>


namespace props
{

std::vector<std::string> ReadArgs(int argc, char * argv[]);
bool FindArgs(std::vector<std::string> args, std::string option, int count, std::vector<std::string> & answer);

std::string ReadColumn(const std::string & line, int column, std::string delim = " ");
float ReadColumnP(const std::string & line, int column, std::string delim = " ");

bool file_exists(const std::string& name);
bool ends_with(std::string const & value, std::string const & ending);
bool begins_with(std::string const & value, std::string const & beginning);

void logfile(std::string msg, std::string filename = "logfile", bool debug = true);
std::string tostring(int num);
std::string tostring(float num);

std::string get_time();
std::string exec(const std::string cmd);
int num_exec(const std::string cmd);
float float_exec(const std::string cmd);

int get_els();
int get_spin();
int get_max();
int get_min();
int get_running_jobs();
int get_waiting();
int get_free_proc();
int get_free_actual();
int jobs_to_do();
bool is_running(long JobID);
long first_num(std::string str);
bool is_empty(const std::string & file);
bool exists(const std::string & name);
bool submit_next(std::ifstream & myifile, int & JobID, std::string & path);
void submit_list(std::string queue, std::string logfile, std::ofstream & fo_running, int & to_do);
void clean_erroneous();

}
