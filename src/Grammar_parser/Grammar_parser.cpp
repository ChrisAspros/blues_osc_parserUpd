//
//  Grammar_parser.cpp
//  
//
//  Created by Christodoulos Aspromallis on 25/04/2016.
//
//

#include "Grammar_parser.h"

G_parser::G_parser() : grammar(grammar_path, std::ifstream::in){

    //"int main(){}"
    
    //grammar_path = "/Users/christodoulosaspromallis/Desktop/UCL/PhD Year 2/Development/OF/of_v0.9.1_osx_release/apps/midiWorkspace/blues/parser_project/bin/data/blues_grammar.txt";
    //cout << "CONTRUCTOR" << endl;
    
    grammar_path = "/Users/christodoulosaspromallis/Desktop/UCL/PhD Year 2/Development/OF/of_v0.9.1_osx_release/apps/midiWorkspace/blues/parser_project/bin/data/blues_grammar.txt";
    
    //ifstream grammar(grammar_path, ifstream::in);
    grammar.open(grammar_path);
    
    if (grammar.fail()){
        cerr << "Error Opening File";
        exit(1);
    };
    
    //cout << "setting up" << endl;
    
    //get main grammar elements
    while(!grammar.fail()){//z<12//!grammar.eof() //or while(grammar)
        nc = get_nc();
        //c = grammar.get();
        //cout << z << "_1: " << nc << endl;
        get_time_signature(nc);
        //cout << z << "_2: " << nc << endl;
        get_form_length(nc);
        //cout << z << "_3: " << nc << endl;
        get_harm_rh(nc);
        //cout << z << "_4: " << nc << endl;
        get_basic_vectors(nc);
        //cout << z << "_5: " << nc << endl;
        store_rules(nc);//normally will finish the significant bit of text file (i.e. the rules are last)
        //cout << z << "_6: " << nc << endl;
        
    }
    //restart reading from top (for next loop)
    grammar.clear();
    grammar.seekg(0, ios::beg);
    
    //initialise curr_cycle with tonics (rootpitch level)
    for(int i=0; i<harm_rh*form_length; i++) {
        
        curr_cycle.push_back(elem_ID());
        curr_cycle[i].time = {0, i};// {beat, harm_rh next..}
        curr_cycle[i].name = "i";
    }
    
    //place decs for the 1st cycle
    vector<int> t_aux = {0, 0, 0, 11, 0};
    update_dec(t_aux);
    
    grammar.close();
    
}


void G_parser::store_rules(string& nc){
    
    //new rule
    if (nc=="rule:"){
        //cout << "rules: entered and harn_rh==" << harm_rh << " and form_length==" << form_length << endl;
        all_rules.push_back(rule());
        all_rules[rule_pop].is_optional = false;
        
        nc = get_nc();
        
        //left side
        while(nc!="->"){
            all_rules[rule_pop].left_str.push_back(nc);
            store_opt_data(nc);
            nc = get_nc();
        }
        
        //if (nc!="->") cout << "ERROR in line " << i << ": arrow required" << endl;
        int prod_pop=0;//production population
        
        while(nc!=":end_rule"){//store right_side
            all_rules[rule_pop].right_side.push_back(right_s());
            nc = get_nc();//production probability
            all_rules[rule_pop].right_side[prod_pop].prob = atof(nc.c_str());
            nc = get_nc();
            
            //store right_str
            while(nc!="->" && nc!=":end_rule"){
                all_rules[rule_pop].right_side[prod_pop].right_str.push_back(nc);//={"STR1", "STR2"};//
                nc = get_nc();
            }
            
            prod_pop++;
        }
        
        bool is_timed = false;
        vector<int> rule_time = {0, 0};//{beat, bar}
        
        //categorise (timed vs general)
        for (int i=0; i<all_rules[rule_pop].left_str.size(); i++) {
            
            for (int j=0; j<all_rules[rule_pop].left_str[i].length(); j++){
                
                if (all_rules[rule_pop].left_str[i][j]=='_'){
                    
                    is_timed = true;
                    int left_centre = i;
                    
                    //rule_time.clear();
                    
                    string bar_time_str, beat_time_str; // rule_semiq_str;//could be one variable only
                    j++;
                    
                    while(all_rules[rule_pop].left_str[i][j] != '_' && j<all_rules[rule_pop].left_str[i].length()){
                        bar_time_str.push_back(all_rules[rule_pop].left_str[i][j]);
                        j++;
                    }
                    
                    rule_time[1] = atoi(bar_time_str.c_str()) - 1;
                    
                    if (all_rules[rule_pop].left_str[i][j] == '_'){
                        
                        while (j<all_rules[rule_pop].left_str[i].length()){
                            j++;
                            beat_time_str.push_back(all_rules[rule_pop].left_str[i][j]);
                        }
                        
                        rule_time[0] = atoi(beat_time_str.c_str()) - 1;
                        
                    } else rule_time[0] = 0;
                    
                    all_rules[rule_pop].leftmost_time =  {rule_time[0], rule_time[1] - (i / harm_rh)};
                    
                    //exclude "_*.."
                    all_rules[rule_pop].left_str[i] = exclude_time(all_rules[rule_pop].left_str[i]);
                    
                    //existing_rules.push_back();
                    break;
                    
                } else all_rules[rule_pop].leftmost_time = {0, 0};
                
            }
            
            if (is_timed) break;
            
        }
        
        all_rules[rule_pop].timed = is_timed;
        
        if (is_timed) {
            //cout << "rule_time: " << rule_time[0] << " " << rule_time[1] << endl;
            existing_times.push_back(rule_time);//to test existence of rule_time, ELSE general_rules
            timed_rules[rule_time].push_back(all_rules[rule_pop]);
            
        } else {
            
            general_rules.push_back(all_rules[rule_pop]);
            //cout << "general rule" << endl;
        }
        
        rule_pop++;
    }
}


