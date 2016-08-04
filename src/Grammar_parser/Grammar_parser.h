//
//  Grammar_parser.h
//  
//
//  Created by Christodoulos Aspromallis on 25/04/2016.
//
//

#ifndef ____Grammar_parser__
#define ____Grammar_parser__

#include <stdio.h>
#include "ofxMidiOut.h"
//#include "ofxMidi.h" //??



class G_parser{

public:
    
    G_parser();// : grammar(grammar_path, std::ifstream::in){}
    //~G_parser();
    
    string grammar_path;// = "/Users/christodoulosaspromallis/Desktop/UCL/PhD Year 2/Development/OF/of_v0.9.1_osx_release/apps/midiWorkspace/blues/parser_project/bin/data/blues_grammar.txt";
    ifstream grammar;
    
    string nc, c;
    
    string get_nc();
    string comment_filter(string& c);
    
    void get_time_signature(string& nc);
    void get_form_length(string& nc);
    void get_harm_rh(string& nc);
    void get_basic_vectors(string& nc);
    
    int t_sign;
    int form_length;
    int harm_rh;
    vector<int> dec_bars;
    vector<int> cad_bars;
    map<string, vector<string>> basic_vector;
    
    void store_rules(string& nc);
    vector<vector<int>> existing_times;
    int rule_pop = 0;
    //int leftmost_time;
    
    int aux = 0;
    
    void store_opt_data(string& nc);
    void store_options(string& nc);
    
    struct right_s{
        vector<string> right_str;
        float prob;
    };
    struct rule{
        vector<string> left_str;
        vector<right_s> right_side;
        vector<int> leftmost_time;//{beat, bar}
        
        bool timed;
        
        //optional elements
        bool is_optional;
        vector<int> opt_positions; //positions of optional elements in left_str
        vector<vector<string>> options;
    };
    
    vector<rule> all_rules;//optimise by sending straight to timed or general rules..
    //vector<rule> timed_rules, general_rules;//subtotal of all_rules
    map<vector<int>, vector<rule>> timed_rules;//specified rule: labels timed rules (t vector)
    vector<rule> general_rules;//store non-timed rules
    
    string exclude_time(string& s_t_r);
    
    void print_rules();
    
    struct elem_ID{
        string name;
        vector<int> time;//beat, bar (harm_rh next)
    };
    
    struct type_ID{
        string name;
        vector<int> time;//corrsponding time in music form
    };
    
    void find_rule(vector<int>& seq_t);
    bool check_optional(rule& r, vector<int>& seq_t);
    bool is_terminal(vector<int>& seq_t);
    string return_terminal(vector<int>& seq_t); //better to overload find_rule??
    void translate_to_midi();
    void trigger();
    
    void get_rootpitch(int& bar);
    
    rule check_context(vector<int>& seq_t);
    void rewrite(rule& r, vector<int>& seq_t);
    int rewrite_choice(rule& r);
    vector<string> get_context(vector<int>& leftmost_t, int& size);
    
    vector<elem_ID> curr_cycle;
    void update_cycle(vector<elem_ID>& production, rule& r, vector<int>& seq_t);//size() = [form_length * harmonic_rhythm]
    void update_dec(vector<int>& seq_t);
    void update_ending(vector<int>& seq_t);
    void update_cad(vector<int>& seq_t);
    void recover(vector<int>& seq_t);
    void update_optimal(vector<int>& seq_t);
    void reconcile(vector<int>& seq_t);
    void update_finals(vector<int>& seq_t);
    void stop_sequencer();
    void check_cadence(vector<int>& seq_t);
    
    void translate(string& chord);//translate to MIDI//what blues / sequencer wanna see
    
    rule the_rule;
    bool soon = false;// <= four bars' time (+ smth maybe..)
    bool approaching = true;
    bool ending = (soon && approaching);//continuously updated
    bool cadenced = false;
    bool goal_reached = false;
    bool cad_updated = false;
    int cad_pos;
    bool recovering = !ending;//before or after cad?
    bool fin_updated = false;
    bool stop_seq = false;
    
};

#endif /* defined(____Grammar_parser__) */
