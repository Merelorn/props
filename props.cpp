#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <locale>
#include <sys/stat.h>
#include <iostream>
#include <sstream>
#include <props.h>
#include <math.h>
#include <algorithm>

namespace props
{

std::vector<std::string> ReadArgs (int argc, char * argv[]){
  std::vector<std::string> answer;
  for (int i = 0; i < argc; i++){
    answer.push_back(argv[i]);
  } 
  return answer;
}

bool FindArgs (std::vector<std::string> args, std::string option, int count, std::vector<std::string> & answer){
  answer.clear();
  std::vector<std::string>::iterator it = std::find(args.begin(), args.end(), option);
  std::vector<std::string>::iterator it2 = it;
  if ( it == args.end() ){ return false; }
  if ( count == 0 ){ answer.clear(); return true; }
  for ( int i = 0; i < count; i++ ){
    it2++;
    if ( it2 == args.end() ){ answer.clear(); return false; }
    answer.push_back(*it2);
  }
  return true;
}


bool file_exists (const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

bool ends_with(std::string const & value, std::string const & ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

bool begins_with(std::string const & value, std::string const & beginning)
{
    if (beginning.size() > value.size()) return false;
    return std::equal(beginning.begin(), beginning.end(), value.begin());
}

void logfile(std::string msg, std::string filename, bool debug)
{
        std::ofstream myofile;
        myofile.open(filename, std::fstream::out | std::fstream::app);
        myofile << msg << std::endl;
        myofile.close();
	if ( debug ) { std::cout << msg << std::endl; }
}

std::string tostring(int num)
{
        std::stringstream ss;
        ss << num;
        return ss.str();
}

std::string tostring(float num)
{
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1);
        ss << num;
        return ss.str();
}



std::string get_time()
{
	time_t t = time(0);   // get time now
	struct tm * now = localtime( & t );
	std::stringstream ss;
	ss	<< (now->tm_year + 1900) << '-'
		<< std::setw(3) << std::setfill('0')
        	<< (now->tm_yday + 1) << '-'
		<< std::setw(2) << std::setfill('0')
	        << now->tm_hour << '-'
		<< std::setw(2) << std::setfill('0')
		<< now->tm_min << "-"
		<< std::setw(2) << std::setfill('0')
		<< now->tm_sec;
	return ss.str();
}

std::string exec(const std::string cmd) {
  FILE *fp;
  char buf[200];
  std::string result(""); 

  /* Open the command for reading. */
  fp = popen(cmd.c_str(), "r");
  if (fp == NULL)
    return "Failed to run command\n";
  

  /* Read the output a line at a time - output it. */
  while (fgets(buf, 200, fp) != NULL) {
	result += buf;
  }

  /* close */
  pclose(fp);
  return result;
}

int num_exec(const std::string cmd) {
	FILE* pipe = popen(cmd.c_str(), "r");
	if (!pipe) return -1;
	char buffer[128];
	std::string result = "";
	while(!feof(pipe)) {
		if(fgets(buffer, 128, pipe) != NULL)
			result += buffer;
	}
	pclose(pipe);
	
	return atoi(result.c_str());	
}

float float_exec(const std::string cmd) {
	FILE* pipe = popen(cmd.c_str(), "r");
	if (!pipe) return -1;
	char buffer[128];
	std::string result = "";
	while(!feof(pipe)) {
		if(fgets(buffer, 128, pipe) != NULL)
			result += buffer;
	}
	pclose(pipe);
	
	return stof(result);	
}

int get_els(){return stoi(props::ReadColumn(exec("ce"), 1));}
int get_spin(){return stoi(props::ReadColumn(exec("ce"), 2));}
int get_max(){return num_exec("max");}
int get_qumax(){return num_exec("qumax");}
int get_min(){return num_exec("min");}
int get_running_jobs(){return num_exec("qstat | grep wondrash | wc -l");}
int get_waiting(){return num_exec("qstat | grep wondrash | grep qw | wc -l");}
int get_free_proc(){return num_exec("free_proc");}
int get_free_actual(){return num_exec("free_actual");}
bool is_running(long JobID)
{
	std::stringstream ss;
	ss << JobID;
	return (!(exec("qstat -u \"*\" | grep " + ss.str()) == ""));
} 


long first_num (std::string str)
{
	int i = 0;
	while (!isdigit(str[i])) i++;
	int j = i;
	while (isdigit(str[j])) j++;
	return atol(str.substr(i,j).c_str());
}

bool is_empty(const std::string & file)
{
	std::ifstream myifile;
	myifile.open(file.c_str());
	return myifile.peek() == std::ifstream::traits_type::eof();
}

bool exists (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

int jobs_to_do()
{
	
        int running_jobs = get_running_jobs();
        int max = get_max();
	int qumax = get_qumax();
        int min = get_min();
        int waiting = get_waiting();
        int free_proc = get_free_proc();
        int free_actual = get_free_actual();
	int processable(0);
	int value;

	if ( max > qumax && qumax != 0 ){max = qumax;}
	if ( min > max ){min = max;}
	if ( running_jobs <= max ){max = max - running_jobs;}else{max=0;}
	if ( running_jobs <= min ){min = min - running_jobs;}else{min=0;}
	if ( free_proc <= free_actual ){processable = free_actual - free_proc;}
	if ( processable <= max ){value = processable;}else{value=max;}
	if ( (value+running_jobs) <= min ){value = min - running_jobs;}
	if ( waiting > 0 ){value = 0;}
	return value;
}

bool submit_next(std::ifstream & myifile, long & JobID, std::string & path)
{
	JobID = -1;
	path = "";
	
	std::string line,dirname;
	std::string home = exec("pwd");


	if ( ! myifile.good() )
	{	
		return false;
	} else {
		getline(myifile, line);
		if ( line == "" ){return false;}
		path = props::ReadColumn(line, 1);
		if ( ! exists(path) ){JobID = -2; return false;}
		dirname = path.substr(0,path.find_last_of("/"));
		line = exec("cd " + dirname + " && submit " + line + " | tail -1 && cd " + home);					
		//line = exec("echo $RANDOM " + line);					
		JobID = first_num(line);
		return true;
	}
}

void submit_list(std::string queue_file, std::string filename, std::ofstream & fo_running, int & to_do)
{
	std::stringstream buf;
	std::ifstream fi_queue;
	std::ofstream fo_queue;
	std::string temp_queue_file = queue_file + "_temp";
        fi_queue.open(queue_file);
        fo_queue.open(temp_queue_file);

        long jobnum = 1;
	std::string line;


        while ( jobnum != -1 && to_do > 0 )
        {
	
                if ( submit_next(fi_queue, jobnum, line) )
                {
                        buf.str(std::string());
			buf << get_time() << "|SUBMIT| " << jobnum << " " << line.substr(0,line.find_last_of("/")) << " / " << line.substr(line.find_last_of("/") + 1) << std::endl;
                        logfile(buf.str());

                        buf.str(std::string());
			buf << jobnum << " " << line.substr(0,line.find_last_of("/")) << std::endl;
                        fo_running << buf.str();

			to_do--;
                }
                buf.str(std::string());
		buf << line.substr(0,line.find_last_of("/")) << std::endl;
        }

	while ( fi_queue.good() )
	{
		getline(fi_queue, line);
		if ( line == "" ){break;}
		fo_queue << line << std::endl;
	}

	fi_queue.close();
	fo_queue.close();
	rename(temp_queue_file.c_str(), queue_file.c_str());
}

void clean_erroneous()
{
	std::string allEqw = exec("qstat | grep wondrash | grep \" Eqw \"");
	long number;
	std::stringstream ss;
	while ( allEqw.size() > 1 )
	{
		number = first_num(allEqw);
		ss.str(""); ss << number;
		exec("qdel " + ss.str());
		allEqw = allEqw.substr(allEqw.find('\n',1));
	}
		
}

}