void G_parser::store_opt_data(string& nc){
    
    //all_rules[rule_pop].options = {};//is this necessary? NOT! - {""} is first element..
    
    for(int i=0; i<nc.length(); i++){
        
        if (nc[i]=='|'){
            all_rules[rule_pop].is_optional = true;
            all_rules[rule_pop].opt_positions.push_back(all_rules[rule_pop].left_str.size() - 1);
            store_options(nc);
        }
    }
}


void G_parser::store_options(string& nc){
    
    string curr_opt;
    vector<string> curr_opt_v;
    
    for (int i=0; i<nc.length(); i++){
        
        while (nc[i]!='|' && i<nc.length()){
            
            curr_opt.push_back(nc[i]);
            i++;
        }
        
        //all_rules[rule_pop].options.push_back(curr_opt);
        curr_opt_v.push_back(curr_opt);
        curr_opt.erase();
    }
    
    all_rules[rule_pop].options.push_back(curr_opt_v);
    /*
     cout << "curr_opt_v added: ";
     for(int i=0; i<curr_opt_v.size(); i++){
     cout << curr_opt_v[i] << " ";
     }
     cout << endl;
     */
}


string G_parser::exclude_time(string& s_t_r){
    
    string new_str;
    for(int i=0; i<s_t_r.length(); i++){
        
        if (s_t_r[i] == '_') break;
        new_str.push_back(s_t_r[i]);
    }
    
    return new_str;
}



