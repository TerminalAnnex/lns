#include <iostream>
#include "errors.h"
#include "scanner.h"
#include "expr.h"
#include "parser.h"
#include "interpreter.h"
#include "debug.h"
#include <iomanip>
#include "signals.h"
#include "update.h"

#define PROGRAM_STARTING "lns rev. " << std::to_string(REVISION_NUMBER) << ", interactive mode.\n"

void register_signal_handlers();

using namespace std;
using namespace errors;
using namespace lns;
using namespace std::chrono;
namespace lns{

    static char * file = nullptr;
    static auto start_time = high_resolution_clock::now(),parsing_time = high_resolution_clock::now(),execution_time = high_resolution_clock::now();

    void version() {
        cout << "lns revision no. " << to_string(REVISION_NUMBER) << endl;
        exit(EXIT_SUCCESS);
    }

    bool starts_with(string &text, const char *ptr) {
        string start(ptr);
        if (start.length() > 0 && text.length() > start.length()) {
            for (int i = 0; i < start.length(); i++)
                if (text[i] != start[i]) return false;
            return true;
        } else return false;
    }


    void parametric_option(string &o) {
        if (starts_with(o, "x=")) {
            exec_from_command_line = true;
            command_line_source = o.substr(2, o.size());
        } else if (starts_with(o, "-use=")) {
            string sub = o.substr(5, o.size());
            for (auto i = 0u; i < sub.length(); ++i) {
                string s1;
                while (sub[i] != ',') {
                    s1 += sub[i++];
                    if (i >= sub.length()) break;
                }
                lns::command_line_use.emplace_back(s1);
            }
        } else {
            throw unknown_option_exception(o);
        }
    }

    void option(string& o){
        if(o == "i" || o == "-ignore-unresolved"){
            ignore_unresolved = true;
        }else if(o == "s" || o == "-silent-execution"){
            silent_execution = true;
        }else if(o == "S" || o == "-silent-full"){
            silent_full = true;
        }else if(o == "-permissive"){
            permissive = true;
        }else if(o == "t" || o == "-time-info"){
            time_count = true;
        }else if(o == "p" || o == "-parse-only"){
            parse_only = true;
        }else if(o == "d" || o == "-debugger"){
            debugger_option = true;
        }else if(o == "f" || o == "-no-format"){
            no_format = true;
        } else if (o == "u" || o == "-update") {
            update_option = true;
        } else if (o == "-no-std") {
            no_std_option = true;
        } else if (o == "v" || o == "-version") {
            version();
        } else
            parametric_option(o);
    }


    void inspect_args(int fc, const char **args){
        int i;
        string s;
        for(i = 1;i <= fc; i++){
            s = args[i];
            if(args[i][0] == '-'){
                s = string(args[i]).substr(1);
                option(s);
            }else{
                if(file == nullptr){
                    file = (char*)malloc((strlen(args[i]) + 1) * sizeof(char));
                    strcpy(file,args[i]);
                }else{
                    throw wrong_usage_exception();
                }
            }
        }
    }

    void run(const char *filename, string &source, interpreter &i, parser &parser, bool ld_std, bool ld_additional) {
        scanner scn (filename,source);
        vector<token*> tokens = scn.scan_tokens(true);
        if(had_parse_error) throw parse_exception();
        start_time = high_resolution_clock::now();
        parser.reset(tokens);
        vector<shared_ptr<stmt>> stmts = parser.parse(ld_std, ld_additional ? lns::command_line_use : vector<string>());
        parsing_time = high_resolution_clock::now();
        if(parse_only){
            if(time_count) cout << "\nParsing time: " << std::setprecision(5) << duration_cast<microseconds>(parsing_time - start_time).count()/1000 << "ms.\n";
            if(lns::prompt) return; else std::exit(errors::exit_status);
        }
        //cout << "Total statements: " << to_string(stmts.size()) << endl;
        if(had_parse_error) throw parse_exception();
        i.interpret(stmts);
        execution_time = high_resolution_clock::now();
        if(time_count){
            cout << endl;
            cout << "\nParse time:   " << std::setprecision(5) << (double)duration_cast<microseconds>(parsing_time - start_time).count()/1000 << "ms.\n";
            cout << "Execution time: " << std::setprecision(5) << (double)duration_cast<microseconds>(execution_time - parsing_time).count()/1000 << "ms.\n";
            cout << "Total time:     " << std::setprecision(5) << (double)duration_cast<microseconds>(execution_time - start_time).count()/1000 << "ms.\n";
        }
    }

    void reinit_errors() {
        had_parse_error = false;
        had_runtime_error = false;
    }

    void move_statements(std::vector<shared_ptr<stmt>> &new_, std::vector<shared_ptr<stmt>> &past) {
        auto iterator = std::next(new_.begin(), new_.size());
        std::move(new_.begin(), iterator, std::back_inserter(past));
        new_.erase(new_.begin(), iterator);
    }

    void run_interactive() {
        lns::prompt = true;
        string source;
        const char filename[] = "stdin";
        scanner s("", source);
        parser p(s.scan_tokens(true));
        vector<shared_ptr<stmt>> past;
        cout << PROGRAM_STARTING;
        interpreter i;
        auto stmts = p.parse(!no_std_option, lns::command_line_use);
        i.interpret(stmts);
        while(true){
            cin.clear();
            cout << "> ";
            getline(cin,source);
            try {
                p.use_allowed = true;
                move_statements(p.statements, past);
                run(filename, source, i, p, false, false);
            } catch (std::exception &e) {}
            reinit_errors();
        }
    }

    void run_file(const char* filename){
        ifstream file (filename);
        string source;
        stringstream ss;
        parser p;
        if(file.is_open()){
            stringstream ss;
            ss << file.rdbuf();
            source = ss.str();
            interpreter i;
            run(filename, source, i, p, !no_std_option, true);
        }else{
            throw file_not_found_exception(filename);
        }
    }

    void run_cmd_line() {
        parser p;
        interpreter i;
        run("stdin", lns::command_line_source, i, p, !no_std_option, true);
    }
}


void register_signal_handlers() {
    signal(SIGINT,__signal_handler);
    signal(SIGSEGV,__signal_handler);
    signal(SIGABRT,__signal_handler);
}

int main(const int argc, const char* argv[]) {
    register_signal_handlers();
    try{
        lns::inspect_args(argc - 1, argv);
        if (lns::debugger_option) {
            debug d(file);
            d.start();
        } else if (lns::update_option) {
            return update();
        } else if (lns::exec_from_command_line) {
            lns::run_cmd_line();
        } else if (lns::file == nullptr) {
            lns::run_interactive();
        } else {
            lns::run_file(lns::file);
        }
    }catch(lns::parse_exception& e){
        fatal_error(e, PARSE_ERROR);
    }catch(std::exception& e){
        fatal_error(e, FATAL_ERROR);
    }
    return EXIT_SUCCESS;
}
