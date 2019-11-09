#ifndef CONFIG_H
#define CONFIG_H

/* user settings */

// parser
#define LINE_BUFFER_SIZE  10240

// definition
#define WILDCARD_MARK     '*'
#define ITEM_SEPERATOR    ','

// TM hardware
#define LIMIT_ENABLE      true
#define LIMIT_TIME        3000
#define LIMIT_SPACE       100

// file devices
#define DEFINITION_FILENAME "test.tm"
#define INPUT_FILENAME      "input.txt"
#define OUTPUT_FILENAME     "result.txt"
#define DEBUG_FILENAME      "console.txt"

// console printings
#define BANNER_WIDTH      78
#define BANNER_SECTION    '='
#define BANNER_SEPERATOR  '-'
#define COMMENT_MARK      ';'

/* os specifics */
#ifdef _WIN32
  #define PATH_SEPERATOR '\\'
#elif __linux__ || __unix__
  #define PATH_SEPERATOR '/'
#endif

/* internal debug */
// #define DEBUG_PARSER
#define DEBUG_TM

#endif  // CONFIG_H