void G_parser::find_rule(vector<int>& seq_t){
    
    //cout << "reached 1" << endl;
    
    //cout << "seq_t: " << seq_t[2] << " & " << seq_t[3] << endl;
    // if (previous (in left_str) is rootpitch) { search for its terminal equivalents; }
    //if IV in previous context (instead of iv) then lookup corresponding chords (IV = iv*, iv6*, iv7*) OR (IV = iv*)
    //what about IV instead of iv later? (what if already translated as iv from IV?) --> e.g. "IV I_3" ->...
    //      if context not found, look for equivalent rootpitch
    //      perhaps categorise IV as iv|iv6|iv7
    
    bool rule_found = false;
    bool opt;
    rule r;
    
    int cont_size;
    vector<int> leftmost_t;
    
    //find (if) timed_rule
    //search existing_times
    for (int i=0; i<existing_times.size(); i++){//replace with while()
        
        //cout << "reached 2" << endl;
        if (seq_t[2]==existing_times[i][0] && seq_t[3]==existing_times[i][1]){//seq_t[2,3] for beat, bar..
            
            //cout << "reached 3" << endl;
            //check context (match rule with current context)
            for (int j=0; j<timed_rules[{seq_t[2], seq_t[3]}].size(); j++){
                
                //cout << "reached 4" << endl;
                //check_context(seq_t);
                
                //i6 could map to I etc. if context not found ?
                cont_size = timed_rules[{seq_t[2], seq_t[3]}][j].left_str.size();
                leftmost_t = timed_rules[{seq_t[2], seq_t[3]}][j].leftmost_time;
                opt = timed_rules[{seq_t[2], seq_t[3]}][j].is_optional;
                
                vector<string> curr_cont = get_context(leftmost_t, cont_size);
                
                if (timed_rules[{seq_t[2], seq_t[3]}][j].is_optional){//timed and optional
                    
                    rule r = timed_rules[{seq_t[2], seq_t[3]}][j];
                    rule_found = check_optional(r, seq_t);
                    
                    if (rule_found) break;
                    
                }
                else {//timed, but not optional
                    
                    //cout << "reached 11" << endl;
                    if (curr_cont == timed_rules[{seq_t[2], seq_t[3]}][j].left_str){
                        
                        //cout << "reached 12" << endl;
                        rule_found = true;
                        
                        //expand to support optional too
                        r = timed_rules[{seq_t[2], seq_t[3]}][j];
                        
                        the_rule = r;
                        //debug
                        cout << "the_rule: ";
                        for (int j=0; j<the_rule.left_str.size(); j++) cout << the_rule.left_str[j] << ", ";
                        cout << endl;
                        
                        rewrite(r, seq_t);
                        cout << "case 2" << endl;//timed, but not optional
                        
                        break;
                    }
                }
                
            }
            
            break;
        }
    }
    
    //else find general_rule
    if(!rule_found) {//replace with while()
        
        //search general_rules (context again)
        for (int i=0; i<general_rules.size(); i++){
            
            //check if context exists before or after current chord (elem)
            int c_off = - general_rules[i].left_str.size();//context offset (back n forth)
            cont_size = general_rules[i].left_str.size();
            //leftmost_t = general_rules[i].leftmost_time;//is {0, 0}
            opt = general_rules[i].is_optional; //need to access specific general_rule
            
            for (int z = c_off; z < 0; z++){
                
                general_rules[i].leftmost_time[1] = seq_t[3] + (z+1) / harm_rh;
                
                vector<string> curr_cont = get_context(general_rules[i].leftmost_time, cont_size);
                
                //check context if(general_rule[i].is_optional)
                if (opt){
                    
                    r = general_rules[i];
                    rule_found = check_optional(r, seq_t);
                    if (rule_found) break;
                    
                }
                
                //check context if (!general_rule[i].is_optional)
                else if(get_context(general_rules[i].leftmost_time, cont_size) == general_rules[i].left_str){
                    
                    rule_found = true;
                    
                    r = general_rules[i];
                    
                    the_rule = r;
                    //debug
                    cout << "the_rule: ";
                    for (int j=0; j<the_rule.left_str.size(); j++) cout << the_rule.left_str[j] << ", ";
                    cout << endl;
                    
                    rewrite(r, seq_t);
                    cout << "case 5" << endl;//general and not optional
                    
                    break;
                }
            }
            
            if (rule_found) break;
        }
    }
    
    cout << "is_terminal?: " << curr_cycle[seq_t[3]].name << endl << endl;
    //stop if terminal
    if (!is_terminal(seq_t)) find_rule(seq_t);
    //else return_terminal(curr_cycle[seq_t[3]].name);
    else translate_to_midi();//translate / spit out (return_terminal())
}


