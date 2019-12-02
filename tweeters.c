#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CHARS 1024
#define MAX_LINES 20000

#define invalid_error()           \
do{                               \
  printf("Invalid Input Format\n"); \
  return 0;                       \
} while(0)

//define macro to print out invalid format

typedef struct tweeter{
  char* name;
  int num_tweets;
}tweeter;

/******************Globals************/
int name_index;
char cells[MAX_CHARS][MAX_CHARS];
struct tweeter tweeters[MAX_LINES];
/*************************************/


bool check_for_quotes(char* str, int num_chars, bool quoted){
    if(strcmp(str, "\"") == 0){
        return false;
    }

    if(quoted){
        if(str[0] != '\"' && str[num_chars-1] != '\"'){
            return false;
        }
    }else{
        if(str[0] == '\"' || str[num_chars-1] == '\"'){
            return false;

        }
    }
  return true;
}

bool has_duplicates(int num_words){

    char* current_item;
    char* item_to_compare_to;
    for(int i = 0; i < num_words; ++i){
        current_item = cells[i];
        for(int j = i + 1 ; j < num_words; ++j){
            item_to_compare_to = cells[j];
            if(strcmp(current_item,item_to_compare_to) == 0 ){
                return true;
            }

        }
    }

    return false;
}

int get_num_name(int num_words){

    char* current_item;
    int num_names = 0;
    for(int i = 0; i < num_words; ++i){
        current_item = cells[i];
        if(strcmp(current_item,"name") == 0 || strcmp(current_item,"\"name\"")  == 0){
            name_index = i;
            ++num_names;
        }

    }

    return num_names;
}

char* get_name(){
    return cells[name_index];
}

void check_for_tweeter(int* num_tweeters, char* tweeter_to_add){

  //check current list to see if tweeter is there
  for(int i = 0; i < *num_tweeters; ++i){
      if(strcmp(tweeters[i].name, tweeter_to_add) == 0){
        ++tweeters[i].num_tweets;
        return;
      }
    }




    tweeters[*num_tweeters].name = malloc(sizeof(char)*strlen(tweeter_to_add));
    strcpy(tweeters[*num_tweeters].name, tweeter_to_add);
    ++tweeters[*num_tweeters].num_tweets;
    ++(*num_tweeters);

    return;
}

void sort_tweeters(int num_tweeters){
  struct tweeter temp;
  for(int i = 0; i < num_tweeters - 1; ++i){
      for(int j = 0; j < num_tweeters - i - 1; ++j){
          if(tweeters[j].num_tweets < tweeters[j +1].num_tweets){
              //swap the elements
              temp = tweeters[j];
              tweeters[j] = tweeters[j+1];
              tweeters[j+1] = temp;

          }
      }

  }
  return;
}

void clear_up_cells(int num_words){
  int j;
  for(int i = 0; i < num_words; ++i){
      j = 0;
      while(cells[i][j] != '\0'){
          cells[i][j]  = '\0';
          ++j;
      }
  }
  return;
}

char* remove_quotes(char* tweeter_name) {
    bool quoted = false;
    int len = strlen(tweeter_name);
    if (strlen(tweeter_name) > 2) {
        if (tweeter_name[0] == '\"' && tweeter_name[len-1] == '\"') {
            quoted = true;
        }
    }

    if(quoted){
        //remove quotes
        char* new_str = (char*)malloc((len-2)  * sizeof(char));
        int j = 0;
        for(int i = 0; i < len; ++i){
            if(i == 0 || i == len-1){
                continue;
            }

            new_str[j] = tweeter_name[i];
            ++j;
        }

        return new_str;

    }else{
        return tweeter_name;
    }


}

void print_results(int num_tweeters){
  //if there are less than 10 tweeters, print those out
  int num = 10;
  if(num_tweeters < 10){
      num = num_tweeters;
  }




  //print top 10 tweeters
  for(int i = 0; i < num; ++i){

      printf("%s: %d \n", remove_quotes(tweeters[i].name), tweeters[i].num_tweets);

  }


  return;
}

int main(int argc, char const *argv[]){

  FILE *f = fopen(argv[1],"r"); //add a check in case nothing is passed in

  //consider making global
  int num_words = 0;
  int num_chars = 0;
  char current_char;
  bool first = true;
  int num_lines = 0;
   //consider making global
  int num_tweeters = 0;
  int num_columns = 0;
  bool* quoted = (bool*) malloc(sizeof(bool));

  if(f == NULL){
    //filepath was Invalid
    invalid_error();
  }else{
  //need to check that if a column is surrounded by quotes
  //everything else has to be surrounded by quotes (???)
    char line[MAX_CHARS];

    while(fgets(line,MAX_CHARS,f))
    {
      ++num_lines;
      if(num_lines > MAX_LINES){
      invalid_error(); //macro
      }

      num_words = 0;
      num_chars = 0;

      size_t line_len = strlen(line);

      for(int i=0; i <=line_len; ++i){
        current_char = line[i];
        if(current_char != ',' && i != line_len ){
          //remove new line at the end of every line
          if(current_char == '\n' && i == line_len-1){
            continue;
          }else if(i == line_len-1 && current_char != '\n' && line_len == MAX_CHARS){
              //line overflow
              printf("LINE OVERFLOW");
              invalid_error();
          }
          cells[num_words][num_chars] = current_char;
          ++num_chars;
        }else{

            if(first){
                quoted = realloc(quoted, num_words + 1 * sizeof(bool));
                if(cells[num_words][0] == '\"'){
                    quoted[num_words] = true;
                }else{
                    quoted[num_words] = false;
                }
            }

          //even number of quotes so this is a cell, add it to array of strings
          if(!check_for_quotes(cells[num_words], num_chars,quoted[num_words])){
            invalid_error(); //macro
          }

          cells[num_words][num_chars+1] = '\0';
          num_chars = 0;
          ++num_words;
        }
      }

      if(!check_for_quotes(cells[num_words],num_chars, quoted[num_words])){
        invalid_error(); //macro
      }
      ++num_words;

      if(first){
          num_columns = num_words;

        //header line
        if(has_duplicates(num_words)){
          invalid_error(); //macro
        }
        if(get_num_name(num_words) != 1){
          invalid_error(); //macro
        }

        first = false;
      }else{

        if(num_words != num_columns){
            invalid_error();
        }

        //look for name in proper column
        char *tweeter_name = get_name();
        if(!check_for_quotes(tweeter_name,num_chars, quoted[name_index])){
          invalid_error(); //macro
        }




        //check if tweeter is already in list add it if not
        check_for_tweeter(&num_tweeters, tweeter_name);


      }

        //clear up cells
        clear_up_cells(num_words);
    }


  }


    if(num_lines == 0){
        invalid_error();
    }

    //find the top 10, sort tweeters by num tweets
    sort_tweeters(num_tweeters);

    print_results(num_tweeters);


    free(quoted);
    return 0;

}