bool G_parser::check_optional(rule& r, vector<int>& seq_t){
    
    bool r_found = false;
    
    //cout << "reached 5" << endl;
    size_t _index;
    int matches = 0;
    
    vector<int> l_t = r.leftmost_time;
    int cont_size = r.left_str.size();
    vector<string> g_c = get_context(l_t, cont_size);
    int k=0;
    //compare context with left_str
    for (int i=0; i < r.left_str.size(); i++){
        
        //cout << "reached 6" << endl;
        vector<int> v_int = r.opt_positions;
        vector<int>::iterator it_int = find(v_int.begin(), v_int.end(), i);
        _index = it_int - v_int.begin();
        
        if (it_int!=v_int.end()){
            
            //cout << "reached 7" << endl;
            vector<string> v_str = r.options[_index];
            vector<string>::iterator it_str = find(v_str.begin(), v_str.end(), g_c[i]);
            
            if (it_str!=v_str.end()) {
                
                //cout << "reached 8" << endl;
                matches++;
            }
        }
        else {
            
            //cout << "reached 9" << endl;
            if (g_c[i]==r.left_str[i]) matches++;
        }
        
    }
    
    if (matches==g_c.size()){
        
        //cout << "reached 10" << endl;
        r_found = true;
        
        the_rule = r;
        //debug
        cout << "the_rule: ";
        for (int j=0; j<the_rule.left_str.size(); j++) cout << the_rule.left_str[j] << ", ";
        cout << endl;
        
        rewrite(r, seq_t);
        cout << "case 1" << endl;//timed and optional
    }
    
    return r_found;
}


G_parser::rule G_parser::check_context(vector<int>& seq_t){
    //if optional check for both
    //else check as was
}


vector<string> G_parser::get_context(vector<int>& leftmost_t, int& size){
    
    //adapt to form_length, time_signature
    leftmost_t[1] = (leftmost_t[1] + form_length) % form_length;//it may be negative initially
    //cout << "leftmost_t[1]: " << leftmost_t[1] << endl;
    
    //leftmost[1] += leftmost[0] / t_sign;
    //leftmost[0] = leftmost[0] % t_sign;
    vector<elem_ID> context;
    vector<string> context_str;
    
    for (int i=0; i<size; i++){
        
        int mod = (leftmost_t[1] + i) % form_length;//to cycle up is form_length is exceeded
        context.push_back(curr_cycle[mod]);
        context_str.push_back(context[i].name);//should be string because left_str is not elem_ID (yet..)
    }
    
    return context_str;
}


void G_parser::rewrite(rule& r, vector<int>& seq_t){
    
    cout << "rewriting_Rule:";
    //for (int i=0; i<r.left_str.size(); i++) cout << r.left_str[i] << ", ";
    cout << endl;
    
    vector<elem_ID> production;
    int choice = rewrite_choice(r);
    
    //produce from rule
    for (int i=0; i<r.right_side[choice].right_str.size(); i++){
        
        production.push_back(elem_ID());
        production[i].name = r.right_side[choice].right_str[i];
        //else (generals.leftmost_time == {0 ,0})
        
        vector<int> t_aux(2);
        
        t_aux[0] = r.leftmost_time[0] % t_sign;
        t_aux[1] = ((r.leftmost_time[1]+i / harm_rh) + form_length) % form_length;
        
        production[i].time = t_aux;
    }
    
    update_cycle(production, r, seq_t);
    
    //production.clear();
    
    /*
     for (int i=0; i<production.size(); i++){
     cout << "name" << i << ": " << production[i].name << ", time: " << production[i].time[0] << " & " << production[i].time[1] << endl;
     }
     */
}


int G_parser::rewrite_choice(rule& r){
    
    int _choice;
    vector<float> p_vec;
    for (int i=0; i<r.right_side.size(); i++) p_vec.push_back(r.right_side[i].prob);
    
    float total_p = 0;
    srand(time(NULL));
    
    //cout << "probs: ";
    //for (int i=0; i<p_vec.size(); i++) cout << p_vec[i] << ", ";
    //cout << endl;
    
    float random = (rand()%100) / 100.0;
    //cout << "random: " << random << endl;
    
    for (int i=0; i<p_vec.size(); i++){
        
        if (random >= total_p && random <= total_p+p_vec[i]) {
            //cout << "p_vec " << i << ": " << p_vec[i] << endl;
            _choice = i;
            //cout << "re_choice: " << i << " // ";
            break;
        }
        
        total_p += p_vec[i];
    }
    
    return _choice;
}


void G_parser::update_cycle(vector<elem_ID>& production, rule& r, vector<int>& seq_t){
    
    for(int i=0; i<production.size(); i++){
        
        //if optional element, don't update that!!
        vector<int>::iterator it = find(r.opt_positions.begin(), r.opt_positions.end(), i);
        if (it==r.opt_positions.end()) curr_cycle[production[i].time[1]] = production[i];//harm_rh-dependent placement..??
        
    }
    
    //at end of cycle (after last rewrite) place dec (or dec_* ??) at decision points
    update_dec(seq_t);
    
    update_ending(seq_t);//will overwrite update_dec()
    
    //debug
    cout << "new cycle (update_cycle()): ";
    for (int i=0; i<form_length; i++) cout << curr_cycle[i].name << " ";
    cout << endl << "=======" << endl;
}


void G_parser::update_dec(vector<int>& seq_t){
    
    if (seq_t[3] == form_length / harm_rh - 1 && !ending && is_terminal(seq_t)){
        
        for (int i=0; i<dec_bars.size(); i++) curr_cycle[dec_bars[i]].name = "dec";
    }
}


void G_parser::update_ending(vector<int>& seq_t){
    
    check_cadence(seq_t);
    
    if (!goal_reached){
        
        if (ending){
            
            if(!cad_updated) update_cad(seq_t);//with EVERY rewrite()??
            
            //upade_finals() one or zero bars before it comes (i.e. 2nd bar of cad) - if not recovering of course
            //int c_p = (cad_pos+1) % form_length;
            
            update_finals(seq_t);
            
        }
        else if (cad_updated) recover(seq_t);//cad_updated??
    }
    else {
        //when goal reached, no turning back..
        
        int end_bar = (cad_pos + 2) % form_length;
        string c_c = curr_cycle[seq_t[3]].name;
        
        if (!cad_updated) update_cad(seq_t);
        update_finals(seq_t);
        //fin_updated;
        
        if (end_bar <= seq_t[3] && (c_c == "i" || c_c == "i6" || c_c == "i7")) stop_sequencer();
        //spit out last I term chord
    }
}

//print in console.txt
//TEST
//reach goal from cad
//reach goal from cad & fin
//reach goal from recovering
//reach goal !ending && !recovering //NOT WORKING for now..

void G_parser::update_cad(vector<int>& seq_t){
    
    cout << "update_cad" << endl;
    
    cad_updated = true;
    
    //place next "cad" point (only..)
    if (seq_t[3] >= cad_bars.back()) {
        
        curr_cycle[cad_bars[0]].name = "cad";
        cad_pos = cad_bars[0];
    }
    
    else {
        for (int i=0; i<cad_bars.size(); i++){
            
            if (seq_t[3] < cad_bars[i]) {
                
                curr_cycle[cad_bars[i]].name = "cad";
                cad_pos = cad_bars[i];
                break;
            }
        }
    }
}


void G_parser::recover(vector<int>& seq_t){
    
    cout << "recover" << endl;
    
    fin_updated = false;
    cad_updated = false;
    cadenced = false;
    
    update_optimal(seq_t);
    reconcile(seq_t);//place "rec" in between!
}


void G_parser::update_optimal(vector<int>& seq_t){
    
    cout << "update_optimal" << endl;
    
    //if end of form
    int optimal_pos = (seq_t[3] + 2) % form_length;
    
    //TWO bars later?
    for (int i=optimal_pos; i<curr_cycle.size(); i++) curr_cycle[i].name = basic_vector["optimal_form"][i];
}


void G_parser::reconcile(vector<int>& seq_t){
    
    cout << "reconcile" << endl;
    
    int rec_pos = (seq_t[3] + 1) % form_length;
    curr_cycle[rec_pos].name = "rec";
}


void G_parser::update_finals(vector<int>& seq_t){
    
    cout << "update_finals" << endl;
    
    //fin meta to cad mexri prin apo kei pou eimai
    
    if (!goal_reached && seq_t[3]==11 && cadenced) {
        cout << "finals_1" << endl;
        for (int i=cad_pos+1; i<curr_cycle.size() - 1; i++) curr_cycle[i].name = "fin";
        //curr_cycle[11].name = "V";
    }
    else if (!fin_updated){
        cout << "finals_2" << endl;
        int fin_pos = (cad_pos + 2) % form_length;
        
        //!=0 otherwise "fin" * 12 -> there is no general && contextless fin rule, so 12*fin -> infinite..
        if (fin_pos!=0) for (int i=fin_pos; i<curr_cycle.size(); i++) curr_cycle[i].name = "fin";
    }
    
    fin_updated = true;
}


void G_parser::check_cadence(vector<int>& seq_t){
    
    if (seq_t[3]==cad_pos) {
        
        cout << "cadenced ture???" << endl;
        cadenced = true;
        cad_pos = -1;
    }
}


void G_parser::stop_sequencer(){
    
    cout << "stop_sequencer" << endl;
    stop_seq = true;
}


bool G_parser::is_terminal(vector<int>& seq_t){
    
    bool is_t = false;
    vector<string>::iterator it = find(basic_vector["terminals"].begin(), basic_vector["terminals"].end(), curr_cycle[seq_t[3]].name);
    
    if (it!=basic_vector["terminals"].end()) is_t = true;
    return is_t;
}


string G_parser::return_terminal(vector<int>& seq_t){//OVERLOAD FIND_RULE(..)
    
    return curr_cycle[seq_t[3]].name;
}


void G_parser::translate_to_midi(){
    
    trigger();
}


void G_parser::trigger(){}


void G_parser::get_rootpitch(int& bar){
    //for bar
}


void G_parser::translate(string& chord){
    //map terminals to what blues/seq wants
}


void G_parser::get_time_signature(string& nc){
    if (nc=="time_signature"){
        nc = get_nc();
        t_sign = atoi(nc.c_str());
        //cout << "t_sign is: " << nc << endl;
    }
}


void G_parser::get_form_length(string& nc){
    if (nc=="form_length"){
        nc = get_nc();
        form_length = atoi(nc.c_str());
        //cout << "form_length is: " << form_length << endl;
    }
}


void G_parser::get_harm_rh(string& nc){
    if (nc=="harmonic_rhythm"){
        nc = get_nc();
        harm_rh = atoi(nc.c_str());
        //cout << "harm_rh is: " << harm_rh << endl;
    }
}


void G_parser::get_basic_vectors(string& nc){//perhaps get all namespace and categorize/throw ERROR if unrecognised?
    bool test = (nc=="decision_bars"||nc=="cadence_bars"||nc=="optimal_form"||nc=="rootpitch"||nc=="type"||nc=="inversion"||nc=="position"||nc=="terminals");
    if (test){
        string v_name = nc;
        nc = get_nc();
        if (nc=="{"){
            nc = get_nc();
            while(nc!="}"){
                if (v_name=="decision_bars"){
                    int curr_bar = atoi(nc.c_str()) - 1;//only if nc=="decision_bars"
                    dec_bars.push_back(curr_bar);
                }
                else if (v_name=="cadence_bars"){
                    int curr_bar = atoi(nc.c_str()) - 1;//only if nc=="decision_bars"
                    cad_bars.push_back(curr_bar);
                }
                else basic_vector[v_name].push_back(nc);
                nc = get_nc();
            }
            sort(dec_bars.begin(), dec_bars.end());
            sort(cad_bars.begin(), cad_bars.end());
        }
    }
}


string G_parser::get_nc(){//get comment-filtered item

    string s;
    grammar >> s;
    nc = comment_filter(s);
    return nc;
}


string G_parser::comment_filter(string& c){
    
    string nc = c;
    vector<int> v = {0, 0};
    
    for (int i=0; i<c.length(); i++){
        if (c[i]=='/' && c[i+1]=='/'){
            v = {1, i};//{comment_found, point_in_string}
            i = c.length();
        } //else v = {0, 0};
    }
    
    if(v[0]){
        
        if (v[1]==0){
            grammar.ignore(numeric_limits<streamsize>::max(), '\n');
            grammar >> c;
            if (!grammar.eof()){
                nc = comment_filter(c);
            }
        }
        
        else {
            nc = c.substr(0, v[1]);//for (int x=0; x<i; i++) nc.push_back(c[x]);
            //com = c.substr(comment_check(c)[1], c.length());
            //http://www.cplusplus.com/reference/string/string/substr/
            grammar.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    
    return nc;
}


void G_parser::print_rules(){
    
    for (int i=0; i<all_rules.size(); i++){
        cout << "rule " << i << ": ";
        
        for (int j=0; j<all_rules[i].left_str.size(); j++){
            cout << all_rules[i].left_str[j] << " ";
        }
        
        for (int j=0; j<all_rules[i].right_side.size(); j++){
            cout << "-> " << all_rules[i].right_side[j].prob << " ";
            
            for (int k=0; k<all_rules[i].right_side[j].right_str.size(); k++){
                cout << all_rules[i].right_side[j].right_str[k] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
    
}